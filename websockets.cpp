#include "websockets.h"

Websockets::Websockets(Chessboard *pChessboard, quint16 port, QObject *parent):
    QObject(parent),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        qDebug() << "WebSocket server listening on port" << port;
        emit addTextToConsole("WebSocket server listening on port" + port, LOG_WEBSOCKET);
    }

    _pChessboard = pChessboard;
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();
    //qDeleteAll(m_clients.begin(), m_clients.end()); //todo: nie wiem jak to usunąć. póki mam
    //jednen obiekt/ramię to nie jest mi to potrzebne
}

void Websockets::onNewConnection()
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClientSocket(pSocket);
    emit addTextToConsole("New connection \n", LOG_WEBSOCKET);

    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
    emit showClientsList(m_clients);
}

//TODO: Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
//póki nie zmieniam wersji to może tak zostać
//TODO2: obsuga komend serwisowych przez websockety

void Websockets::resetPlayersStartConfirmInfo()
{
    qDebug() << "Websockets::resetPlayersStartConfirmInfo()";
    if (!this->isStartClickedByPlayer(PT_WHITE))
           this->setClientState(PT_WHITE, false);
    if (!this->isStartClickedByPlayer(PT_BLACK))
        this->setClientState(PT_BLACK, false);
}

QString Websockets::getTableDataAsJSON()
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt","wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + this->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + this->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" + _pChessboard->getStrWhoseTurn() +
            "\",\"wtime\":" + QString::number(_pChessboard->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pChessboard->getBlackTimeLeft()) +
            ",\"queue\":\"" + this->getQueuedClientsList();

    if (_pChessboard->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart = this->isStartClickedByPlayer(PT_WHITE) ? "w" : "x";
        QString QStrBlackClickedStart = this->isStartClickedByPlayer(PT_BLACK) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pChessboard->getStartTimeLeft());
    }

    if (!_pChessboard->getHisotyMoves().isEmpty())
        QStrTableData += "\",\"history\":\"" + _pChessboard->getHisotyMovesAsQStr();

    QStrTableData += "\"}";

    qDebug() << "Websockets::getTableDataAsJSON(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Websockets::receivedMsg(QString QStrWbstMsgToProcess)
{    
    if (QStrWbstMsgToProcess != "keepConnected")
    {
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWbstMsgToProcess;
    }

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (QStrWbstMsgToProcess == "keepConnected")
    {
        emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    }
    else if (QStrWbstMsgToProcess == "newGame" || QStrWbstMsgToProcess == "new")
    {
        QString QStrWhoSent = "";
        if (pClient == this->getPlayerSocket(PT_WHITE))  QStrWhoSent = "WHITE";
        else if (pClient == this->getPlayerSocket(PT_BLACK)) QStrWhoSent = "BLACK";

        this->sendToChess(QStrWbstMsgToProcess + " " + QStrWhoSent);
    }
    else if (QStrWbstMsgToProcess.left(4) == "move") { this->sendToChess(QStrWbstMsgToProcess); }
    else if (QStrWbstMsgToProcess == "getTableDataAsJSON") pClient->sendTextMessage(this->getTableDataAsJSON());
    else if (QStrWbstMsgToProcess == "giveUp")
    {
        if (pClient) //todo: zrozumieć to i dać w razie czego więcej tych warunków tam gdzie są...
            //...QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        {
            //todo: praktycznie jest to powtórzenie tego co jest w socketDisconnected
            if (pClient == this->getPlayerSocket(PT_WHITE))
            {
                this->addTextToConsole("Player has logouted. New white: WHITE\n", LOG_WEBSOCKET);
                this->endOfGame(ET_GIVE_UP, pClient);
                emit setBoardDataLabels(this->getPlayerName(PT_WHITE), BDL_WHITE_NAME);

                Q_FOREACH (Clients client, m_clients)
                    client.socket->sendTextMessage("givedUpWhite " + this->getTableDataAsJSON());
                //todo: tu się wyślą wszystkie ifnromacje, ale nie opcja startu gry- czy to dobrze?

                emit MsgFromWebsocketsToChess("reset");
            }
            else if (pClient == this->getPlayerSocket(PT_BLACK))
            {
                qDebug() << "Websockets::receivedMsg(): logout black player";
                this->addTextToConsole("Player has logouted. New black: BLACK\n", LOG_WEBSOCKET);
                this->endOfGame(ET_GIVE_UP, pClient);
                emit setBoardDataLabels(this->getPlayerName(PT_BLACK), BDL_BLACK_NAME);

                Q_FOREACH (Clients client, m_clients)
                    client.socket->sendTextMessage("givedUpBlack " + this->getTableDataAsJSON());

                emit MsgFromWebsocketsToChess("reset");
            }
            else qDebug() << "ERROR: Websockets::receivedMsg(): non-player tried to logout (hacker?)";
        }
        else qDebug() << "ERROR: Websockets::receivedMsg(): giveUp: !isset pClient";
    }
    else if (QStrWbstMsgToProcess.left(5) == "sitOn")
    {
        if (this->getClientName(pClient) == "-1")
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): sitOn: client not loggedIn";
            return;
        }

        QString QStrPlayerType = QStrWbstMsgToProcess.mid(5);
        PLAYERS_TYPES playerChair;
        if (QStrPlayerType == "White") playerChair = PT_WHITE;
        else if (QStrPlayerType == "Black") playerChair = PT_BLACK;
        else
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): sitOn: wrong sufix:" << QStrPlayerType;
            return;
        }

        _pChessboard->setWhoseTurn(NO_TURN);
        if (!this->isPlayerChairEmpty(playerChair))
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): client wanted to sit on occupied chair. client name:"
                     << this->getClientName(pClient) << ", player name:" <<
                        this->getPlayerName(playerChair);
            return;
        }
        else
            this->setPlayerType(pClient, playerChair);

        //todo: funkcje informacyjne powinny wykonywać się wewnątrz funkcji zmieniającej nazwy
        qDebug() << "Websockets::receivedMsg(): new" << QStrPlayerType << "player:" <<
                    this->getPlayerName(playerChair);
        emit this->addTextToConsole("New white player: " + this->getPlayerName(playerChair) +
                                    "\n", LOG_WEBSOCKET);
        emit setBoardDataLabels(this->getPlayerName(playerChair),
                                playerChair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();

        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else if (QStrWbstMsgToProcess == "standUp")
    {
        if (pClient == this->getPlayerSocket(PT_WHITE) || pClient == this->getPlayerSocket(PT_BLACK))
        {
            PLAYERS_TYPES chairToClear = (pClient == this->getPlayerSocket(PT_WHITE)) ? PT_WHITE : PT_BLACK;

            //funckje są w tym endtype nieczytelne
            this->endOfGame(ET_STAND_UP, pClient);

            QString QStrPlayer = (chairToClear == PT_WHITE) ? "white" : "black";
            qDebug() << "Websockets::receivedMsg(): new" << QStrPlayer << "player:" <<
                        this->getPlayerName(chairToClear);
            emit this->addTextToConsole("New " + QStrPlayer + " player: " + this->getPlayerName(chairToClear) +
                                        "\n", LOG_WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerName(chairToClear),
                                    chairToClear == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableDataAsJSON());

            emit MsgFromWebsocketsToChess("reset");
        }
        else
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): non-player client wanted to leave chair. client name:"
                     << this->getClientName(pClient);
            return;
        }
    }
    else if (QStrWbstMsgToProcess.left(3) == "im ")
    {
        QString QStrName = QStrWbstMsgToProcess.mid(3);
        if (this->isClientNameExists(QStrName))
        {
            this->getClientSocket(QStrName)->sendTextMessage("logout:doubleLogin");
            this->endOfGame(ET_SOCKET_LOST, pClient);
            this->setClientName(pClient, QStrName);
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableDataAsJSON());
            //todo: brak info o tym dlaczego gra się zakończyła
        }
        else
        {
            this->setClientName(pClient, QStrName);
            pClient->sendTextMessage(this->getTableDataAsJSON());
        }
    }
    else if (QStrWbstMsgToProcess.left(9) == "promoteTo") { this->sendToChess(QStrWbstMsgToProcess); }
    else if (QStrWbstMsgToProcess == "queueMe")
    {
        this->addClientToQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else if (QStrWbstMsgToProcess == "leaveQueue")
    {
        this->removeClientFromQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else  qDebug() << "ERROR: Websockets::receivedMsg(): unknown parameter:" << QStrWbstMsgToProcess;

    emit showClientsList(m_clients);
}

void Websockets::sendMsg(QString QStrWsMsg) //todo: ta funkcja to syf
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", LOG_WEBSOCKET);

    if (QStrWsMsg == "newOk" || QStrWsMsg.left(8) == "history ")
    {
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg.left(6) == "moveOk" || QStrWsMsg.left(7) == "badMove")
    {
        if (QStrWsMsg.left(7) == "badMove" || QStrWsMsg.right(7) == "promote")
        {
            if (_pChessboard->getWhoseTurn() == WHITE_TURN && this->getPlayerSocket(PT_WHITE) != nullptr)
            {
                qDebug() << "Sending to site" << QStrWsMsg;
                this->getPlayerSocket(PT_WHITE)->sendTextMessage(QStrWsMsg);
            }
            else if (_pChessboard->getWhoseTurn() == BLACK_TURN && this->getPlayerSocket(PT_BLACK) != nullptr)
            {
                qDebug() << "Sending to site" << QStrWsMsg;
                this->getPlayerSocket(PT_BLACK)->sendTextMessage(QStrWsMsg);
            }
            else
                qDebug() << "ERROR: Websockets::sendMsg: uknown/wrong turn:" << _pChessboard->getWhoseTurn();
        }
        else
        {
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(QStrWsMsg);
        }
    }
    else if (QStrWsMsg.left(8) == "promoted" || QStrWsMsg == "reseting" || QStrWsMsg == "resetComplited")
    {
        if (QStrWsMsg == "resetComplited")
        {
            //todo: muszę tu chyba znowu pamiętać o tym, żeby zmieniać nazwy graczom w formie. ...
            //...niech to się dzieje jakoś automatycznie

            if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
                _pChessboard->startQueueTimer(); //todo: to też
            QStrWsMsg = QStrWsMsg + " " + this->getTableDataAsJSON();
        }
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg == "timeOutWhite" || QStrWsMsg == "timeOutBlack")
    {
        //todo: ogarnąć w końcu funkcję czyszczenia danych przy końcach...
        //...gry- w sumie Websockets::endOfGame daje chyba radę- ogarnąć na spokojnie
        this->endOfGame(QStrWsMsg.right(5) == "White" ? ET_WHIE_WON : ET_BLACK_WON);
        //todo: doklejać info z którego wiemy że nadszedł timeOut (najlepiej jako argument do getTableDataAsJSON)
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else if (QStrWsMsg == "timeOutStartQueue" )
    {
        this->endOfGame(ET_TIMEOUT_START);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else qDebug() << "ERRROR: uknown Websockets::sendMsg() parameter:" << QStrWsMsg;
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    emit this->addTextToConsole("Sending to 'chess' class: " + QsMsgForChessClass + "\n", LOG_WEBSOCKET);
    emit this->MsgFromWebsocketsToChess(QsMsgForChessClass);
}

void Websockets::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) //todo: co to??? czyżby to że sprawdzam czy udało mi się zrzutować obiekt...
        //... z powyższej linjki?
    {
        if (pClient == this->getPlayerSocket(PT_WHITE))
        {
            this->endOfGame(ET_SOCKET_LOST, pClient);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableDataAsJSON());

            emit MsgFromWebsocketsToChess("reset");
        }
        else if (pClient == this->getPlayerSocket(PT_BLACK))
        {
            this->endOfGame(ET_SOCKET_LOST, pClient);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableDataAsJSON());

            emit MsgFromWebsocketsToChess("reset");
        }
        //todo: wyświetla mi się disconnect w dziwnym miejscu
        else
        {
            qDebug() << "non-player disconnected";
            emit addTextToConsole("non-player disconnected\n", LOG_WEBSOCKET);
        }

        pClient->deleteLater();

        qDebug() << "Websockets::socketDisconnected(): remove client from list";
        for(int i = 0; i < m_clients.count(); ++i)
        { //nie umiem obsłużyć removeAll na structurach
          if(m_clients.at(i).socket == pClient)
          {
            m_clients.removeAt(i);
            break;
          }
        }
    }

    emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    emit showClientsList(m_clients);
}

