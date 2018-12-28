#include "chess.h"

Chess::Chess(PieceController* pPieceController)
{
    m_pPieceController = pPieceController;
    m_pDobot = m_pPieceController->getDobotPointer();
    m_pBoardMain = m_pPieceController->getBoardMainPointer();
    m_pBoardRemoved = m_pPieceController->getBoardRemovedPointer();
    m_pBoardChenard = new Chessboard(B_MAIN, IMAGINARY);
    m_pWebsockets = new Websockets();
    m_pClientsList = m_pWebsockets->getClientsListPointer();
    m_pTCPMsgs = new TCPMsgs();
    m_pUSB = new ArduinoUsb();

    m_pStatus = new ChessStatus(m_pPieceController, m_pBoardMain, m_pClientsList);
    m_pMovements = new ChessMovements(m_pPieceController, m_pBoardMain, m_pBoardRemoved);
    m_pTimers = new ChessTimers(m_pClientsList);
    m_pConditions = new ChessConditions(m_pClientsList, m_pStatus);

    m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    m_executingClientRequestTimer = new QTimer();
    m_executingClientRequestTimer->setSingleShot(true);
    connect(m_executingClientRequestTimer, SIGNAL(timeout()), this, SLOT(clientRequesTimeOut()));
    m_keepConnectedTimer = new QTimer();
    m_keepConnectedTimer->setSingleShot(false);
    connect(m_keepConnectedTimer, SIGNAL(timeout()), this, SLOT(keepConnectedTimeOut()));
    m_keepConnectedTimer->start(60*1000*30); //future: 30 mins is temporary, to not spam output

    connect(m_pTCPMsgs, SIGNAL(msgFromTcpToChess(QString, QString)),
            this, SLOT(checkMsgFromChessEngine(QString, QString)));
    connect(m_pWebsockets, SIGNAL(msgFromWebsocketsToChess(QString, uint64_t)),
            this, SLOT(checkMsgFromClient(QString, uint64_t)));
    connect(m_pUSB, SIGNAL(msgFromUsbToChess(Sensors)),
            this, SLOT(checkMsgFromUSB(Sensors)));
    connect(m_pUSB, SIGNAL(updatePortsComboBox(int)),
            this, SLOT(updatePortsComboBoxInUI(int)));
    connect(m_pStatus, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabelInUI(QString, BOARD_DATA_LABEL)));
    connect(m_pTimers, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
            this, SLOT(setBoardDataLabelInUI(QString, BOARD_DATA_LABEL)));
    connect(m_pStatus, SIGNAL(showLegalMoves(QStringList)),
            this, SLOT(showLegalMovesInForm(QStringList)));
    connect(m_pStatus, SIGNAL(showHistoryMoves(QStringList)),
            this, SLOT(showHistoryMovesInForm(QStringList)));
    connect(m_pTimers, SIGNAL(timeOutStart()), this, SLOT(timeOutStart()));
    connect(m_pTimers, SIGNAL(timeOutPlayer(PLAYER_TYPE)),
            this, SLOT(timeOutPlayer(PLAYER_TYPE)));
    connect(m_pTimers, SIGNAL(timeOutTurn()),
            this, SLOT(timeOutTurn()));
}

Chess::~Chess()
{
    delete m_pTimers;
    delete m_pMovements;
    delete m_pStatus;
    delete m_pConditions;
}


///communication methods
//future: do I use/need (or will) bService? can it even work now?
void Chess::checkMsgFromClient(QString QStrMsg, uint64_t un64SenderID, bool bService /*= false*/)
{
    Client client = m_pClientsList->getClient(un64SenderID);
    if (ChessConditions::isRequestMsgInProperFormat(QStrMsg))
    {
        clientRequest request(un64SenderID, requestTypeFromQStr(QStrMsg, SHOW_ERRORS),
                              ChessConditions::extractParamIfExists(QStrMsg), bService);
        if (request.type == RT_GET_TABLE_DATA)
        {
            //respond to client with tableData w/o queueing this request (nothing is changed)
            this->sendDataToClient(client);
            return;
        }

        emit this->addTextToLogPTE("accepted client's request. " + request.dumpAllData()
                                   + "\n", LOG_CORE);

        m_requestList << request;
        this->startExecutingClientsRequests();
    }
    else
    {
        qWarning() << "client request can't be accepted. client sqlID =" << client.sqlID()
                   << ", client name =" << client.name(DONT_SHOW_ERRORS);
        this->killClient(client, RRR_REMOVE_AND_REFRESH_CLIENT);
        return;
    }
}

