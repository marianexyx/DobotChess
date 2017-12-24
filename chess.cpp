#include "chess.h"

Chess::Chess(Dobot *pDobot, Chessboard *pBoardMain, Chessboard *pBoardRemoved, ArduinoUsb *pUsb,
             Websockets* pWebsockets, TCPMsgs *pTCPMsgs, COMMUNICATION_TYPES PlayerSource)
{
    _pTimers = new ChessTimers;
    _pMovements = new ChessMovements;
    _pBot = new ChessBot;
    _pStatus = new ChessStatus;

    _pDobot = pDobot;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pWebsockets = pWebsockets;
    _pTCPMsgs = pTCPMsgs;
    _pUsb = pUsb;

    _PlayerSource = PlayerSource;

    for (int i=1; i>=32; ++i)
        *_pPiece[i] = new Piece(i);

    connect(_pTimers, SIGNAL(sendMsgToPlayer(QString)), this, SLOT(sendDataToPlayer(QString)));
}

Chess::~Chess()
{
    delete _pTimers;
    delete _pMovements;
    delete _pBot;
    delete _pStatus;
}


void Chess::continueGameplay()
{
    if (_PlayerSource == WEBSITE)
    {
        _pTimers->switchPlayersTimers();
        this->sendDataToPlayer("moveOk " + _pMovements->getMove().asQStr() + " " +
                               turnTypeAsQstr(_pStatus->getWhoseTurn()) + " continue");
    }
    else if (_PlayerSource == ARDUINO)
    {
        if (!_pBot->getAI) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
        {
            if (_pBot->getAIAsPlayer2())
                this->sendDataToPlayer("EnterSimulatedIgorsMove");
            else this->SendMsgToTcp("think 5000"); //wymyśl kolejny ruch bota białego Igora
        }
        else  //a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku
        {
            _pBot->setAI(false);
            this->sendDataToPlayer("IgorHasEndedMove"); //niech gracz wykonuje swój kolejny ruch
        }
    }
    else
        qDebug() << "ERROR: Chess::continueGameplay: unknown _PlayerSource val ="
                 << _PlayerSource;
}

void Chess::sendDataToPlayer(QString msg)
{
    qDebug() << "Sending to" << communicationTypeAsQStr(_PlayerSource) << ":" << msg;
    this->addTextToConsole("Sending to " + communicationTypeAsQStr(_PlayerSource) + ": "
                           + msg, LOG_CORE);

    if (_PlayerSource == WEBSITE)
    {
        _pWebsockets->sendMsg(msg);
    }
    else if (_PlayerSource == ARDUINO)
    {
        //todo: nieujednolicone typy komunikatów web z arduino są, przez co...
        //...takie dziwne zmiany w locie (wynika to z virtualow i dziedziwczenia)
        if (msg.contains("promote")) msg = "promote";
        else if (msg.contains("newOk")) msg = "started";
        else if (msg.contains("badMove")) msg = "BAD_MOVE";

        _pUsb->sendDataToUsb(msg);
    }
    else
        qDebug() << "ERROR: Chess::sendDataToPlayer: unknown _PlayerSource val ="
                 << _PlayerSource;
}


void Chess::startNewGameInChenard() //przesyłanie prośby o nową grę na TCP
{
    //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
    if ((!_pWebsockets->isPlayerChairEmpty(PT_WHITE) && !_pWebsockets->isPlayerChairEmpty(PT_BLACK))
            || _PlayerSource == ARDUINO)
    {
        this->SendMsgToTcp("new");
    }
    else
        qDebug() << "ERROR: Chess::startNewGameInChenard(): Wrong players names";
}

void Chess::Promote(QString msg)
{
    if (_PlayerSource == WEBSITE)
        _pMovements->promoteMoveSequence(*this);

    _pMovements->setMoveType(ST_PROMOTION);
    this->SendMsgToTcp("move " + _pStatus->getFuturePromoteAsQStr() + msg);
    _pStatus->clearFuturePromote();
}

