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

//TODO2: obsuga komend serwisowych przez websockety

void Websockets::receivedMsg(QString QStrWsMsgToProcess)
{    
    if (QStrWsMsgToProcess != "keepConnected")
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWsMsgToProcess;
    else return;

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender()); //przysyłający 
    int64_t clientID = _pClients->getClientID(_pClients->getClient(pClient));

    this->sendToChess(QStrWsMsg, clientID);
}

void Websockets::sendMsg(QString QStrWsMsg, int64_t ID = -1)
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;

    if (ID == -1)
    {
        emit addTextToConsole("send to all: " + QStrWsMsg + "\n", LOG_WEBSOCKET);

        Q_FOREACH (Client client, _pClients->getClientsList())
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if(!_pClients->isClientIDExists(ID))
    {
        qDebug() << "ERROR: Websockets::sendMsg: client ID doesn't exists";
        return;
    }
    else
    {
        Client client = _pClients->getClient(ID);
        emit addTextToConsole("send to: " + client.name + " " +
                              QStrWsMsg + "\n", LOG_WEBSOCKET);
        client.socket->sendTextMessage(QStrWsMsg);
    }
}

//TODO: można stąd usunąć jeżeli komunikaty będą się wyświetlać w klasie chess
void Websockets::sendToChess(QString QStrMsg, int64_t clientID)
{
    qDebug() << "Sending to 'chess' class: " << QStrMsg;
    emit this->addTextToConsole("Sending to 'chess' class: " + QStrMsg + "\n",
                                LOG_WEBSOCKET);
    emit this->msgFromWebsocketsToChess(QStrMsg, clientID);
}

void Websockets::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    int64_t clientID = _pClients->getClientID(_pClients->getClient(pClient));
    emit sendToChess("clientLeft", clientID);
}

void Websockets::sendMsgToAllClients(QString msg)
{
    Q_FOREACH (Clients client, _pClients->getClientsList())
        client.socket->sendTextMessage(msg);
}
