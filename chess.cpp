#include "chess.h"

Chess::Chess(Clients *pClients, Dobot *pDobot, Chessboard *pBoardMain,
             Chessboard *pBoardRemoved, ArduinoUsb *pUsb, Websockets* pWebsockets,
             TCPMsgs *pTCPMsgs, COMMUNICATION_TYPES PlayerSource)
{
    _pTimers = new ChessTimers(*this);
    _pMovements = new ChessMovements(*this);
    _pBot = new ChessBot(*this);
    _pStatus = new ChessStatus(*this);
    _pResets = new ChessResets(*this) ;

    _pClients = pClients;
    _pDobot = pDobot;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pWebsockets = pWebsockets;
    _pTCPMsgs = pTCPMsgs;
    _pUsb = pUsb;

    _PlayerSource = PlayerSource;

    for (int i=1; i>=32; ++i)
        *_pPiece[i] = new Piece(i);

    //todo: przy zrobionych nowych odniesieniach connect raczej zbędny
    connect(_pTimers, SIGNAL(sendMsgToPlayer(QString)), this, SLOT(sendDataToClient(QString)));
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
        _pWebsockets->sendMsgToAllClients("moveOk " + _pMovements->getMove().asQStr() + " " +
                                          turnTypeAsQstr(_pStatus->getWhoseTurn()) + " continue")
    }
    else if (_PlayerSource == ARDUINO)
    {
        //todo: to kto wysłał ruch (gracz/bot) powinno być kontrolowane przed, a nie po akcji
        //todo: zrobić kiedyś diagram działań z arduino i przemyśleć ułozenie kodu
        if (!_pBot->getAI) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
        {
            if (_pBot->getAIAsPlayer2()) //todo: powinno być zawsze true dla arduino
                this->sendDataToClient("EnterSimulatedIgorsMove");
            else this->sendMsgToTcp("think 5000"); //wymyśl kolejny ruch bota białego Igora
        }
        else  //a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku
        {
            _pBot->setAI(false);
            this->sendDataToClient("IgorHasEndedMove"); //niech gracz wykonuje swój kolejny ruch
        }
    }
    else
        qDebug() << "ERROR: Chess::continueGameplay: unknown _PlayerSource val ="
                 << _PlayerSource;

    this->sendMsgToTcp("status");
}

void Chess::sendDataToClient(QString msg, int64_t clientID = -1)
{
    qDebug() << "Sending to" << communicationTypeAsQStr(_PlayerSource) << ":" << msg;
    this->addTextToConsole("Sending to " + communicationTypeAsQStr(_PlayerSource) + ": "
                           + msg, LOG_CORE);

    if (_PlayerSource == WEBSITE)
    {
        _pWebsockets->sendMsg(msg, clientID);
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
        qDebug() << "ERROR: Chess::sendDataToClient: unknown _PlayerSource val ="
                 << _PlayerSource;
}


void Chess::startNewGameInChenard() //przesyłanie prośby o nową grę na TCP
{
    //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
    //todo: clients?
    if ((!_pWebsockets->isPlayerChairEmpty(PT_WHITE) && !_pWebsockets->isPlayerChairEmpty(PT_BLACK))
            || _PlayerSource == ARDUINO)
    {
        this->sendMsgToTcp("new");
    }
    else
        qDebug() << "ERROR: Chess::startNewGameInChenard(): Wrong players names";
}

void Chess::sendMsgToTcp(QString msg)
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

    _pWebsockets->sendMsgToAllClients("newOk");
}