void Chess::SendMsgToTcp(QString msg)
{
    qDebug() << "Sending to tcp:" << msg;
    this->addTextToConsole("Sending to tcp:" + msg + "\n", LOG_CORE);
    _pTCPMsgs->TcpQueueMsg(_PlayerSource, msg);
}

void Chess::GameStarted()
{
    qDebug() << "Sending 'newOk' to player";
    this->addTextToConsole("newGame\n", LOG_CORE);

    _pTimers->resetGameTimers();
    _pTimers->startGameTimer();

    this->sendDataToPlayer("newOk");
}

void Chess::BadMove(QString msg)
{
    this->sendDataToPlayer("badMove " + msg + " " + turnTypeAsQstr(_pStatus->getWhoseTurn()));
}

void Chess::EndOfGame(QString msg)
{
    END_TYPE ETWhoWon;
    if (msg.left(3) == "1-0") ETWhoWon = ET_WHIE_WON;
    else if (msg.left(3) == "0-1") ETWhoWon = ET_BLACK_WON;
    else if (msg.left(7) == "1/2-1/2") ETWhoWon = ET_DRAW;
    else
    {
        qDebug() << "ERROR: Chess::EndOfGame: unknown arg val =" << msg;
        return;
    }

    //todo: ta funkcja bedzie gdzie indziej, dostepna rowniez dla arduino
    _pWebsockets->endOfGame(ETWhoWon); //todo: odpalam endOfGame() w endOfGame(). robi to syf

    //todo: troche dziwnie to tu jest że pobieram z obiektu websocket dane by mu je zaraz przesłać
    this->sendDataToPlayer("moveOk " + _pMovements->getMove().asQStr() + " nt " +
                           endTypeAsQstr(ETWhoWon) + " " +
                           _pWebsockets->getTableDataAsJSON()); //todo: json przeniesc
    //todo: wysyłam wiadomość na websockety trochę nie z poziomu websocketów (czy większość...
    //...informacji leci od websocketów? sprawdzić, jeżeli tak to zobaczyć czy da się wysyłanie...
    //...każdego rodzaju inforacji upchać w websocketach dla porządku i czy jest taka potrzeba
    //todo: wygląda na to że funkcja resetu załącza się jeszcze zanim odpowiedź poleci na...
    //...stronę, przez co trzeba czekać aż resetowanie się zakończy zanim gracze się dowiedzą...
    //...że nastąpił koniec gry
    this->reset();
}

void Chess::resetBoardData() //todo: troche bodajze nieadekwatna nazwa
{
    //todo: zastanowić się na spokojnie jakie czyszczenia jeszcze tu upchać
    //todo: sprawdzić czy zresetowałem inne dane: zegary, tury, planszę fizyczną/ w pamięci itd
    //todo: przeniesione metody
    _pTimers->stopBoardTimers();
    _pStatus->setWhoseTurn(NO_TURN);
    _pStatus->clearLegalMoves();
    _pStatus->clearHistoryMoves();
    _pStatus->clearFormBoard();
}

bool Chess::isPiecesSetOk()
{
    for (short piece=1; piece>=32; ++piece)
    {
        bool isPieceExists = false;
        for (short field=1; field>=64; ++field)
        {
            if (_pBoardMain->getField(field)->getPieceNrOnField() == piece ||
                    _pBoardRemoved->getField(field)->getPieceNrOnField() == piece ||
                    _pDobot->getItemInGripper() == piece)
            {
                isPieceExists = true;
                break;
            }
        }
        if (!isPieceExists)
        {
            qDebug() << "ERROR: Chess::isPiecesSetOk- it isn't, missing piece nr:" << piece;
            return false;
        }
    }
    return true;
}

