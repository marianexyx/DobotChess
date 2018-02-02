#include "client.h"

//future: jak bardzo zachcę, to mogę zagęścić kod większości metod, bo to zgrubsza to samo
void Clients::newClient(QWebSocket* pClientSocket)
{
    Client newClient;
    newClient.ID = this->getNextAvailableClientID();
    newClient.socket = pClientSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    _clients << newClient;
}

void Clients::setClientName(Client client, QString name)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.name == name)
        {
            qDebug() << "ERROR: Clients::setClientName(): name" << name << "already exists.";
            return;
        }
    }

    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.name = name;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::setClientName: new name ="
                         << _clients.at(nClientPos).name;
            }
            else qDebug() << "ERROR: Clients::setClientName(): iteration error. iter val ="
                          << nClientPos;

            return;
        }
    }

    qDebug() << "ERROR: Clients::setClientName(): client not found";
}

void Clients::setPlayerType(Client client, PLAYER_TYPE type)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if (type != PT_NONE && client.queue > 0)
            {
                qDebug() << "ERROR: Clients::setPlayerType(): client in queue can not"
                            " sit on chair";
                return;
            }

            Client changedClient = cl;
            changedClient.type = type;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
;
                emit this->addTextToLogPTE("New " + playerTypeAsQStr(_clients.at(nClientPos).type)
                                            + " player: " + this->getPlayerName(type) +
                                            "\n", LOG_CORE);
                emit this->setBoardDataLabel(this->getPlayerName(type),
                                        type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qDebug() << "ERROR: Clients::setPlayerType(): iteration error. iter val ="
                         << nClientPos;

            return;
        }
    }

    qDebug() << "ERROR: Clients::setPlayerType(): client not found";
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

                int nClientPos = _clients.indexOf(client);
                if (nClientPos >= 0 && nClientPos < _clients.size())
                {
                    qDebug() << "Clients::clearPlayerType():" << playerTypeAsQStr(type);
                    _clients.replace(nClientPos, changedClient);
                }
                else qDebug() << "ERROR: Clients::clearPlayerType(): iteration error. iter"
                                 " val =" << nClientPos;

                return;
            }
        }
        qDebug() << "WARNING: Clients::clearPlayerType(): client.type not found";
    }
    else qDebug() << "WARNING: Clients::clearPlayerType(): already clear";
}

void Clients::setClientState(Client client, bool state)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if(state && cl.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Clients::setClientState(Client client, "
                            "bool state): client has already clicked start";

            Client changedClient = cl;
            changedClient.isStartClickedByPlayer = state;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::setClientState(): new state =" <<
                            _clients.at(nClientPos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientState(): iteration error. iter val ="
                          << nClientPos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientState(): client not found";
}

void Clients::setClientState(PLAYER_TYPE type, bool state)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == type)
        {
            if(state && cl.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Clients::setClientState(PLAYER_TYPE type, "
                            "bool state): client has already clicked start";

            Client changedClient = cl;
            changedClient.isStartClickedByPlayer = state;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::setClientState(): new state =" <<
                            _clients.at(nClientPos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientState(): iteration error. iter val ="
                          << nClientPos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientState(): client not found";
}

void Clients::addClientToQueue(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if (cl.queue > 0)
            {
                qDebug() << "ERROR: Clients::addClientToQueue(): "
                            "client already in queue:" << cl.queue;
                return;
            }
            if (cl.type != PT_NONE)
            {
                qDebug() << "ERROR: Clients::addClientToQueue(): client is" <<
                            playerTypeAsQStr(cl.type);
                return;
            }
        }
    }

    int64_t maxQueue = 0;
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.queue > maxQueue)
            maxQueue = cl.queue;
    }
    Q_FOREACH (Client client, _clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::addClientToQueue(): new queue ="
                         << _clients.at(nClientPos).queue;
            }
            else qDebug() << "ERROR: Clients::addClientToQueue(): iteration error. iter val ="
                          << nClientPos;

            emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()),
                                         BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Clients::removeClient(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            cl.socket->deleteLater(); //todo: jest sens to usuwać skoro to nie jest wskaźnik?...
            //...a może to musi być wskaźnik by dało się to wogle usunąć?

            if(!_clients.removeOne(cl))
                qDebug() << "ERROR: Clients::removeClient(): client not found";;

            //return;

            //info: erlier working code in chess class:
            /*for(int i = 0; i < _pClientsList->getClientsList().count(); ++i)
            { //nie męczyć się teraz z obsługą removeAll na structurach
              if(_pClientsList->getClientsList().at(i).socket == pClient->socket)
              {
                _pClientsList->getClientsList().removeAt(i);
                break;
              }
            }*/
        }
    }
}

