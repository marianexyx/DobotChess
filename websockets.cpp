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
        emit addTextToConsole("WebSocket server listening on port" + port, WEBSOCKET);
    }

    _pChessboard = pChessboard;
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();
    //qDeleteAll(m_clients.begin(), m_clients.end()); //todo: nie wiem jak to usunąć. póki mam
    //jednen obiekt/ramię to nie jest mi to potrzebne
}

void Websockets::onNewConnection() //nowe połączenia
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClientSocket(pSocket);
    emit addTextToConsole("New connection \n", WEBSOCKET);

    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
}

//TODO: Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
//póki nie zmieniam wersji to może tak zostać
//TODO2: komendy serwisowe do wejdą tutaj- jak to się ma do nich?

void Websockets::sendTableDataToWeb(QWebSocket *pClient, bool bCheckPlayers)
{
    if (bCheckPlayers)
    {
        if (this->getClientStateByType(PT_WHITE) != CS_CLICKED_START)
            this->clearPlayerType(PT_WHITE);
        if (this->getClientStateByType(PT_BLACK) != CS_CLICKED_START)
            this->clearPlayerType(PT_BLACK);
    }

    QString QStrTableData =
            this->getPlayerNameByType(PT_WHITE) + " " +
            this->getPlayerNameByType(PT_BLACK) + " " +
            _pChessboard->getStrWhoseTurn() + " " +
            this->getQueuedClients();
    emit addTextToConsole("sent: checkedTableData " + QStrTableData + "\n", WEBSOCKET);
    pClient->sendTextMessage("checkedTableData " + QStrTableData);
    qDebug() << "Websockets sent: checkedTableData " << QStrTableData;
}

