#include "client.h"

void Clients::newClientSocket(QWebSocket *clientSocket)
{
    Client newClient;
    newClient.ID = this->getNextAvailableClientID();
    newClient.socket = clientSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    _clients << newClient;
}

void Clients::setClientName(QWebSocket *clientSocket, QString name)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == name)
        {
            qDebug() << "ERROR: Clients::setClientName: name" << name << "already exists.";
            return;
        }
    }

    if (this->getClientName(clientSocket) == name)

    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.name = name;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
            {
                _clients.replace(nClientpos, changedClient);
                qDebug() << "Clients::setClientName: new name ="
                         << _clients.at(nClientpos).name;
            }
            else qDebug() << "ERROR: Clients::setClientName: iteration error. iter val ="
                          << nClientpos;

            return;
        }
    }

    qDebug() << "ERROR: Clients::setClientName: socket not found";
}

void Clients::setPlayerType(QWebSocket *clientSocket, PLAYER_TYPE type)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            if (type != PT_NONE && client.queue > 0)
            {
                qDebug() << "ERROR: Clients::setPlayerType: client in queue can not sit on chair";
                return;
            }

            Clients changedClient = client;
            changedClient.type = type;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
            {
                _clients.replace(nClientpos, changedClient);
                qDebug() << "Clients::setPlayerType: new" <<
                            playerTypeAsQStr(_clients.at(nClientpos).type) << "player:" <<
                            this->getPlayerName(type) ;
                emit this->addTextToConsole("New " + playerTypeAsQStr(_clients.at(nClientpos).type)
                                            + " player: " + this->getPlayerName(type) +
                                            "\n", LOG_WEBSOCKET);
                emit setBoardDataLabels(this->getPlayerName(type),
                                        type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qDebug() << "ERROR: Clients::setPlayerType: iteration error. iter val ="
                         << nClientpos;

            return;
        }
    }

    qDebug() << "ERROR: Clients::setPlayerType: socket not found";
}

void Clients::clearPlayerType(PLAYER_TYPE type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Client client, _clients)
        {
            if (client.type == type)
            {
                Client changedClient = client;
                changedClient.type = PT_NONE;
                changedClient.isStartClickedByPlayer = false;

                int nClientpos = _clients.indexOf(client);
                if (nClientpos >= 0 && nClientpos < _clients.size())
                {
                    qDebug() << "Clients::clearPlayerType:" << playerTypeAsQStr(type);
                    _clients.replace(nClientpos, changedClient);
                }
                else qDebug() << "ERROR: Clients::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
        }
        qDebug() << "WARNING: Clients::clearPlayerType- client.type not found";
    }
    else qDebug() << "WARNING: Clients::clearPlayerType- already clear";
}

void Clients::setClientState(QWebSocket *clientSocket, bool state)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Clients::setClientState(QWebSocket *clientSocket, "
                            "bool state)- client has already clicked start";

            Client changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
            {
                _clients.replace(nClientpos, changedClient);
                qDebug() << "Clients::setClientState: new state =" <<
                            _clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientState- client not found";
}

void Clients::setClientState(PLAYER_TYPE type, bool state)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.type == type)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Clients::setClientState(PLAYER_TYPE type, "
                            "bool state)- client has already clicked start";

            Client changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
            {
                _clients.replace(nClientpos, changedClient);
                qDebug() << "Clients::setClientState: new state =" <<
                            _clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientState- client not found";
}

void Clients::addClientToQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue > 0)
            {
                qDebug() << "ERROR: Clients::addClientToQueue- "
                            "client already in queue:" << client.queue;
                return;
            }
            if (client.type != PT_NONE)
            {
                qDebug() << "ERROR: Clients::addClientToQueue- client is" <<
                            playerTypeAsQStr(client.type);
                return;
            }
        }
    }

    int64_t maxQueue = 0;
    Q_FOREACH (Client client, _clients)
    {
        if (client.queue > maxQueue)
            maxQueue = client.queue;
    }
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            Client changedClient = client;
            changedClient.queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
            {
                _clients.replace(nClientpos, changedClient);
                qDebug() << "Clients::addClientToQueue: new queue ="
                         << _clients.at(nClientpos).queue;
            }
            else qDebug() << "ERROR: Clients::addClientToQueue: iteration error. iter val ="
                          << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Clients::removeClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            if(!_clients.removeOne(client))
                qDebug() << "ERROR: Clients::removeClient- client not found";;
            return;
        }
    }
}