QString Chess::getTableDataAsJSON()
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt",
    //"wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + this->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + this->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" + _pChessboard->getStrWhoseTurn() +
            "\",\"wtime\":" + QString::number(_pChessboard->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pChessboard->getBlackTimeLeft()) +
            ",\"queue\":\"" + this->getQueuedClientsList();

    if (_pTimers->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart = this->isStartClickedByPlayer(PT_WHITE) ? "w" : "x";
        QString QStrBlackClickedStart = this->isStartClickedByPlayer(PT_BLACK) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pChessboard->getStartTimeLeft());
    }

    if (!_pChessStatus->getHistoryMoves().isEmpty())
        QStrTableData += "\",\"history\":\"" + _pChessStatus->getHistoryMovesAsQStr();

    QStrTableData += "\"}";

    qDebug() << "Websockets::getTableDataAsJSON(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Chess::TcpMoveOk()
{
    qDebug() << "Chess::TcpMoveOk()";

    if (_PlayerSource == WEBSITE)
        this->continueGameplay();

    this->SendMsgToTcp("status");
}

void Chess::reset() //todo: przemyśleć co tu musi być
{
    //todo: pewnie bota udało by się scalić z tą funkcją w pewnych momentach
    if (_PlayerSource == WEBSITE)
    {
        _pStatus->setWhoseTurn(NO_TURN);
        _pWebsockets->resetPlayersStartConfirmInfo();
        this->sendDataToPlayer("reseting");
        _pTimers->resetGameTimers();
        this->resetPiecePositions();
    }
    else
        qDebug() << "ERROR: Chess::reset: _PlayerSource != WEBSITE. it ==" << _PlayerSource;
}

void Chess::resetBoardCompleted()
{
    if (_PlayerSource == WEBSITE)
    {
        //TODO: prewencyjnie ustawić wszystkie wartości na startowe (rozpisać to: jakie, które i ...
        //...po co w sumie- tj. czy to nie występuje zawsze w otoczeniu WebChess::reset()?)

        //todo: raz wiadomość jest cała sklejana w websockecie, a raz geTableData jest doklejane w tym pliku
        _pWebsockets->sendMsg("resetComplited");
    }
    else if (_PlayerSource == ARDUINO)
    {
        this->startNewGameInChenard();
    }
    else
        qDebug() << "ERROR: Chess::resetBoardCompleted: unknown _PlayerSource val ="
                  << _PlayerSource;
}

