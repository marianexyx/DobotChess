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

void Websockets::resetPlayersStartConfirmInfo()
{
    if (this->getClientStateByType(PT_WHITE) != CS_CLICKED_START)
        this->clearPlayerType(PT_WHITE);
    if (this->getClientStateByType(PT_BLACK) != CS_CLICKED_START)
        this->clearPlayerType(PT_BLACK);
}

QString Websockets::getTableData()
{
    //TABLE_DATA{"wplr":WHITE,"bplr":BLACK,"turn":wt,"wtime":3458349,"btime":init,"queue":empty}
    QString QStrTableData = "TABLE_DATA{\"wplr\":" + this->getPlayerNameByType(PT_WHITE) +
            ",\"bplr\":" + this->getPlayerNameByType(PT_BLACK) +
            ",\"turn\":" + _pChessboard->getStrWhoseTurn() +
            ",\"wtime\":" + _pChessboard->getWhiteTimeLeft() +
            ",\"btime\":" + _pChessboard->getBlackTimeLeft() +
            ",\"queue\":" + this->getQueuedClients() + "}";
    return QStrTableData;
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
    else if (QStrWsMsg == "getTableData")
    {
        pClient->sendTextMessage(this->getTableData());
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

            emit this->addTextToConsole("New white player: " + this->getPlayerNameByType(PT_WHITE) +
                                        "\n", WEBSOCKET);
            qDebug() << "New white player:" << this->getPlayerNameByType(PT_WHITE);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableData());

            if (QStrPlayerName == "WHITE")
                emit MsgFromWebsocketsToChess("reset");
        }
        else if (QStrPlayerType.left(11) == "blackPlayer")
        {
            if (QStrPlayerName == "BLACK")
                this->clearPlayerType(PT_BLACK);
            else
                this->setPlayerType(pClient, PT_BLACK);

            this->addTextToConsole("New black player: " + this->getPlayerNameByType(PT_BLACK) +
                                   "\n", WEBSOCKET);
            qDebug() << "New black player:" << this->getPlayerNameByType(PT_BLACK);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableData());

            if (QStrPlayerName == "BLACK")
                emit MsgFromWebsocketsToChess("reset");
        }
        else  qDebug() << "ERROR: Websockets::receivedMsg(): wrong change parameter: " << QStrWsMsg;

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();
    }
    else if (QStrWsMsg.left(3) == "im ")
    {
        this->setClientName(pClient, QStrWsMsg.mid(3));
        pClient->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg.left(9) == "promoteTo") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "queueMe")
    {
        this->addClientToQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg == "leaveQueue")
    {
        this->removeClientFromQueueBySocket(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
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
    else if (QStrWsMsg.left(8) == "promoted" || QStrWsMsg == "reseting" || QStrWsMsg == "ready")
    {
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg == "timeOutWhite" || QStrWsMsg == "timeOutBlack")
    {
        //todo: ogarnąć w końcu funkcję czyszczenia danych przy końcach...
        //...gry- w sumie Websockets::endOfGame daje chyba radę- ogarnąć na spokojnie
        this->endOfGame(QStrWsMsg.right(5) == "White" ? ET_WHIE_WON : ET_BLACK_WON);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg == "timeOutStartQueue" )
    {
        this->endOfGame(ET_TIMEOUT_START);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
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
    qDebug() << "Websockets::socketDisconnected()";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) //todo: co to??? czyżby to że sprawdzam czy udało mi się zrzutować obiekt...
        //... z powyższej linjki?
    {
        if (pClient == this->getPlayerSocket(PT_WHITE))
        {
            _pChessboard->setWhoseTurn(NO_TURN);
            this->endOfGame(ET_SOCKET_LOST, pClient);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableData());

            qDebug() << "newWhite: WHITE";
            this->addTextToConsole("New white player: WHITE\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        else if (pClient == this->getPlayerSocket(PT_BLACK))
        {
            _pChessboard->setWhoseTurn(NO_TURN);
            this->endOfGame(ET_SOCKET_LOST, pClient);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableData());

            qDebug() << "newBlack: BLACK";
            this->addTextToConsole("New black player: BLACK\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        //todo: wyświetla mi się disconnect w dziwnym miejscu
        else emit addTextToConsole("non-player disconnected\n", WEBSOCKET);

        pClient->deleteLater();

        for(int i = 0; i < m_clients.count(); ++i)
        { //nie umiem obsłużyć removeAll na structurach
          if(m_clients.at(i).socket == pClient)
          {
            m_clients.removeAt(i);
            break;
          }
        }
    }
    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
}

void Websockets::msgFromChessboardToWebsockets(QString QStrWsMsg) //todo: niepotrzebne to przejście
{
    this->sendMsg(QStrWsMsg);
}

void Websockets::moveClientsFromQueueToTableIfExists(PLAYERS_TYPES chair)
{
    Clients nextQueuedClient;
    nextQueuedClient.socket = this->getNextQueuedClientsSocket();
    this->setPlayerType(nextQueuedClient.socket, chair);
    this->removeClientFromQueueBySocket(nextQueuedClient.socket);
    this->setClientStateBySocket(nextQueuedClient.socket, CS_NONE);
}

void Websockets::endOfGame(END_TYPE EndType, QWebSocket *clientToClear)
{
    //todo: zastanowić się na spokojnie jakie czyszczenia jeszcze tu upchać
    this->resetPlayersStartConfirmInfo();

    switch(EndType)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        this->clearPlayerType(PT_WHITE); //todo: to się pewnie musi dziać gdzieś jeszcze. ...
        this->clearPlayerType(PT_BLACK); //...sprawdzić czy nie dubluję tego
        if (this->getQueuedClients() != "queueEmpty")
            this->moveClientsFromQueueToTableIfExists(PT_WHITE);
        if (this->getQueuedClients() != "queueEmpty")
            this->moveClientsFromQueueToTableIfExists(PT_BLACK);
        break;

    case ET_SOCKET_LOST:
        if (this->getClientTypeBySocket(clientToClear) == PT_WHITE)
        {
            if (this->getQueuedClients() != "queueEmpty")
                this->moveClientsFromQueueToTableIfExists(PT_WHITE);
        }
        else if (this->getClientTypeBySocket(clientToClear) == PT_BLACK)
        {
            if (this->getQueuedClients() != "queueEmpty")
                this->moveClientsFromQueueToTableIfExists(PT_BLACK);
        }
        else qDebug() << "ERROR: Websockets::endOfGame: wrong player type";
        break;

    case ET_TIMEOUT_START:
        if (this->getClientStateByType(PT_WHITE) != CS_CLICKED_START)
        {
            this->clearPlayerType(PT_WHITE);
            this->setClientStateByType(PT_WHITE, CS_NONE);
            if (this->getQueuedClients() != "queueEmpty")
                this->moveClientsFromQueueToTableIfExists(PT_WHITE);
        }
        if (this->getClientStateByType(PT_BLACK) != CS_CLICKED_START)
        {
            this->clearPlayerType(PT_BLACK);
            this->setClientStateByType(PT_BLACK, CS_NONE);
            if (this->getQueuedClients() != "queueEmpty")
                this->moveClientsFromQueueToTableIfExists(PT_BLACK);
        }
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
            changedClient.queue = -1; //todo: poza zakresem unsigned

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
    int64_t minQueue = std::numeric_limits<int64_t>::max();
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
    int64_t maxQueue = 0;
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    int64_t lastBigggestQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue < minQueue && client.queue > 0)
            minQueue = client.queue;
    }
    //qDebug() << "minQueue =" << minQueue;
    int nNumberOFClients = m_clients.size();
    int nLoopBreakingCounter = 0;
    do
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.queue > maxQueue && client.queue < lastBigggestQueue)
                maxQueue = client.queue;
        }

        lastBigggestQueue = maxQueue;
        //qDebug() << "new lastBigggestQueue =" << lastBigggestQueue;

        Q_FOREACH (Clients client, m_clients)
        {
            if (client.queue == maxQueue)
            {
                QStrQueuedClients = client.name + "," + QStrQueuedClients;
                /*qDebug() << "found last queued client:" << client.queue
                         << ", new queue list =" << QStrQueuedClients;*/
                maxQueue = 0;
            }
        }
        if (nLoopBreakingCounter > nNumberOFClients + 1)
        {
            qDebug() << "ERROR: Websockets::getQueuedClients: incorrect loop break";
            break;
        }
        nLoopBreakingCounter++;
        //qDebug() << "lastBigggestQueue =" << lastBigggestQueue << ", minQueue =" << minQueue;
    } while (lastBigggestQueue != minQueue); //do till false
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
        testClient.state = CS_NONE;
        testClient.type = PT_NONE;

        m_clients << testClient;
    }

    qDebug() << "Websockets::testQueuedClients(): m_clients.size =" << m_clients.size();
    QString QStrQueuedClients = this->getQueuedClients();
    qDebug() << "Websockets::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

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

int64_t Websockets::getQueuedClientByName(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientByName(): client not found";
    return -1;
}

int64_t Websockets::getQueuedClientBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientBySocket(): client not found";
    return -1;
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

CLIENT_STATE Websockets::getClientStateByType(PLAYERS_TYPES type) //todo: zamienić na bool
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