void Chess::checkMsgFromUSB(Sensors sensors)
{   
    if (sensors.fDustDensity > 350.f || sensors.fCurrent > 4.f
            || sensors.fTemp1 > 40.f || sensors.fTemp2 > 40.f)
        qWarning() << "sensor exceeded maximum value:" << sensors.getAsQStr();

    if (sensors.bStopButton == true)
        m_pDobot->forceStopArm();
}

void Chess::sendDataToClient(Client client, ACTION_TYPE AT /*= AT_NONE*/,
                             END_TYPE ET /*= ET_NONE*/)
{
    qInfo() << "client ID =" << QString::number(client.ID()) << ", client sqlID ="
            << QString::number(client.sqlID()) << ", ACTION_TYPE ="
            << actionTypeAsQstr(AT);
    m_pWebsockets->sendMsgToClient(this->getTableData(AT, ET), client.ID());
}

void Chess::sendDataToAllClients(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    qInfo()<< "ACTION_TYPE =" << actionTypeAsQstr(AT);
    m_pWebsockets->sendMsgToAllClients(this->getTableData(AT, ET));
}

void Chess::sendMsgToChessEngine(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp: " + QStrMsg + "\n", LOG_CORE);
    m_pTCPMsgs->queueCmd(QStrMsg);
}

void Chess::killClient(const Client& client, REJECTED_REQUEST_REACTION RRR)
{
    this->restorateGameIfDisconnectedClientAffectIt(client);
    m_pClientsList->clearClientSqlID(client);
    if (RRR == RRR_DOUBLE_LOGIN)
    {
        m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED_DOUBLE_LOGIN);
        this->sendDataToClient(client);
        m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED);
    }
    else if (RRR == RRR_REMOVE_AND_REFRESH_CLIENT)
    {
        m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED_REMOVE_AND_REFRESH_CLIENT);
        this->sendDataToClient(client);
        //don't remove client. his msgs will be blocked, till he will refresh page (client will die)
    }

    this->updateClientsInUI();
}

void Chess::startExecutingClientsRequests()
{
    if (!m_bExecutingClientRequest && !m_requestList.isEmpty())
    {
        clientRequest request = m_requestList.takeFirst();
        REJECTED_REQUEST_REACTION checkedRequest =
                m_pConditions->isClientRequestCanBeExecuted(request, m_ChessGameStatus);
        if (checkedRequest == RRR_NONE)
        {
            qInfo() << "checkedRequest == RRR_NONE";
            m_executingClientRequestTimer->start(7000);
            this->executeClientRequest(request);
        }
        else
        {
            qInfo() << "checkedRequest != RRR_NONE";
            Client client;
            if (m_pClientsList->isClientIDExists(request.clientID)) //client can be offline already
                client = m_pClientsList->getClient(request.clientID);
            else return;

            if (checkedRequest == RRR_RESEND_TABLE_DATA)
                this->sendDataToClient(client);
            else if (checkedRequest == RRR_REMOVE_AND_REFRESH_CLIENT)
                this->killClient(client, RRR_REMOVE_AND_REFRESH_CLIENT);
        }
    }
}

void Chess::closeClientRequest()
{
    if (!m_bExecutingClientRequest)
    {
        qCritical() << "m_bExecutingClientRequest is already false";
        return;
    }
    else qInfo();

    m_bExecutingClientRequest = false;
    m_executingClientRequestTimer->stop();

    if (!m_requestList.isEmpty())
        this->startExecutingClientsRequests();
}

