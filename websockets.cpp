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
    this->newClientSocket(pSocket);
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
    int64_t clientID = _pClients->getClientID(pClient);

    this->sendToChess(QStrWsMsg, clientID);
}

void Websockets::sendMsg(QString QStrWsMsg) //todo: przepychac wiadomosci, nei realizowac ich tutaj
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", LOG_WEBSOCKET);

    Q_FOREACH (Client client, _pClients->getClientsList())
        client.socket->sendTextMessage(QStrWsMsg);
}

void Websockets::sendMsg(int64_t ID, QString QStrWsMsg)
{
    if(!_pClients->isClientIDExists(ID)) return;

    Client client = _pClientsList->getClient(ID);
    client.socket->sendTextMessage(QStrWsMsg);
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

    if (pClient) //todo: co to??? czyżby to że sprawdzam czy udało mi się zrzutować obiekt...
        //... z powyższej linjki?
        //todo: wnętrze tej funkcji przerzucić do chess
    {
        if (this->isClientAPlayer(pClient))
            this->playerIsLeavingGame(pClient, ET_SOCKET_LOST);
        else
        {
            //todo: wyświetla mi się disconnect w dziwnym miejscu
            qDebug() << "non-player disconnected";
            emit addTextToConsole("non-player disconnected\n", LOG_WEBSOCKET);
        }

        pClient->deleteLater();

        qDebug() << "Websockets::socketDisconnected(): remove client from list";
        for(int i = 0; i < m_clients.count(); ++i)
        { //nie umiem obsłużyć removeAll na structurach
          if(m_clients.at(i).socket == pClient)
          {
            m_clients.removeAt(i);
            break;
          }
        }
    }

    emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    emit showClientsList(m_clients);
}

//todo: funkcja w sumie nic nie mówi o tym co dokładnie robi. i znajduje si w zlej klasie
void Websockets::endOfGame(END_TYPE EndType, QWebSocket *playerToClear)
{
    QString QStrPlayer = "";
    if (playerToClear != nullptr) QStrPlayer = this->getClientName(playerToClear) + ":";
    qDebug() << "Websockets::endOfGame():" << QStrPlayer << endTypeAsQstr(EndType);

    _pChessboard->resetBoardData();

    switch(EndType)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
    case ET_TIMEOUT_GAME:
        this->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        this->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        break;

    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (this->getClientType(playerToClear) == PT_WHITE)
            this->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        else if (this->getClientType(playerToClear) == PT_BLACK)
            this->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        else qDebug() << "ERROR: Websockets::endOfGame(): wrong player type:" << EndType;
        break;

    default:
        qDebug() << "ERROR: Websockets::endOfGame: unknown EndType val=" << EndType;
        break; 
    }
}

void Websockets::sendMsgToAllClients(QString msg)
{
    Q_FOREACH (Clients client, _pClients->getClientsList())
        client.socket->sendTextMessage(msg);
}
