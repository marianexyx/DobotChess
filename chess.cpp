#include "chess.h"

Chess::Chess(PieceController* pPieceController, Chessboard* pBoardChenard,
             Websockets* pWebsockets, TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource)
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

    _PlayerSource = PlayerSource;

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
        this->sendDataToClient(this->getTableData(), client);
        break;
    case RT_SIT_ON:
        _pClientsList->setPlayerType(client, playerTypeFromQStr(_request.param));
        if (_pClientsList->isGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_STAND_UP:
        if (whoseTurnFromGameStatus(_ChessGameStatus) == NO_TURN)
        {
            _pClientsList->clearPlayerType(client.type);
            _pTimers->stopQueueTimer();
            _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
            this->sendDataToAllClients(this->getTableData());
        }
        else this->restartGame(ET_GIVE_UP, client.type);
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
        this->removeClientFromList(client);
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
        _pStatus->saveStatusData(QStrTcpRespond);
        if (_pStatus->getFENGameState() == ET_NONE)
        {
            this->sendMsgToTcp(chenardMsgTypeAsQStr(CMT_HISTORY));
            this->continueGameplay(); //future: send move confirmation to web after...
            //..."legal" response (i.e. realise blockade before next move)- is this old comment?
        }
        else //ET_WHIE_WON, ET_BLACK_WON, ET_DRAW
            this->restartGame(_pStatus->getFENGameState());
        break;
    case CMT_LEGAL:
        _pStatus->setLegalMoves(QStrTcpRespond);
        break;
    case CMT_HISTORY:
        _pStatus->setHistoryMoves(QStrTcpRespond);
        //todo: change site responses for endian node with parameters, instead strings
        this->sendDataToAllClients(this->getTableData());
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
            + turnTypeAsBriefQstr(_pStatus->getWhoseTurn());
    Client player = _pClientsList->getPlayer(_pStatus->getActivePlayerType());

    this->sendDataToClient(QStrMsgForActivePlayer, player);
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp:" + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(_PlayerSource, QStrMsg);
}

void Chess::newClientName(Client& client, clientRequest request)
{    
    QString QStrIncomingClientName = request.param;
    qDebug() << "Chess::newClientName():" << QStrIncomingClientName;

    if (!_pClientsList->isClientNameExists(QStrIncomingClientName))
    {
        _pClientsList->setClientName(client, QStrIncomingClientName);
        this->sendDataToClient(this->getTableData(), client);
    }
    else //double login
    {
        qDebug() << "Chess::newClientName(): logout:doubleLogin";

        Client oldClient = _pClientsList->getClient(QStrIncomingClientName);

        if (_pClientsList->isClientAPlayer(oldClient))
            this->restartGame(ET_SOCKET_LOST, oldClient.type);
        else if (_pClientsList->isClientInQueue(oldClient))
        {
            _pClientsList->removeClientFromQueue(oldClient);
            this->sendDataToAllClients(this->getTableData()); //need to refresh queued clients
        }
        else this->sendDataToClient(this->getTableData());

        _pClientsList->setClientName(oldClient, ""); //clear old client name
        oldClient.socket->sendTextMessage("logout:doubleLogin");
        _pClientsList->setClientName(client, QStrIncomingClientName); //set new client
    }
}

void Chess::removeClientFromList(Client& client)
{
    qDebug() << "Chess::removeClientFromList()";
    if (_pClientsList->isClientAPlayer(client))
    {
        emit this->addTextToLogPTE(playerTypeAsQStr(client.type)
                                   + " player disconnected\n", LOG_CORE);
        this->restartGame(ET_SOCKET_LOST, client.type);
    }
    else //future: strange behavior with those info- seen more often then we should
        emit this->addTextToLogPTE("non-player disconnected\n", LOG_CORE);

    _pClientsList->removeClientFromList(client);

    emit this->setBoardDataLabelInUI(std::to_string(_pClientsList->getClientsList().size()).c_str(),
                            BDL_SOCKETS_ONLINE);
    emit _pClientsList->showClientsList(_pClientsList->getClientsList());
}

