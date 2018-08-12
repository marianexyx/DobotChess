#include "chess.h"

Chess::Chess(PieceController* pPieceController)
{
    _pPieceController = pPieceController;
    _pDobot = _pPieceController->getDobotPointer();
    _pBoardMain = _pPieceController->getBoardMainPointer();
    _pBoardRemoved = _pPieceController->getBoardRemovedPointer();
    _pBoardChenard = new Chessboard(B_MAIN, IMAGINARY);
    _pWebsockets = new Websockets();
    _pClientsList = _pWebsockets->getClientsListPointer();
    _pTCPMsgs = new TCPMsgs();

    _pStatus = new ChessStatus(_pPieceController, _pBoardMain, _pClientsList);
    _pMovements = new ChessMovements(_pPieceController, _pBoardMain, _pBoardRemoved);
    _pTimers = new ChessTimers(_pClientsList);
    _pConditions = new ChessConditions(_pClientsList, _pStatus);

    _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    _request.clear();

    connect(_pTCPMsgs, SIGNAL(msgFromTcpToChess(QString, QString)),
            this, SLOT(checkMsgFromChenard(QString, QString)));
    connect(_pWebsockets, SIGNAL(msgFromWebsocketsToChess(QString, int64_t)),
            this, SLOT(checkMsgFromWebsockets(QString, int64_t)));
    connect(_pStatus, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabelInUI(QString, BOARD_DATA_LABEL)));
    connect(_pTimers, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabelInUI(QString, BOARD_DATA_LABEL)));
    connect(_pStatus, SIGNAL(showLegalMoves(QStringList)),
            this, SLOT(showLegalMovesInForm(QStringList)));
    connect(_pStatus, SIGNAL(showHistoryMoves(QStringList)),
            this, SLOT(showHistoryMovesInForm(QStringList)));
    connect(_pTimers, SIGNAL(timeOutStart()), this, SLOT(timeOutStart()));
    connect(_pTimers, SIGNAL(timeOutPlayer(PLAYER_TYPE)),
            this, SLOT(timeOutPlayer(PLAYER_TYPE)));
}

Chess::~Chess()
{
    delete _pTimers;
    delete _pMovements;
    delete _pStatus;
    delete _pConditions;
}

QString Chess::dumpAllData()
{
    QString QStrData;

    QStrData = "[chess.h]\n";
    QStrData += "_ChessGameStatus: " + gameStatusAsQStr(_ChessGameStatus) + "\n";
    QStrData += "_request.type: " + requestTypeAsQStr(_request.type) + "\n";
    QStrData += "_request.param: " + _request.param + "\n";
    QStrData += "\n";
    QStrData += _pStatus->dumpAllData();
    QStrData += "\n";
    QStrData += _pTimers->dumpAllData();

    return QStrData;
}

