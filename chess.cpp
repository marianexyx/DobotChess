#include "chess.h"

Chess::Chess(PieceController* pPieceController, Chessboard* pBoardChenard,
             Websockets* pWebsockets, TCPMsgs* pTCPMsgs,
             COMMUNICATION_TYPE PlayerSource)
{
    _pPieceController = pPieceController;
    _pDobot = _pPieceController->getDobotPointer();
    _pBoardMain = _pPieceController->getBoardMainPointer();
    _pBoardRemoved = _pPieceController->getBoardRemovedPointer();
    _pBoardChenard = pBoardChenard;
    _pWebsockets = pWebsockets;
    _pClientsList = _pWebsockets->getClientsListPointer();
    _pTCPMsgs = pTCPMsgs;
    _pUsb = _pDobot->getArduinoPointer();

    _pBot = new ChessBot();
    _pStatus = new ChessStatus(_pPieceController, _pBoardMain, _pClientsList);
    _pMovements = new ChessMovements(_pPieceController, _pBoardMain, _pBoardRemoved);
    _pTimers = new ChessTimers(_pClientsList);
    _pConditions = new ChessConditions(_pClientsList, _pStatus);

    _PlayerSource = PlayerSource; //future: remove all arduino arm code (doesn't work no more)

    _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    _request.clear();

    connect(_pUsb, SIGNAL(msgFromUsbToChess(QString)),
            this, SLOT(checkMsgFromUsb(QString)));
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
    delete _pBot;
    delete _pStatus;
    delete _pConditions;
}

void Chess::checkMsgFromWebsockets(QString QStrMsg, int64_t n64SenderID)
{
    //future: use ID everywhere
    Client client = _pClientsList->getClient(n64SenderID);

    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    if (_pConditions->isClientRequestCanBeAccepted(QStrMsg, &client, _ChessGameStatus))
    {
        _request.type = requestTypeFromQStr(QStrMsg, SHOW_ERRORS);
        _request.param = _pConditions->extractParameterIfTypeIsInProperFormat(_request.type,
                                                                              QStrMsg);
        qDebug() << "Chess::checkMsgFromWebsockets(): accepted client request: type ="
                 << requestTypeAsQStr(_request.type) << ", param =" << _request.param;
    }
    else
    {
        qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): "
                    "client request can't be accepted";
        return;
    }

    switch(_request.type)
    {
    case RT_NEW_GAME:
        if (client == _pClientsList->getPlayer(PT_WHITE))
            this->playerWantToStartNewGame(PT_WHITE);
        else if (client == _pClientsList->getPlayer(PT_BLACK))
            this->playerWantToStartNewGame(PT_BLACK);
        else qDebug() << "ERROR: Chess::checkMsgFromWebsockets(): switch: "
                         "RT_NEW_GAME: client isn't a player";
        break;
    case RT_PROMOTE_TO:
        _request.param = _pStatus->getMove().asQStr() + _request.param; // w/o break
        _pStatus->promotePawn(_pStatus->getMove().from, _request.param.right(1));
    case RT_MOVE:
        this->manageMoveRequest(_request);
        break;
    case RT_GET_TABLE_DATA:
        this->sendDataToClient(client);
        break;
    case RT_SIT_ON:
        _pClientsList->setPlayerType(client, playerTypeFromQStr(_request.param));
        if (_pClientsList->isWholeGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        this->sendDataToAllClients();
        break;
    case RT_STAND_UP:
        if (whoseTurnFromGameStatus(_ChessGameStatus) == NO_TURN)
        {
            _pClientsList->clearPlayerType(client.type);
            _pTimers->stopQueueTimer();
            _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
            this->sendDataToAllClients();
        }
        else this->restartGame(client.type == PT_WHITE ? ET_GIVE_UP_WHITE : ET_GIVE_UP_BLACK);
        break;
    case RT_IM:
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
        this->sendDataToAllClients();
        this->updateClientsInUI();
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
    qDebug() << "Chess::checkMsgFromChenard(): ProcessedChenardMsgType ="
             << chenardMsgTypeAsQStr(ProcessedChenardMsgType);
    if (!isChenardAnswerCorrect(ProcessedChenardMsgType, QStrTcpRespond, SHOW_ERRORS)) return;

    qDebug() << "Chess::checkMsgFromChenard(): switch: case:"
             << chenardMsgTypeAsQStr(ProcessedChenardMsgType);
    switch(ProcessedChenardMsgType)
    {
    case CMT_NEW:
        this->startNewGameInCore();
        break;
    case CMT_STATUS:
    {
        _pStatus->saveStatusData(QStrTcpRespond);
        END_TYPE ET = _pStatus->getFENGameState();
        if (ET == ET_NONE)
        {
            this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_HISTORY));
            this->continueGameplay(); //future: send move confirmation to web after...
            //..."legal" response (i.e. realise blockade before next move)- is this old comment?
        }
        else if (ET == ET_NORMAL_WIN_WHITE || ET == ET_NORMAL_WIN_BLACK || ET == ET_DRAW)
            this->restartGame(ET);
        else qDebug() << "ERROR: Chess::checkMsgFromChenard(): case CMT_STATUS: unacceptable "
                         "END_TYPE parameter =" << ET;
        break;
    }
    case CMT_LEGAL:
        _pStatus->setLegalMoves(QStrTcpRespond);
        break;
    case CMT_HISTORY:
        _pStatus->setHistoryMoves(QStrTcpRespond);
        this->sendDataToAllClients();
        this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_LEGAL));
        break;
    case CMT_MOVE:
        this->sendMsgToTcp("status");
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

    if ((_pClientsList->isWholeGameTableOccupied() &&
         _pClientsList->isStartClickedByBothPlayers()) || _PlayerSource == ARDUINO || bService)
    {
        qDebug() << "Chess::playerWantToStartNewGame(): both have clicked start. "
                    "Try to start a game";
        _pTimers->stopQueueTimer();
        this->sendMsgToTcp("new");
        _pClientsList->setClientStartConfirmation(PT_WHITE, false);
        _pClientsList->setClientStartConfirmation(PT_BLACK, false);
    }
}