void Chess::sendDataToClient(QString QStrMsg, Client client /* = Client */)
{
    //QString QStrMsg = QStrMsg.contains("TABLE_DATA{") ? "TABLE_DATA{...}" : QStrMsg;
    emit this->addTextToLogPTE("Sending msg to " + communicationTypeAsQStr(_PlayerSource)
                                + ": " + QStrMsg + "\n", LOG_CORE);

    if (_PlayerSource == WEBSITE)
    {
        _pWebsockets->sendMsgToClient(QStrMsg, client.ID);
    }
    else if (_PlayerSource == ARDUINO)
    {
        //future: non-harmonized types of communicates between site and arduino, so...
        //...communications got to be changed in a fly
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

QString Chess::getEndGameMsg(END_TYPE WhoWon, QString QStrTableData,
                             PLAYER_TYPE PlayerToClear /* = PT_NONE */)
{
    QString QStrMove;
    QString QStrReturnMsg;

    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        QStrMove = _pStatus->getMove().asQStr();
        //future: if TABLE_DATA is sent, then there is no need for sending "nt"
        //future: last move communicate can be wiped, cuz info about it can be found in "history"

        qDebug() << "Chess::getEndGameMsg(): move =" << QStrMove;
        QStrReturnMsg = "moveOk " + QStrMove + " " + turnTypeAsBriefQstr(NO_TURN) + " "
                + endTypeAsQstr(WhoWon) + " " + QStrTableData;
        return QStrReturnMsg;
    case ET_TIMEOUT_GAME:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        QStrReturnMsg = endTypeAsQstr(WhoWon) + playerTypeAsQStr(PlayerToClear)
                + " " + QStrTableData;
        return QStrReturnMsg;
    default:
        qDebug() << "ERROR: Chess::getEndGameMsg: unknown WhoWon val=" << WhoWon;
        return "";
    }
}

void Chess::coreIsReadyForNewGame() //future: not best name?
{
    qDebug() << "Chess::coreIsReadyForNewGame()";

    if (_PlayerSource == WEBSITE)
    {
        if (_pClientsList->isGameTableOccupied())
            _ChessGameStatus = _pTimers->startQueueTimer();
        else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

        this->sendDataToAllClients("resetComplited"); //don't add tableData- only
        //...chess data has changed during reset
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

    this->sendDataToAllClients("newOk");
}

void Chess::manageMoveRequest(clientRequest request)
{
    if (_pStatus->isMoveARequestForPromotion(request.param))
    {
        _pStatus->setMove(request.param);
        _ChessGameStatus = _pStatus->getWhoseTurn() == WHITE_TURN ? GS_TURN_WHITE_PROMOTE :
                                                                    GS_TURN_BLACK_PROMOTE;
        this->sendDataToClient("promoteToWhat", _pClientsList->getPlayer(
                                   _pStatus->getActivePlayerType()));
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
        //future: enums, not strings
        this->sendDataToAllClients("moveOk " + _pStatus->getMove().asQStr() + " " +
                                   turnTypeAsBriefQstr(_pStatus->getWhoseTurn()) + " cont");
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
    else qDebug() << "ERROR: Chess::continueGameplay(): unknown _PlayerSource val ="
                 << communicationTypeAsQStr(_PlayerSource);
}

void Chess::restartGame(END_TYPE WhoWon, PLAYER_TYPE PlayerToClear /* = PT_NONE */)
{
    //info
    QString QStrPlayer;
    if (PlayerToClear != PT_NONE)
        QStrPlayer = _pClientsList->getClientName(_pClientsList->getPlayer(PlayerToClear)) + ":";
    qDebug() << "Chess::restartGame():" << QStrPlayer << endTypeAsQstr(WhoWon);

    _ChessGameStatus = GS_TURN_NONE_RESETING;

    //reset data
    _pClientsList->resetPlayersStartConfirmInfo();
    _pTimers->resetGameTimers();
    //_pStatus->clearMove(); //future: move clearing must be after sending it, unless it will...
    //...be read from "history"
    _pStatus->resetStatusData();

    this->changePlayersOnChairs();
    this->sendDataToAllClients(this->getEndGameMsg(WhoWon, this->getTableData(), PlayerToClear));
    _pStatus->clearMove();

    if(_pMovements->resetPiecePositions())
        this->coreIsReadyForNewGame();

    this->sendDataToAllClients(this->getTableData());
}

void Chess::changePlayersOnChairs()
{
    _pClientsList->clearPlayerType(PT_WHITE);
    _pClientsList->clearPlayerType(PT_BLACK);

    _pClientsList->putOnChairNextQueuedClientIfExist(PT_WHITE);
    _pClientsList->putOnChairNextQueuedClientIfExist(PT_BLACK);
}

//example: "TABLE_DATA{\"wplr\":\"marianexyx\",\"bplr\":\"test\",\"turn\":\"bt\",\"wtime\":
// 1713531,\"btime\":879094,\"queue\":\"queueEmpty\",\"history\":\"b4 b5 Nc3 Nc6
// Nxb5 Na5 bxa5 Rb8 a6 Rb7 axb7 a6 b8=Q \",\"promoted\":\"b8:p\"}\n"
QString Chess::getTableData()
{
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + _pClientsList->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + _pClientsList->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" +  turnTypeAsBriefQstr(_pStatus->getWhoseTurn()) +
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

    if (_pPieceController->isAnyPawnPromoted())
        QStrTableData += "\",\"promoted\":\""  +
                _pPieceController->getPromotedPawnsPositions();
    //QStrTableData += "\",\"promoted\":\"b2:Q c7:q g5:N b2:Q c7:q g5:N"; //tests

    QStrTableData += "\"}";

    qDebug() << "Chess::getTableData(): QStrTableData =" << QStrTableData;
    return QStrTableData;
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

    if (_pClientsList->isGameTableOccupied())
        _ChessGameStatus = _pTimers->startQueueTimer();
    else _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;

    this->sendDataToAllClients(this->getTableData());
}

void Chess::timeOutPlayer(PLAYER_TYPE Player)
{
    this->restartGame(ET_TIMEOUT_GAME, Player);
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