//future: divide it to function with cases with only core calculations, and with tcp requests (?)
void Chess::executeClientRequest(clientRequest request)
{
    Client client = m_pClientsList->getClient(request.clientID);
    qInfo() << "request data:" << request.dumpAllData()
            << ", client data:" << client.dumpCrucialData();

    m_bExecutingClientRequest = true;
    bool bSentMsgToChessEngine = false;

    //don't mix case order
    switch(request.type)
    {
    case RT_GET_TABLE_DATA: //redundant code- let it be here for safety
        this->sendDataToClient(client);
        break;
    case RT_LOGIN:
    {
        QString QStrLogin = request.param.left(request.param.indexOf("&"));
        QString QStrPass = request.param.mid(request.param.indexOf("&")+1);
        if (Sql::isLoginAndPassExistInDB(QStrLogin, QStrPass))
        {
            int64_t n64SqlID = Sql::getClientIdFromDB(QStrLogin);

            if (m_pClientsList->isClientSqlIDExists(n64SqlID)) //double login- kill old client 1st
                this->killClient(m_pClientsList->getClient(n64SqlID, CID_SQL), RRR_DOUBLE_LOGIN);

            m_pClientsList->setClientSqlIDAndName(client, n64SqlID);
            this->sendDataToClient(client);
        }
        else
        {
            m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED_LOGIN_FAILED);
            this->sendDataToClient(client);
            m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED);
        }
        break;
    }
    case RT_SIT_ON:
    {
        PLAYER_TYPE PT = playerTypeFromQStr(request.param);
        if (PT == PT_NONE)
            PT = m_pClientsList->getAvailableGuest(SHOW_ERRORS);
        if (!m_pClientsList->isClientSqlIDExists(client))
        {
            if (PT == PT_WHITE)
                m_pClientsList->setClientSqlIDAndName(client, SY_LOGGED_GUEST1);
            else if (PT == PT_BLACK)
                m_pClientsList->setClientSqlIDAndName(client, SY_LOGGED_GUEST2);
        }
        m_pClientsList->setPlayerType(client, PT);
        if (m_pClientsList->isWholeGameTableOccupied())
            m_ChessGameStatus = m_pTimers->startQueueTimer();
        this->sendDataToAllClients(PT == PT_WHITE ? AT_NEW_WHITE_PLAYER : AT_NEW_BLACK_PLAYER);
        break;
    }
    case RT_NEW_GAME:
        if (request.service)
            bSentMsgToChessEngine = this->playerWantToStartNewGame(PT_NONE, request.service);
        else if (client == m_pClientsList->getPlayer(PT_WHITE))
            bSentMsgToChessEngine = this->playerWantToStartNewGame(PT_WHITE);
        else if (client == m_pClientsList->getPlayer(PT_BLACK))
            bSentMsgToChessEngine = this->playerWantToStartNewGame(PT_BLACK);
        else qCritical() << "client isn't a player";
        break;
    case RT_PROMOTE_TO:
        request.param = m_pStatus->m_PosMove.asQStr() + request.param;
        m_pStatus->promotePawn(m_pStatus->m_PosMove.from, request.param.right(1));
        //without break;
    case RT_MOVE: //RT_PROMOTE_TO case must be be4 RT_MOVE case
        bSentMsgToChessEngine = this->manageMoveRequest(request);
        break;
    case RT_STAND_UP:
        this->restorateGameIfDisconnectedClientAffectIt(client);
        if (m_pClientsList->isClientAGuest(client))
        {
            m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED);
            this->sendDataToClient(client);
        }
        break;
    case RT_QUEUE_ME:
        m_pClientsList->addClientToQueue(client);
        this->sendDataToAllClients();
        break;
    case RT_LEAVE_QUEUE:
        m_pClientsList->removeClientFromQueue(client);
        this->sendDataToAllClients();
        break;
    case RT_CLIENT_LEFT:
        this->restorateGameIfDisconnectedClientAffectIt(client);
        if (m_pClientsList->isClientInList(client.ID()))
            m_pClientsList->removeClientFromList(client);
        break;
    case RT_LOGOUT:
        this->restorateGameIfDisconnectedClientAffectIt(client);
        m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED_LOGOUT);
        this->sendDataToClient(client);
        m_pClientsList->setClientSqlIDAndName(client, SY_UNLOGGED);
        break;
    default:
        qCritical() << "received request.type:" << QString::number(request.type);
        this->restartGame(ET_DRAW); //future: inform clients about server errors be4 restarting
    }

    if (!bSentMsgToChessEngine)
        this->closeClientRequest();

    this->updateClientsInUI();
}

bool Chess::playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService /*= false*/)
{
    qInfo();

    if (bService)
        qInfo() << "service start";
    else if (PlayerType == PT_WHITE || PlayerType == PT_BLACK)
        m_pClientsList->setClientStartConfirmation(PlayerType, true);
    else
        qCritical() << "unknown playerWantToStartNewGame val:" << playerTypeAsQStr(PlayerType);

    if ((m_pClientsList->isWholeGameTableOccupied() &&
         m_pClientsList->isStartClickedByBothPlayers()) || bService)
    {
        qInfo() << "both have clicked start. Try to start a game";
        m_pTimers->stopQueueTimer();
        this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_NEW));
        if (!bService)
        {
            m_pClientsList->setClientStartConfirmation(PT_WHITE, false);
            m_pClientsList->setClientStartConfirmation(PT_BLACK, false);
        }
        return MSG_WAS_SENT_TO_CHESS_ENGINE;
    }
    else return MSG_WAS_NOT_SENT_TO_CHESS_ENGINE;
}