void Websockets::receivedMsg(QString QStrWsMsg)
{
    if (QStrWsMsg != "keepConnected")
    {
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWsMsg;
        emit addTextToConsole("received: " + QStrWsMsg + "\n", WEBSOCKET);
    }

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (QStrWsMsg == "keepConnected") {  qDebug() <<"connectionOnline"; }
    else if (QStrWsMsg == "newGame")
    {
        QString QStrWhoSent = "";
        if (pClient == this->getPlayerSocket(PT_WHITE))  QStrWhoSent = "WHITE";
        else if (pClient == this->getPlayerSocket(PT_BLACK)) QStrWhoSent = "BLACK";

        this->sendToChess(QStrWsMsg + " " + QStrWhoSent);
    }
    else if (QStrWsMsg.left(4) == "move") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "reset") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg.left(5) == "check")
    {
        QStrWsMsg = QStrWsMsg.mid(6);
        if (QStrWsMsg == "whitePlayer")
        {
            emit addTextToConsole("sent: checkedWhite " + this->getPlayerNameByType(PT_WHITE) + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedWhite " + this->getPlayerNameByType(PT_WHITE));
            qDebug() << "Websockets sent: checkedWhite " << this->getPlayerNameByType(PT_WHITE);
        }
        else if (QStrWsMsg == "blackPlayer")
        {
            emit addTextToConsole("sent: checkedBlack " + this->getPlayerNameByType(PT_BLACK) + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedBlack " + this->getPlayerNameByType(PT_BLACK));
            qDebug() << "Websockets sent: checkedBlack " << this->getPlayerNameByType(PT_BLACK);
        }
        else if (QStrWsMsg == "whoseTurn")
        {
            emit addTextToConsole("sent: checkedTurn " + _pChessboard->getStrWhoseTurn() + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedTurn " + _pChessboard->getStrWhoseTurn() );
            qDebug() << "Websockets sent: checkedTurn " << _pChessboard->getStrWhoseTurn();
        }
        else if  (QStrWsMsg == "tableData")
        {
            this->sendTableDataToWeb(pClient);
        }
        else  qDebug() << "ERROR: Websockets::receivedMsg(): wrong check parameter: " << QStrWsMsg;
    }
    else if (QStrWsMsg.left(6) == "change")
    {
        QStrWsMsg = QStrWsMsg.mid(7);
        QString QStrPlayerType = QStrWsMsg.left(11);
        QString QStrPlayerName = QStrWsMsg.mid(12);

        _pChessboard->setWhoseTurn(NO_TURN);

        if (QStrPlayerType == "whitePlayer")
        {
            if (QStrPlayerName == "WHITE")
                this->clearPlayerType(PT_WHITE);
            else
                this->setPlayerType(pClient, PT_WHITE);

            this->addTextToConsole("New white player: " + this->getPlayerNameByType(PT_WHITE) + "\n", WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

            QString QStrNewWhite = "newWhite " + this->getPlayerNameByType(PT_WHITE);
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(QStrNewWhite);

            if (QStrPlayerName == "WHITE") emit MsgFromWebsocketsToChess("reset");
        }
        else if (QStrPlayerType.left(11) == "blackPlayer")
        {
            if (QStrPlayerName == "BLACK")
                this->clearPlayerType(PT_BLACK);
            else
                this->setPlayerType(pClient, PT_BLACK);

            this->addTextToConsole("New black player: " + this->getPlayerNameByType(PT_BLACK) + "\n", WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

            QString QStrNewBlack = "newBlack " + this->getPlayerNameByType(PT_BLACK);
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(QStrNewBlack);

            if (QStrPlayerName == "BLACK") emit MsgFromWebsocketsToChess("reset");
        }
        else  qDebug() << "ERROR: Websockets::receivedMsg(): wrong change parameter: " << QStrWsMsg;

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();
    }
    else if (QStrWsMsg.left(3) == "im ")
    {
        this->setClientName(pClient, QStrWsMsg.mid(3));
        this->sendTableDataToWeb(pClient);
    }
    else if (QStrWsMsg.left(9) == "promoteTo") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "queueMe")
    {
        this->addClientToQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage("updateQueue " + this->getQueuedClients());
    }
    else if (QStrWsMsg == "leaveQueue")
    {
        this->removeClientFromQueueBySocket(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage("updateQueue " + this->getQueuedClients());
    }
    else  qDebug() << "ERROR: Websockets::receivedMsg(): unknown parameter:" << QStrWsMsg;
}

void Websockets::sendMsg(QString QStrWsMsg)
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", WEBSOCKET);

    if (QStrWsMsg == "newOk")
    {
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage("newOk");
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
    else if (QStrWsMsg.left(8) == "promoted" || QStrWsMsg == "reseting" || QStrWsMsg == "ready" ||
             QStrWsMsg == "timeOutWhite" || QStrWsMsg == "timeOutBlack")
    {
        if (QStrWsMsg == "reseting" || QStrWsMsg == "ready")
            _pChessboard->setWhoseTurn(NO_TURN); //todo: niech tym zajmie się jakaś funckja po drodze od tego
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg == "timeOutStartQueue" )
    {
        Q_FOREACH (Clients client, m_clients)
            this->sendTableDataToWeb(client.socket, true);
            //pNextClient->sendTextMessage("newOk");
    }
    else qDebug() << "ERRROR: uknown Websockets::sendMsg() parameter:" << QStrWsMsg;
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    emit this->addTextToConsole("Sending to 'chess' class: " + QsMsgForChessClass + "\n", WEBSOCKET);
    emit this->MsgFromWebsocketsToChess(QsMsgForChessClass);
}

void Websockets::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient)
    {
        if (pClient == this->getPlayerSocket(PT_WHITE))
        {
            _pChessboard->setWhoseTurn(NO_TURN);
            this->clearPlayerType(PT_WHITE);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

            this->clearPlayerType(PT_WHITE);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage("newWhite WHITE");

            qDebug() << "newWhite: WHITE";
            this->addTextToConsole("New white player: WHITE\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        else if (pClient == this->getPlayerSocket(PT_BLACK))
        {
            _pChessboard->setWhoseTurn(NO_TURN);
            this->clearPlayerType(PT_BLACK);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

            this->clearPlayerType(PT_BLACK);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage("newBlack BLACK");

            qDebug() << "newBlack: BLACK";
            this->addTextToConsole("New black player: BLACK\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        //todo: wyświetla mi się disconnect w dziwnym miejscu
        else emit addTextToConsole("non-player disconnected\n", WEBSOCKET);

        for(int i = 0; i < m_clients.count(); ++i)
        { //nie umiem obsłużyć removeAll na structurach
          if(m_clients.at(i).socket == pClient)
          {
            m_clients.removeAt(i);
            break;
          }
        }

        pClient->deleteLater();
    }
    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
}

void Websockets::msgFromChessboardToWebsockets(QString QStrWsMsg)
{
    this->sendMsg(QStrWsMsg);
}

void Websockets::newClientSocket(QWebSocket *playerSocket)
{
    Clients newClient;
    newClient.socket = playerSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.state = CS_NONE;
    newClient.type = PT_NONE;

    //todo: sprawdzać czy któreś dane się w liście nie powtarzają

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
                qDebug() << "Websockets::setPlayerType: new type =" << playerTypeAsQStr(m_clients.at(nClientpos).type);
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
                changedClient.state = CS_NONE;

                int nClientpos = m_clients.indexOf(client);
                if (nClientpos >= 0 && nClientpos < m_clients.size())
                {
                    m_clients.replace(nClientpos, changedClient);
                    qDebug() << "Websockets::clearPlayerType: new type =" << playerTypeAsQStr(m_clients.at(nClientpos).type);
                }
                else qDebug() << "ERROR: Websockets::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
            else qDebug() << "WARNING: Websockets::clearPlayerType- client.type not found";
        }
    }
    else qDebug() << "WARNING: Websockets::clearPlayerType- already clear";
}

void Websockets::setClientStateBySocket(QWebSocket *playerSocket, CLIENT_STATE state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.state = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientStateBySocket: new state =" << m_clients.at(nClientpos).state;
            }
            else qDebug() << "ERROR: Websockets::setClientStateBySocket: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientStateBySocket- client not found";
}

void Websockets::setClientStateByType(PLAYERS_TYPES type, CLIENT_STATE state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            Clients changedClient = client;
            changedClient.state = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientStateByType: new state =" << m_clients.at(nClientpos).state;
            }
            else qDebug() << "ERROR: Websockets::setClientStateByType: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientStateByType- client not found";
}

void Websockets::addClientToQueue(QWebSocket *playerSocket)
{
    uint64_t maxQueue = 0;
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

            return;
        }
    }
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

void Websockets::removeClientFromQueueBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.queue = -1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
            }
            else qDebug() << "ERROR: Websockets::removeClientFromQueueBySocket: iteration error. iter val ="
                          << nClientpos;

            return;
        }
    }
}