void Clients::removeClientFromQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            Client changedClient = client;
            changedClient.queue = -1;

            int nClientpos = _clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < _clients.size())
                _clients.replace(nClientpos, changedClient);
            else qDebug() << "ERROR: Clients::removeClientFromQueue: iteration error. iter val ="
                          << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Clients::resetPlayersStartConfirmInfo()
{
    qDebug() << "Clients::resetPlayersStartConfirmInfo()";
    if (this->isStartClickedByPlayer(PT_WHITE))
           this->setClientState(PT_WHITE, false);
    if (this->isStartClickedByPlayer(PT_BLACK))
        this->setClientState(PT_BLACK, false);
}

void Clients::cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE chair)
{
    this->clearPlayerType(chair);

    if (this->getQueuedClientsList() != "queueEmpty")
    {
        Clients nextQueuedClient;
        if (this->isClientInList(getNextQueuedClientSocket()))
            nextQueuedClient = this->getClient(this->getNextQueuedClientSocket());
        else return;

        this->removeClientFromQueue(nextQueuedClient.socket);
        this->setPlayerType(nextQueuedClient.socket, chair);
        this->setClientState(nextQueuedClient.socket, false);

        emit this->setBoardDataLabels(this->getPlayerName(chair),
                                      chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        qDebug() << "Clients::cleanChairAndPutThereNextQueuedClientIfExist(): new" <<
                    playerTypeAsQStr(chair) << "player name =" << this->getPlayerName(chair);
        emit this->addTextToConsole("New " + playerTypeAsQStr(chair) + " player: " +
                                    this->getPlayerName(chair) + "\n", LOG_WEBSOCKET);
    }
}

bool Clients::isClientInList(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return true;
    }
    qDebug() << "ERROR: Clients::isClientInList(): client not found";
    return false;
}

Clients Clients::getClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return client;
    }
    qDebug() << "ERROR: Clients::getClient(QWebSocket): client not found";
    Clients errorClient;
    return errorClient;
}

Clients Clients::getClient(int64_t clientID)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.ID == clientID)
            return client;
    }
    qDebug() << "ERROR: Clients::getClient(int64_t): client not found";
    Clients errorClient;
    return errorClient;
}

QWebSocket *Clients::getClientSocket(QString playerName)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == playerName)
            return client.socket;
    }
    return nullptr;
}

QString Clients::getClientName(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return client.name;
    }
    qDebug() << "ERROR: Clients::getClientName(): client not found";
    return "";
}

QWebSocket *Clients::getNextQueuedClientSocket()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Client client, _clients)
    {
        if (minQueue > client.queue && client.queue >= 0)
            minQueue = client.queue;
    }
    qDebug() << "Clients::getNextQueuedClientSocket(): minQueue =" << minQueue;
    Q_FOREACH (Client client, _clients)
    {
        if (client.queue == minQueue)
        {
            qDebug() << "Clients::getNextQueuedClientSocket(): client.name =" <<
                        client.name;
            return client.socket;
        }
    }
    qDebug() << "ERROR: Clients::getNextQueuedClientSocket(): client not found";
    return nullptr;
}

QString Clients::getQueuedClientsList()
{
    QString QStrQueuedClients;
    QStrQueuedClients.clear();
    int64_t maxQueue = 0;
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    int64_t lastBigggestQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Client client, _clients)
    {
        if (minQueue > client.queue && client.queue > 0)
            minQueue = client.queue;
    }
    int nNumberOFClients = _clients.size();
    int nLoopBreakingCounter = 0;
    if (minQueue != 0 && minQueue != std::numeric_limits<int64_t>::max())
    {
        do
        {
            Q_FOREACH (Client client, _clients)
            {
                if (client.queue > maxQueue && client.queue < lastBigggestQueue)
                    maxQueue = client.queue;
            }

            lastBigggestQueue = maxQueue;

            Q_FOREACH (Client client, _clients)
            {
                if (client.queue == maxQueue)
                {
                    QStrQueuedClients = client.name + "," + QStrQueuedClients;
                    maxQueue = 0;
                }
            }
            if (nLoopBreakingCounter > nNumberOFClients + 1)
            {
                qDebug() << "ERROR: Clients::getQueuedClientsList: incorrect loop break. "
                            "nLoopBreakingCounter =" << nLoopBreakingCounter <<
                            ", nNumberOFClients =" << nNumberOFClients+1;
                break;
            }
            nLoopBreakingCounter++;
        } while (lastBigggestQueue != minQueue);
    }
    if (QStrQueuedClients.isEmpty())
        QStrQueuedClients = "queueEmpty";
    else //usuń ostatni przecinek
        QStrQueuedClients = QStrQueuedClients.left(QStrQueuedClients.length() - 1);

    return QStrQueuedClients;
}

