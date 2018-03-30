#include "websockets.h"

Websockets::Websockets(Clients* pClientsList, quint16 port, QObject* parent):
    QObject(parent),
    _pClientsList()
{
    _pClientsList = pClientsList;

    _pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode, this);
    if (_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Websockets::Websockets(): connecting to port:" << port;

        connect(_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        emit this->addTextToLogPTE("WebSocket server listening on port" + port, LOG_WEBSOCKET);
    }
}

Websockets::~Websockets()
{
    _pWebSocketServer->close();

    //future: póki mam jednen obiekt/ramię to nie jest mi to potrzebne
    //qDeleteAll(_pClientsList.begin(), _pClientsList.end());
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

//future: Q_FOREACH (QWebSocket *pNextClient, _pClientsList) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(_pClientsList))"
//póki nie zmieniam wersji to może tak zostać

//TODO: obsuga komend serwisowych przez websockety

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

    QString QStrSenderName = _pClientsList->getClient(pSocket).name;
    qDebug() << "Websockets::receivedMsg(): client: name =" << QStrSenderName
             << "queue =" << QString::number(_pClientsList->getClient(pSocket).queue)
             << "ID =" << QString::number(_pClientsList->getClient(pSocket).ID);

    emit this->msgFromWebsocketsToChess(QStrMsg, _pClientsList->getClient(pSocket).ID);
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

    Q_FOREACH (Client client, _pClientsList->getClientsList())
        client.socket->sendTextMessage(QStrMsg);
}

void Websockets::socketDisconnected()
{
    QWebSocket* pSocket = qobject_cast<QWebSocket *>(sender());

    Client client = _pClientsList->getClient(pSocket);
    qDebug() << "Websockets::socketDisconnected(): approaching isClientInList()";
    if (_pClientsList->isClientInList(client))
        emit this->msgFromWebsocketsToChess("clientLeft", client.ID);
}