void Chess::checkMsgFromWebsockets(QString QStrMsg, int64_t n64SenderID,
                                   bool bService /*= false*/)
{
    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    Client client = _pClientsList->getClient(n64SenderID);
    REJECTED_REQUEST_REACTION BadRequestFromClient = RRR_NONE;

    if (_pConditions->isClientRequestCanBeAccepted(QStrMsg, &client, _ChessGameStatus,
                                                   BadRequestFromClient) || bService)
    {
        _request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
        _request.param = _pConditions->extractParameterIfTypeIsInProperFormat(_request.type,
                                                                              QStrMsg);
        qInfo() << "accepted client request type =" << requestTypeAsQStr(_request.type)
                << ", param =" << _request.param << ", rejectedRequestReaction type ="
                << rejectedRequestReactionAsQStr(BadRequestFromClient);
    }
    else
    {
        if (BadRequestFromClient != RRR_NONE)
        {
            //BadRequestFromClient was set up in isClientRequestCanBeAccepted()- passed by &(ref)
            if (BadRequestFromClient == RRR_RESEND_TABLE_DATA)
                this->sendDataToClient(client);
            else if (BadRequestFromClient == RRR_REMOVE_AND_REFRESH_CLIENT)
                ;
        }
        qWarning() << "client request can't be accepted"; //todo: react somehow on player
        return;
    }

    switch(_request.type)
    {
    case RT_NEW_GAME:
        if (client == _pClientsList->getPlayer(PT_WHITE))
            this->playerWantToStartNewGame(PT_WHITE);
        else if (client == _pClientsList->getPlayer(PT_BLACK))
            this->playerWantToStartNewGame(PT_BLACK);
        else if (bService)
            this->playerWantToStartNewGame(PT_NONE, bService);
        else qCritical() << "switch: RT_NEW_GAME: client isn't a player";
        break;
    case RT_PROMOTE_TO:
        _request.param = _pStatus->getMove().asQStr() + _request.param;
        _pStatus->promotePawn(_pStatus->getMove().from, _request.param.right(1));
        //without break;
    case RT_MOVE:
        this->manageMoveRequest(_request);
        break;
    case RT_GET_TABLE_DATA:
        this->sendDataToClient(client);
        break;
    case RT_SIT_ON:
    {
        PLAYER_TYPE PT = playerTypeFromQStr(_request.param);
        _pClientsList->setPlayerType(client, PT);
        if (_pClientsList->isWholeGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        this->sendDataToAllClients(PT == PT_WHITE ? AT_NEW_WHITE_PLAYER : AT_NEW_BLACK_PLAYER);
        break;
    }
    case RT_STAND_UP:
        this->playerLeftChair(client.type());
        break;
    case RT_IM:
        //todo:
        this->newClientLogged(client, _request.param.left(_request.param.indexOf("&")).toInt());
        break;
    case RT_QUEUE_ME:
        _pClientsList->addClientToQueue(client);
        this->sendDataToAllClients();
        break;
    case RT_LEAVE_QUEUE:
        _pClientsList->removeClientFromQueue(client);
        this->sendDataToAllClients();
        break;
    case RT_CLIENT_LEFT:
        this->restorateGameIfDisconnectedClientAffectIt(client);
        _pClientsList->removeClientFromList(client);
        this->updateClientsInUI();
        break;
    default:
        qCritical() << "received _request.type:" << QString::number(_request.type);
    }

    _pClientsList->showClientsInUI();
}

void Chess::checkMsgFromChenard(QString QStrTcpMsgType, QString QStrTcpRespond)
{
    qInfo() << "QStrTcpMsgType=" << QStrTcpMsgType << ", QStrTcpRespond:" << QStrTcpRespond;
    CHENARD_MSG_TYPE ProcessedChenardMsgType = ChenardMsgType(QStrTcpMsgType);
    qInfo() << "ProcessedChenardMsgType =" << chenardMsgTypeAsQStr(ProcessedChenardMsgType);
    if (!isChenardAnswerCorrect(ProcessedChenardMsgType, QStrTcpRespond, SHOW_ERRORS)) return;

    switch(ProcessedChenardMsgType)
    {
    case CMT_NEW:
        this->startNewGameInCore();
        break;
    case CMT_MOVE:
        this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_STATUS));
        break;
    case CMT_STATUS:
    {
        _pStatus->saveStatusData(QStrTcpRespond);
        END_TYPE ET = _pStatus->getFENGameState();
        if (ET == ET_NONE)
            this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_HISTORY));
        else if (ET == ET_NORMAL_WIN_WHITE || ET == ET_NORMAL_WIN_BLACK || ET == ET_DRAW)
            this->restartGame(ET);
        else qCritical() << "unacceptable END_TYPE param =" << endTypeAsQstr(ET);
        break;
    }
    case CMT_HISTORY:
        _pStatus->setHistoryMoves(QStrTcpRespond);
        this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_LEGAL));
        break;
    case CMT_LEGAL:
        _pStatus->setLegalMoves(QStrTcpRespond);
        this->continueGameplay();
        break;
    default: qCritical() << "unknown ProcessedChenardMsgType:"
                         << QString::number(ProcessedChenardMsgType);
    }
}