void Websockets::msgFromChessboardToWebsockets(QString QStrWsMsg) //todo: niepotrzebne to przejście
{
    this->sendMsg(QStrWsMsg);
}

void Websockets::replaceClientOnChairWithQueuedPlayerIfExist(PLAYERS_TYPES chair, LOG formMsg)
{
    this->clearPlayerType(chair);

    if (this->getQueuedClientsList() != "queueEmpty")
    {
        Clients nextQueuedClient = this->getClient(this->getNextQueuedClientSocket());
        this->removeClientFromQueue(nextQueuedClient.socket);
        this->setPlayerType(nextQueuedClient.socket, chair);
        this->setClientState(nextQueuedClient.socket, false);

        emit this->setBoardDataLabels(this->getPlayerName(chair),
                                      chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        qDebug() << "Websockets::replaceClientOnChairWithQueuedPlayerIfExist(): new" <<
                    playerTypeAsQStr(chair) << "player name =" << this->getPlayerName(chair);
        emit this->addTextToConsole("New " + playerTypeAsQStr(chair) + " player: " +
                                    this->getPlayerName(chair) + "\n", formMsg);
    }
}

//todo: funkcja w sumie nic nie mówi o tym co dokładnie robi. i znajduje si w zlej klasie
void Websockets::endOfGame(END_TYPE EndType, QWebSocket *playerToClear)
{
    QString QStrPlayer = "";
    if (playerToClear != nullptr) QStrPlayer = this->getClientName(playerToClear);
    qDebug() << "Websockets::endOfGame():" << QStrPlayer << ":" << endTypeAsQstr(EndType);

    //todo: zastanowić się na spokojnie jakie czyszczenia jeszcze tu upchać
    //todo: ...po czym zamknąć je w przystępnej funkcji (w innej klasie)
    this->resetPlayersStartConfirmInfo();
    _pChessboard->stopQueueTimer();
    _pChessboard->stopBoardTimers();
    this->clearBothPlayersStates();
    _pChessboard->setWhoseTurn(NO_TURN);
    _pChessboard->clearLegalMoves();
    _pChessboard->clearHistoryMoves();
    _pChessboard->clearBoard();

    switch(EndType)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        this->replaceClientOnChairWithQueuedPlayerIfExist(PT_WHITE);
        this->replaceClientOnChairWithQueuedPlayerIfExist(PT_BLACK);
        break;

    case ET_STAND_UP:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (this->getClientType(playerToClear) == PT_WHITE)
            this->replaceClientOnChairWithQueuedPlayerIfExist(PT_WHITE);
        else if (this->getClientType(playerToClear) == PT_BLACK)
            this->replaceClientOnChairWithQueuedPlayerIfExist(PT_BLACK);
        else qDebug() << "ERROR: Websockets::endOfGame(): wrong player type:" << EndType;
        break;

    case ET_TIMEOUT_START:
        if (!this->isStartClickedByPlayer(PT_WHITE))
            this->replaceClientOnChairWithQueuedPlayerIfExist(PT_WHITE);
        if (!this->isStartClickedByPlayer(PT_BLACK))
            this->replaceClientOnChairWithQueuedPlayerIfExist(PT_BLACK);

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();
        break;
    case ET_TIMEOUT_WHITE:
    case ET_TIMEOUT_BLACK:
        //todo
        break;

    default:
        qDebug() << "ERROR: Websockets::endOfGame: unknown EndType val=" << EndType;
        break;

        //todo: sprawdzić czy zresetowałem inne dane: zegary, tury, planszę fizyczną/ w pamięci itd
    }
}

