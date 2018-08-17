#include "client.h"

void Clients::newClient(QWebSocket* const clientSocket)
{
    Client newClient;
    newClient.m_ID = this->getNextAvailableClientID();
    newClient.m_sqlID = 0;
    newClient.m_socket = clientSocket;
    newClient.m_queue = 0;
    newClient.m_isStartClickedByPlayer = false;
    newClient.m_Type = PT_NONE;
    newClient.m_name = "";

    m_clients << newClient;
}

void Clients::putOnChairNextQueuedClient(PLAYER_TYPE Chair)
{
    if (!this->isPlayerChairEmpty(Chair))
    {
        qCritical() << playerTypeAsQStr(Chair) << "chair isn't empty";
        return;
    }

    if (this->getQueuedClientsSqlIDsList() != "0")
    {
        if (!this->isClientInList(this->getNextQueuedClient(), SHOW_ERRORS)) return;

        uint64_t nextQueuedClientID = this->getNextQueuedClient().m_ID;
        this->removeClientFromQueue(this->getClient(nextQueuedClientID));
        this->setPlayerType(this->getClient(nextQueuedClientID), Chair);
        this->setClientStartConfirmation(this->getClient(nextQueuedClientID), false);

        emit this->setBoardDataLabel(this->getPlayerName(Chair),
                                      Chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        emit this->addTextToLogPTE("New " + playerTypeAsQStr(Chair) + " player: " +
                                    this->getPlayerName(Chair) + "\n", LOG_CORE);
    }
    else
    {
        qCritical() << "there are no clients in queue";
        return;
    }
}

void Clients::addClientToQueue(const Client& client)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            if (cl.m_queue > 0)
            {
                qCritical() << "client already in queue:" << QString::number(cl.m_queue);
                return;
            }
            if (cl.m_Type != PT_NONE)
            {
                qCritical() << "client is" << playerTypeAsQStr(cl.m_Type);
                return;
            }
        }
    }

    uint64_t maxQueue = 0;
    foreach (Client cl, m_clients) //find max client queue
    {
        if (cl.m_queue > maxQueue)
            maxQueue = cl.m_queue;
    }

    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.m_queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
                qInfo() << "client named =" << m_clients.at(nClientPos).name()
                        << "queued in list with nr ="
                        << QString::number(m_clients.at(nClientPos).m_queue);
            }
            else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            /*emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()),
                                         BDL_QUEUE_PLAYERS);
            emit this->showQueuedClientsListInUI(this->getQueuedClientsNamesList());
            return;*/
            break; //todo: check it
        }
    }

    emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
    emit this->showQueuedClientsListInUI(this->getQueuedClientsNamesList());
}

bool Clients::isClientInList(const Client &client, bool bErrorLog /*= false*/)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_ID == client.m_ID)
            return true;
    }

    if (bErrorLog)
        qCritical() << "client of not found";

    return false;
}

bool Clients::isClientInList(QWebSocket* pClientSocket, bool bErrorLog /*= false*/)
{
    foreach (Client cl, m_clients)
    {
        if (cl.socket() == pClientSocket)
            return true;
    }

    if (bErrorLog)
        qCritical() << "client of not found";

    return false;
}

bool Clients::isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog /*= false*/)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_Type == Type)
            return false;
    }

    if (bErrorLog)
        qCritical() << playerTypeAsQStr(Type) << "player chair is empty";

    return true;
}