void Chess::playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService /* = false*/)
{
    if (PlayerType == PT_WHITE)
    {
        qInfo() << "white";
        _pClientsList->setClientStartConfirmation(PT_WHITE, true);
    }
    else if (PlayerType == PT_BLACK)
    {
        _pClientsList->setClientStartConfirmation(PT_BLACK, true);
        qInfo() << "black";
    }
    else if (bService)
        qInfo() << "service start";
    else
        qCritical() << "unknown playerWantToStartNewGame val:" << playerTypeAsQStr(PlayerType);

    if ((_pClientsList->isWholeGameTableOccupied() &&
         _pClientsList->isStartClickedByBothPlayers()) || bService)
    {
        qInfo() << "both have clicked start. Try to start a game";
        _pTimers->stopQueueTimer();
        this->sendMsgToTcp("new");
        if (!bService)
        {
            _pClientsList->setClientStartConfirmation(PT_WHITE, false);
            _pClientsList->setClientStartConfirmation(PT_BLACK, false);
        }
    }
}

void Chess::tellPlayerThatHeGaveBadMove(QString QStrMsg) //move not included in answer
{
    qInfo() << QStrMsg;
    Client player = _pClientsList->getPlayer(_pStatus->getActivePlayerType());

    this->sendDataToClient(player, AT_BAD_MOVE);
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp: " + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(QStrMsg);
}

void Chess::newClientLogged(Client& client, int64_t sqlID)
{    
    if (!_pClientsList->isClientSqlIDExists(sqlID))
    {
        _pClientsList->setClientSqlIDAndName(client, sqlID);
        this->sendDataToClient(client);
    }
    else //client is already in list
    {
        //todo: why it must be checked here, but not in conditions?
        if (client.sqlID() > 0 && client.sqlID() != sqlID)
            this->killClient(client, RRR_REMOVE_AND_REFRESH_CLIENT); //todo: F5 his page
        //todo: test it again, and is everything done with 2nd client?
        else this->killClient(client, RRR_DOUBLE_LOGIN); //"logout:doubleLogin"
    }

    this->updateClientsInUI();
}

void Chess::killClient(Client& client, REJECTED_REQUEST_REACTION RRR)
{
    if (RRR == RRR_REMOVE_AND_REFRESH_CLIENT)
        qWarning() << rejectedRequestReactionAsQStr(RRR);
    else
        qInfo() << rejectedRequestReactionAsQStr(RRR);

    this->restorateGameIfDisconnectedClientAffectIt(client);
    _pClientsList->clearClientSqlID(client);
    client.socket()->sendTextMessage(rejectedRequestReactionAsQStr(RRR));
    _pClientsList->removeClientFromList(client);
    this->updateClientsInUI();
}

void Chess::updateClientsInUI()
{
    QString QStrClientsInList = std::to_string(_pClientsList->getClientsList().size()).c_str();
    emit this->setBoardDataLabelInUI(QStrClientsInList, BDL_SOCKETS_ONLINE);
    emit _pClientsList->showClientsListInUI(_pClientsList->getClientsList());
}

void Chess::resetTableData()
{
    _pClientsList->resetPlayersStartConfirmInfo();
    _pTimers->stopQueueTimer();
    _pTimers->resetGameTimers();
    _pStatus->clearMove();
    _pStatus->resetStatusData();
}

void Chess::restorateGameIfDisconnectedClientAffectIt(Client& clientToDisconnect)
{
    qInfo();

    if (_pClientsList->isClientAPlayer(clientToDisconnect))
        this->playerLeftChair(clientToDisconnect.type());
    else if (_pClientsList->isClientInQueue(clientToDisconnect))
    {
        _pClientsList->removeClientFromQueue(clientToDisconnect);
        this->sendDataToAllClients(); //queued clients need to refreshed
    }
    else emit this->addTextToLogPTE("client disconnected\n", LOG_CORE);
}

void Chess::sendDataToClient(Client client, ACTION_TYPE AT /*= AT_NONE*/,
                             END_TYPE ET /*= ET_NONE*/)
{
    _pWebsockets->sendMsgToClient(this->getTableData(AT, ET), client.ID());
}