void Chess::tellPlayerThatHeGaveBadMove(QString QStrMsg) //move not included in answer
{
    qDebug() << "Chess::tellPlayerThatHeGaveBadMove():" << QStrMsg;
    Client player = _pClientsList->getPlayer(_pStatus->getActivePlayerType());

    this->sendDataToClient(player, AT_BAD_MOVE);
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp: " + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(_PlayerSource, QStrMsg);
}

void Chess::newClientLogged(Client& client, int64_t sqlID)
{    
    //todo: kill client if he sends his name, while he already has one...
    //... (on WWW create addition for client to not be able send his...
    //... name, if he is not logged). its propably done in conditions

    if (!_pClientsList->isClientSqlIDExists(sqlID))
    {
        qDebug() << "Chess::newClientLogged(): ID =" << sqlID
                 << ", name =" << Sql::getClientName(sqlID);
        _pClientsList->setClientSqlID(client, sqlID);
        this->sendDataToClient(client);
    }
    else //double login
    {
        qDebug() << "Chess::newClientLogged(): logout:doubleLogin";
        Client oldClient = _pClientsList->getClient(sqlID, CID_SQL);
        this->restorateGameIfDisconnectedClientAffectIt(oldClient);
        _pClientsList->setClientSqlID(oldClient, 0); //clear old client sqlID
        oldClient.socket->sendTextMessage("logout:doubleLogin");
        _pClientsList->removeClientFromList(client);
        _pClientsList->setClientSqlID(client, sqlID);
        this->updateClientsInUI();
    }

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
    _pTimers->resetGameTimers();
    //_pStatus->clearMove(); //future: move clearing must be after sending it, unless...
    //...it will be read from "history"
    _pStatus->resetStatusData();
}

