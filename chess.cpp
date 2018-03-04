#include "chess.h"

Chess::Chess(Clients* pClientsList, Dobot* pDobot, PieceController* pPieceController,
             Chessboard *pBoardRealMain, Chessboard *pBoardRealRemoved,
             Chessboard* pBoardCoreMain, Chessboard* pBoardCoreRemoved, Chessboard* pBoardChenard,
             Websockets* pWebsockets, TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource)
{
    _pClientsList = pClientsList;
    _pDobot = pDobot;
    _pPieceController = pPieceController;
    _pBoardRealMain = pBoardRealMain;
    _pBoardRealRemoved = pBoardRealRemoved;
    _pBoardCoreMain = pBoardCoreMain;
    _pBoardCoreRemoved = pBoardCoreRemoved;
    _pBoardChenard = pBoardChenard;
    _pWebsockets = pWebsockets;
    _pTCPMsgs = pTCPMsgs;
    _pUsb = _pDobot->getArduinoPointer();

    _pBot = new ChessBot();
    _pStatus = new ChessStatus(_pPieceController, _pBoardCoreMain, _pClientsList);
    _pMovements = new ChessMovements(_pPieceController, _pBoardCoreMain, _pBoardCoreRemoved);
    _pResets = new ChessResets(_pClientsList, _pBoardCoreMain) ;
    _pTimers = new ChessTimers(_pClientsList);
    _pConditions = new ChessConditions(_pClientsList);

    _PlayerSource = PlayerSource;

    _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    _request.clear();

    connect(_pUsb, SIGNAL(msgFromUsbToChess(QString)),
            this, SLOT(checkMsgFromUsb(QString)));
    connect(_pTCPMsgs, SIGNAL(msgFromTcpToChess(QString, QString)),
            this, SLOT(checkMsgFromChenard(QString, QString)));
    connect(_pWebsockets, SIGNAL(msgFromWebsocketsToChess(QString, Client*)),
            this, SLOT(checkMsgFromWebsockets(QString, Client*)));
    connect(_pStatus, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pTimers, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pStatus, SIGNAL(showLegalMovesInUI(QStringList)),
            this, SLOT(showLegalMovesInUI(QStringList)));
    connect(_pStatus, SIGNAL(showHistoryMovesInUI(QStringList)),
            this, SLOT(showHistoryMovesInUI(QStringList)));
    connect(_pTimers, SIGNAL(timeOutStart()), this, SLOT(timeOutStart()));
    connect(_pTimers, SIGNAL(timeOutPlayer(PLAYER_TYPE)), this, SLOT(timeOutPlayer(PLAYER_TYPE)));
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

void Chess::checkMsgFromWebsockets(QString QStrMsg, Client& client)
{
    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    if (_pConditions->isClientRequestCanBeAccepted(QStrMsg, client))
    {
        _request.type = requestType(QStrMsg, SHOW_ERRORS);
        _request.param = _pConditions->extractParameter(_request.type, QStrMsg);
    }
    else
    {
        qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): client request can't be accepted";
        return;
    }

    switch(_request.type)
    {
    case RT_NEW_GAME:
        if (client == *_pClientsList->getPlayer(PT_WHITE))
            this->playerWantToStartNewGame(PT_WHITE);
        else if (client == *_pClientsList->getPlayer(PT_BLACK))
            this->playerWantToStartNewGame(PT_BLACK);
        break;
    case RT_PROMOTE_TO:
        _request.param = _pMovements->getMove().asQStr() + _request.param; // w/o break
    case RT_MOVE:
        this->manageMoveRequest(_request);
        break;
    case RT_GET_TABLE_DATA:
        this->sendDataToClient(this->getTableData(), &client);
        break;
    case RT_GIVE_UP:
        _pResets->restartGame(ET_GIVE_UP, &client);
        break;
    case RT_SIT_ON:
        _pClientsList->setPlayerType(client, playerTypeFromQStr(_request.param));
        if (_pClientsList->isGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_STAND_UP:
        _pClientsList->clearPlayerType(_pClientsList->getClientType(client));
        _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_IM:
        this->newClientName(client, _request);
        break;
    case RT_QUEUE_ME:
        _pClientsList->addClientToQueue(client);
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_LEAVE_QUEUE:
        _pClientsList->removeClientFromQueue(client);
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_CLIENT_LEFT:
        this->removeClient(client);
        break;
    default:
        qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): received _request.type:"
                          << _request.type;
    }

    _pClientsList->showClientsInUI();
}

