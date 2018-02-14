#include "chess.h"

Chess::Chess(Clients* pClientsList, Dobot* pDobot, Chessboard* pBoardMain,
             Chessboard* pBoardRemoved, Chessboard* pBoardChenard, Websockets* pWebsockets,
             TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource)
{
    _pTimers = new ChessTimers(this);
    _pMovements = new ChessMovements(this);
    _pBot = new ChessBot(this);
    _pStatus = new ChessStatus(this);
    _pResets = new ChessResets(this) ;
    _pConditions = new ChessConditions(this);

    _pClientsList = pClientsList;
    _pDobot = pDobot;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pBoardChenard = pBoardChenard;
    _pWebsockets = pWebsockets;
    _pTCPMsgs = pTCPMsgs;
    _pUsb = _pDobot->getArduinoPointer();

    _pBoardMain->isBoardReal(B_MAIN); //todo: to sa testy

    _PlayerSource = PlayerSource;

    _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
    _request.clear();

    for (int i=1; i>=32; ++i)
        _pPiece[i] = new Piece(i);

    connect(_pUsb, SIGNAL(msgFromUsbToChess(QString)),
            this, SLOT(checkMsgFromUsb(QString)));
    connect(_pTCPMsgs, SIGNAL(msgFromTcpToChess(QString, QString)),
            this, SLOT(checkMsgFromChenard(QString, QString)));
    connect(_pWebsockets, SIGNAL(msgFromWebsocketsToChess(QString, Client*)),
            this, SLOT(checkMsgFromWebsockets(QString, Client*)));
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

void Chess::checkMsgFromWebsockets(QString QStrMsg, Client* pClient)
{
    emit this->addTextToLogPTE("received: " + QStrMsg + "\n", LOG_CORE);

    if (_pConditions->isClientRequestCanBeAccepted(QStrMsg, pClient))
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
        if (pClient == _pClientsList->getPlayer(PT_WHITE))
            this->playerWantToStartNewGame(PT_WHITE);
        else if (pClient == _pClientsList->getPlayer(PT_BLACK))
            this->playerWantToStartNewGame(PT_BLACK);
        break;
    case RT_PROMOTE_TO:
        _request.param = _pMovements->getMove().asQStr() + _request.param; // w/o break
    case RT_MOVE:
        this->manageMoveRequest(_request);
        break;
    case RT_GET_TABLE_DATA:
        this->sendDataToClient(this->getTableData(), pClient);
        break;
    case RT_GIVE_UP:
        _pResets->restartGame(ET_GIVE_UP, pClient);
        break;
    case RT_SIT_ON:
        _pClientsList->setPlayerType(pClient, playerTypeFromQStr(_request.param));
        if (_pClientsList->isGameTableOccupied())
            _pTimers->startQueueTimer();
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_STAND_UP:
        _pClientsList->clearPlayerType(_pClientsList->getClientType(pClient));
        _ChessGameStatus = GS_TURN_NONE_WAITING_FOR_PLAYERS;
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_IM:
        this->newClientName(pClient, _request);
        break;
    case RT_QUEUE_ME:
        _pClientsList->addClientToQueue(pClient);
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_LEAVE_QUEUE:
        _pClientsList->removeClientFromQueue(pClient);
        this->sendDataToAllClients(this->getTableData());
        break;
    case RT_CLIENT_LEFT:
        this->removeClient(pClient);
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

void Chess::movePieceWithManipulator(Chessboard* pRealBoard, Field* pField,
                                              VERTICAL_MOVE VertMove)
{
    if (!Chessboard::isBoardReal(pRealBoard->getBoardType()), SHOW_ERRORS) return;

    if (VertMove == VM_GRAB)
    {
        if (!this->isPieceSetOk()) return;
        if (pField->getPieceOnField(SHOW_ERRORS) == nullptr) return;

        emit this->addTextToLogPTE("Queue: grab piece " +
                                   QString::number(pField->getPieceOnField()->getNr()) +
                                   " on " + boardTypeAsQstr(pRealBoard->getBoardType()) +
                                   " from field " + pField->getNrAsQStr(), LOG_CORE);
        _pDobot->setItemInGripper(pField->getPieceOnField()->getNr());
        pRealBoard->clearField(pField);

        if (!this->isPieceSetOk()) return;
    }
    else if (VertMove == VM_PUT)
    {
        if (!this->isPieceSetOk()) return;

        emit this->addTextToLogPTE("Queue: put piece " +
                                   _pPiece[_pDobot->getItemInGripper()]->getName() +
                                   " on " + boardTypeAsQstr(pRealBoard->getBoardType()) +
                                   " on field " + pField->getNrAsQStr(), LOG_CORE);
        pRealBoard->setPieceOnField(_pPiece[_pDobot->getItemInGripper()], pField);
        _pDobot->clearGripper();

        if (!this->isPieceSetOk()) return;
    }

    Point3D xyz = pField->getLocation3D();
    _pDobot->queueMoveSequence(xyz, Piece::dMaxPieceHeight, VertMove);
}

void Chess::sendMsgToTcp(QString QStrMsg)
{
    emit this->addTextToLogPTE("Sending to tcp:" + QStrMsg + "\n", LOG_CORE);

    _pTCPMsgs->queueCmd(_PlayerSource, QStrMsg);
}

void Chess::newClientName(Client* pClient, clientRequest request)
{
    if (!_pClientsList->isClientNameExists(request.param))
    {
        _pClientsList->setClientName(pClient, request.param);
        this->sendDataToClient(this->getTableData(), pClient);
    }
    else //double login
    {
        _pClientsList->getClientSocket(request.param)->sendTextMessage("logout:doubleLogin");
        if (_pClientsList->isClientAPlayer(pClient))
            _pResets->restartGame(ET_SOCKET_LOST, pClient);
        _pClientsList->setClientName(pClient, request.param);
    }
}

void Chess::removeClient(Client* pClient)
{
    if (_pClientsList->isClientAPlayer(pClient))
    {
        emit this->addTextToLogPTE(playerTypeAsQStr(pClient->type)
                                   + " player disconnected\n", LOG_CORE);
        _pResets->restartGame(ET_SOCKET_LOST, pClient);
    }
    else //future: strange behavior with those info- seen more often then we should
        emit this->addTextToLogPTE("non-player disconnected\n", LOG_CORE);

    _pClientsList->removeClient(pClient);

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
            _pTimers->startQueueTimer();
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
        _pMovements->setMove(request.param); //no need for earlier clear
        _pMovements->doMoveSequence();
        this->sendMsgToTcp("move " + request.param);
    }
}

void Chess::continueGameplay()
{
    if (_PlayerSource == WEBSITE)
    {
        _pTimers->switchPlayersTimers();
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