void Websockets::newClientSocket(QWebSocket *playerSocket)
{
    Clients newClient;
    newClient.socket = playerSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    //todo: sprawdzać czy któreś dane się w liście nie powtarzają
    //todo: zabezpieczać się przed mozliwością nadawania kolejki jeżeli gracz jest w grze i na odwrót

    m_clients << newClient;
}

void Websockets::setClientName(QWebSocket *playerSocket, QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.name = name;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientName: new name =" << m_clients.at(nClientpos).name;
            }
            else qDebug() << "ERROR: Websockets::setClientName: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    Clients client;
    qDebug() << "ERROR: Websockets::setClientName: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(playerSocket) =" << typeid(playerSocket).name();
}

void Websockets::setPlayerType(QWebSocket *playerSocket, PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.type = type;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setPlayerType: new player type =" <<
                            playerTypeAsQStr(m_clients.at(nClientpos).type);
            }
            else qDebug() << "ERROR: Websockets::setPlayerType: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    Clients client;
    qDebug() << "ERROR: Websockets::setPlayerType: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(playerSocket) =" << typeid(playerSocket).name();

}

void Websockets::clearPlayerType(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                Clients changedClient = client;
                changedClient.type = PT_NONE;
                changedClient.isStartClickedByPlayer = false;

                int nClientpos = m_clients.indexOf(client);
                if (nClientpos >= 0 && nClientpos < m_clients.size())
                {
                    m_clients.replace(nClientpos, changedClient);
                    qDebug() << "Websockets::clearPlayerType";
                }
                else qDebug() << "ERROR: Websockets::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
            else qDebug() << "WARNING: Websockets::clearPlayerType- client.type not found";
        }
    }
    else qDebug() << "WARNING: Websockets::clearPlayerType- already clear";
}

