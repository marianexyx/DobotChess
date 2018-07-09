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

    //future: not needed for just one instance of class
    //qDeleteAll(_pClientsList.begin(), _pClientsList.end());
}

void Websockets::listenOnPort(quint16 port)
{
    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode, this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Websockets::Websockets(): connecting to port:" << port;

        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        emit this->addTextToLogPTE("Server listening on port " +
                                   QString::number(port) + "\n", LOG_WEBSOCKET);
    }
}

void Websockets::onNewConnection()
{
    qDebug() << "Websockets::onNewConnection()";

    QWebSocket* pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    _pClientsList->newClient(*pSocket);
    emit this->addTextToLogPTE("New connection \n", LOG_WEBSOCKET);
    _pClientsList->showClientsInUI();
}

//future: Q_FOREACH (QWebSocket *pNextClient, _pClientsList) will be depreciated...
//...to: "for (QWebSocket *pClient : qAsConst(_pClientsList))".
//it can be this way in old QT

//TODO: let websockets service the service messages

void Websockets::receivedMsg(QString QStrMsg)
{    
    if (QStrMsg != "keepConnected")
        qDebug() << "Websockets::receivedMsg():" << QStrMsg;
    else return;

    QWebSocket* pSocket = qobject_cast<QWebSocket*>(sender());
    if (!_pClientsList->isClientInList(_pClientsList->getClient(pSocket)))
    {
        qDebug() << "WARNING: Websockets::receivedMsg(): sender not in clients list";
        return;
    }

    emit this->msgFromWebsocketsToChess(QStrMsg, _pClientsList->getClient(pSocket).ID);
}

void Websockets::sendMsgToClient(QString QStrMsg, int64_t n64ReceiverID)
{
    qDebug() << "Websockets::sendMsgToClient(): received:" << QStrMsg;

    if (n64ReceiverID < 1)
    {
        qDebug() << "ERROR: Websockets::sendMsgToClient(): receiver ID < 1. "
                    "it's =" << n64ReceiverID;
        return;
    }
    else
    {
        Client receiver = _pClientsList->getClient(n64ReceiverID);
        emit this->addTextToLogPTE("send to: " + receiver.name() + " : "
                                   + QStrMsg + "\n", LOG_WEBSOCKET);
        receiver.socket->sendTextMessage(QStrMsg);
    }
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    emit this->addTextToLogPTE("send to all: " + QStrMsg + "\n", LOG_WEBSOCKET);

    Q_FOREACH (Client client, _pClientsList->getClientsList())
        client.socket->sendTextMessage(QStrMsg);
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());

    Client client = _pClientsList->getClient(pSocket);
    if (_pClientsList->isClientInList(client))
        emit this->msgFromWebsocketsToChess("clientLeft", client.ID);
}
