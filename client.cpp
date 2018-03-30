#include "client.h"

//future: jak bardzo zachcę, to mogę zagęścić kod większości metod, bo to zgrubsza to samo
void Clients::newClient(QWebSocket& clientSocket)
{
    Client newClient;
    newClient.ID = this->getNextAvailableClientID();
    newClient.socket = &clientSocket; //future: jeżeli ten parametr jest jako const, to nie...
    //...zadziała. dlaczego? przecież tylko przypisuje, a nie zmieniam
    newClient.name.clear();
    newClient.queue = 0; //todo: wcześniej było -1. będzie ok?
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    qDebug() << "Clients::newClient(): created new client. ID =" << newClient.ID
             << ", name =" << newClient.name << ", queue =" << newClient.queue
             << ", start =" << newClient.isStartClickedByPlayer
             << ", type =" << playerTypeAsQStr(newClient.type);

    _clients << newClient;
}

void Clients::setClientName(const Client& client, QString QStrName)
{
    qDebug() << "inside Clients::setClientName(): incoming name parameter:" << QStrName;

    Q_FOREACH (Client cl, _clients)
    {
        if (cl.name == QStrName && !cl.name.isEmpty())
        {
            qDebug() << "ERROR: Clients::setClientName(): name" << QStrName << "already exists.";
            return;
        }
    }

    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.name = QStrName;

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

void Clients::setPlayerType(Client& client, PLAYER_TYPE Type)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if (Type != PT_NONE && client.queue > 0)
            {
                qDebug() << "ERROR: Clients::setPlayerType(): client" << client.name
                            << "in queue can not sit on chair. player type ="
                            << playerTypeAsQStr(Type) << ", client.queue =" << client.queue;
                return;
            }

            Client changedClient = cl;
            changedClient.type = Type;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
;
                emit this->addTextToLogPTE("New "
                                           + playerTypeAsQStr(_clients.at(nClientPos).type)
                                           + " player: " + this->getPlayerName(Type)
                                           + "\n", LOG_CORE);
                emit this->setBoardDataLabel(this->getPlayerName(Type),
                                        Type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qDebug() << "ERROR: Clients::setPlayerType(): iteration error. iter val ="
                         << nClientPos;

            return;
        }
    }

    qDebug() << "ERROR: Clients::setPlayerType(): client not found";
}