bool Chess::manageMoveRequest(clientRequest request)
{
    if (m_pStatus->isMoveARequestForPromotion(request.param))
    {
        m_pStatus->setMove(request.param);
        m_ChessGameStatus = m_pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE_PROMOTE :
                                                                    GS_TURN_BLACK_PROMOTE;

        this->sendDataToClient(m_pClientsList->getPlayer(m_pStatus->getActivePlayerType()));
        return MSG_WAS_NOT_SENT_TO_CHESS_ENGINE;
    }
    else if (!m_pStatus->isMoveLegal(request.param))
    {
        Client player = m_pClientsList->getPlayer(m_pStatus->getActivePlayerType());
        this->sendDataToClient(player, AT_BAD_MOVE); //move itself is not included in answer
        return MSG_WAS_NOT_SENT_TO_CHESS_ENGINE;
    }
    else
    {
        m_pStatus->setMove(request.param); //no need for earlier clear
        m_pMovements->doMoveSequence(m_pStatus->m_PosMove, m_pStatus->m_MoveType);
        this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_MOVE) + request.param);
        return MSG_WAS_SENT_TO_CHESS_ENGINE;
    }
}

void Chess::updateClientsInUI()
{
    QString QStrClientsInList = std::to_string(m_pClientsList->getClientsList().size()).c_str();
    emit this->setBoardDataLabelInUI(QStrClientsInList, BDL_SOCKETS_ONLINE);
    emit m_pClientsList->showClientsListInUI(m_pClientsList->getClientsList());
}


///gameplay methods
void Chess::checkMsgFromChessEngine(QString QStrTcpMsgType, QString QStrTcpRespond)
{
    CHENARD_MSG_TYPE ProcessedChenardMsgType = ChenardMsgType(QStrTcpMsgType);
    qInfo() << "ProcessedChenardMsgType =" << chenardMsgTypeAsQStr(ProcessedChenardMsgType);
    if (!isChenardAnswerCorrect(ProcessedChenardMsgType, QStrTcpRespond, SHOW_ERRORS)) return;

    //client's 'move' request execute following TCP msgs: move... -> status -> history -> legal
    switch(ProcessedChenardMsgType)
    {
    case CMT_NEW:
        this->startNewGameInCore();
        break;
    case CMT_MOVE:
        this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_STATUS));
        break;
    case CMT_STATUS:
    {
        m_pStatus->saveStatusData(QStrTcpRespond);
        END_TYPE ET = m_pStatus->m_FENGameState;
        if (ET == ET_NONE)
            this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_HISTORY));
        else if (ET == ET_NORMAL_WIN_WHITE || ET == ET_NORMAL_WIN_BLACK || ET == ET_DRAW)
            this->restartGame(ET);
        else qCritical() << "unacceptable END_TYPE param =" << endTypeAsQstr(ET);
        break;
    }
    case CMT_HISTORY:
        m_pStatus->setHistoryMoves(QStrTcpRespond);
        this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_LEGAL));
        break;
    case CMT_LEGAL:
        m_pStatus->setLegalMoves(QStrTcpRespond);
        this->continueGameplay();
        break;
    default: qCritical() << "unknown ProcessedChenardMsgType:"
                         << QString::number(ProcessedChenardMsgType);
    }

    if (ProcessedChenardMsgType == CMT_NEW || ProcessedChenardMsgType == CMT_LEGAL)
        this->closeClientRequest();
}

void Chess::startNewGameInCore()
{
    emit this->addTextToLogPTE("new game\n", LOG_CORE);

    m_ChessGameStatus = GS_TURN_WHITE_FIRST_TURN;

    //first legal and status always looks the same:
    m_pStatus->saveStatusData(FIRST_STATUS);
    m_pStatus->setLegalMoves(FIRST_LEGAL);

    m_pTimers->resetGameTimers();
    m_pTimers->startGameTimer();
    m_pClientsList->resetPlayersStartConfirmInfo();

    this->sendDataToAllClients(AT_NEW_GAME_STARTED);
}

void Chess::restartGame(END_TYPE ET)
{
    qInfo() << endTypeAsQstr(ET);
    m_ChessGameStatus = GS_TURN_NONE_RESETING;
    this->resetTableData();
    //future: php could detect who left chair direct from this one table data below(?)
    this->sendDataToAllClients(AT_END_GAME, ET);
    this->changePlayersOnChairs();
    if (m_pMovements->resetPiecePositions())
        this->makeCoreReadyForNewGame();
    this->sendDataToAllClients();
}

