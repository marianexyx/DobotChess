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

void Websockets::onNewConnection()
{
    QWebSocket* pSocket = m_pWebsocketserver->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    m_pClientsList->newClient(pSocket);
    emit this->addTextToLogPTE("New connection \n", LOG_WEBSOCKET);
    m_pClientsList->showClientsInUI();
}

//future: make a container of incomming msgs (not easy task)
void Websockets::receivedMsg(QString QStrMsg)
{    
    if (QStrMsg != "keepConnected") //todo: also keep connect on server side
        qInfo() << QStrMsg;
    else return;

    QWebSocket* pSocket = qobject_cast<QWebSocket*>(sender());
    if (!m_pClientsList->isClientInList(m_pClientsList->getClient(pSocket)))
    {
        qWarning() << "sender not in clients list";
        return;
    }

    emit this->msgFromWebsocketsToChess(QStrMsg, m_pClientsList->getClient(pSocket).ID());
}

void Websockets::sendMsgToClient(QString QStrMsg, int64_t n64ReceiverID)
{
    qInfo() << "received:" << QStrMsg;

    if (n64ReceiverID < 1)
    {
        qCritical() << "receiver ID < 1. it's =" << QString::number(n64ReceiverID);
        return;
    }
    else
    {
        Client receiver = m_pClientsList->getClient(n64ReceiverID);
        QString QStrReceiverName = receiver.sqlID() > 0 ? receiver.name() : "client";
        emit this->addTextToLogPTE("send to: " + QStrReceiverName + " : "
                                   + QStrMsg + "\n", LOG_WEBSOCKET);
        receiver.socket()->sendTextMessage(QStrMsg);
    }
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    emit this->addTextToLogPTE("send to all: " + QStrMsg + "\n", LOG_WEBSOCKET);

    foreach (Client client, m_pClientsList->getClientsList())
    {
        qInfo() << "client name =" << client.name();
        client.socket()->sendTextMessage(QStrMsg);
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