void Websockets::setClientState(QWebSocket *playerSocket, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Websockets::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientState- client not found";
}

void Websockets::setClientState(PLAYERS_TYPES type, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Websockets::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientState- client not found";
}

void Websockets::addClientToQueue(QWebSocket *playerSocket)
{
    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue > maxQueue)
            maxQueue = client.queue;
    }
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::addClientToQueue: new queue =" << m_clients.at(nClientpos).queue;
            }
            else qDebug() << "ERROR: Websockets::addClientToQueue: iteration error. iter val =" << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Websockets::removeClient(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            if(!m_clients.removeOne(client))
                qDebug() << "ERROR: Websockets::removeClient- client not found";;
            return;
        }
    }
}

void Websockets::removeClientFromQueue(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.queue = -1; //todo: poza zakresem unsigned

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
                m_clients.replace(nClientpos, changedClient);
            else qDebug() << "ERROR: Websockets::removeClientFromQueue: iteration error. iter val ="
                          << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Websockets::clearBothPlayersStates()
{
    this->setClientState(PT_WHITE, false);
    this->setClientState(PT_BLACK, false);
}

Clients Websockets::getClient(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
            return client;
    }
    qDebug() << "ERROR: Websockets::getClient(): client not found";
    Clients errorClient;
    return errorClient; //todo: oszukiwanie kompilatora- powinny tu być wyjątki
}

