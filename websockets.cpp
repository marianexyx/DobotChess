#include "websockets.h"

Websockets::Websockets(QObject* parent):
    QObject(parent)
{
    m_pClientsList = new Clients();

}

Websockets::~Websockets()
{
    m_pWebsocketserver->close();

    //future: code below isn't needed for just one instance of class
    //qDeleteAll(m_pClientsList.begin(), m_pClientsList.end());
}

void Websockets::listenOnPort(quint16 port)
{
    m_pWebsocketserver = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode, this);
    if (m_pWebsocketserver->listen(QHostAddress::Any, port))
    {
        qInfo() << "connecting to port:" << QString::number(port);

        connect(m_pWebsocketserver, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        emit this->addTextToLogPTE("Server listening on port " +
                                   QString::number(port) + "\n", LOG_WEBSOCKET);
    }
}

/*static*/ QString Websockets::toJSON(QString QStrData)
{
    QStrData = "{\"" + QStrData;
    QStrData.replace(":", "\":\"");
    QStrData.replace(",", "\",\"");
    QStrData += "\"}";
    return QStrData;
}

void Websockets::onNewConnection()
{
    QWebSocket* pSocket = m_pWebsocketserver->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    m_pClientsList->newClient(pSocket);
    uint64_t clientID = m_pClientsList->getClient(pSocket).ID();
    emit this->addTextToLogPTE("New connection (new ID = " + QString::number(clientID) + ")\n",
                               LOG_WEBSOCKET);
    m_pClientsList->showClientsInUI();
    //future: do this function immadietely, like in return, f.e.: getStartData() from...
    //...chess class, where i can just return table data. BUT websockets are inside chess...
    //...class, so until i changed it, it could be only emitted by singal
    emit this->msgFromWebsocketsToChess("getTableData", clientID);
}

//future: make a container of incomming msgs (not easy task)
//todo: test bad request reactions, and site implementation for this reaction
void Websockets::receivedMsg(QString QStrMsg)
{    
    QWebSocket* pSocket = qobject_cast<QWebSocket*>(sender());
    if (!m_pClientsList->isClientInList(m_pClientsList->getClient(pSocket)))
    {
        qWarning() << "sender not in clients list";
        return;
    }
    else qInfo() << "from client with ID ="
                 << QString::number(m_pClientsList->getClient(pSocket).ID())
                 << "; msg =" << QStrMsg;

    Client cl = m_pClientsList->getClient(pSocket);
    if (cl.synchronized() != SY_DOUBLE_LOGIN && cl.synchronized() != SY_REMOVE_AND_REFRESH_CLIENT)
        emit this->msgFromWebsocketsToChess(QStrMsg, m_pClientsList->getClient(pSocket).ID());
    else qWarning() << "incoming msg from client with forbidden synchronization type:"
                    << synchronizedAsQStr(cl.synchronized()) << ", cl:" << cl.dumpCrucialData();
}

void Websockets::sendMsgToClient(QString QStrMsg, uint64_t un64ReceiverID)
{
    if (un64ReceiverID >= 1)
    {
        if (!m_pClientsList->isClientIDExists(un64ReceiverID, SHOW_ERRORS))
            return;

        Client receiver = m_pClientsList->getClient(un64ReceiverID);
        QString QStrSynchronization = "0:" + QString::number(receiver.synchronized());
        QStrMsg = QStrSynchronization + "," + QStrMsg;
        QStrMsg = Websockets::toJSON(QStrMsg);
        QString QStrReceiverName = receiver.sqlID() > 0 ? receiver.name() : "client";
        emit this->addTextToLogPTE("send to: " + QStrReceiverName + " : "
                                   + QStrMsg + "\n", LOG_WEBSOCKET);
        receiver.socket()->sendTextMessage(QStrMsg);
    }
    else qCritical() << "receiver ID < 1. it's =" << QString::number(un64ReceiverID);
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    foreach (Client client, m_pClientsList->getClientsList())
    {
        QString QStrSynchronization = "0:" + QString::number(client.synchronized());
        QString QStrFullInfo = QStrSynchronization + "," + QStrMsg;
        QString QStrFullInfoJSON = Websockets::toJSON(QStrFullInfo);

        if (QStrMsg != "getTableData")
            emit this->addTextToLogPTE("send to all (" + client.name(DONT_SHOW_ERRORS)
                                       + "): "  + QStrFullInfoJSON + "\n", LOG_WEBSOCKET);

        client.socket()->sendTextMessage(QStrFullInfoJSON);
    }
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());
    if (m_pClientsList->isClientInList(pSocket))
    {
        Client client = m_pClientsList->getClient(pSocket);
        if (m_pClientsList->isClientInList(client))
            emit this->msgFromWebsocketsToChess("clientLeft", client.ID());
    }
}
