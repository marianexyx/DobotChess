#include "websockets.h"

Websockets::Websockets(Clients* pClientsList,  QObject* parent):
    QObject(parent),
    _pClientsList()
{
    _pClientsList = pClientsList;
}

Websockets::~Websockets()
{
    _pWebSocketServer->close();

    //future: code below isn't needed for just one instance of class
    //qDeleteAll(_pClientsList.begin(), _pClientsList.end());
}

void Websockets::listenOnPort(quint16 port)
{
    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode, this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qInfo() << "connecting to port:" << port;

        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        emit this->addTextToLogPTE("Server listening on port " +
                                   QString::number(port) + "\n", LOG_WEBSOCKET);
    }
}

void Websockets::onNewConnection()
{
    QWebSocket* pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    _pClientsList->newClient(*pSocket);
    emit this->addTextToLogPTE("New connection \n", LOG_WEBSOCKET);
    _pClientsList->showClientsInUI();
}

//todo: Q_FOREACH (QWebSocket *pNextClient, _pClientsList) will be depreciated...
//...to: "for (QWebSocket *pClient : qAsConst(_pClientsList))".
//it can be this way in old QT. or try change all to "foreach" maybe?

void Websockets::receivedMsg(QString QStrMsg)
{    
    if (QStrMsg != "keepConnected")
        qInfo() << QStrMsg;
    else return;

    QWebSocket* pSocket = qobject_cast<QWebSocket*>(sender());
    if (!_pClientsList->isClientInList(_pClientsList->getClient(pSocket)))
    {
        qWarning() << "sender not in clients list";
        return;
    }

    emit this->msgFromWebsocketsToChess(QStrMsg, _pClientsList->getClient(pSocket).ID());
}

void Websockets::sendMsgToClient(QString QStrMsg, int64_t n64ReceiverID)
{
    qInfo() << "received:" << QStrMsg;

    if (n64ReceiverID < 1)
    {
        qCritical() << "receiver ID < 1. it's =" << n64ReceiverID;
        return;
    }
    else
    {
        Client receiver = _pClientsList->getClient(n64ReceiverID);
        QString QStrReceiverName = receiver.sqlID() > 0 ? receiver.name() : "client";
        emit this->addTextToLogPTE("send to: " + QStrReceiverName + " : "
                                   + QStrMsg + "\n", LOG_WEBSOCKET);
        receiver.socket()->sendTextMessage(QStrMsg);
    }
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    emit this->addTextToLogPTE("send to all: " + QStrMsg + "\n", LOG_WEBSOCKET);

    Q_FOREACH (Client client, _pClientsList->getClientsList())
    {
        qInfo() << "client name =" << client.name();
        client.socket()->sendTextMessage(QStrMsg);
    }
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());
    if (_pClientsList->isClientInList(pSocket))
    {
        Client client = _pClientsList->getClient(pSocket);
        if (_pClientsList->isClientInList(client))
            emit this->msgFromWebsocketsToChess("clientLeft", client.ID());
    }
}