QWebSocket *Websockets::getClientSocket(QString playerName)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == playerName)
            return client.socket;
    }
    return nullptr;
}

QString Websockets::getClientName(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
            return client.name;
    }
    qDebug() << "ERROR: Websockets::getClientName(): client not found";
    return "";
}

QWebSocket *Websockets::getNextQueuedClientSocket()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue >= 0)
            minQueue = client.queue;
    }
    qDebug() << "Websockets::getNextQueuedClientSocket(): minQueue =" << minQueue;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue == minQueue)
        {
            qDebug() << "Websockets::getNextQueuedClientSocket(): client.name =" <<
                        client.name;
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getNextQueuedClientSocket(): client not found";
    return nullptr;
}

QString Websockets::getQueuedClientsList()
{
    QString QStrQueuedClients;
    QStrQueuedClients.clear();
    int64_t maxQueue = 0;
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    int64_t lastBigggestQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue > 0)
            minQueue = client.queue;
    }
    int nNumberOFClients = m_clients.size();
    int nLoopBreakingCounter = 0;
    if (minQueue != 0 && minQueue != std::numeric_limits<int64_t>::max())
    {
        do
        {
            Q_FOREACH (Clients client, m_clients)
            {
                if (client.queue > maxQueue && client.queue < lastBigggestQueue)
                    maxQueue = client.queue;
            }

            lastBigggestQueue = maxQueue;

            Q_FOREACH (Clients client, m_clients)
            {
                if (client.queue == maxQueue)
                {
                    QStrQueuedClients = client.name + "," + QStrQueuedClients;
                    maxQueue = 0;
                }
            }
            if (nLoopBreakingCounter > nNumberOFClients + 1)
            {
                qDebug() << "ERROR: Websockets::getQueuedClientsList: incorrect loop break. nLoopBreakingCounter ="
                         << nLoopBreakingCounter << ", nNumberOFClients =" << nNumberOFClients+1;
                break;
            }
            nLoopBreakingCounter++;
        } while (lastBigggestQueue != minQueue);
    }
    if (QStrQueuedClients.isEmpty()) QStrQueuedClients = "queueEmpty";
    else QStrQueuedClients = QStrQueuedClients.left(QStrQueuedClients.length() - 1); //usuń ostatni przecinek

    return QStrQueuedClients;
}

