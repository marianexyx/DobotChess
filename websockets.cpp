#include "websockets.h"

Websockets::Websockets(Clients* pClients, quint16 port, QObject* parent): //todo: pClients??
    QObject(parent),
    _pClients()
{
    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode, this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        emit this->addTextToLogPTE("WebSocket server listening on port" + port, LOG_WEBSOCKET);
    }
}

Websockets::~Websockets()
{
    _pWebSocketServer->close();

    //future: póki mam jednen obiekt/ramię to nie jest mi to potrzebne
    //qDeleteAll(_pClients.begin(), _pClients.end());
}

void Websockets::onNewConnection()
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket* pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClient(pSocket);
    emit this->addTextToLogPTE("New connection \n", LOG_WEBSOCKET);
    _pClients->showClientsInForm();
}

//Q_FOREACH (QWebSocket *pNextClient, _pClients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(_pClients))"
//póki nie zmieniam wersji to może tak zostać

//TODO: obsuga komend serwisowych przez websockety

void Websockets::receivedMsg(QString QStrMsg)
{    
    if (QStrMsg != "keepConnected")
        qDebug() << "Websockets::receivedMsg():" << QStrMsg;
    else return;

    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());
    emit this->msgFromWebsocketsToChess(QStrMsg, _pClients->getClient(pSocket));
}

void Websockets::sendMsgToClient(QString QStrMsg, Client* pClient)
{
    qDebug() << "Websockets::sendMsgToClient(): received:" << QStrMsg;

    if (pClient == nullptr)
    {
        qDebug() << "ERROR: Websockets::sendMsgToClient(): pClient == nullptr";
        return;
    }
    else
    {
        emit this->addTextToLogPTE("send to: " + pClient->name + " " + QStrMsg + "\n",
                                    LOG_WEBSOCKET);
        pClient->socket->sendTextMessage(QStrMsg);
    }
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    emit this->addTextToLogPTE("send to all: " + QStrMsg + "\n", LOG_WEBSOCKET);

    Q_FOREACH (Client client, _pClients->getClientsList())
        client.socket->sendTextMessage(QStrMsg);
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());
    emit this->msgFromWebsocketsToChess("clientLeft", _pClients->getClient(pSocket));
}