void Chess::sendDataToAllClients(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    _pWebsockets->sendMsgToAllClients(this->getTableData(AT, ET));
}

void Chess::makeCoreReadyForNewGame()
{
    qInfo();

    if (_pClientsList->isWholeGameTableOccupied())
        _ChessGameStatus = _pTimers->startQueueTimer();
    else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    this->sendDataToAllClients(AT_RESET_COMPLITED);
}

void Chess::startNewGameInCore()
{
    emit this->addTextToLogPTE("new game\n", LOG_CORE);

    _ChessGameStatus = GS_TURN_WHITE_FIRST_TURN;

    //first legal and status always looks the same:
    _pStatus->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    _pStatus->setLegalMoves("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 "
                            "d2d4 e2e3 e2e4 f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");

    _pTimers->resetGameTimers();
    _pTimers->startGameTimer();
    _pClientsList->resetPlayersStartConfirmInfo();

    this->sendDataToAllClients(AT_NEW_GAME_STARTED);
}

void Chess::manageMoveRequest(clientRequest request)
{
    if (_pStatus->isMoveARequestForPromotion(request.param))
    {
        _pStatus->setMove(request.param);
        _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE_PROMOTE :
                                                                    GS_TURN_BLACK_PROMOTE;
        this->sendDataToClient(_pClientsList->getPlayer(_pStatus->getActivePlayerType()));
    }
    else if (!_pStatus->isMoveLegal(request.param))
    {
        this->tellPlayerThatHeGaveBadMove(request.param);
    }
    else
    {
        _pStatus->setMove(request.param); //no need for earlier clear
        _pMovements->doMoveSequence(_pStatus->getMove(), _pStatus->getMoveType());
        this->sendMsgToTcp("move " + request.param);
    }
}

void Chess::continueGameplay()
{
    _pTimers->switchPlayersTimers(_pStatus->getWhoseTurn());
    _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE : GS_TURN_BLACK;
    this->sendDataToAllClients();
}

void Chess::restartGame(END_TYPE ET)
{
    qInfo() << endTypeAsQstr(ET);
    _ChessGameStatus = GS_TURN_NONE_RESETING;
    this->resetTableData();
    this->sendDataToAllClients(AT_END_GAME, ET);
    this->changePlayersOnChairs();
    if (_pMovements->resetPiecePositions())
        this->makeCoreReadyForNewGame();
    this->sendDataToAllClients();
}

void Chess::changePlayersOnChairs()
{
    qInfo();

    _pClientsList->clearPlayerType(PT_WHITE);
    this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    _pClientsList->clearPlayerType(PT_BLACK);
    this->sendDataToAllClients(AT_NEW_BLACK_PLAYER);

    this->fillTableWithNextQueuedClientsIfTheyExist();
}

void Chess::playerLeftChair(PLAYER_TYPE PT)
{
    emit this->addTextToLogPTE(playerTypeAsQStr(PT) + " player left chair\n", LOG_CORE);

    if (whoseTurnFromGameStatus(_ChessGameStatus) == NO_TURN)
    {
        _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
        this->resetTableData();
        _pClientsList->clearPlayerType(PT);
        this->sendDataToAllClients(PT == PT_WHITE ? AT_NEW_WHITE_PLAYER : AT_NEW_BLACK_PLAYER);
        this->fillTableWithNextQueuedClientsIfTheyExist();
    }
    else this->restartGame(PT == PT_WHITE ? ET_SOCKET_LOST_WHITE : ET_SOCKET_LOST_BLACK);
}

void Chess::fillTableWithNextQueuedClientsIfTheyExist()
{
    if (whoseTurnFromGameStatus(_ChessGameStatus) != NO_TURN)
    {
        qCritical() << "whose turn != NO_TURN. status =" << gameStatusAsQStr(_ChessGameStatus);
        return;
    }

    if (_pClientsList->isWholeGameTableOccupied())
    {
        qCritical() << "table is already occupied";
        return;
    }
    else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    if (_pClientsList->getAmountOfQueuedClients() <= 0) return;
    else
    {
        if (_pClientsList->isPlayerChairEmpty(PT_WHITE))
        {
            _pClientsList->putOnChairNextQueuedClient(PT_WHITE);
            this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
        }
        if (_pClientsList->isPlayerChairEmpty(PT_BLACK))
        {
            _pClientsList->putOnChairNextQueuedClient(PT_BLACK);
            this->sendDataToAllClients(AT_NEW_BLACK_PLAYER);
        }
    }

    if (_pClientsList->isWholeGameTableOccupied())
    {
        _ChessGameStatus = _pTimers->startQueueTimer();
        this->sendDataToAllClients();
    }
}