void Clients::clearPlayerType(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        Q_FOREACH (Client client, _clients)
        {
            if (client.type == Type)
            {
                Client changedClient = client;
                changedClient.type = PT_NONE;
                changedClient.isStartClickedByPlayer = false;

                int nClientPos = _clients.indexOf(client);
                if (nClientPos >= 0 && nClientPos < _clients.size())
                {
                    qDebug() << "Clients::clearPlayerType():" << playerTypeAsQStr(Type);
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

void Clients::setClientStartConfirmation(Client& client, bool bState)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            if(bState && cl.isStartClickedByPlayer == bState)
                qDebug() << "WARNING: Clients::setClientStartConfirmation(Client client, "
                            "bool state): client has already clicked start";

            Client changedClient = cl;
            changedClient.isStartClickedByPlayer = bState;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::setClientStartConfirmation(): new state ="
                         << _clients.at(nClientPos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientStartConfirmation(): iteration "
                             "error. iter val =" << nClientPos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientStartConfirmation(): client not found";
}

void Clients::setClientStartConfirmation(PLAYER_TYPE Type, bool bState)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == Type)
        {
            if(bState && cl.isStartClickedByPlayer == bState)
                qDebug() << "WARNING: Clients::setClientStartConfirmation(PLAYER_TYPE type, "
                            "bool state): client has already clicked start";

            Client changedClient = cl;
            changedClient.isStartClickedByPlayer = bState;

            int nClientPos = _clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < _clients.size())
            {
                _clients.replace(nClientPos, changedClient);
                qDebug() << "Clients::setClientStartConfirmation(): new state ="
                         << _clients.at(nClientPos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Clients::setClientStartConfirmation(): iteration "
                             "error. iter val =" << nClientPos;

            return;
        }
    }
    qDebug() << "ERROR: Clients::setClientStartConfirmation(): client not found";
}

void Clients::addClientToQueue(Client& client)
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
    Q_FOREACH (Client cl, _clients)
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

void Clients::removeClientFromList(Client& client)
{
    qDebug() << "Clients::removeClientFromList()";
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
        {
            cl.socket->deleteLater(); //todo: jest sens to usuwać skoro to nie jest wskaźnik?...
            //...a może to musi być wskaźnik by dało się to wogle usunąć?

            if(!_clients.removeOne(cl))
                qDebug() << "ERROR: Clients::removeClientFromList(): client can't be removed";

            return;

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
    qDebug() << "ERROR: Clients::removeClientFromList(): client not found";
}

void Clients::removeClientFromQueue(Client &client)
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
           this->setClientStartConfirmation(PT_WHITE, false);
    if (this->isStartClickedByPlayer(PT_BLACK))
        this->setClientStartConfirmation(PT_BLACK, false);
}

void Clients::cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE Chair)
{
    this->clearPlayerType(Chair);

    if (this->getQueuedClientsList() != QUEUE_EMPTY)
    {
        Client nextQueuedClient;
        qDebug() << "Clients::cleanChairAndPutThereNextQueuedClientIfExist(): "
                    "approaching isClientInList()";
        if (this->isClientInList(this->getNextQueuedClient(), SHOW_ERRORS))
            nextQueuedClient = this->getNextQueuedClient();
        else return;

        this->removeClientFromQueue(nextQueuedClient);
        this->setPlayerType(nextQueuedClient, Chair);
        this->setClientStartConfirmation(nextQueuedClient, false);

        emit this->setBoardDataLabel(this->getPlayerName(Chair),
                                      Chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        emit this->addTextToLogPTE("New " + playerTypeAsQStr(Chair) + " player: " +
                                    this->getPlayerName(Chair) + "\n", LOG_CORE);
    }
}

bool Clients::isClientInList(const Client &client, bool bErrorLog /*= false*/)
{
    qDebug() << "Clients::isClientInList(): client basic data: ID ="
             << QString::number(client.ID) << ", name =" << client.name
             << ", queue =" << QString::number(client.queue);

    Q_FOREACH (Client cl, _clients)
    {
        if (cl.ID == client.ID)
            return true;
    }

    if (bErrorLog)
        qDebug() << "ERROR: Clients::isClientInList(): client of not found";
    else qDebug() << "WARNING: Clients::isClientInList(): client not found";

    return false;
}

Client Clients::getClient(QWebSocket* pClientSocket)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.socket == pClientSocket)
        {
            Client& client = cl;
            qDebug() << "Clients::getClient(QWebSocket*): client found. his name ="
                     << client.name << ", ID =" << client.ID << ", queue =" << client.queue;
            return client;
        }
    }
    qDebug() << "ERROR: Clients::getClient(QWebSocket): client not found";
    Client client;
    return client;
}

Client Clients::getClient(int64_t n64ClientID)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.ID == n64ClientID)
        {
            qDebug() << "Clients::getClient(int64_t): found client ID. cl basic"
                        " data: name =" << cl.name
                     << ", queue =" << QString::number(cl.queue)
                     << ", ID =" << QString::number(cl.ID);

            Client& client = cl; //todo: tworzę tu pointer, więc
            //on ginie po wyjściu z funkcji??

            qDebug() << "Clients::getClient(): returning ref to found client. "
                        "his basic data: name =" << client.name
                     << ", queue =" << QString::number(client.queue)
                     << ", ID =" << QString::number(client.ID);

            return client;
        }
    }
    qDebug() << "ERROR: Clients::getClient(int64_t): client not found. ID =" << n64ClientID;
    Client client;
    return client;
}

Client* Clients::getPlayer(PLAYER_TYPE Type)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == Type)
        {
            Client* pClient = &cl;
            return pClient;
        }
    }
    qDebug() << "ERROR: Clients::getPlayer(PLAYER_TYPE): client not found";
    return nullptr;
}

QWebSocket* Clients::getClientSocket(QString QStrPlayerName)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.name == QStrPlayerName)
            return client.socket;
    }
    return nullptr;
}