void Chess::checkMsgFromChenard(QString QStrTcpMsgType, QString QStrTcpRespond)
{
    qDebug() << "Chess::checkMsgFromChenard(): QStrTcpMsgType=" << QStrTcpMsgType
             << ", QStrTcpRespond:" << QStrTcpRespond;

    if(_PlayerSource == ARDUINO)
        _pBot->checkAI();

    CHENARD_MSG_TYPE ProcessedChenardMsgType = ChenardMsgType(QStrTcpMsgType);
    if (!isChenardAnswerCorrect(ProcessedChenardMsgType, QStrTcpMsgType, SHOW_ERRORS)) return;

    switch(ProcessedChenardMsgType)
    {
    case CMT_NEW:
        this->startNewGameInCore();
        break;
    case CMT_STATUS:
        _pStatus->saveStatusData(QStrTcpRespond);

        if (_pStatus->getFENGameState() == ET_NONE)
        {
            this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_HISTORY));
            this->continueGameplay(); //future: zrobić to kiedyś tak by dopiero w...
            //...odpowiedzi na legal core wysyłał na stronę potwiedzenie wykonania...
            //...ruchu (tj. zdjął blokadę przed kojenym ruchem)
        }
        else //ET_WHIE_WON, ET_BLACK_WON, ET_DRAW
            _pResets->restartGame(_pStatus->getFENGameState());
        break;
    case CMT_LEGAL:
        _pStatus->setLegalMoves(QStrTcpRespond);
        break;
    case CMT_HISTORY:
        _pStatus->setHistoryMoves(QStrTcpRespond);
        //todo: typy odpowiedzi na WWW też jako enumy zrobić z parametrami
        this->sendDataToAllClients("history " + _pStatus->getHistoryMovesAsQStr());
        this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_LEGAL));
        break;
    case CMT_MOVE:
        this->continueGameplay();
        break;
    case CMT_THINK:
        if (_PlayerSource == ARDUINO)
            _pBot->undoOk();
        else qDebug() << "ERROR: Chess::checkMsgFromChenard(): ProcessedChenardMsgType = "
                         "CMT_THINK, but _PlayerSource != ARDUINO";
        break;
    case CMT_UNDO:
        if (_PlayerSource == ARDUINO)
            _pBot->thinkOk(QStrTcpRespond); //f.e.: QStrTcpRespond = "OK d1h5 Qh5#"
        else qDebug() << "ERROR: Chess::checkMsgFromChenard(): ProcessedChenardMsgType = "
                         "CMT_UNDO, but _PlayerSource != ARDUINO";
        break;
    default:
        qDebug() << "ERROR: Chess:checkMsgFromChenard(): unknown ProcessedChenardMsgType:"
                  << ProcessedChenardMsgType;
    }
}

void Chess::checkMsgFromUsb(QString QStrMsg)
{ //future:
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

void Chess::playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService /* = false*/)
{
    if (PlayerType == PT_WHITE)
    {
        qDebug() << "Chess::playerWantToStartNewGame(): white";
        _pClientsList->setClientStartConfirmation(PT_WHITE, true);
    }
    else if (PlayerType == PT_BLACK)
    {
        _pClientsList->setClientStartConfirmation(PT_BLACK, true);
        qDebug() << "Chess::playerWantToStartNewGame(): black";
    }
    else if (_PlayerSource == ARDUINO)
        qDebug() << "Chess::playerWantToStartNewGame()";
    else
        qDebug() << "ERROR: Chess::playerWantToStartNewGame(): unknown playerWantToStartNewGame"
                    " val:" << playerTypeAsQStr(PlayerType);

    if ((_pClientsList->isGameTableOccupied() && _pClientsList->isStartClickedByBothPlayers())
            || _PlayerSource == ARDUINO || bService)
    {
        qDebug() << "Chess::playerWantToStartNewGame(): both have clicked start. "
                    "Try to start a game";
        _pTimers->stopQueueTimer();
        this->sendMsgToTcp("new");
        _pClientsList->setClientStartConfirmation(PT_WHITE, false);
        _pClientsList->setClientStartConfirmation(PT_BLACK, false);
    }
}