Clients Websockets::getClientBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client;
        }
    }
    qDebug() << "ERROR: Websockets::getClientBySocket(): client not found";
    Clients errorClient;
    return errorClient; //todo: oszukiwanie kompilatora- powinny tu być wyjątki
}

QWebSocket *Websockets::getClientSocketByName(QString playerName)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == playerName)
        {
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getClientSocketByName(): client not found";
    return nullptr;
}

QString Websockets::getClientNameBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.name;
        }
    }
    qDebug() << "ERROR: Websockets::getClientNameBySocket(): client not found";
    return "";
}

QWebSocket *Websockets::getNextQueuedClientsSocket()
{
    uint64_t minQueue = std::numeric_limits<uint64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue < minQueue && client.queue != 0)
            minQueue = client.queue;
    }
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue == minQueue)
        {
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getNextQueuedClientsSocket(): client not found";
    return nullptr;
}

QString Websockets::getQueuedClients()
{
    QString QStrQueuedClients;
    QStrQueuedClients.clear();
    uint64_t maxQueue = 0;
    uint64_t minQueue = std::numeric_limits<uint64_t>::max();
    uint64_t lastBigggestQueue = std::numeric_limits<uint64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue < minQueue && client.queue != 0)
            minQueue = client.queue;
    }
    int nNumberOFClients = m_clients.size();
    int nLoopBreakingCounter = 0;
    do
    {
        nLoopBreakingCounter++;
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.queue > maxQueue && client.queue < lastBigggestQueue)
                maxQueue = client.queue;
        }
        Q_FOREACH (Clients client, m_clients)
        {
            lastBigggestQueue = maxQueue;
            if (client.queue == maxQueue)
            {
                QStrQueuedClients = client.name + "," + QStrQueuedClients;
                maxQueue = 0;
                break;
            }
        }
        if (nLoopBreakingCounter > nNumberOFClients + 1)
        {
            qDebug() << "ERROR: Websockets::getQueuedClients: incorrect loop break";
            break;
        }
    } while (maxQueue == minQueue);
    if (QStrQueuedClients.isEmpty()) QStrQueuedClients = "queueEmpty";
    else QStrQueuedClients = QStrQueuedClients.left(QStrQueuedClients.length() - 1); //usuń ostatni przecinek
    qDebug() << "updated queue =" << QStrQueuedClients;
    return QStrQueuedClients;
}

PLAYERS_TYPES Websockets::getClientTypeBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.type;
        }
    }
    qDebug() << "ERROR: Websockets::getClientTypeBySocket(): socket not found";
    return PT_NONE;
}

bool Websockets::isPlayerChairEmpty(PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            return false;
        }
    }
    return true;
}

uint64_t Websockets::getQueuedClientByName(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientByName(): client not found";
    return 0;
}

uint64_t Websockets::getQueuedClientBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientBySocket(): client not found";
    return 0;
}

QWebSocket *Websockets::getPlayerSocket(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.socket;
            }
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

CLIENT_STATE Websockets::getClientStateByType(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.state;
            }
        }
        return CS_NONE;
    }
    else
    {
        qDebug() << "ERROR: Websockets::getClientStateByType- wrong parameter:"
                  << type;
        return CS_NONE;
    }
}

QString Websockets::getPlayerNameByType(PLAYERS_TYPES type) //todo: źle ta funkcja jest zrobiona?
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE)) return "WHITE";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK)) return "BLACK";
    else
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.name;
            }
        }
        qDebug() << "ERROR: Websockets::getPlayerNameByType- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}