QString Clients::getClientName(Client& client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.name;
    }
    qDebug() << "ERROR: Clients::getClientName(): client not found";
    return "";
}

bool Clients::isClientLoggedIn(Client& client)
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
        QStrQueuedClients = QUEUE_EMPTY;
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

PLAYER_TYPE Clients::getClientType(Client& client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.type;
    }
    qDebug() << "ERROR: Clients::getClientType(): socket not found";
    return PT_NONE;
}

bool Clients::isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog /*= false*/)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.type == Type)
            return false;
    }

    if (bErrorLog)
        qDebug() << "ERROR: Clients::isPlayerChairEmpty():" << playerTypeAsQStr(Type)
                 << "player chair is empty";

    return true;
}

bool Clients::isGameTableOccupied()
{
    if (this->isPlayerChairEmpty(PT_WHITE) || this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

int64_t Clients::getClientPosInQueue(Client& client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl == client)
            return cl.queue;
    }
    qDebug() << "ERROR: Clients::getQueuedClient(): client not found";
    return -1;
}

QWebSocket* Clients::getPlayerSocket(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == Type)
                return cl.socket;
        }
        return nullptr;
    }
    else
    {
        qDebug() << "ERROR: Clients::getPlayerSocket(): wrong parameter:"
                  << Type;
        return nullptr;
    }
}

bool Clients::isStartClickedByPlayer(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == Type)
                return cl.isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qDebug() << "ERROR: Clients::isStartClickedByPlayer(): wrong parameter:"
                  << Type;
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

QString Clients::getPlayerName(PLAYER_TYPE Type)
{
    if (Type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return playerTypeAsQStr(PT_WHITE);
    else if (Type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return playerTypeAsQStr(PT_BLACK);
    else
    {
        Q_FOREACH (Client cl, _clients)
        {
            if (cl.type == Type)
                return cl.name;
        }
        qDebug() << "ERROR: Clients::getPlayerName(): player not found:"
                  << playerTypeAsQStr(Type);
        return "";
    }
}

bool Clients::isClientInQueue(Client& client)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.ID == client.ID)
        {
            if (cl.queue > 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Clients::isClientInQueue(): client not found";
    return false;
}

bool Clients::isClientNameExists(QString QStrName, bool bErrorLog /*= false*/)
{
    Q_FOREACH (Client cl, _clients)
    {
        if (cl.name == QStrName)
            return true;
    }

    if (bErrorLog)
        qDebug() << "ERROR: Clients::isClientNameExists(): name:" << QStrName
                 << "already exists";

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

bool Clients::isClientAPlayer(Client& client, bool bErrorLog /*= false*/)
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

bool Clients::isClientIDExists(int64_t n64ID)
{
    Q_FOREACH (Client client, _clients)
    {
        if (client.ID == n64ID)
            return true;
    }
    return false;
}

int64_t Clients::getClientID(Client& client)
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
    int64_t maxID = 0;

    Q_FOREACH (Client client, _clients) //find max queued ID
    {
        if (client.ID > maxID)
            maxID = client.ID;
    }
    return ++maxID;

    /*
    int64_t minID = std::numeric_limits<int64_t>::max();

    if (maxID > 0) //if any client is connected
    {
        Q_FOREACH (Client client, _clients) //find min queued ID
        {
            if (minID > client.ID && client.ID > 0)
                minID = client.ID;
        }

        if (maxID - minID == 1) //if there is no empty IDs between min and max ID
            return maxID+1; //return next biggest nr
        else if (maxID - minID > 1) //if next max ID isn't next (from min) free ID to assign
            return minID+1; //return 1st free nr in ID list
        //future: nie mam wyszukiwania pierwszego pustego miejsca. zwracam tylko min+1, co w sumie może być zajęte
        else
        {
            qDebug() << "ERROR: Clients::getNextAvailableClientID(): minID > maxID";
            return 0;
        }
    }
    else return 1; //give 1st ID to 1st client
    */
}

void Clients::showClientsInUI()
{
    emit this->showClientsList(_clients);
    emit this->setBoardDataLabel(std::to_string(_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
}