void Chess::tellPlayerThatHeGaveBadMove(QString QStrMsg)
{
    qDebug() << "Chess::tellPlayerThatHeGaveBadMove():" << QStrMsg;
    QString QStrMsgForActivePlayer = "badMove " + QStrMsg + " "
            + turnTypeAsQstr(_pStatus->getWhoseTurn());
    Client* pPlayer = _pClientsList->getPlayer(_pStatus->getActivePlayerType());

    this->sendDataToClient(QStrMsgForActivePlayer, pPlayer);
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp:" + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(_PlayerSource, QStrMsg);
}

void Chess::newClientName(Client& client, clientRequest request)
{
    if (!_pClientsList->isClientNameExists(request.param))
    {
        _pClientsList->setClientName(client, request.param);
        this->sendDataToClient(this->getTableData(), &client);
    }
    else //double login
    {
        _pClientsList->getClientSocket(request.param)->sendTextMessage("logout:doubleLogin");
        if (_pClientsList->isClientAPlayer(client))
            _pResets->restartGame(ET_SOCKET_LOST, &client);
        _pClientsList->setClientName(client, request.param);
    }
}

void Chess::removeClient(Client& client)
{
    if (_pClientsList->isClientAPlayer(client))
    {
        emit this->addTextToLogPTE(playerTypeAsQStr(client.type)
                                   + " player disconnected\n", LOG_CORE);
        _pResets->restartGame(ET_SOCKET_LOST, &client);
    }
    else //future: strange behavior with those info- seen more often then we should
        emit this->addTextToLogPTE("non-player disconnected\n", LOG_CORE);

    _pClientsList->removeClient(client);

    emit this->setBoardDataLabel(std::to_string(_pClientsList->getClientsList().size()).c_str(),
                            BDL_SOCKETS_ONLINE);
    emit _pClientsList->showClientsList(_pClientsList->getClientsList());
}

void Chess::sendDataToClient(QString QStrMsg, Client* pClient /* = nullptr*/)
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