void Chess::restorateGameIfDisconnectedClientAffectIt(Client& client)
{
    qDebug() << "Chess::restorateGameIfDisconnectedClientAffectIt()";

    if (_pClientsList->isClientAPlayer(client))
    {
        emit this->addTextToLogPTE(playerTypeAsQStr(client.type)
                                   + " player disconnected\n", LOG_CORE);
        if (whoseTurnFromGameStatus(_ChessGameStatus) == NO_TURN)
        {
            this->resetTableData();
            _pClientsList->clearPlayerType(client.type);
        }
        else this->restartGame(client.type == PT_WHITE ? ET_SOCKET_LOST_WHITE :
                                                         ET_SOCKET_LOST_BLACK);
    }
    else if (_pClientsList->isClientInQueue(client))
    {
        _pClientsList->removeClientFromQueue(client);
        this->sendDataToAllClients(); //queued clients need to refreshed
    }
    else emit this->addTextToLogPTE("non-player disconnected\n", LOG_CORE);
}

void Chess::sendDataToClient(Client client /*= Client*/, ACTION_TYPE AT /*= AT_NONE*/,
                             END_TYPE ET /*= ET_NONE*/)
{
    /*emit this->addTextToLogPTE("Sending msg to " + communicationTypeAsQStr(_PlayerSource)
                                + ": " + AT + "\n", LOG_CORE);*/

    if (_PlayerSource == WEBSITE)
    {
        _pWebsockets->sendMsgToClient(this->getTableData(AT, ET), client.ID);
    }
    /*else if (_PlayerSource == ARDUINO)
    {
        //future: non-harmonized types of communicates between site and arduino, so...
        //...communications got to be changed in a fly
        if (AT.contains("promote")) AT = "promote";
        else if (AT.contains(actionTypeAsQstr(AT_NEW_GAME_STARTED)))
            AT = "started";
        else if (AT.contains(actionTypeAsQstr(AT_BAD_MOVE)))
            AT = "BAD_MOVE";

        _pUsb->sendDataToUsb(AT);
    }*/
    else
        qDebug() << "ERROR: Chess::sendDataToClient(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::sendDataToAllClients(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    if (_PlayerSource == WEBSITE)
        _pWebsockets->sendMsgToAllClients(this->getTableData(AT, ET));
    else
        qDebug() << "ERROR: Chess::sendDataToAllClients(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::coreIsReadyForNewGame() //future: not best name?
{
    qDebug() << "Chess::coreIsReadyForNewGame()";

    if (_PlayerSource == WEBSITE)
    {
        if (_pClientsList->isWholeGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

        this->sendDataToAllClients(AT_RESET_COMPLITED);
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: don't start automaticly new game? require it from players?
        this->sendMsgToTcp("new");
    }
    else
        qDebug() << "ERROR: Chess::coreIsReadyForNewGame(): unknown _PlayerSource"
                    " val =" << communicationTypeAsQStr(_PlayerSource);
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
        this->sendDataToClient(_pClientsList->getPlayer(_pStatus->getActivePlayerType()),
                               AT_PROMOTE_TO_WHAT);
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
    if (_PlayerSource == WEBSITE)
    {
        _pTimers->switchPlayersTimers(_pStatus->getWhoseTurn());
        _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE :
                                                                    GS_TURN_BLACK;
        this->sendDataToAllClients(AT_CONTINUE);
    }
    /*else if (_PlayerSource == ARDUINO)
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
    }*/
    else qDebug() << "ERROR: Chess::continueGameplay(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::restartGame(END_TYPE ET)
{
    qDebug() << "Chess::restartGame():" << endTypeAsQstr(ET);

    _ChessGameStatus = GS_TURN_NONE_RESETING;

    this->resetTableData();

    this->changePlayersOnChairs();
    this->sendDataToAllClients(AT_END_GAME, ET);
    _pStatus->clearMove();

    if(_pMovements->resetPiecePositions())
        this->coreIsReadyForNewGame();

    this->sendDataToAllClients();
}

void Chess::changePlayersOnChairs()
{
    _pClientsList->clearPlayerType(PT_WHITE);
    _pClientsList->clearPlayerType(PT_BLACK);

    _pClientsList->putOnChairNextQueuedClientIfExist(PT_WHITE);
    _pClientsList->putOnChairNextQueuedClientIfExist(PT_BLACK);
}

QString Chess::getTableData(ACTION_TYPE AT /*= AT_NONE*/, END_TYPE ET /*= ET_NONE*/)
{
    if (ET > ET_NONE && AT != AT_END_GAME)
    {
        qDebug() << "ERROR: Chess::getTableData(): END_TYPE must be ET_NONE "
                    "if ACTION_TYPE isn't AT_END_GAME";
        return "";
    }

    QString QStrWhitePlayerID, QStrBlackPlayerID;
    if (_pClientsList->isPlayerChairEmpty(PT_WHITE)) QStrWhitePlayerID = "0";
    else QStrWhitePlayerID = QString::number(_pClientsList->getPlayer(PT_WHITE).sqlID);
    if (_pClientsList->isPlayerChairEmpty(PT_BLACK)) QStrBlackPlayerID = "0";
    else QStrBlackPlayerID = QString::number(_pClientsList->getPlayer(PT_BLACK).sqlID);

    QString TD = "{\"";
    TD += decToHex(TD_ACTION) + ":" + decToHex(AT + ET);
    TD += "," + decToHex(TD_WHITE_PLAYER) + ":" + QStrWhitePlayerID;
    TD += "," + decToHex(TD_BLACK_PLAYER) + ":" + QStrBlackPlayerID;
    TD += "," + decToHex(TD_GAME_STATE) + ":" + decToHex(_ChessGameStatus);
    TD += "," + decToHex(TD_WHITE_TIME) + ":" + QString::number(_pTimers->getWhiteTimeLeft(true));
    TD += "," + decToHex(TD_BLACK_TYPE) + ":" + QString::number(_pTimers->getBlackTimeLeft(true));
    TD += "," + decToHex(TD_QUEUE) + ":" + _pClientsList->getQueuedClientsList();

    if (_pTimers->isStartTimerRunning()) //todo: check from game status instead time function?
    {
        int nWhiteClickedStart = _pClientsList->isStartClickedByPlayer(PT_WHITE) ? 1 : 0;
        int nBlackClickedStart = _pClientsList->isStartClickedByPlayer(PT_BLACK) ? 2 : 0;
        int nPlayersClickedStart = nWhiteClickedStart + nBlackClickedStart;

        TD += "," + decToHex(TD_START_TIME) + ":" + QString::number(nPlayersClickedStart)
                + QString::number(_pTimers->getStartTimeLeft(true));
    }

    //todo: always return this data, even when it's empty? (return "0"?)
    if (!_pStatus->getHistoryMoves().isEmpty())
        TD += "," + decToHex(TD_HISTORY) + ":" + _pStatus->getHistoryMovesAsQStr();

    //todo: always return this data, even when it's empty? (return "0"?)
    if (_pPieceController->isAnyPawnPromoted())
        TD += "," + decToHex(TD_PROMOTIONS) + ":" +
                _pPieceController->getPromotedPawnsPositions();

    //convert string to JSON format
    TD.replace(":", "\":\"");
    TD.replace(",", "\",\"");

    TD += "\"}";

    qDebug() << "Chess::getTableData(): QStrTableData =" << TD;
    return TD;
}

void Chess::timeOutStart()
{
    _pClientsList->resetPlayersStartConfirmInfo();

    if (!_pClientsList->isStartClickedByPlayer(PT_WHITE))
    {
        _pClientsList->clearPlayerType(PT_WHITE);
        _pClientsList->putOnChairNextQueuedClientIfExist(PT_WHITE);
    }
    if (!_pClientsList->isStartClickedByPlayer(PT_BLACK))
    {
        _pClientsList->clearPlayerType(PT_BLACK);
        _pClientsList->putOnChairNextQueuedClientIfExist(PT_BLACK);
    }

    if (_pClientsList->isWholeGameTableOccupied())
        _ChessGameStatus = _pTimers->startQueueTimer();
    else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

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