bool Clients::isWholeGameTableEmpty()
{
    if (this->isPlayerChairEmpty(PT_WHITE) && this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

bool Clients::isWholeGameTableOccupied()
{
    if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
        return true;
    else return false;
}

bool Clients::isClientIDExists(uint64_t un64ID)
{
    foreach (Client client, m_clients)
    {
        if (client.m_ID == un64ID)
            return true;
    }
    return false;
}

bool Clients::isClientSqlIDExists(uint64_t un64ID, bool bErrorLog /*= false*/)
{
    if (un64ID == 0)
    {
        if (bErrorLog)
            qCritical() << "sqlID == 0";
        return false;
    }

    foreach (Client cl, m_clients)
    {
        if (cl.m_sqlID == un64ID)
            return true;
    }

    if (bErrorLog)
        qCritical() << "sqlID:" << QString::number(un64ID) << "not found in clients list";

    return false;
}

bool Clients::isClientAPlayer(const Client &client, bool bErrorLog /*= false*/)
{
    if (client.m_Type == PT_WHITE || client.m_Type  == PT_BLACK )
        return true;
    else
    {
        if (bErrorLog)
            qCritical() << "client isn't a player";

        return false;
    }
}

bool Clients::isStartClickedByPlayer(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        foreach (Client cl, m_clients)
        {
            if (cl.m_Type == Type)
                return cl.m_isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qCritical() << "wrong parameter:" << QString::number(Type);
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

bool Clients::isClientInQueue(const Client &client)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_ID == client.m_ID)
        {
            if (cl.m_queue > 0)
                return true;
            else return false;
        }
    }
    qCritical() << "client not found";
    return false;
}

void Clients::clearClientSqlID(const Client& client)
{
    qInfo();
    this->setClientSqlIDAndName(client, 0);
}

void Clients::clearPlayerType(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        foreach (Client client, m_clients)
        {
            if (client.m_Type == Type)
            {
                Client changedClient = client;
                changedClient.m_Type = PT_NONE;
                changedClient.m_isStartClickedByPlayer = false;

                int nClientPos = m_clients.indexOf(client);
                if (nClientPos >= 0 && nClientPos < m_clients.size())
                {
                    qInfo() << playerTypeAsQStr(Type);
                    m_clients.replace(nClientPos, changedClient);
                }
                else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

                return;
            }
        }
        qWarning() << "client.type not found";
    }
    else qWarning() << "already clear";
}

void Clients::resetPlayersStartConfirmInfo()
{
    if (this->isStartClickedByPlayer(PT_WHITE))
           this->setClientStartConfirmation(PT_WHITE, false);
    if (this->isStartClickedByPlayer(PT_BLACK))
        this->setClientStartConfirmation(PT_BLACK, false);
}

void Clients::removeClientFromList(const Client& client)
{
    qInfo();

    if (!this->isClientInList(client)) return;

    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            cl.m_socket->deleteLater();

            if(!m_clients.removeOne(cl))
                qCritical() << "client can't be removed";

            return;
        }
    }
    qCritical() << "client not found";
}

void Clients::removeClientFromQueue(const Client &client)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.m_queue = 0;

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
            }
            else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()),
                                         BDL_QUEUE_PLAYERS);
            emit this->showQueuedClientsListInUI(this->getQueuedClientsNamesList());
            return;
        }
    }
    emit this->setBoardDataLabel(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
    emit this->showQueuedClientsListInUI(this->getQueuedClientsNamesList());
}

void Clients::setClientSqlIDAndName(const Client& client, uint64_t un64SqlID)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_sqlID == un64SqlID && cl.m_sqlID > 0)
        {
            qCritical() << "sqlID" << QString::number(un64SqlID) << "already exists";
            return;
        }
    }

    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            Client changedClient = cl;
            changedClient.m_sqlID = un64SqlID;
            changedClient.m_name = un64SqlID == 0 ? "" : Sql::getClientNameFromDB(un64SqlID);

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
                qInfo() << "new sqlID =" << QString::number(m_clients.at(nClientPos).m_sqlID)
                        << "and name =" << m_clients.at(nClientPos).m_name;
            }
            else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            return;
        }
    }

    qCritical() << "client not found";
}

void Clients::setPlayerType(const Client& client, PLAYER_TYPE Type)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            if (Type != PT_NONE && client.m_queue > 0)
            {
                qCritical() << "client" << client.name() << "in queue can't sit on chair. PT ="
                         << playerTypeAsQStr(Type) << ", client.queue ="
                         << QString::number(client.m_queue);
                return;
            }

            Client changedClient = cl;
            changedClient.m_Type = Type;

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
;
                emit this->addTextToLogPTE("New "
                                           + playerTypeAsQStr(m_clients.at(nClientPos).m_Type)
                                           + " player: " + this->getPlayerName(Type)
                                           + "\n", LOG_CORE);
                emit this->setBoardDataLabel(this->getPlayerName(Type),
                                        Type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            return;
        }
    }

    qCritical() << "client not found";
}

void Clients::setClientStartConfirmation(const Client& client, bool bState)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
        {
            if(bState && cl.m_isStartClickedByPlayer == bState)
                qWarning() << "client has already clicked start";

            Client changedClient = cl;
            changedClient.m_isStartClickedByPlayer = bState;

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
                qInfo() << "new state ="
                        << QString::number(m_clients.at(nClientPos).m_isStartClickedByPlayer);
            }
            else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            return;
        }
    }
    qCritical() << "client not found";
}