void Chess::wrongTcpAnswer(QString msgType, QString respond)
{
    qDebug() << "ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void Chess::playerClickedStart(QString QStrWhoClicked)
{
    if (QStrWhoClicked == "WHITE")
    {
        _pWebsockets->setClientState(PT_WHITE, true);
        qDebug() << "white player clicked start";
    }
    else if (QStrWhoClicked == "BLACK")
    {
        _pWebsockets->setClientState(PT_BLACK, true);
        qDebug() << "black player clicked start";
    }
    else qDebug() << "ERROR:unknown playerClickedStart val:" << QStrWhoClicked;

    if (_pWebsockets->isStartClickedByPlayer(PT_WHITE) &&
            _pWebsockets->isStartClickedByPlayer(PT_BLACK))
    {
        qDebug() << "both players have clicked start. try to start game";
        _pTimers->stopQueueTimer();
        this->startNewGameInChenard();
        _pWebsockets->setClientState(PT_WHITE, false);
        _pWebsockets->setClientState(PT_BLACK, false);
    }
}

void Chess::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    //todo: długi syf
    if (_PlayerSource == WEBSITE)
    {
        qDebug() << "WebChess::checkMsgFromChenard: tcpMsgType=" << tcpMsgType <<
                    ", tcpRespond:" << tcpRespond;

        if (tcpMsgType == "new")
        {
            //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki
            if (tcpRespond == "OK\n" || tcpRespond == "OK")
            {
                //pierwszy legal i status wyglądają zawsze tak samo:
                _pStatus->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
                _pStatus->setLegalMoves("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 d2d4 e2e3 e2e4 "
                              "f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");
                this->GameStarted();
            }
            else
                this->wrongTcpAnswer(tcpMsgType, tcpRespond);
        }
        else if (tcpMsgType.left(4) == "move")
        {
            //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki poniżej
            if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") this->TcpMoveOk();
            else if (tcpRespond/*.left(8)*/ == "BAD_MOVE") this->BadMove(tcpRespond);
            else this->wrongTcpAnswer(tcpMsgType, tcpRespond);
        }
        else if (tcpMsgType == "status")
        {
            _pStatus->saveStatusData(tcpRespond);

            if (_pStatus->getFENGameState() == FGS_IN_PROGRESS)
            {
                this->SendMsgToTcp("history pgn");
                this->continueGameplay(); //TODO: zrobić to kiedyś tak by dopiero w odpowiedzi...
                //...na legal wysyłał na stronę potwiedzenie wykonania ruchu (tj. zdjął...
                //...blokadę przed kojenym ruchem)
            }
            else if (_pStatus->getFENGameState() == FGS_WHITE_WON ||
                     _pStatus->getFENGameState() == FGS_BLACK_WON ||
                     _pStatus->getFENGameState() == FGS_DRAW)
            {
                _pStatus->clearLegalMoves();
                _pStatus->clearHistoryMoves();
                this->EndOfGame(tcpRespond);
            }
            else
                this->wrongTcpAnswer(tcpMsgType, _pChessboard->getGameStatus());
        }
        else if (tcpMsgType.left(7) == "history" && tcpRespond.left(3) == "OK ")
        {
            qDebug() << "manage history tcp answer";
            _pStatus->historyOk(tcpRespond);
            _pWebsockets->sendMsg("history " + _pChessboard->getHisotyMovesAsQStr());
            this->SendMsgToTcp("legal");
            //todo: jeszcze odpowiedź na site
        }
        else if (tcpMsgType == "legal" && tcpRespond.left(3) == "OK ")
        {
            _pStatus->setLegalMoves(tcpRespond);
        }
        else qDebug() << "ERROR: WebChess:checkMsgFromChenard() received unknown tcpMsgType: "
                      << tcpMsgType;
    }
    else if (_PlayerSource == ARDUINO)
    {
        this->checkAI();

        if (tcpMsgType == "new" && (tcpRespond == "OK\n" || tcpRespond == "OK"))
        {
            this->SendMsgToTcp("status");
            this->GameStarted();
        }
        else if (tcpMsgType.left(4) == "move")
        {
            //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego 2 warunki
            if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") this->TcpMoveOk();
            else if (tcpRespond.left(8) == "BAD_MOVE") this->BadMove(tcpRespond);
            else wrongTcpAnswer(tcpMsgType, tcpRespond);
        }
        else if (tcpMsgType == "status")
        {
            _pChessboard->saveStatusData(tcpRespond);
            if (_pChessboard->getGameStatus() == "*")
            {
                this->SendMsgToTcp("history pgn");
            }
            else
            {
                _pStatus->clearLegalMoves();
                _pStatus->clearHistoryMoves();
                this->EndOfGame(tcpRespond);
            }
        }
        else if (tcpMsgType == "undo 1" && (tcpRespond == "OK\n" || tcpRespond == "OK"))
        {
            this->UndoOk();
        }
        else if (tcpMsgType == "think 5000" && (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1"))
        {
            this->ThinkOk(tcpRespond); //"f.e.: OK d1h5 Qh5#"
        }
        else if (tcpMsgType == "legal" && (tcpRespond.left(3) == "OK "))
        {
            _pStatus->setLegalMoves(tcpRespond);
        }
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else
        qDebug() << "ERROR: Chess::checkMsgFromChenard: unknown _PlayerSource val ="
                 << _PlayerSource;
}

void Chess::checkMsgForChenard(QString msgFromWs)
{
    //todo: upchac to ladnie
    if (_PlayerSource == WEBSITE)
    {
        qDebug() << "WebChess::checkMsgForChenard: received: " << msgFromWs;
        if (msgFromWs.left(7) == "newGame") this->playerClickedStart(msgFromWs.mid(8));
        else if (msgFromWs.left(4) == "move") this->handleMove(msgFromWs.mid(5));
        else if (msgFromWs.left(9) == "promoteTo") this->Promote(msgFromWs.right(1));
        else if (msgFromWs.left(5) == "reset") this->reset();
        else qDebug() << "ERROR: received not recognized msg in WebChess::checkMsgForChenard: "
                      << msgFromWs;
    }
    else if (_PlayerSource == ARDUINO)
    {
        qDebug() << "IgorBot::checkMsgForChenard: received: " << msg;
        if (msg == "new") this->startNewGameInChenard();
        else if (msg.left(4) == "move") this->handleMove(msg.mid(5));
        else if (msg.left(9) == "promoteTo") this->Promote(msg.mid(10));
        else if (msg.left(5) == "reset") this->resetPiecePositions();
        else qDebug() << "ERROR: received not recognized msg in IgorBot::checkMsgForChenard: "
                      << msg;
    }
    else
        qDebug() << "ERROR: Chess::checkMsgForChenard: unknown _PlayerSource val ="
                 << _PlayerSource;

}

void Chess::resetPiecePositions()
{
    if (!this->compareArrays(_pChessboard->m_asBoardMain, _pChessboard->m_anBoardStart))
        //jeżeli bierki stoją na szachownicy nieruszone, to nie ma potrzeby ich odstawiać
    {
        bool  bArraysEqual; //warunek wyjścia z poniższej pętli do..for
        bool isChessboardInvariable; //do sprawdzania czy pętla do..for się nie zacieła

        do //wertuj w kółko planszę przestawiając bierki, dopóki...
            //...szachownica nie wróci do stanu pierwotnego/startowego.
        {
            //aktualnie sprawdzane pole szachownicy
            for (int nCheckingField = 1; nCheckingField <= 64; nCheckingField++)
            {
                int nPieceNrOnCheckingField = _pChessboard->getPieceOnBoardAsNr(B_MAIN, nCheckingField);
                int nPieceStartPosOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_START, nCheckingField);
                short sCheckingFieldsStartingPieceNr = this->findInitialFieldOfGivenPiece(nPieceNrOnCheckingField);

                bool bIsStartPieceOnRemovedFieldOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_REMOVED, nPieceStartPosOfCheckingField);

                if (nPieceNrOnCheckingField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit <= D_2  //...a coś tam powinno być
                            || _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit >= D_7))

                {
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(bIsStartPieceOnRemovedFieldOfCheckingField) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        short sStartPieceOnRemovedFieldOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_REMOVED, nPieceStartPosOfCheckingField);
                        this->listMovesForDobot(ST_RESTORE, //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                _pChessboard->fieldNrToPosOnBoard(sStartPieceOnRemovedFieldOfCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(sStartPieceOnRemovedFieldOfCheckingField).Digit,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit);
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy to ...
                    {
                        for (int nField = 1; nField <= 64; nField++) //...przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nPieceStartPosOfCheckingField == _pChessboard->getPieceOnBoardAsNr(B_MAIN, nField)
                                    && nCheckingField != nField)
                                //jeżeli na tym wertowanym poluszachownicy jest bierka której szukamy ale nie może to być
                                // oryginalna pozycja bierki (musi być na obcym polu startowym)
                            { //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                                this->listMovesForDobot(ST_REGULAR,
                                                        _pChessboard->fieldNrToPosOnBoard(nField).Letter,
                                                        _pChessboard->fieldNrToPosOnBoard(nField).Digit,
                                                        _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                        _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit);
                            }
                        }
                    }
                }
                //natomiast jeżeli na sprawdzanym polu jest bierka, która nie znajdowała się tam podczas startu gry...
                //...to trzeba sprawdzić czy oryginalne pole tej bierki nie jest zajęte i tam ją odstawić
                else if (nPieceNrOnCheckingField != 0 && //jeżeli jakaś bierka na pewno tam jest
                         nPieceStartPosOfCheckingField != nPieceNrOnCheckingField) // i nie to bierka która jest tam orygnalnie
                {
                    short sPieceNrOnStartPosOfPieceOnCheckingField = _pChessboard->getPieceOnBoardAsNr(B_MAIN, sCheckingFieldsStartingPieceNr);

                    if (sPieceNrOnStartPosOfPieceOnCheckingField == 0) //czy startowe pole bierki jest puste, by tam można było ją odstawić
                    {
                        this->listMovesForDobot(ST_REGULAR,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit,
                                                _pChessboard->fieldNrToPosOnBoard(sCheckingFieldsStartingPieceNr).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(sCheckingFieldsStartingPieceNr).Digit);
                    }
                    else //jeżeli startowe pole bierki na polu sprawdzanym jest zajęte przez inną bierkę
                    {
                        //pole startowe bierki która jest na polu startowym bierki aktualnie sprawdzanej
                        int nStartNrOf2ndPieceOn1stPieceStartPos = _pChessboard->getPieceOnBoardAsNr(B_START, sPieceNrOnStartPosOfPieceOnCheckingField);

                        if (nStartNrOf2ndPieceOn1stPieceStartPos == nCheckingField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            //todo: zrobić funkcję z 1 argumentem która wyczai łapaną bierkę i odstawi ją na swoje pole
                            this->listMovesForDobot(ST_REMOVING,
                                                    _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                    _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit,
                                                    _pChessboard->fieldNrToPosOnBoard(nPieceNrOnCheckingField).Letter,
                                                    _pChessboard->fieldNrToPosOnBoard(nPieceNrOnCheckingField).Digit);
                        }
                        else qDebug() << "Chess::resetPiecePositions(): nStartNrOf2ndPieceOn1stPieceStartPos:" <<
                                         nStartNrOf2ndPieceOn1stPieceStartPos << "!= nCheckingField:" << nCheckingField;
                    }
                    //everything else: jeżeli oryginalne pole bierki na polu sprawdzanym też jest zajęte, a nie...
                    //...jest to bierka, której pozycja startowa to pole aktualnie sprawdzaneto niech pętla wykona...
                    //...się jeszcze raz, a wtedy układ bierek się na pewno zmieni i będzie dało się dalej coś przestawiać.
                }
                //else: jeśli żadna opcja nie wystąpiła, tzn. że na danym polu leży prawidłowa/startowa bierka...
                //...albo bierki nie da się odstawić, to sprawdź kolejne pole
            }

            //sprawdzanie czy ustawienie bierek na szachownicy jest ostateczne, tj. takie same jak na...
            //...starcie. Jeżeli którakolwiek bierka nie znajduje się na swoim startowym polu, to stan...
            //...rozmieszczenia bierek na szachownicy nie jest ostateczny.
            bArraysEqual = this->compareArrays(_pChessboard->m_asBoardMain,
                                                       _pChessboard->m_anBoardStart);

            if (!bArraysEqual) //jeżeli plansza jest w stanie startowym, to można nie robić reszty...
                //...poleceń i wyskoczyć z pętli
            {
                //sprawdzanie czy ustawienie bierek zmieniło się od ostatniego przejścia przez pętlę...
                //...przestawiającą bierki. Jeżeli jest różnica od ostatniego razu, to zapamiętaj że...
                //...tak jest. Pozwoli to sprawdzać czy pętla do..for nie zacięła się, a co za tym...
                //...idzie- spradzane jest czy odstawianie bierek na planszę się nie zacieło.
                isChessboardInvariable = this->compareArrays(_pChessboard->m_asBoardMain,
                                                                     _pChessboard->m_asBoardTemp);

                //jeżeli aktualne przewertowane planszy w poszukiwaniu bierek do odstawienia...
                //...spowodowało przemieszczenie się jakiejś bierki, to zapamiętaj rozmieszczenie...
                //...bierek na szachownicy do sprawdzania tego warunku w kolejnym przejściu pętli.
                if (!isChessboardInvariable)
                {
                    memcpy(_pChessboard->m_asBoardTemp, _pChessboard->m_asBoardMain,
                           sizeof(_pChessboard->m_asBoardMain));
                }
                else
                {
                    qDebug() << "ERROR: Chess::resetPiecePositions() stuck- shouldn't be possible. isChessboardInvariable ="
                             << isChessboardInvariable;
                    bArraysEqual = true; //wyjdź z pętli, bo inaczej program się tutaj zatnie
                }
            }
            else qDebug() << "Chess::resetPiecePositions(): pieces are on start positions. end of loop";
        }
        //wertuj w kółko planszę przestawiając bierki, dopóki
        //...szachownica nie wróci do stanu pierwotnego/startowego.
        while (!bArraysEqual); //wykonuj pętlę dopóki warunek jest prawdziwy

        qDebug() << "End of: Chess::resetPiecePositions()";
    }
    qDebug() << "Chess::resetBoardCompleted(): imaginary start board and real board are equal";
    this->resetBoardCompleted();
}

