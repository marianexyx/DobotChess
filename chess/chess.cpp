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

    m_pStatus = new ChessStatus(m_pPieceController, m_pBoardMain, m_pClientsList);
    m_pMovements = new ChessMovements(m_pPieceController, m_pBoardMain, m_pBoardRemoved);
    m_pTimers = new ChessTimers(m_pClientsList);
    m_pConditions = new ChessConditions(m_pClientsList, m_pStatus);

    m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    m_request.clear();

    connect(m_pTCPMsgs, SIGNAL(msgFromTcpToChess(QString, QString)),
            this, SLOT(checkMsgFromChessEngine(QString, QString)));
    connect(m_pWebsockets, SIGNAL(msgFromWebsocketsToChess(QString, int64_t)),
            this, SLOT(checkMsgFromClient(QString, int64_t)));
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
}

Chess::~Chess()
{
    delete m_pTimers;
    delete m_pMovements;
    delete m_pStatus;
    delete m_pConditions;
}


///communication methods
void Chess::checkMsgFromClient(QString QStrMsg, int64_t n64SenderID,
                                   bool bService /*= false*/)
{
    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    Client client = m_pClientsList->getClient(n64SenderID);
    REJECTED_REQUEST_REACTION BadRequestFromClient = RRR_NONE;

    if (m_pConditions->isClientRequestCanBeAccepted(QStrMsg, &client, m_ChessGameStatus,
                                                   BadRequestFromClient) || bService)
    {
        m_request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
        m_request.param = m_pConditions->extractParamIfTypeIsInProperFormat(m_request.type, QStrMsg,
                                                                          BadRequestFromClient);
        qInfo() << "accepted client request type =" << requestTypeAsQStr(m_request.type)
                << ", param =" << m_request.param << ", rejectedRequestReaction type ="
                << rejectedRequestReactionAsQStr(BadRequestFromClient);
    }
    else
    {
        if (m_pClientsList->isClientInList(client)) return;

        //BadRequestFromClient was set up in isClientRequestCanBeAccepted()- passed by &(ref)
        if (BadRequestFromClient == RRR_NONE)
            BadRequestFromClient = RRR_RESEND_TABLE_DATA;

        if (BadRequestFromClient == RRR_RESEND_TABLE_DATA)
            this->sendDataToClient(client);
        else if (BadRequestFromClient == RRR_REMOVE_AND_REFRESH_CLIENT)
            this->killClient(client, RRR_REMOVE_AND_REFRESH_CLIENT);

        qWarning() << "client request can't be accepted";
        return;
    }

    this->executeClientRequest(client, bService);
}

void Chess::sendDataToClient(Client client, ACTION_TYPE AT /*= AT_NONE*/,
                             END_TYPE ET /*= ET_NONE*/)
{
    m_pWebsockets->sendMsgToClient(this->getTableData(AT, ET), client.ID());
}

void Chess::sendDataToAllClients(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
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
    client.socket()->sendTextMessage(rejectedRequestReactionAsQStr(RRR));
    m_pClientsList->removeClientFromList(client);
    this->updateClientsInUI();
}

void Chess::executeClientRequest(Client &client, bool bService /*= false*/)
{
    switch(m_request.type)
    {
    case RT_GET_TABLE_DATA:
        this->sendDataToClient(client);
        break;
    case RT_IM:
    {
        int nSqlID = m_request.param.left(m_request.param.indexOf("&")).toInt();
        if (m_pClientsList->isClientSqlIDExists(nSqlID)) //double login- kill old client
            this->killClient(m_pClientsList->getClient(nSqlID, CID_SQL), RRR_DOUBLE_LOGIN);
        m_pClientsList->setClientSqlIDAndName(client, nSqlID);
        this->sendDataToClient(client);
        break;
    }
    case RT_SIT_ON:
    {
        PLAYER_TYPE PT = playerTypeFromQStr(m_request.param);
        m_pClientsList->setPlayerType(client, PT);
        if (m_pClientsList->isWholeGameTableOccupied())
            m_ChessGameStatus = m_pTimers->startQueueTimer();
        this->sendDataToAllClients(PT == PT_WHITE ? AT_NEW_WHITE_PLAYER : AT_NEW_BLACK_PLAYER);
        break;
    }
    case RT_NEW_GAME:
        if (client == m_pClientsList->getPlayer(PT_WHITE))
            this->playerWantToStartNewGame(PT_WHITE);
        else if (client == m_pClientsList->getPlayer(PT_BLACK))
            this->playerWantToStartNewGame(PT_BLACK);
        else if (bService)
            this->playerWantToStartNewGame(PT_NONE, bService);
        else qCritical() << "client isn't a player";
        break;
    case RT_MOVE:
        this->manageMoveRequest(m_request);
        break;
    case RT_PROMOTE_TO:
        m_request.param = m_pStatus->m_PosMove.asQStr() + m_request.param;
        m_pStatus->promotePawn(m_pStatus->m_PosMove.from, m_request.param.right(1));
        //without break;
    case RT_STAND_UP:
        this->playerLeftChair(client.type());
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
        m_pClientsList->removeClientFromList(client);
        break;
    default:
        qCritical() << "received m_request.type:" << QString::number(m_request.type);
    }

    this->updateClientsInUI();
}