void Clients::setClientStartConfirmation(PLAYER_TYPE Type, bool bState)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_Type == Type)
        {
            if (bState && cl.m_isStartClickedByPlayer == bState)
                qWarning() << "client has already clicked start";

            Client changedClient = cl;
            changedClient.m_isStartClickedByPlayer = bState;

            int nClientPos = m_clients.indexOf(cl);
            if (nClientPos >= 0 && nClientPos < m_clients.size())
            {
                m_clients.replace(nClientPos, changedClient);
                qInfo() << "new state ="
                        << QString::number(m_clients.at(nClientPos).m_isStartClickedByPlayer);
            }
            else qCritical() << "iteration error. iter val =" << QString::number(nClientPos);

            this->showClientsInUI();

            return;
        }
    }
    qCritical() << "client not found";
}

Client Clients::getClient(QWebSocket* pClientSocket)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_socket == pClientSocket)
        {
            Client& client = cl;
            return client;
        }
    }
    qCritical() << "client not found";
    Client client;
    return client;
}

Client Clients::getClient(uint64_t un64ClientID, CLIENT_ID IdType /* = CID_CORE */)
{
    if (un64ClientID <= 0)
    {
        qCritical() << "tried to find client with " << clientIDAsQStr(IdType)
                    << "<= 0:" << QString::number(un64ClientID);
        return *new Client;
    }

    if (IdType == CID_CORE)
    {
        foreach (Client cl, m_clients)
        {
            if (cl.m_ID == un64ClientID)
            {
                Client& client = cl;
                return client;
            }
        }
    }
    else if (IdType == CID_SQL)
    {
        foreach (Client cl, m_clients)
        {
            if (cl.m_sqlID == un64ClientID)
            {
                Client& client = cl;
                return client;
            }
        }
    }
    else
    {
        qCritical() << "unknown CLIENT_ID type =" << QString::number(IdType);
        return *new Client;
    }

    qCritical() << "client not found. ID =" << QString::number(un64ClientID);
    return *new Client;
}

Client Clients::getPlayer(PLAYER_TYPE Type)
{
    foreach (Client cl, m_clients)
    {
        if (cl.m_Type == Type)
            return cl;
    }

    qCritical() << "client not found";
    Client cl;
    return cl;
}

Client Clients::getNextQueuedClient()
{
    uint64_t minQueue = std::numeric_limits<uint64_t>::max();
    foreach (Client cl, m_clients)
    {
        if (minQueue > cl.m_queue && cl.m_queue > 0)
            minQueue = cl.m_queue;
    }
    qInfo() << "minQueue =" << QString::number(minQueue);
    foreach (Client cl, m_clients)
    {
        if (cl.m_queue == minQueue)
        {
            qInfo() << "client.name =" << cl.name();
            return cl;
        }
    }
    qCritical() << "client not found";
    Client client;
    return client;
}

QString Clients::getQueuedClientsSqlIDsList()
{
    QString QStrQueuedClients = "";
    uint64_t maxQueue = 0;
    uint64_t minQueue = std::numeric_limits<uint64_t>::max();
    uint64_t lastBiggestQueue = std::numeric_limits<uint64_t>::max();
    foreach (Client cl, m_clients)
    {
        if (minQueue > cl.m_queue && cl.m_queue > 0)
            minQueue = cl.m_queue;
    }
    uint nNumberOfClients = m_clients.size();

    uint nLoopBreakingCounter = 0;
    if (minQueue > 0 && minQueue != std::numeric_limits<uint64_t>::max())
    {
        do
        {
            //find biggest queued nr, but smaller then last one
            foreach (Client cl, m_clients)
            {
                if (cl.m_queue > maxQueue && cl.m_queue < lastBiggestQueue)
                    maxQueue = cl.m_queue;
            }

            lastBiggestQueue = maxQueue; //remember last biggest client for next iteration

            //add last biggest found queued client sqlID to string for return
            foreach (Client cl, m_clients)
            {
                if (cl.m_queue == maxQueue)
                    QStrQueuedClients = QString::number(cl.m_sqlID) + " " + QStrQueuedClients;
            }
            maxQueue = 0; //clear for next iteration

            if (nLoopBreakingCounter > nNumberOfClients + 1)
            {
                qCritical() << "incorrect loop break. counter ="
                            << QString::number(nLoopBreakingCounter)
                            << ", nNumberOfClients =" << QString::number(nNumberOfClients+1);
                return "ERROR";
            }
            nLoopBreakingCounter++;

        } while (lastBiggestQueue != minQueue);

        //remove last comma
        QStrQueuedClients = QStrQueuedClients.left(QStrQueuedClients.length() - 1);
    }
    else QStrQueuedClients = "0";

    return QStrQueuedClients;
}