void Chess::handleMove(QString QStrMove)
{
    _pMovements->setMove(QStrMove);
    _pMovements->setMoveType(_pStatus->findMoveType(QStrMove));

    switch(_pChessboard->getMoveType())
    {
    //todo: ST_PROMOTE_TO_WHAT to nie ruch- to brak ruchu. wywalic stad
    /*case ST_PROMOTE_TO_WHAT: this->PromoteToWhat(move); break;*/
    case ST_REGULAR: _pMovements->regularMoveSequence(*this); break;
    case ST_REMOVING: _pMovements->removeMoveSequence(*this);  break;
    //restore available only in reset command
    //case ST_RESTORE: _pMovements->restoreMoveSequence(*this, );  break;
    case ST_ENPASSANT: _pMovements->enpassantMoveSequence(*this); break;
    case ST_CASTLING: this->castlingMovingSequence(); break;
    //todo: bad_move to nie ruch- to brak ruchu. wywalic stad
    /*case ST_BADMOVE: this->BadMove(move); break;*/
    case ST_PROMOTION: _pMovements->promoteMoveSequence(*this); break;
    case ST_NONE:
    default:
        qDebug() << "ERROR: Chess::handleMove: wrong MoveType:" << _pMovements->getMoveType();
        break;
    }

    this->SendMsgToTcp("move " + QStrMove);
    //_pStatus->clearFuturePromote(); //todo: sprawdzic jak ogarniac czyszczenia po ruchu
}