void Clients::removeClientFromQueue(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.queue = -1;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
                _clients.replace(nClientPos, changedClient);
            else qDebug() << "ERROR: Clients::removeClientFromQueue(): iteration error. "
                             "iter val =" << nClientPos;

            emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()),
                                         BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
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
        Client nextQueuedClient;
        if (this->isClientInList(this->getNextQueuedClient()))
            nextQueuedClient = this->getNextQueuedClient();
        else return;

        this->removeClientFromQueue(nextQueuedClient);
        this->setPlayerType(nextQueuedClient, chair);
        this->setClientState(nextQueuedClient, false);

        emit this->setBoardDataLabel(this->getPlayerName(chair),
                                      chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        emit this->addTextToLogPTE("New " + playerTypeAsQStr(chair) + " player: " +
                                    this->getPlayerName(chair) + "\n", LOG_CORE);
    }
}

bool Clients::isClientInList(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return true;
    }
    qDebug() << "ERROR: Clients::isClientInList(): client not found";
    return false;
}

Clients* Clients::getClient(QWebSocket* pClientSocket)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.socket == pClientSocket)
            return *cl;
    }
    qDebug() << "ERROR: Clients::getClient(QWebSocket): client not found";
    return nullptr;
}

Clients* Clients::getClient(int64_t clientID)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.ID == clientID)
            return *cl;
    }
    qDebug() << "ERROR: Clients::getClient(int64_t): client not found";
    return nullptr;
}

Client* Clients::getPlayer(PLAYER_TYPE type)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == type)
            return *cl;
    }
    qDebug() << "ERROR: Clients::getPlayer(PLAYER_TYPE): client not found";
    return nullptr;
}

QWebSocket* Clients::getClientSocket(QString playerName)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == playerName)
            return *client.socket;
    }
    return nullptr;
}

QString Clients::getClientName(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.name;
    }
    qDebug() << "ERROR: Clients::getClientName(): client not found";
    return "";
}

bool Clients::isClientLoggedIn(Client client)
{
    if (!this->getClientName(client).isEmpty()) return true;
    else return false;
}

Client Clients::getNextQueuedClient()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Client cl, _clients)
    {
        if (minQueue > cl.queue && cl.queue >= 0)
            minQueue = cl.queue;
    }
    qDebug() << "Clients::getNextQueuedClient(): minQueue =" << minQueue;
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.queue == minQueue)
        {
            qDebug() << "Clients::getNextQueuedClient(): client.name =" <<
                        cl.name;
            return cl;
        }
    }
    qDebug() << "ERROR: Clients::getNextQueuedClient(): client not found";
    Client client;
    return client;
}

