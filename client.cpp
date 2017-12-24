#include "client.h"

Client::Client()
{

}

bool Client::operator ==(const struct Clients& st)
{
    return socket == st.socket &&
            name == st.name &&
            type == st.type &&
            isStartClickedByPlayer == st.isStartClickedByPlayer &&
            queue == st.queue;
}

void Client::newClientSocket(QWebSocket *clientSocket)
{
    Clients newClient;
    newClient.socket = clientSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    m_clients << newClient;
}

void Client::setClientName(QWebSocket *clientSocket, QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
        {
            qDebug() << "ERROR: Client::setClientName: name" << name << "already exists.";
            return;
        }
    }

    if (this->getClientName(clientSocket) == name)

    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.name = name;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Client::setClientName: new name =" << m_clients.at(nClientpos).name;
            }
            else qDebug() << "ERROR: Client::setClientName: iteration error. iter val =" << nClientpos;

            return;
        }
    }

    Clients client;
    qDebug() << "ERROR: Client::setClientName: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(clientSocket) =" << typeid(clientSocket).name();
}

void Client::setPlayerType(QWebSocket *clientSocket, PLAYER_TYPE type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (type != PT_NONE && client.queue > 0)
            {
                qDebug() << "ERROR: Client::setPlayerType: client in queue can not sit on chair";
                return;
            }

            Clients changedClient = client;
            changedClient.type = type;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Client::setPlayerType: new" <<
                            playerTypeAsQStr(m_clients.at(nClientpos).type) << "player:" <<
                            this->getPlayerName(type) ;
                emit this->addTextToConsole("New " + playerTypeAsQStr(m_clients.at(nClientpos).type)
                                            + " player: " + this->getPlayerName(type) +
                                            "\n", LOG_WEBSOCKET);
                emit setBoardDataLabels(this->getPlayerName(type),
                                        type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qDebug() << "ERROR: Client::setPlayerType: iteration error. iter val ="
                         << nClientpos;

            return;
        }
    }
    Clients client;
    qDebug() << "ERROR: Client::setPlayerType: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(clientSocket) =" << typeid(clientSocket).name();

}

void Client::clearPlayerType(PLAYER_TYPE type)
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
                    qDebug() << "Client::clearPlayerType:" << playerTypeAsQStr(type);
                    m_clients.replace(nClientpos, changedClient);
                }
                else qDebug() << "ERROR: Client::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
        }
        qDebug() << "WARNING: Client::clearPlayerType- client.type not found";
    }
    else qDebug() << "WARNING: Client::clearPlayerType- already clear";
}

void Client::setClientState(QWebSocket *clientSocket, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Client::setClientState(QWebSocket *clientSocket, "
                            "bool state)- client has already clicked start";

            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Client::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Client::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Client::setClientState- client not found";
}

void Client::setClientState(PLAYER_TYPE type, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Client::setClientState(PLAYER_TYPE type, "
                            "bool state)- client has already clicked start";

            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Client::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Client::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Client::setClientState- client not found";
}

void Client::addClientToQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue > 0)
            {
                qDebug() << "ERROR: Client::addClientToQueue- "
                            "client already in queue:" << client.queue;
                return;
            }
            if (client.type != PT_NONE)
            {
                qDebug() << "ERROR: Client::addClientToQueue- client is" <<
                            playerTypeAsQStr(client.type);
                return;
            }
        }
    }

    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue > maxQueue)
            maxQueue = client.queue;
    }
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Client::addClientToQueue: new queue =" << m_clients.at(nClientpos).queue;
            }
            else qDebug() << "ERROR: Client::addClientToQueue: iteration error. iter val =" << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Client::removeClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if(!m_clients.removeOne(client))
                qDebug() << "ERROR: Client::removeClient- client not found";;
            return;
        }
    }
}

void Client::removeClientFromQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.queue = -1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
                m_clients.replace(nClientpos, changedClient);
            else qDebug() << "ERROR: Client::removeClientFromQueue: iteration error. iter val ="
                          << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

bool Client::isClientInList(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return true;
    }
    qDebug() << "ERROR: Client::isClientInList(): client not found";
    return false;
}

Clients Client::getClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client;
    }
    qDebug() << "ERROR: Client::getClient(): client not found";
    Clients errorClient;
    return errorClient;
}

QWebSocket *Client::getClientSocket(QString playerName)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == playerName)
            return client.socket;
    }
    return nullptr;
}

QString Client::getClientName(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.name;
    }
    qDebug() << "ERROR: Client::getClientName(): client not found";
    return "";
}

QWebSocket *Client::getNextQueuedClientSocket()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue >= 0)
            minQueue = client.queue;
    }
    qDebug() << "Client::getNextQueuedClientSocket(): minQueue =" << minQueue;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue == minQueue)
        {
            qDebug() << "Client::getNextQueuedClientSocket(): client.name =" <<
                        client.name;
            return client.socket;
        }
    }
    qDebug() << "ERROR: Client::getNextQueuedClientSocket(): client not found";
    return nullptr;
}

QString Client::getQueuedClientsList()
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
                qDebug() << "ERROR: Client::getQueuedClientsList: incorrect loop break. nLoopBreakingCounter ="
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

/*void Client::testQueuedClients()
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

    qDebug() << "Client::testQueuedClients(): m_clients.size =" << m_clients.size();
    QString QStrQueuedClients = this->getQueuedClientsList();
    qDebug() << "Client::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

PLAYER_TYPE Client::getClientType(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.type;
    }
    qDebug() << "ERROR: Client::getClientType(): socket not found";
    return PT_NONE;
}

bool Client::isPlayerChairEmpty(PLAYER_TYPE type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
            return false;
    }
    return true;
}

bool Client::isGameTableOccupied()
{
    if (this->isPlayerChairEmpty(PT_WHITE) || this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

int64_t Client::getQueuedClient(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return client.queue;
    }
    qDebug() << "ERROR: Client::getQueuedClient(): client not found";
    return -1;
}

int64_t Client::getQueuedClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.queue;
    }
    qDebug() << "ERROR: Client::getQueuedClient(): client not found";
    return -1;
}

QWebSocket *Client::getPlayerSocket(PLAYER_TYPE type)
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
        qDebug() << "ERROR: Client::getPlayerSocket- wrong parameter:"
                  << type;
        return nullptr;
    }
}

bool Client::isStartClickedByPlayer(PLAYER_TYPE type)
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
        qDebug() << "ERROR: Client::isStartClickedByPlayer- wrong parameter:"
                  << type;
        return false;
    }
}

QString Client::getPlayerName(PLAYER_TYPE type)
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return "White";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return "Black";
    else
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.name;
        }
        qDebug() << "ERROR: Client::getPlayerName- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Client::isClientInQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Client::isClientInQueue(): client not found";
    return false;
}

bool Client::isClientNameExists(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return true;
    }
    return false;
}

int Client::getAmountOfQueuedClients()
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

bool Client::isClientAPlayer(QWebSocket *clientSocket)
{
    if (this->getClientType(clientSocket) == PT_WHITE ||
            this->getClientType(clientSocket) == PT_BLACK )
        return true;
    else return false;

}
