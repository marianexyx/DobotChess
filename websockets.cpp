#include "websockets.h"

Websockets::Websockets(Clients *pClients, quint16 port, QObject *parent):
    QObject(parent),
    m_clients()
{
    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        qDebug() << "WebSocket server listening on port" << port;
        emit addTextToConsole("WebSocket server listening on port" + port, LOG_WEBSOCKET);
    }
}

Websockets::~Websockets()
{
    _pWebSocketServer->close();

    //póki mam jednen obiekt/ramię to nie jest mi to potrzebne
    //qDeleteAll(m_clients.begin(), m_clients.end());

}

void Websockets::onNewConnection()
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = _pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClient(pSocket);
    emit addTextToConsole("New connection \n", LOG_WEBSOCKET);
    _pClients->showClientsInForm();
}

//Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
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
        emit addTextToConsole("send to: " + pClient->name + " " + QStrMsg + "\n", LOG_WEBSOCKET);
        pClient->socket->sendTextMessage(QStrMsg);
    }
}

void Websockets::sendMsgToAllClients(QString QStrMsg)
{
    emit addTextToConsole("send to all: " + QStrMsg + "\n", LOG_WEBSOCKET);

    Q_FOREACH (Client client, _pClients->getClientsList())
        client.socket->sendTextMessage(QStrMsg);
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());
    emit this->msgFromWebsocketsToChess("clientLeft", _pClients->getClient(pSocket));
}