QString Clients::getQueuedClientsList()
{
    QString QStrQueuedClients;
    int64_t maxQueue = 0;
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    int64_t lastBigggestQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Client cl, _clients)
    {
        if (minQueue > cl.queue && cl.queue > 0)
            minQueue = cl.queue;
    }
    int nNumberOFClients = _clients.size();
    int nLoopBreakingCounter = 0;
    if (minQueue != 0 && minQueue != std::numeric_limits<int64_t>::max())
    {
        do
        {
            Q_FOREACH (Client cl, _clients)
            {
                if (cl.queue > maxQueue && cl.queue < lastBigggestQueue)
                    maxQueue = cl.queue;
            }

            lastBigggestQueue = maxQueue;

            Q_FOREACH (Client cl, _clients)
            {
                if (cl.queue == maxQueue)
                {
                    QStrQueuedClients = cl.name + "," + QStrQueuedClients;
                    maxQueue = 0;
                }
            }
            if (nLoopBreakingCounter > nNumberOFClients + 1)
            {
                qDebug() << "ERROR: Clients::getQueuedClientsList(): incorrect loop break. "
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

PLAYER_TYPE Clients::getClientType(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.type;
    }
    qDebug() << "ERROR: Clients::getClientType(): socket not found";
    return PT_NONE;
}

bool Clients::isPlayerChairEmpty(PLAYER_TYPE type, bool bErrorLog = false)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == type)
            return false;
    }

    if (bErrorLog)
        qDebug() << "ERROR: Clients::isPlayerChairEmpty():" << playerTypeAsQStr(type)
                 << "player chair is empty";

    return true;
}

bool Clients::isGameTableOccupied()
{
    if (this->isPlayerChairEmpty(PT_WHITE) || this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

int64_t Clients::getClientPosInQueue(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.queue;
    }
    qDebug() << "ERROR: Clients::getQueuedClient(): client not found";
    return -1;
}

QWebSocket *Clients::getPlayerSocket(PLAYER_TYPE type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == type)
                return cl.socket;
        }
        return nullptr;
    }
    else
    {
        qDebug() << "ERROR: Clients::getPlayerSocket(): wrong parameter:"
                  << type;
        return nullptr;
    }
}

bool Clients::isStartClickedByPlayer(PLAYER_TYPE type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == type)
                return cl.isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qDebug() << "ERROR: Clients::isStartClickedByPlayer(): wrong parameter:"
                  << type;
        return false;
    }
}

bool Clients::isStartClickedByBothPlayers()
{
    if (this->isStartClickedByPlayer(PT_WHITE) &&
            this->isStartClickedByPlayer(PT_BLACK))
        return true;
    else return false;
}

QString Clients::getPlayerName(PLAYER_TYPE type)
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return playerTypeAsQStr(PT_WHITE);
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return playerTypeAsQStr(PT_BLACK);
    else
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == type)
                return cl.name;
        }
        qDebug() << "ERROR: Clients::getPlayerName(): player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Clients::isClientInQueue(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if (cl.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Clients::isClientInQueue(): client not found";
    return false;
}

bool Clients::isClientNameExists(QString name, bool bErrorLog = false)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.name == name)
            return true;
    }

    if (bErrorLog)
        qDebug() << "ERROR: Clients::isClientNameExists(): name:" << name << "already exists";

    return false;
}

int Clients::getAmountOfQueuedClients()
{
    int64_t maxQueue = 0;
    Q_FOREACH (Client cl, _clients)
    {
        if (maxQueue < cl.queue && cl.queue > 0)
            maxQueue = cl.queue;
    }

    if (maxQueue > 0)
    {
        int64_t minQueue = std::numeric_limits<int64_t>::max();
        Q_FOREACH (Client cl, _clients)
        {
            if (minQueue > cl.queue && cl.queue > 0)
                minQueue = cl.queue;
        }
        int nAmountOfQueuedClients = 1 + maxQueue - minQueue;

        return nAmountOfQueuedClients;
    }
    else return 0;
}

bool Clients::isClientAPlayer(Client client, bool bErrorLog = false)
{
    if (this->getClientType(client) == PT_WHITE ||
            this->getClientType(client) == PT_BLACK )
        return true;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR: Clients::isClientAPlayer(): client isnt a player";

        return false;
    }
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

int64_t Clients::getClientID(Client client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.ID;
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
    emit this->showClientsList(_clients);
    emit this->setBoardDataLabel(std::to_string(_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
}