//todo: jezeli movements beda dziedziczone, to to powinno byc wewnatrz tamtej klasy
void Chess::movePieceWithManipulator(Chessboard* pRealBoard, PosOnBoard FieldPos,
                                              VERTICAL_MOVE vertMove = VM_NONE)
{
    if (pRealBoard->getBoardType() != B_MAIN && pRealBoard->getBoardType() != B_REMOVED)
    {
        qDebug() << "ERROR: Chess::movePieceWithManipulator: board isnt real. board ="
                 << boardTypeAsQstr(pRealBoard->getBoardType());
        return;
    }

    if (vertMove == VM_GRAB)
    {
        if (!this->isPiecesSetOk()) return;
        short sPieceNrOnField = pRealBoard->getField(FieldPos)->getPieceNrOnField();
        if (!Piece::isInRange(sPieceNrOnField)) return;

        _pDobot->setItemInGripper(sPieceNrOnField);
        pRealBoard->clearField(Field::nr(FieldPos));
        if (!this->isPiecesSetOk()) return;
    }
    else if (vertMove == VM_PUT)
    {
        if (!this->isPiecesSetOk()) return;
        pRealBoard->setPieceOnField(_pDobot->getItemInGripper(), Field::nr(FieldPos));
        _pDobot->clearGripper();
        if (!this->isPiecesSetOk()) return;
    }

    Point3D xyz = pRealBoard->getField(Field::nr(FieldPos))->getLocation3D();
    _pDobot->doMoveSequence(xyz, vertMove);
}

bool Chess::compareArrays(short nArray1[][8], short nArray2[][8])
{
    for (short i=0; i<8; i++)
    {
        for (short j=0; j<8; j++)
        {
            if (nArray1[i][j] != nArray2[i][j])
                return false;
        }
    }
    return true;
}