void Chess::playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService /* = false*/)
{
    if (PlayerType == PT_WHITE)
        m_pClientsList->setClientStartConfirmation(PT_WHITE, true);
    else if (PlayerType == PT_BLACK)
        m_pClientsList->setClientStartConfirmation(PT_BLACK, true);
    else if (bService)
        qInfo() << "service start";
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
    }
}

void Chess::manageMoveRequest(clientRequest request)
{
    if (m_pStatus->isMoveARequestForPromotion(request.param))
    {
        m_pStatus->setMove(request.param);
        m_ChessGameStatus = m_pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE_PROMOTE :
                                                                    GS_TURN_BLACK_PROMOTE;
        this->sendDataToClient(m_pClientsList->getPlayer(m_pStatus->getActivePlayerType()));
    }
    else if (!m_pStatus->isMoveLegal(request.param))
    {
        Client player = m_pClientsList->getPlayer(m_pStatus->getActivePlayerType());
        this->sendDataToClient(player, AT_BAD_MOVE); //move itself is not included in answer
    }
    else
    {
        m_pStatus->setMove(request.param); //no need for earlier clear
        m_pMovements->doMoveSequence(m_pStatus->m_PosMove, m_pStatus->m_MoveType);
        this->sendMsgToChessEngine(chenardMsgTypeAsQStr(CMT_MOVE) + request.param);
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
}

void Chess::startNewGameInCore()
{
    emit this->addTextToLogPTE("new game\n", LOG_CORE);

    m_ChessGameStatus = GS_TURN_WHITE_FIRST_TURN;

    //first legal and status always looks the same:
    m_pStatus->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    m_pStatus->setLegalMoves("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 "
                            "d2d4 e2e3 e2e4 f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");

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
    qInfo();
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
        qCritical() << "table is already occupied";
        return;
    }
    else m_ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    if (m_pClientsList->getAmountOfQueuedClients() <= 0) return;
    else
    {
        if (m_pClientsList->isPlayerChairEmpty(PT_WHITE))
        {
            m_pClientsList->putOnChairNextQueuedClient(PT_WHITE);
            this->sendDataToAllClients(AT_NEW_WHITE_PLAYER);
        }
        if (m_pClientsList->isPlayerChairEmpty(PT_BLACK))
        {
            m_pClientsList->putOnChairNextQueuedClient(PT_BLACK);
            this->sendDataToAllClients(AT_NEW_BLACK_PLAYER);
        }
    }

    if (m_pClientsList->isWholeGameTableOccupied())
    {
        m_ChessGameStatus = m_pTimers->startQueueTimer();
        this->sendDataToAllClients();
    }
}

void Chess::makeCoreReadyForNewGame()
{
    qInfo();

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
    qInfo();

    if (m_pClientsList->isClientAPlayer(clientToDisconnect))
        this->playerLeftChair(clientToDisconnect.type());
    else if (m_pClientsList->isClientInQueue(clientToDisconnect))
    {
        m_pClientsList->removeClientFromQueue(clientToDisconnect);
        this->sendDataToAllClients(); //queued clients need to refreshed
    }
    else emit this->addTextToLogPTE("client disconnected\n", LOG_CORE);
}

void Chess::playerLeftChair(PLAYER_TYPE PT)
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

    QString TD = "{\"";
    TD += QString::number(TD_ACTION) + ":" + QString::number(AT + ET);
    TD += "," + QString::number(TD_WHITE_PLAYER) + ":" + QStrWhitePlayerID;
    TD += "," + QString::number(TD_BLACK_PLAYER) + ":" + QStrBlackPlayerID;
    TD += "," + QString::number(TD_GAME_STATE) + ":" + QString::number(m_ChessGameStatus);
    TD += "," + QString::number(TD_WHITE_TIME) + ":"
            + QString::number(m_pTimers->getWhiteTimeLeft(true));
    TD += "," + QString::number(TD_BLACK_TYPE) + ":"
            + QString::number(m_pTimers->getBlackTimeLeft(true));
    TD += "," + QString::number(TD_QUEUE) + ":" + m_pClientsList->getQueuedClientsList();
    if (m_ChessGameStatus == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS)
        TD += "," + QString::number(TD_START_TIME) + ":"
                + QString::number(m_pTimers->getStartTimeLeft(true));
    TD += "," + QString::number(TD_HISTORY) + ":"
            + m_pStatus->getHistoryMovesAsQStr();
    TD += "," + QString::number(TD_PROMOTIONS) + ":"
            + m_pPieceController->getPromotedPawnsPositions();

    //convert string to JSON format
    TD.replace(":", "\":\"");
    TD.replace(",", "\",\"");

    TD += "\"}";

    qInfo() << "QStrTableData =" << TD;
    return TD;
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
    QStrData += "m_request.type: " + requestTypeAsQStr(m_request.type) + "\n";
    QStrData += "m_request.param: " + m_request.param + "\n";
    QStrData += "\n";
    QStrData += m_pStatus->dumpAllData();
    QStrData += "\n";
    QStrData += m_pTimers->dumpAllData();

    return QStrData;
}
