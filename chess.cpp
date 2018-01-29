#include "chess.h"

Chess::Chess(Clients *pClients, Dobot *pDobot, Chessboard *pBoardMain,
             Chessboard *pBoardRemoved, ArduinoUsb *pUsb, Websockets* pWebsockets,
             TCPMsgs *pTCPMsgs, COMMUNICATION_TYPE PlayerSource)
{
    _pTimers = new ChessTimers(*this);
    _pMovements = new ChessMovements(*this);
    _pBot = new ChessBot(*this);
    _pStatus = new ChessStatus(*this);
    _pResets = new ChessResets(*this) ;
    _pConditions = new ChessConditions(*this);

    _pClients = pClients;
    _pDobot = pDobot;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pWebsockets = pWebsockets;
    _pTCPMsgs = pTCPMsgs;
    _pUsb = pUsb;

    _PlayerSource = PlayerSource;

    _ChessGameStatus = GS_ERROR;
    _request.clear();

    for (int i=1; i>=32; ++i)
        *_pPiece[i] = new Piece(i);

    connect(_pUsb, msgFromUsbToChess(QString), this, checkMsgFromUsb(QString));
    connect(_pTCPMsgs, msgFromTcpToChess(QString, QString),
            this, checkMsgFromChenard(QString, QString));
    connect(_pWebsockets, msgFromWebsocketsToChess(QString, Client*),
            this, checkMsgFromWebsockets(QString, Client*));
}

Chess::~Chess()
{
    delete _pTimers;
    delete _pMovements;
    delete _pBot;
    delete _pStatus;
    delete _pResets;
    delete _pConditions;
}