/*void Clients::testQueuedClients()
{
    //for (int i=1; i<10; i++)
    for (int i=10; i>1; i--)
    {
        Client testClient;
        testClient.name = "test" + QString::number(i);
        testClient.queue = i;
        qDebug() << "client" << testClient.name << "queue =" << testClient.queue;
        testClient.socket = nullptr;
        testClient.isStartClickedByPlayer = false;
        testClient.type = PT_NONE;

        _clients << testClient;
    }

    qDebug() << "Clients::testQueuedClients(): _clients.size =" << _clients.size();
    QString QStrQueuedClients = this->getQueuedClientsList();
    qDebug() << "Clients::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

PLAYER_TYPE Clients::getClientType(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return client.type;
    }
    qDebug() << "ERROR: Clients::getClientType(): socket not found";
    return PT_NONE;
}

bool Clients::isPlayerChairEmpty(PLAYER_TYPE type)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.type == type)
            return false;
    }
    return true;
}

bool Clients::isGameTableOccupied()
{
    if (this->isPlayerChairEmpty(PT_WHITE) || this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

int64_t Clients::getQueuedClient(QString name)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == name)
            return client.queue;
    }
    qDebug() << "ERROR: Clients::getQueuedClient(): client not found";
    return -1;
}

int64_t Clients::getQueuedClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return client.queue;
    }
    qDebug() << "ERROR: Clients::getQueuedClient(): client not found";
    return -1;
}

QWebSocket *Clients::getPlayerSocket(PLAYER_TYPE type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Client client, _clients)
        {
            if (client.type == type)
                return client.socket;
        }
        return nullptr;
    }
    else
    {
        qDebug() << "ERROR: Clients::getPlayerSocket- wrong parameter:"
                  << type;
        return nullptr;
    }
}

bool Clients::isStartClickedByPlayer(PLAYER_TYPE type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Client client, _clients)
        {
            if (client.type == type)
                return client.isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qDebug() << "ERROR: Clients::isStartClickedByPlayer- wrong parameter:"
                  << type;
        return false;
    }
}

QString Clients::getPlayerName(PLAYER_TYPE type)
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return "White";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return "Black";
    else
    {
        Q_FOREACH (Client client, _clients)
        {
            if (client.type == type)
                return client.name;
        }
        qDebug() << "ERROR: Clients::getPlayerName- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Clients::isClientInQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Clients::isClientInQueue(): client not found";
    return false;
}

bool Clients::isClientNameExists(QString name)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == name)
            return true;
    }
    return false;
}

int Clients::getAmountOfQueuedClients()
{
    int64_t maxQueue = 0;
    Q_FOREACH (Client client, _clients)
    {
        if (maxQueue < client.queue && client.queue > 0)
            maxQueue = client.queue;
    }

    if (maxQueue > 0)
    {
        int64_t minQueue = std::numeric_limits<int64_t>::max();
        Q_FOREACH (Client client, _clients)
        {
            if (minQueue > client.queue && client.queue > 0)
                minQueue = client.queue;
        }
        int nAmountOfQueuedClients = 1 + maxQueue - minQueue;

        return nAmountOfQueuedClients;
    }
    else return 0;
}

bool Clients::isClientAPlayer(QWebSocket *clientSocket)
{
    if (this->getClientType(clientSocket) == PT_WHITE ||
            this->getClientType(clientSocket) == PT_BLACK )
        return true;
    else return false;

}

bool Clients::isClientIDExists(int64_t ID)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.ID == ID)
            return true;
    }
    return false;
}

int64_t Clients::getClientID(QWebSocket *clientSocket)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.socket == clientSocket)
            return client.ID;
    }
    qDebug() << "ERROR: Clients::getClientID(): socket not found";
    return 0;
}

int64_t Clients::getNextAvailableClientID()
{
    int64_t minID = std::numeric_limits<int64_t>::max();
    int64_t maxID = 0;
    Q_FOREACH (Client client, _clients)
    {
        if (client.ID > 0 && client.ID > maxID)
            maxID = client.ID;
    }
    if (maxID > 0)
    {
        Q_FOREACH (Client client, _clients)
        {
            if (minID > client.ID && client.ID > 0)
                minID = client.ID;
        }
    }
    else return 0;

    if (maxID - minID == 1)
        return maxID+1;
    else if (maxID - minID > 1)
        return minID+1;
    else
    {
        qDebug() << "ERROR: Clients::getNextAvailableClientID(): minID > maxID";
        return 0;
    }
}

void Clients::showClientsInForm()
{
    emit showClientsList(_clients);
    emit setBoardDataLabels(std::to_string(_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
}