void Chess::coreIsReadyForNewGame() //future: taka sobie ta nazwa?
{
    qDebug() << "Chess::coreIsReadyForNewGame()";

    if (_PlayerSource == WEBSITE)
    {
        if (_pClientsList->isGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

        this->sendDataToAllClients("resetComplited"); //don't add tableData- only chess data...
        //...has changed during reset
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: nie startować automatycznie nowej gry? lepiej wymagać od gracza potwierdzenia
        this->sendMsgToTcp("new");
    }
    else
        qDebug() << "ERROR: Chess::coreIsReadyForNewGame(): unknown _PlayerSource val ="
                  << communicationTypeAsQStr(_PlayerSource);
}

void Chess::startNewGameInCore()
{
    emit this->addTextToLogPTE("new game\n", LOG_CORE);

    _ChessGameStatus = GS_TURN_WHITE_FIRST_TURN;

    //pierwszy legal i status wyglądają zawsze tak samo:
    _pStatus->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    _pStatus->setLegalMoves("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 "
                            "d2d4 e2e3 e2e4 f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");

    _pTimers->resetGameTimers();
    _pTimers->startGameTimer();
    _pClientsList->resetPlayersStartConfirmInfo();

    this->sendDataToAllClients("newOk");
}

void Chess::manageMoveRequest(clientRequest request)
{
    if (_pStatus->isMoveARequestForPromotion(request.param))
    {
        _pMovements->setMove(request.param);
        _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE_PROMOTE :
                                                                    GS_TURN_BLACK_PROMOTE;
        this->sendDataToClient("promoteToWhat", _pClientsList->getPlayer(
                                   _pStatus->getActivePlayerType()));
    }
    else if (!_pStatus->isMoveLegal(request.param))
        this->tellPlayerThatHeGaveBadMove(request.param);
    else
    {
        _pStatus->setMove(request.param); //no need for earlier clear
        if (this->isMoveOkForCoreBoards(_pStatus->getMove(), _pStatus->getMoveType()))
            _pMovements->doMoveSequence(_pStatus->getMove(), _pStatus->getMoveType());
        else return;
        this->sendMsgToTcp("move " + request.param);
    }
}

void Chess::continueGameplay()
{
    if (_PlayerSource == WEBSITE)
    {
        _pTimers->switchPlayersTimers(_pStatus->getWhoseTurn());
        _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE :
                                                                    GS_TURN_BLACK;
        this->sendDataToAllClients("moveOk " + _pMovements->getMove().asQStr() + " " +
                                   turnTypeAsQstr(_pStatus->getWhoseTurn()) + " continue");
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: to kto wysłał ruch (gracz/bot) powinno być kontrolowane przed, a nie po akcji
        //future: zrobić kiedyś diagram działań z arduino i przemyśleć ułozenie kodu
        if (!_pBot->getAI()) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
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

void Chess::restartGame(END_TYPE WhoWon, Client* pPlayerToClear /*= nullptr*/)
{
    //info
    QString QStrPlayer = "";
    if (pPlayerToClear != nullptr)
        QStrPlayer = _pClientsList->getClientName(*pPlayerToClear) + ":";
    qDebug() << "Chess::restartGame():" << QStrPlayer << endTypeAsQstr(WhoWon);

    _ChessGameStatus = GS_TURN_NONE_RESETING;

    //reset data
    _pClientsList->resetPlayersStartConfirmInfo();
    _pTimers->resetGameTimers();
    _pMovements->clearMove();
    _pStatus->resetStatusData();

    _pResets->changePlayersOnChairs(WhoWon, pPlayerToClear);
    this->sendDataToAllClients(_pResets->getEndGameMsg(WhoWon, this->getTableData()));
    //todo: wygląda na to że funkcja resetPiecePositions załącza się jeszcze zanim odpowiedź
    //poleci na. stronę, przez co trzeba czekać aż resetowanie się zakończy zanim gracze się
    //dowiedzą że nastąpił koniec gry
    _pResets->resetPiecePositions();
}

bool Chess::isMoveOkForCoreBoards(PosFromTo PosMove, SEQUENCE_TYPE Type)
{
    //wykonaj fikcyjny ruch
    //jezeli nie było problemów to zwróć true
}

QString Chess::getTableData()
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt",
    //"wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + _pClientsList->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + _pClientsList->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" +  turnTypeAsQstr(_pStatus->getWhoseTurn()) +
            "\",\"wtime\":" + QString::number(_pTimers->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pTimers->getBlackTimeLeft()) +
            ",\"queue\":\"" + _pClientsList->getQueuedClientsList();

    if (_pTimers->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart =
                _pClientsList->isStartClickedByPlayer(PT_WHITE) ? "w" : "x";
        QString QStrBlackClickedStart =
                _pClientsList->isStartClickedByPlayer(PT_BLACK) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pTimers->getStartTimeLeft());
    }

    if (!_pStatus->getHistoryMoves().isEmpty())
        QStrTableData += "\",\"history\":\"" + _pStatus->getHistoryMovesAsQStr();

    QStrTableData += "\"}";

    qDebug() << "Chess::getTableData(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Chess::timeOutStart()
{
    _pClientsList->resetPlayersStartConfirmInfo();

    if (!_pClientsList->isStartClickedByPlayer(PT_WHITE))
        _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
    if (!_pClientsList->isStartClickedByPlayer(PT_BLACK))
        _pClientsList->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);

    if (_pClientsList->isGameTableOccupied())
        _ChessGameStatus = _pTimers->startQueueTimer();
    else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    this->sendDataToAllClients(this->getTableData());
}

void Chess::timeOutPlayer(PLAYER_TYPE Player)
{
    _pResets->restartGame(ET_TIMEOUT_GAME, _pClientsList->getPlayer(Player));
}

void Chess::setBoardDataLabel(QString QStrLabel, BOARD_DATA_LABEL LabelType)
{
    emit this->setBoardDataLabel(QStrLabel, LabelType);
}