/*void Websockets::testQueuedClients()
{
    //for (int i=1; i<10; i++)
    for (int i=10; i>1; i--)
    {
        Clients testClient;
        testClient.name = "test" + QString::number(i);
        testClient.queue = i;
        qDebug() << "client" << testClient.name << "queue =" << testClient.queue;
        testClient.socket = nullptr;
        testClient.isStartClickedByPlayer = false;
        testClient.type = PT_NONE;

        m_clients << testClient;
    }

    qDebug() << "Websockets::testQueuedClients(): m_clients.size =" << m_clients.size();
    QString QStrQueuedClients = this->getQueuedClientsList();
    qDebug() << "Websockets::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

PLAYERS_TYPES Websockets::getClientType(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
            return client.type;
    }
    qDebug() << "ERROR: Websockets::getClientType(): socket not found";
    return PT_NONE;
}

bool Websockets::isPlayerChairEmpty(PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
            return false;
    }
    return true;
}

int64_t Websockets::getQueuedClient(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return client.queue;
    }
    qDebug() << "ERROR: Websockets::getQueuedClient(): client not found";
    return -1;
}

int64_t Websockets::getQueuedClient(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
            return client.queue;
    }
    qDebug() << "ERROR: Websockets::getQueuedClient(): client not found";
    return -1;
}

QWebSocket *Websockets::getPlayerSocket(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.socket;
        }
        return nullptr;
    }
    else
    {
        qDebug() << "ERROR: Websockets::getPlayerSocket- wrong parameter:"
                  << type;
        return nullptr;
    }
}

bool Websockets::isStartClickedByPlayer(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qDebug() << "ERROR: Websockets::isStartClickedByPlayer- wrong parameter:"
                  << type;
        return false;
    }
}

QString Websockets::getPlayerName(PLAYERS_TYPES type) //todo: źle ta funkcja jest zrobiona?
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE)) return "WHITE";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK)) return "BLACK";
    else
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.name;
        }
        qDebug() << "ERROR: Websockets::getPlayerName- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Websockets::isClientInQueue(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            if (client.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Websockets::isClientInQueue(): client not found";
    return false;
}

bool Websockets::isClientNameExists(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return true;
    }
    return false;
}

int Websockets::getAmountOfQueuedClients()
{
    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (maxQueue < client.queue && client.queue > 0)
            maxQueue = client.queue;
    }

    if (maxQueue > 0)
    {
        int64_t minQueue = std::numeric_limits<int64_t>::max();
        Q_FOREACH (Clients client, m_clients)
        {
            if (minQueue > client.queue && client.queue > 0)
                minQueue = client.queue;
        }
        int nAmountOfQueuedClients = 1 + maxQueue - minQueue;

        return nAmountOfQueuedClients;
    }
    else return 0;
}