QString Chess::getTableData(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    if (ET > ET_NONE && AT != AT_END_GAME)
    {
        qCritical() << "END_TYPE must be ET_NONE if ACTION_TYPE isn't AT_END_GAME";
        return "";
    }

    QString QStrWhitePlayerID, QStrBlackPlayerID;
    if (_pClientsList->isPlayerChairEmpty(PT_WHITE)) QStrWhitePlayerID = "0";
    else QStrWhitePlayerID = QString::number(_pClientsList->getPlayer(PT_WHITE).sqlID());
    if (_pClientsList->isPlayerChairEmpty(PT_BLACK)) QStrBlackPlayerID = "0";
    else QStrBlackPlayerID = QString::number(_pClientsList->getPlayer(PT_BLACK).sqlID());

    QString TD = "{\"";
    TD += QString::number(TD_ACTION) + ":" + QString::number(AT + ET);
    TD += "," + QString::number(TD_WHITE_PLAYER) + ":" + QStrWhitePlayerID;
    TD += "," + QString::number(TD_BLACK_PLAYER) + ":" + QStrBlackPlayerID;
    TD += "," + QString::number(TD_GAME_STATE) + ":" + QString::number(_ChessGameStatus);
    TD += "," + QString::number(TD_WHITE_TIME) + ":"
            + QString::number(_pTimers->getWhiteTimeLeft(true));
    TD += "," + QString::number(TD_BLACK_TYPE) + ":"
            + QString::number(_pTimers->getBlackTimeLeft(true));
    TD += "," + QString::number(TD_QUEUE) + ":" + _pClientsList->getQueuedClientsList();
    if (_ChessGameStatus == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS)
        TD += "," + QString::number(TD_START_TIME) + ":"
                + QString::number(_pTimers->getStartTimeLeft(true));
    TD += "," + QString::number(TD_HISTORY) + ":"
            + _pStatus->getHistoryMovesAsQStr();
    TD += "," + QString::number(TD_PROMOTIONS) + ":"
            + _pPieceController->getPromotedPawnsPositions();

    //convert string to JSON format
    TD.replace(":", "\":\"");
    TD.replace(",", "\",\"");

    TD += "\"}";

    qInfo() << "QStrTableData =" << TD;
    return TD;
}

void Chess::timeOutStart()
{
    _pClientsList->resetPlayersStartConfirmInfo();

    if (!_pClientsList->isStartClickedByPlayer(PT_WHITE))
    {
        _pClientsList->clearPlayerType(PT_WHITE);
        this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    }
    if (!_pClientsList->isStartClickedByPlayer(PT_BLACK))
    {
        _pClientsList->clearPlayerType(PT_BLACK);
        this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    }

    this->fillTableWithNextQueuedClientsIfTheyExist();

    this->sendDataToAllClients();
}

void Chess::timeOutPlayer(PLAYER_TYPE Player)
{
    this->restartGame(Player == PT_WHITE ? ET_TIMEOUT_GAME_WHITE : ET_TIMEOUT_GAME_BLACK);
}

void Chess::setBoardDataLabelInUI(QString QStrLabel, BOARD_DATA_LABEL LabelType)
{
    emit this->setBoardDataLabel(QStrLabel, LabelType);
}

void Chess::showLegalMovesInForm(QStringList legalMoves)
{
    emit this->showLegalMovesInUI(legalMoves);
}

void Chess::showHistoryMovesInForm(QStringList historyMoves)
{
    emit this->showHistoryMovesInUI(historyMoves);
}