void Chess::resetTableData()
{
    m_pClientsList->resetPlayersStartConfirmInfo();
    m_pTimers->stopQueueTimer();
    m_pTimers->resetGameTimers();
    m_pStatus->clearMove();
    m_pStatus->resetStatusData();
}

void Chess::changePlayersOnChairs()
{
    m_pClientsList->clearPlayerType(PT_WHITE);
    this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    m_pClientsList->clearPlayerType(PT_BLACK);
    this->sendDataToAllClients(AT_NEW_BLACK_PLAYER);
    this->fillTableWithNextQueuedClientsIfTheyExist();
}

void Chess::fillTableWithNextQueuedClientsIfTheyExist()
{
    if (whoseTurnFromGameStatus(m_ChessGameStatus) != NO_TURN)
    {
        qCritical() << "whose turn != NO_TURN. status =" << gameStatusAsQStr(m_ChessGameStatus);
        return;
    }

    if (m_pClientsList->isWholeGameTableOccupied())
    {
        qCritical() << "table is already fully occupied";
        return;
    }

    this->putOnNextEmptyChairNextQueuedClientIfItsPossible();
    this->putOnNextEmptyChairNextQueuedClientIfItsPossible();

    if (m_pClientsList->isWholeGameTableOccupied())
    {
        m_ChessGameStatus = m_pTimers->startQueueTimer();
        this->sendDataToAllClients();
    }
}

void Chess::putOnNextEmptyChairNextQueuedClientIfItsPossible()
{
    if (!m_pClientsList->isWholeGameTableOccupied())
    {
        m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
        if (m_pClientsList->getAmountOfQueuedClients() > 0)
        {
            if (m_pClientsList->isPlayerChairEmpty(PT_WHITE))
            {
                m_pClientsList->putOnChairNextQueuedClient(PT_WHITE);
                this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
            }
            else if (m_pClientsList->isPlayerChairEmpty(PT_BLACK))
            {
                m_pClientsList->putOnChairNextQueuedClient(PT_BLACK);
                this->sendDataToAllClients(AT_NEW_BLACK_PLAYER);
            }
        }
    }
}

void Chess::makeCoreReadyForNewGame()
{
    if (m_pClientsList->isWholeGameTableOccupied())
        m_ChessGameStatus = m_pTimers->startQueueTimer();
    else m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    this->sendDataToAllClients(AT_RESET_COMPLITED);
}

void Chess::continueGameplay()
{
    m_pTimers->switchPlayersTimers(m_pStatus->getWhoseTurn());
    m_ChessGameStatus = m_pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE : GS_TURN_BLACK;
    this->sendDataToAllClients();
}

void Chess::restorateGameIfDisconnectedClientAffectIt(const Client &clientToDisconnect)
{
    qInfo() << "client's ID =" << QString::number(clientToDisconnect.ID());

    if (m_pClientsList->isClientAPlayer(clientToDisconnect))
        this->playerLeftChair(clientToDisconnect.type());
    else if (m_pClientsList->isClientInQueue(clientToDisconnect))
    {
        m_pClientsList->removeClientFromQueue(clientToDisconnect);
        this->sendDataToAllClients(); //queued clients need to refreshed
    }
    else emit this->addTextToLogPTE("disconnecting client with ID = "
                                    + QString::number(clientToDisconnect.ID()) + "\n", LOG_CORE);
}

void Chess::playerLeftChair(PLAYER_TYPE PT) //todo: change name
{
    emit this->addTextToLogPTE(playerTypeAsQStr(PT) + " player left chair\n", LOG_CORE);

    if (whoseTurnFromGameStatus(m_ChessGameStatus) == NO_TURN)
    {
        m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
        this->resetTableData();
         m_pClientsList->clearPlayerType(PT);
        this->sendDataToAllClients(PT == PT_WHITE ? AT_NEW_WHITE_PLAYER : AT_NEW_BLACK_PLAYER);
        this->fillTableWithNextQueuedClientsIfTheyExist();
    }
    else this->restartGame(PT == PT_WHITE ? ET_SOCKET_LOST_WHITE : ET_SOCKET_LOST_BLACK);
}


///private slots:
void Chess::clientRequesTimeOut()
{
    if (m_bExecutingClientRequest)
    {
        qCritical();
        this->restartGame(ET_DRAW);
    }
}