QString Clients::getQueuedClientsNamesList()
{
    QString QStrQueuedClientsNamesList;

    QStringList list = this->getQueuedClientsSqlIDsList().split(" ");
    QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
    foreach (QString QStrClientSqlId, list)
    {
        QStrClientSqlId = QStrClientSqlId.trimmed();
        if (!re.exactMatch(QStrClientSqlId))
        {
           qCritical() << "QStrClientSqlId has something else than digits only:"
                       << QStrClientSqlId;
           return "";
        }
        else QStrQueuedClientsNamesList +=
                this->getClient(QStrClientSqlId.toInt(), CID_SQL).name() + " ";
    }
    QStrQueuedClientsNamesList = QStrQueuedClientsNamesList.trimmed();

    return QStrQueuedClientsNamesList;
}

uint64_t Clients::getClientPosInQueue(const Client &client)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
            return cl.m_queue;
    }
    qCritical() << "client not found";
    return 0;
}

QWebSocket* Clients::getPlayerSocket(PLAYER_TYPE Type)
{
    if (Type != PT_NONE)
    {
        foreach (Client cl, m_clients)
        {
            if (cl.m_Type == Type)
                return cl.m_socket;
        }
        return nullptr;
    }
    else
    {
        qCritical() << "wrong parameter:" << QString::number(Type);
        return nullptr;
    }
}

QString Clients::getPlayerName(PLAYER_TYPE Type)
{
    if (Type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return playerTypeAsQStr(PT_WHITE);
    else if (Type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return playerTypeAsQStr(PT_BLACK);
    else
    {
        foreach (Client cl, m_clients)
        {
            if (cl.m_Type == Type)
                return cl.name();
        }
        qCritical() << "player not found:" << playerTypeAsQStr(Type);
        return "";
    }
}

uint Clients::getAmountOfQueuedClients()
{
    uint64_t maxQueue = 0;
    foreach (Client cl, m_clients)
    {
        if (maxQueue < cl.m_queue && cl.m_queue > 0)
            maxQueue = cl.m_queue;
    }

    if (maxQueue > 0)
    {
        uint64_t minQueue = std::numeric_limits<uint64_t>::max();
        foreach (Client cl, m_clients)
        {
            if (minQueue > cl.m_queue && cl.m_queue > 0)
                minQueue = cl.m_queue;
        }
        uint nAmountOfQueuedClients = 1 + maxQueue - minQueue;

        return nAmountOfQueuedClients;
    }
    else return 0;
}

uint64_t Clients::getClientID(const Client &client)
{
    foreach (Client cl, m_clients)
    {
        if (cl == client)
            return cl.m_ID;
    }
    qCritical() << "socket not found";
    return 0;
}

uint64_t Clients::getNextAvailableClientID()
{
    uint64_t maxID = 0;

    foreach (Client client, m_clients) //find max queued ID
    {
        if (client.m_ID > maxID)
            maxID = client.m_ID;
    }
    return ++maxID;
}

QString Clients::dumpAllData()
{
    QString QStrData;

    foreach (Client cl, m_clients)
    {
        QStrData += "[client.h] m_ID: " + QString::number(cl.ID())
                + ", m_sqlID:" + QString::number(cl.sqlID())
                + ", m_socket->origin():" + cl.socket()->origin()
                + ", m_socket->peerAddress():" + cl.socket()->peerAddress().toString()
                + ", m_Type: " + playerTypeAsQStr(cl.type())
                + ", m_isStartClickedByPlayer: " + QString::number(cl.isStartClickedByPlayer())
                + ", m_queue: " + QString::number(cl.queue())
                + ", name: " + cl.name(DONT_SHOW_ERRORS) + "\n";
    }

    return QStrData;
}

void Clients::showClientsInUI()
{
    emit this->showClientsListInUI(m_clients);
    emit this->setBoardDataLabel(std::to_string(m_clients.size()).c_str(),
                                 BDL_SOCKETS_ONLINE);
}