void Chess::sendDataToClient(QString QStrMsg, Client* pClient = nullptr)
{
    emit this->addTextToLogPTE("Sending msg to " + communicationTypeAsQStr(_PlayerSource)
                                + ": " + QStrMsg, LOG_CORE);

    if (_PlayerSource == WEBSITE)
    {
        if (pClient == nullptr)
        {
            qDebug() << "ERROR: Chess::sendDataToClient(): client can't be null "
                        "with _PlayerSource == WEBSITE";
            return;
        }
        _pWebsockets->sendMsgToClient(QStrMsg, pClient);
    }
    else if (_PlayerSource == ARDUINO)
    {
        if (pClient != nullptr)
        {
            qDebug() << "ERROR: Chess::sendDataToClient(): client must be null "
                        "with _PlayerSource == ARDUINO";
            return;
        }
        //future: nieujednolicone typy komunikatów web z arduino są, przez co...
        //...takie dziwne zmiany w locie (wynika to z virtualow i dziedziwczenia)
        if (QStrMsg.contains("promote")) QStrMsg = "promote";
        else if (QStrMsg.contains("newOk")) QStrMsg = "started";
        else if (QStrMsg.contains("badMove")) QStrMsg = "BAD_MOVE";

        _pUsb->sendDataToUsb(QStrMsg);
    }
    else
        qDebug() << "ERROR: Chess::sendDataToClient(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::sendDataToAllClients(QString QStrMsg)
{
    if (_PlayerSource == WEBSITE)
        _pWebsockets->sendMsgToAllClients(QStrMsg);
    else
        qDebug() << "ERROR: Chess::sendDataToAllClients(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::continueGameplay()
{
    if (_PlayerSource == WEBSITE)
    {
        _pTimers->switchPlayersTimers();
        this->sendDataToAllClients("moveOk " + _pMovements->getMove().asQStr() + " " +
                                   turnTypeAsQstr(_pStatus->getWhoseTurn()) + " continue");
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: to kto wysłał ruch (gracz/bot) powinno być kontrolowane przed, a nie po akcji
        //future: zrobić kiedyś diagram działań z arduino i przemyśleć ułozenie kodu
        if (!_pBot->getAI) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
        {
            if (_pBot->getAIAsPlayer2()) //future: powinno być zawsze true dla arduino
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
        qDebug() << "ERROR: Chess::continueGameplay(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);

    this->sendMsgToTcp("status");
}

void Chess::startNewGameInChenard(bool bService = false)
{
    if (_pClients->isGameTableOccupied && _pClients->isStartClickedByBothPlayers()
            || _PlayerSource == ARDUINO || bService)
    {
        this->sendMsgToTcp("new");
    }
    else
        qDebug() << "ERROR: Chess::startNewGameInChenard(): game couldn't start";
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp:" + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(_PlayerSource, QStrMsg);
}

void Chess::startNewGameInCore()
{
    emit this->addTextToLogPTE("new game\n", LOG_CORE);

    //pierwszy legal i status wyglądają zawsze tak samo:
    _pStatus->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    _pStatus->setLegalMoves("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 "
                            "d2d4 e2e3 e2e4 f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");

    _pTimers->resetGameTimers();
    _pTimers->startGameTimer();
    _pClients->resetPlayersStartConfirmInfo();

    this->sendDataToAllClients("newOk");
}

void Chess::tellPlayerThatHeGaveBadMove(QString QStrMsg)
{
    qDebug() << "Chess::tellPlayerThatHeGaveBadMove():" << QStrMsg;
    QString QStrMsgForActivePlayer = "badMove " + QStrMsg + " "
            + turnTypeAsQstr(_pStatus->getWhoseTurn());
    Client* pPlayer = _pClients->getPlayer(this->getActivePlayerType());
    this->sendDataToClient(QStrMsgForActivePlayer, QStrMsg);
}

bool Chess::isPieceSetOk()
{
    for (short sPiece=1; sPiece>=32; ++sPiece)
    {
        bool bPieceExists = false;
        for (short sField=1; sField>=64; ++sField)
        {
            if (_pBoardMain->getField(sField)->getPieceOnField() == _pPiece[sPiece] ||
                    _pBoardRemoved->getField(sField)->getPieceOnField() == _pPiece[sPiece] ||
                    _pDobot->getItemInGripper() == sPiece)
            {
                bPieceExists = true;
                break;
            }
        }
        if (!bPieceExists)
        {
            qDebug() << "ERROR: Chess::isPieceSetOk(): it isn't, missing piece nr:" << sPiece;
            return false;
        }
    }
    return true;
}

bool Chess::isPieceStayOnItsStartingField(Piece* pPiece)
{
    if (pPiece == nullptr)
    {
        qDebug() << "ERROR: Chess::isPieceStayOnItsStartingField(): piece can't be nullptr";
        return false;
    }

    Field* pStartFieldOfPiece = _pBoardMain->getFieldWithGivenPieceIfExists(pPiece);
    if (pPiece == pStartFieldOfPiece->getPieceOnField())
        return true;
    else return false;
}

Field* Chess::searchForPieceActualFieldOnMainBoard(Piece* pPiece)
{
    if (pPiece == nullptr)
    {
        qDebug() << "ERROR: Chess::searchForPieceActualFieldOnMainBoard(): piece == nullptr";
        return nullptr;
    }

    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = _pBoardMain->getField(sField);
        if (pPiece == pField->getPieceOnField())
            return pField;
    }

    return nullptr;
}

void Chess::removeClient(Client* pClient)
{
    if (_pClients->isClientAPlayer(pClient))
        _pResets->restartGame(ET_SOCKET_LOST, pClient);
    else //future: strange behavior with those info- seen more often then we should
        emit this->addTextToLogPTE("non-player disconnected\n", LOG_CORE);

    _pClients->removeClient(pClient);

    emit this->setBoardDataLabel(std::to_string(_pClients->getClientsList().size()).c_str(),
                            BDL_SOCKETS_ONLINE);
    emit _pClients->showClientsList(_pClients->getClientsList());
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
                    turnTypeAsQstr(_pStatus->getWhoseTurn());
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

void Chess::coreIsReadyForNewGame()
{
    qDebug() << "Chess::coreIsReadyForNewGame()";

    if (_PlayerSource == WEBSITE)
    {
        if (_pClients->isGameTableOccupied())
            _pTimers->startQueueTimer();

        this->sendDataToAllClients("resetComplited");
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: nie startować automatycznie nowej gry? lepiej wymagać od gracza potwierdzenia
        this->startNewGameInChenard();
    }
    else
        qDebug() << "ERROR: Chess::coreIsReadyForNewGame(): unknown _PlayerSource val ="
                  << communicationTypeAsQStr(_PlayerSource);
}

void Chess::playerWantToStartNewGame(PLAYER_TYPE playerType)
{
    if (playerType == PT_WHITE)
    {
        qDebug() << "Chess::playerWantToStartNewGame(): white";
        _pClients->setClientState(PT_WHITE, true);
    }
    else if (playerType == PT_BLACK)
    {
        _pClients->setClientState(PT_BLACK, true);
        qDebug() << "Chess::playerWantToStartNewGame(): black";
    }
    else if (_PlayerSource == ARDUINO)
        qDebug() << "Chess::playerWantToStartNewGame()";
    else
        qDebug() << "ERROR: Chess::playerWantToStartNewGame(): unknown playerWantToStartNewGame "
                     "val:" << playerTypeAsQStr(playerType);

    if (_pClients->isGameTableOccupied && _pClients->isStartClickedByBothPlayers()
            || _PlayerSource == ARDUINO || bService)
    {
        qDebug() << "Chess::playerWantToStartNewGame(): both have clicked start. "
                    "Try to start a game";
        _pTimers->stopQueueTimer();
        this->startNewGameInChenard();
        _pClients->setClientState(PT_WHITE, false);
        _pClients->setClientState(PT_BLACK, false);
    }
}

//todo: pociąć tą funkcję na mniejsze kawałki (przerobić ją podobnie jak checkMsgFromWebsockets
void Chess::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    qDebug() << "Chess::checkMsgFromChenard(): tcpMsgType=" << tcpMsgType <<
                ", tcpRespond:" << tcpRespond;

    if(_PlayerSource == ARDUINO)
        this->checkAI();

    //future: zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są...
    //...czasami 2 warunki typu 'ok'
    if (tcpMsgType == "new")
    {
        if (tcpRespond == "OK\n" || tcpRespond == "OK")            
            this->startNewGameInCore();
        else
            qDebug() << "ERROR: Chess::checkMsgFromChenard(): unknown tcpRespond="
                     << tcpRespond << "for tcpMsgType =" << tcpMsgType;
    }
    else if (tcpMsgType.left(4) == "move")
    {
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1")
            this->continueGameplay();
        else if (tcpRespond == "BAD_MOVE")
            this->tellPlayerThatHeGaveBadMove(tcpRespond);
        else
            qDebug() << "ERROR: Chess::checkMsgFromChenard(): unknown tcpRespond="
                     << tcpRespond << "for tcpMsgType =" << tcpMsgType;
    }
    else if (tcpMsgType == "status")
    {
        _pStatus->saveStatusData(tcpRespond);

        if (_pStatus->getFENGameState() == ET_NONE)
        {
            this->sendMsgToTcp("history pgn");
            this->continueGameplay(); //future: zrobić to kiedyś tak by dopiero w odpowiedzi...
            //...na legal core wysyłał na stronę potwiedzenie wykonania ruchu (tj. zdjął...
            //...blokadę przed kojenym ruchem)
        }
        else if (_pStatus->getFENGameState() == ET_WHIE_WON ||
                 _pStatus->getFENGameState() == ET_BLACK_WON ||
                 _pStatus->getFENGameState() == ET_DRAW)
        {
            _pResets->restartGame(_pStatus->getFENGameStateAsEndType());
        }
        else
            qDebug() << "ERROR: Chess::checkMsgFromChenard(): unknown tcpRespond="
                     << _pStatus->getFENGameState() << "for tcpMsgType =" << tcpMsgType;
    }
    else if (tcpMsgType.left(7) == "history" && tcpRespond.left(3) == "OK ")
    {
        _pStatus->historyOk(tcpRespond);
        this->sendDataToAllClients("history " + _pStatus->getHisotyMovesAsQStr());
        this->sendMsgToTcp("legal");
    }
    else if (tcpMsgType == "legal" && tcpRespond.left(3) == "OK ")
        _pStatus->setLegalMoves(tcpRespond);
    else if (tcpMsgType == "undo 1" && _PlayerSource == ARDUINO &&
             (tcpRespond == "OK\n" || tcpRespond == "OK"))
        _pBot->undoOk();
    else if (tcpMsgType == "think 5000" && _PlayerSource == ARDUINO &&
             (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1"))
        _pBot->thinkOk(tcpRespond); //"f.e.: OK d1h5 Qh5#"
    else
        qDebug() << "ERROR: Chess:checkMsgFromChenard(): received unknown tcpMsgType:"
                  << tcpMsgType;
}

//todo: funkcja ma 120 linii kodu...
void Chess::checkMsgFromWebsockets(QString QStrMsg, Client* pClient)
{
    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    if (_pConditions->isClientRequestCanBeAccepted(QStrMsg, pClient))
    {
        _request.type = requestType(QStrMsg, SHOW_ERRORS);
        _request.parameter = _pConditions->extractParameter(_request.type, QStrMsg);
    }
    else
    {
        qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): client request cannot be accepted";
        return;
    }

    if (QStrMsg == "newGame")
    {
        PLAYER_TYPE PlayerType = PT_NONE;
        if (pClient == _pClients->getPlayer(PT_WHITE))
            PlayerType = PT_WHITE;
        else if (pClient == _pClients->getPlayer(PT_BLACK))
            PlayerType = PT_BLACK;

        this->playerWantToStartNewGame(PlayerType);
    }
    else if (QStrMsg.left(4) == "move") this->findAndSaveMoveAndSendItToTcp(QStrMsg.mid(5));
    else if (QStrMsg.left(9) == "promoteTo") this->findAndSaveMoveAndSendItToTcp(QStrMsg);
    else if (QStrMsg == "getTableData") this->sendDataToClient(this->getTableData(), pClient);
    else if (QStrMsg == "giveUp")
    {
        if (_pClients->isClientAPlayer(pClient))
            _pResets->restartGame(ET_GIVE_UP, pClient);
        else
            qDebug() << "ERROR: Chess::receivedMsg(): non-player tried to logout (hacker?)";
    }
    else if (QStrMsg.left(5) == "sitOn")
    {
        if (_pClients->getClientName(pClient) == "-1")
        {
            qDebug() << "ERROR: Chess::receivedMsg(): sitOn: client not loggedIn";
            return;
        }

        PLAYER_TYPE PlayerChair = playerTypeFromQStr(QStrMsg.mid(5));

        _pStatus->setWhoseTurn(NO_TURN);
        if (!_pClients->isPlayerChairEmpty(PlayerChair))
        {
            qDebug() << "ERROR: Chess::receivedMsg(): client wanted to "
                        "sit on occupied chair. client name:"
                     << _pClients->getClientName(pClient) << ", player name:" <<
                        _pClients->getPlayerName(PlayerChair);
            return;
        }
        else
            _pClients->setPlayerType(pClient, PlayerChair);

        if (this->isGameTableOccupied())
            _pTimers->startQueueTimer();

        this->sendDataToAllClients(this->getTableData());
    }
    else if (QStrMsg == "standUp")
    {
        if (_pClients->isClientAPlayer(pClient))
        {
            if (_pStatus->getWhoseTurn() == NO_TURN)
            {
                _pClients->clearPlayerType(_pClients->getClientType(pClient));
                this->sendDataToAllClients(this->getTableData());
            }
            else
            {
                qDebug() << "ERROR: Chess::receivedMsg(): player wanted to "
                            "leave chair during game. client name:"
                          << _pClients->getClientName(pClient);
                return;
            }
        }
        else
        {
            qDebug() << "ERROR: Chess::receivedMsg(): non-player client"
                        " wanted to leave chair. client name:"
                     << _pClients->getClientName(pClient);
            return;
        }
    }
    else if (QStrMsg.left(3) == "im ")
    {
        QString QStrName = QStrMsg.mid(3);
        if (!_pClients->isClientNameExists(QStrName))
        {
            _pClients->setClientName(pClient, QStrName);
            this->sendDataToClient(this->getTableData(), pClient);
        }
        else //double login
        {
            _pClients->getClientSocket(QStrName)->sendTextMessage("logout:doubleLogin");
            if (_pClients->isClientAPlayer(pClient))
                _pResets->restartGame(ET_SOCKET_LOST, pClient);
            _pClients->setClientName(pClient, QStrName);
        }
    }
    else if (QStrMsg == "queueMe")
    {
        _pClients->addClientToQueue(pClient);
        this->sendDataToAllClients(this->getTableData());
    }
    else if (QStrMsg == "leaveQueue")
    {
        this->removeClientFromQueue(pClient);
        this->sendDataToAllClients(this->getTableData());
    }
    else if (QStrMsg == "clientLeft") this->removeClient(pClient);
    else qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): received unknown msg:"
                  << QStrMsg;

    _pClients->showClientsInForm();
}

void Chess::checkMsgFromUsb(QString QStrMsg)
{
    //future:
    if (QStrMsg == "start") //queue to tcp msg "think 5000"
        qDebug() << "Chess::checkMsgFromUsb(): msg =" << QStrMsg;
    else if (QStrMsg ==  "move")
        qDebug() << "Chess::checkMsgFromUsb(): msg =" << QStrMsg;
    else if (QStrMsg ==  "reset") //resetPiecePositions()
        qDebug() << "Chess::checkMsgFromUsb(): msg =" << QStrMsg;
    else if (QStrMsg ==  "promoteTo")
        qDebug() << "Chess::checkMsgFromUsb(): msg =" << QStrMsg;
    else
        qDebug() << "ERROR: Chess::checkMsgFromUsb(): unknown msg =" << QStrMsg;
}

void Chess::findAndSaveMoveAndSendItToTcp(QString QStrMove)
{
    //todo: czy wykonywane jest gdzieś wcześniej sprawdzenie PosFromTo::isMoveInProperFormat()?
    if (_pStatus->getWhoseTurn() == NO_TURN)
    {
        qDebug() << "ERROR: Chess::findAndSaveMoveAndSendItToTcp(): turn == 0 (game is off)";
        return;
    }

    if (QStrMove.left(9) == "promoteTo")
    {
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
        Client* pClient = _pClients->getPlayer(this->getActivePlayerType());
        this->sendDataToClient(QStrMsgForActiveClient, pClient);

        return;
    }
    else if (!_pStatus->isMoveLegal(QStrMove))
    {
        this->tellPlayerThatHeGaveBadMove;
        return;
    }

    _pMovements->setMove(QStrMove); //no need for earlier clear
    _pMovements->doMoveSequence();
    this->sendMsgToTcp("move " + QStrMove);
}

void Chess::movePieceWithManipulator(Chessboard* pRealBoard, Field* pField,
                                              VERTICAL_MOVE vertMove = VM_NONE)
{    
    if (!Chessboard::isBoardReal(pRealBoard->getBoardType()), true) return;

    if (vertMove == VM_GRAB)
    {
        if (!this->isPieceSetOk()) return;
        if (pField->getPieceOnField(true) == nullptr) return;

        emit this->addTextToLogPTE("Queue: grab piece " + pField->getPieceOnField()->getNr() +
                                   " on " + boardTypeAsQstr(pRealBoard->getBoardType()) +
                                   " from field " + pField->getNrAsQStr(), LOG_CORE);
        _pDobot->setItemInGripper(pField->getPieceOnField()->getNr());
        pRealBoard->clearField(pField);

        if (!this->isPieceSetOk()) return;
    }
    else if (vertMove == VM_PUT)
    {
        if (!this->isPieceSetOk()) return;

        emit this->addTextToLogPTE("Queue: put piece " + _pPiece[_pDobot->getItemInGripper()] +
                                   " on " + boardTypeAsQstr(pRealBoard->getBoardType()) +
                                   " on field " + pField->getNrAsQStr(), LOG_CORE);
        pRealBoard->setPieceOnField(_pPiece[_pDobot->getItemInGripper()], pField);
        _pDobot->clearGripper();

        if (!this->isPieceSetOk()) return;
    }

    Point3D xyz = pField->getLocation3D();
    _pDobot->doMoveSequence(xyz, vertMove, Piece::dMaxPieceHeight);
}