void Chess::keepConnectedTimeOut()
{
    qInfo();
    if (m_pClientsList->getClientsList().size() > 0)
        this->sendDataToAllClients();
}

///slots
void Chess::timeOutStart()
{
    m_pClientsList->resetPlayersStartConfirmInfo();

    if (!m_pClientsList->isStartClickedByPlayer(PT_WHITE))
    {
        m_pClientsList->clearPlayerType(PT_WHITE);
        this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    }
    if (!m_pClientsList->isStartClickedByPlayer(PT_BLACK))
    {
        m_pClientsList->clearPlayerType(PT_BLACK);
        this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
    }

    this->fillTableWithNextQueuedClientsIfTheyExist();
    this->sendDataToAllClients();
}

void Chess::timeOutPlayer(PLAYER_TYPE Player)
{
    this->restartGame(Player == PT_WHITE ? ET_TIMEOUT_GAME_WHITE : ET_TIMEOUT_GAME_BLACK);
}

void Chess::timeOutTurn()
{
    this->restartGame(m_pStatus->getWhoseTurn() == WHITE_TURN ? ET_TIMEOUT_GAME_WHITE
                                                              : ET_TIMEOUT_GAME_BLACK);
}

QString Chess::getTableData(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    if (ET > ET_NONE && AT != AT_END_GAME)
    {
        qCritical() << "END_TYPE must be ET_NONE if ACTION_TYPE isn't AT_END_GAME";
        return "";
    }

    QString QStrWhitePlayerID, QStrBlackPlayerID;
    if (m_pClientsList->isPlayerChairEmpty(PT_WHITE)) QStrWhitePlayerID = "0";
    else QStrWhitePlayerID = QString::number(m_pClientsList->getPlayer(PT_WHITE).sqlID());
    if (m_pClientsList->isPlayerChairEmpty(PT_BLACK)) QStrBlackPlayerID = "0";
    else QStrBlackPlayerID = QString::number(m_pClientsList->getPlayer(PT_BLACK).sqlID());

    QString TD;
    //TD += QString::number(TD_ACTION) + ":6" /*+ QString::number(AT + ET)*/; //tests
    TD += QString::number(TD_ACTION) + ":" + QString::number(AT + ET);
    TD += "," + QString::number(TD_WHITE_PLAYER) + ":" + QStrWhitePlayerID;
    TD += "," + QString::number(TD_BLACK_PLAYER) + ":" + QStrBlackPlayerID;
    TD += "," + QString::number(TD_GAME_STATE) + ":" + QString::number(m_ChessGameStatus);
    TD += "," + QString::number(TD_WHITE_TIME) + ":"
            + QString::number(m_pTimers->getWhiteTimeLeft(true));
    TD += "," + QString::number(TD_BLACK_TIME) + ":"
            + QString::number(m_pTimers->getBlackTimeLeft(true));
    TD +="," + QString::number(TD_TURN_TIME) + ":"
            + QString::number(m_pTimers->getTurnTimeLeft(true));
    TD += "," + QString::number(TD_QUEUE) + ":" + m_pClientsList->getQueuedClientsSqlIDsList();
    if (m_ChessGameStatus == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS)
        TD += "," + QString::number(TD_START_TIME) + ":"
                + QString::number(m_pTimers->getStartTimeLeft(true));
    TD += "," + QString::number(TD_HISTORY) + ":"
            + m_pStatus->getHistoryMovesAsQStr();
    TD += "," + QString::number(TD_PROMOTIONS) + ":"
            + m_pPieceController->getPromotedPawnsPositions();

    return TD;
}

void Chess::updatePortsComboBoxInUI(int nPorts)
{
    emit this->updatePortsComboBox(nPorts);
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

///public
QString Chess::dumpAllData()
{
    QString QStrData;

    QStrData = "[chess.h]\n";
    QStrData += "m_ChessGameStatus: " + gameStatusAsQStr(m_ChessGameStatus) + "\n";
    QString QStrExecutingClientReques = m_bExecutingClientRequest ? "true" : "false";
    QStrData += "m_bExecutingClientRequest: " + QStrExecutingClientReques + "\n";
    foreach (clientRequest clReq, m_requestList)
        QStrData += clReq.dumpAllData() + "\n";
    QStrData += "\n";
    QStrData += m_pStatus->dumpAllData();
    QStrData += "\n";
    QStrData += m_pTimers->dumpAllData();

    return QStrData;
}