void Chess::tellPlayerThatHeGaveBadMove(QString msg)
{
    qDebug() << "Chess::tellPlayerThatHeGaveBadMove:" << msg;
    QString QStrMsgForActiveClient = "badMove " + msg + " "
            + turnTypeAsQstr(_pStatus->getWhoseTurn());
    int64_t activePlayerID = _pClients->getClientID(_pClients->getPlayer(
                                                            this->getActivePlayerType()));
    this->sendDataToClient(QStrMsgForActiveClient, activePlayerID);
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

bool Chess::isPieceStayOnItsStartingField(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return 0;

    Field* pStartFieldOfPiece = _pBoardMain->getField(Piece::StartFieldNr(sPieceNr));
    if (sPieceNr == pStartFieldOfPiece->getPieceNrOnField());
}

//todo: prześledzić dokładnie to co się dzieje w tej funkcji
void Chess::removeClient(int64_t clientID)
{
    Client client = _pClients->getClient(clientID);
    if (client.socket) //todo: co to??? czyżby to że sprawdzam czy udało mi się...
        //...zrzutować obiekt z powyższej linjki?
    {
        if (_pClients->isClientAPlayer(client))
            this->restartGame(ET_SOCKET_LOST, client);
        else
        {
            //todo: wyświetla mi się disconnect w dziwnym miejscu
            qDebug() << "non-player disconnected";
            emit addTextToConsole("non-player disconnected\n", LOG_WEBSOCKET);
        }

        client.socket->deleteLater();

        qDebug() << "Chess::removeClient(): remove client from list";
        for(int i = 0; i < _pClients->getClientsList().count(); ++i)
        { //nie męczyć się teraz z obsługą removeAll na structurach
          if(_pClients->getClientsList().at(i).socket == client.socket)
          {
            _pClients->getClientsList().removeAt(i);
            break;
          }
        }
    }

    emit setBoardDataLabels(std::to_string(_pClients->getClientsList().size()).c_str(),
                            BDL_SOCKETS_ONLINE);
    emit showClientsList(_pClients->getClientsList());
}

PLAYER_TYPE Chess::getActivePlayerType()
{
    if (_pStatus->getWhoseTurn() == WHITE_TURN &&
            _pClients->getPlayerSocket(PT_WHITE) != nullptr)
        return PT_WHITE;
    else if (_pStatus->getWhoseTurn() == BLACK_TURN &&
             _pClients->getPlayerSocket(PT_BLACK) != nullptr)
        return PT_BLACK;
    else
    {
        qDebug() << "ERROR: Chess::getActivePlayerType(): wrong turn:" <<
                    _pStatus->getWhoseTurn();
        return PT_NONE;
    }
}

QString Chess::getTableData()
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt",
    //"wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + _pClients->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + _pClients->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" + _pStatus->getStrWhoseTurn() +
            "\",\"wtime\":" + QString::number(_pTimers->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pTimers->getBlackTimeLeft()) +
            ",\"queue\":\"" + _pClients->getQueuedClientsList();

    if (_pTimers->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart =
                _pClients->isStartClickedByPlayer(PT_WHITE) ? "w" : "x";
        QString QStrBlackClickedStart =
                _pClients->isStartClickedByPlayer(PT_BLACK) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pTimers->getStartTimeLeft());
    }

    if (!_pStatus->getHistoryMoves().isEmpty())
        QStrTableData += "\",\"history\":\"" + _pStatus->getHistoryMovesAsQStr();

    QStrTableData += "\"}";

    qDebug() << "Chess::getTableData(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Chess::resetBoardCompleted()
{
    if (_PlayerSource == WEBSITE)
    {
        //TODO: prewencyjnie ustawić wszystkie wartości na startowe (rozpisać to: jakie,...
        //... które i po co w sumie)

        //todo: raz wiadomość jest cała sklejana w websockecie, a raz geTableData jest...
        //...doklejane w tym pliku
        if (_pClients->isGameTableOccupied())
            _pTimers->startQueueTimer();

        _pWebsockets->sendMsg("resetComplited");
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: nie startować automatycznie nowej gry?
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

//todo: cała funkcja do zmiany
void Chess::playerClickedStart(PLAYER_TYPE playerType)
{
    if (playerType == PT_WHITE)
    {
        _pWebsockets->setClientState(PT_WHITE, true);
        qDebug() << "white player clicked start";
    }
    else if (playerType == PT_BLACK)
    {
        _pWebsockets->setClientState(PT_BLACK, true);
        qDebug() << "black player clicked start";
    }
    else if (_PlayerSource == ARDUINO) qDebug() << "player clicked start";
    else qDebug() << "ERROR: Chess::playerClickedStart(): unknown playerClickedStart "
                     "val:" << playerTypeAsQStr(playerType);

    if (_pWebsockets->isStartClickedByPlayer(PT_WHITE) &&
            _pWebsockets->isStartClickedByPlayer(PT_BLACK)
            || _PlayerSource == ARDUINO)
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
    qDebug() << "Chess::checkMsgFromChenard: tcpMsgType=" << tcpMsgType <<
                ", tcpRespond:" << tcpRespond;

    //todo: pociąć tą funkcję na mniejsze kawałki

    if(_PlayerSource == ARDUINO) this->checkAI();

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
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1")
            this->continueGameplay();
        else if (tcpRespond == "BAD_MOVE")
            this->BadMove(tcpRespond);
        else
            this->wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        _pStatus->saveStatusData(tcpRespond);

        //TODO: naprawdę nie da się FGSów zrobić jako ETów?
        if (_pStatus->getFENGameState() == FGS_IN_PROGRESS)
        {
            this->sendMsgToTcp("history pgn");
            this->continueGameplay(); //TODO: zrobić to kiedyś tak by dopiero w odpowiedzi...
            //...na legal wysyłał na stronę potwiedzenie wykonania ruchu (tj. zdjął...
            //...blokadę przed kojenym ruchem)
        }
        else if (_pStatus->getFENGameState() == FGS_WHITE_WON ||
                 _pStatus->getFENGameState() == FGS_BLACK_WON ||
                 _pStatus->getFENGameState() == FGS_DRAW)
        {
            this->restartGame(_pStatus->getFENGameStateAsEndType());
        }
        else
            this->wrongTcpAnswer(tcpMsgType, _pStatus->getFENGameState());
    }
    else if (tcpMsgType.left(7) == "history" && tcpRespond.left(3) == "OK ")
    {
        _pStatus->historyOk(tcpRespond);
        _pWebsockets->sendMsgToAllClients("history " + _pStatus->getHisotyMovesAsQStr());
        this->sendMsgToTcp("legal");
    }
    else if (tcpMsgType == "legal" && tcpRespond.left(3) == "OK ")
        _pStatus->setLegalMoves(tcpRespond);
    else if (tcpMsgType == "undo 1" && _PlayerSource == ARDUINO &&
             (tcpRespond == "OK\n" || tcpRespond == "OK"))
        this->UndoOk();
    else if (tcpMsgType == "think 5000" && _PlayerSource == ARDUINO &&
             (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1"))
        this->ThinkOk(tcpRespond); //"f.e.: OK d1h5 Qh5#"
    else qDebug() << "ERROR: Chess:checkMsgFromChenard(): received unknown tcpMsgType:"
                  << tcpMsgType;
}

void Chess::checkMsgFromWebsockets(QString msg, int64_t clientID)
{
    qDebug() << "Chess::checkMsgFromWebsockets: received:" << msg;

    Client client = _pClients->getClient(clientID);

    if (msgFromWs == "newGame")
    {
        PLAYER_TYPE PlayerType = PT_NONE;
        if (client == _pClients->getPlayer(PT_WHITE))
            PlayerType = PT_WHITE;
        else if (client == _pClients->getPlayer(PT_BLACK))
            PlayerType = PT_BLACK;

        this->playerClickedStart(PlayerType);
    }
    else if (msg.left(4) == "move")
        this->findAndSaveMoveAndSendItToTcp(msg.mid(5));
    else if (msg.left(9) == "promoteTo")
        this->findAndSaveMoveAndSendItToTcp(msg);
    else if (msg == "getTableData")
        _pWebsockets->sendMsg(this->getTableData());
    else if (msg == "giveUp")
    {
        if (client.socket) //todo: zrozumieć to i dać w razie czego więcej tych warunków tam...
            //...gdzie są QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        {
            if (_pClients->isClientAPlayer(client))
                this->restartGame(ET_GIVE_UP, client);
            else
                qDebug() << "ERROR: Chess::receivedMsg(): non-player tried to logout (hacker?)";
        }
        else qDebug() << "ERROR: Chess::receivedMsg(): giveUp: !isset client";
    }
    else if (msg.left(5) == "sitOn")
    {
        if (_pClients->getClientName(client) == "-1")
        {
            qDebug() << "ERROR: Chess::receivedMsg(): sitOn: client not loggedIn";
            return;
        }

        PLAYER_TYPE PlayerChair = playerTypeFromQStr(msg.mid(5));

        _pStatus->setWhoseTurn(NO_TURN);
        if (!_pClients->isPlayerChairEmpty(PlayerChair))
        {
            qDebug() << "ERROR: Chess::receivedMsg(): client wanted to "
                        "sit on occupied chair. client name:"
                     << _pClients->getClientName(client) << ", player name:" <<
                        _pClients->getPlayerName(PlayerChair);
            return;
        }
        else
            _pClients->setPlayerType(client, PlayerChair);

        if (this->isGameTableOccupied())
            _pTimers->startQueueTimer();

        _pWebsockets->sendMsgToAllClients(this->getTableData());
    }
    else if (QStrWsMsgToProcess == "standUp")
    {
        if (_pClients->isClientAPlayer(client))
        {
            if (_pStatus->getWhoseTurn() == NO_TURN)
            {
                _pClients->clearPlayerType(_pClients->getClientType(client));
                _pWebsockets->sendMsgToAllClients(this->getTableData());
            }
            else
            {
                qDebug() << "ERROR: Chess::receivedMsg(): player wanted to "
                            "leave chair during game. client name:"
                          << _pClients->getClientName(client);
                return;
            }
        }
        else
        {
            qDebug() << "ERROR: Chess::receivedMsg(): non-player client"
                        " wanted to leave chair. client name:"
                     << _pClients->getClientName(client);
            return;
        }
    }
    else if (QStrWsMsgToProcess.left(3) == "im ")
    {
        QString QStrName = QStrWsMsgToProcess.mid(3);
        if (!_pClients->isClientNameExists(QStrName))
        {
            _pClients->setClientName(client, QStrName);
            this->sendDataToClient(this->getTableData());
        }
        else //double login
        {
            _pClients->getClientSocket(QStrName)->sendTextMessage("logout:doubleLogin");
            if (_pClients->isClientAPlayer(client))
                this->restartGame(ET_SOCKET_LOST, client);
            _pClients->setClientName(client, QStrName);
        }
    }
    else if (QStrWsMsgToProcess == "queueMe")
    {
        _pClients->addClientToQueue(client);
        _pWebsockets->sendMsgToAllClients(this->getTableData());
    }
    else if (QStrWsMsgToProcess == "leaveQueue")
    {
        this->removeClientFromQueue(client);
        _pWebsockets->sendMsgToAllClients(this->getTableData());
    }
    else if (QStrWsMsgToProcess == "clientLeft")
        this->removeClient(clientID);
    else qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): received unknown msg:"
                  << msg;

    _pClients->showClientsInForm();
}

void Chess::findAndSaveMoveAndSendItToTcp(QString QStrMove)
{
    if (QStrMove.left(9) == "promoteTo")
    {
        //todo: zabezpieczenia
        QString QStrPromotionType = QStrMove.right(1);
        QStrMove = _pMovements->getMove().asQStr() + QStrPromotionType;
    }
    //todo: troche nieadekwatne jest to że normalny przypadek złego ruchu czy promocji...
    //...jest tu taj traktowany jako przerwanie i nie wykonujemy całej funkcji wg nazwy
    else if (_pStatus->isMoveARequestForPromotion(QStrMove))
    {
        _pMovements->setMove(QStrMove);

        //todo: trochę chyba zmieniłem poniższą linijkę (komunikacja ws<->www)
        QString QStrMsgForActiveClient = "promoteToWhat";
        int64_t activePlayerID = _pClients->getClientID(_pClients->getPlayer(
                                                                this->getActivePlayerType()));
        this->sendDataToClient(QStrMsgForActiveClient, activePlayerID);

        return;
    }
    else if (!_pStatus->isMoveLegal(QStrMove))
    {
        this->tellPlayerThatHeGaveBadMove;
        return;
    }

    //todo: całe poniższe możnaby nazwać np. saveMove i upchać do podklasy movements
    _pMovements->setMoveType(_pMovements->findMoveType(QStrMove));
    _pMovements->setMove(QStrMove);
    switch(_pMovements->getMoveType())
    {
    case ST_REGULAR: _pMovements->regularMoveSequence(); break;
    case ST_REMOVING: _pMovements->removeMoveSequence();  break;
    case ST_ENPASSANT: _pMovements->enpassantMoveSequence(); break;
    case ST_CASTLING: _pMovements->castlingMoveSequence(); break;
    case ST_PROMOTION: _pMovements->promoteMoveSequence(); break;
    case ST_NONE:
    default:
        qDebug() << "ERROR: Chess::findAndSaveMoveAndSendItToTcp: wrong MoveType:"
                 << _pMovements->getMoveType();
        break;
    }

    this->sendMsgToTcp("move " + QStrMove);
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


