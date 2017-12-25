#include "websockets.h"

Websockets::Websockets(quint16 port, QObject *parent):
    QObject(parent),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        qDebug() << "WebSocket server listening on port" << port;
        emit addTextToConsole("WebSocket server listening on port" + port, LOG_WEBSOCKET);
    }
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();

    //póki mam jednen obiekt/ramię to nie jest mi to potrzebne
    //qDeleteAll(m_clients.begin(), m_clients.end());

}

void Websockets::onNewConnection()
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClientSocket(pSocket);
    emit addTextToConsole("New connection \n", LOG_WEBSOCKET);

    emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    emit showClientsList(m_clients);
}

//Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
//póki nie zmieniam wersji to może tak zostać

//TODO2: obsuga komend serwisowych przez websockety

void Websockets::receivedMsg(QString QStrWsMsgToProcess)
{    
    if (QStrWsMsgToProcess != "keepConnected")
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWsMsgToProcess;

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender()); //przysyłający

    if (QStrWsMsgToProcess == "keepConnected")
        emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    else if (QStrWsMsgToProcess == "newGame" || QStrWsMsgToProcess == "new")
    {
        QString QStrWhoSent = "";
        if (pClient == this->getPlayerSocket(PT_WHITE))
            QStrWhoSent = "WHITE";
        else if (pClient == this->getPlayerSocket(PT_BLACK))
            QStrWhoSent = "BLACK";

        this->sendToChess(QStrWsMsgToProcess + " " + QStrWhoSent);
    }
    else if (QStrWsMsgToProcess.left(4) == "move")
        this->sendToChess(QStrWsMsgToProcess);
    else if (QStrWsMsgToProcess == "getTableDataAsJSON")
        pClient->sendTextMessage(this->getTableDataAsJSON());
    else if (QStrWsMsgToProcess == "giveUp")
    {
        if (pClient) //todo: zrozumieć to i dać w razie czego więcej tych warunków tam gdzie są...
            //...QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        {
            if (this->isClientAPlayer(pClient))
                this->playerIsLeavingGame(pClient, ET_GIVE_UP);
            else qDebug() << "ERROR: Websockets::receivedMsg(): non-player tried to logout (hacker?)";
        }
        else qDebug() << "ERROR: Websockets::receivedMsg(): giveUp: !isset pClient";
    }
    else if (QStrWsMsgToProcess.left(5) == "sitOn")
    {
        if (this->getClientName(pClient) == "-1")
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): sitOn: client not loggedIn";
            return;
        }

        PLAYER_TYPE playerChair = playerTypeFromQStr(QStrWsMsgToProcess.mid(5));

        _pChessboard->setWhoseTurn(NO_TURN);
        if (!this->isPlayerChairEmpty(playerChair))
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): client wanted to "
                        "sit on occupied chair. client name:"
                     << this->getClientName(pClient) << ", player name:" <<
                        this->getPlayerName(playerChair);
            return;
        }
        else
            this->setPlayerType(pClient, playerChair);

        if (this->isGameTableOccupied())
            _pChessboard->startQueueTimer();

        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else if (QStrWsMsgToProcess == "standUp")
    {
        if (this->isClientAPlayer(pClient))
        {
            if (_pChessboard->getWhoseTurn() == NO_TURN)
            {
                this->clearPlayerType(this->getClientType(pClient));
                _pChessboard->resetBoardData();

                Q_FOREACH (Clients client, m_clients)
                    client.socket->sendTextMessage(this->getTableDataAsJSON());
            }
            else
            {
                qDebug() << "ERROR: Websockets::receivedMsg(): player wanted to "
                            "leave chair during game. client name:"
                          << this->getClientName(pClient);
                return;
            }
        }
        else
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): non-player client"
                        " wanted to leave chair. client name:"
                     << this->getClientName(pClient);
            return;
        }
    }
    else if (QStrWsMsgToProcess.left(3) == "im ")
    {
        QString QStrName = QStrWsMsgToProcess.mid(3);
        if (this->isClientNameExists(QStrName))
        {
            this->getClientSocket(QStrName)->sendTextMessage("logout:doubleLogin");
            if (this->isClientAPlayer(pClient))
                this->playerIsLeavingGame(pClient, ET_SOCKET_LOST);
            this->setClientName(pClient, QStrName);
        }
        else
        {
            this->setClientName(pClient, QStrName);
            pClient->sendTextMessage(this->getTableDataAsJSON());
        }
    }
    else if (QStrWsMsgToProcess.left(9) == "promoteTo")
    {
        this->sendToChess(QStrWsMsgToProcess);
    }
    else if (QStrWsMsgToProcess == "queueMe")
    {
        this->addClientToQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else if (QStrWsMsgToProcess == "leaveQueue")
    {
        this->removeClientFromQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableDataAsJSON());
    }
    else  qDebug() << "ERROR: Websockets::receivedMsg(): unknown parameter:" << QStrWsMsgToProcess;

    emit showClientsList(m_clients);
}

void Websockets::sendMsg(QString QStrWsMsg) //todo: przepychac wiadomosci, nei realizowac ich tutaj
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", LOG_WEBSOCKET);

    if (QStrWsMsg == "newOk" || QStrWsMsg.left(8) == "history " || QStrWsMsg.left(8) == "promoted" ||
            QStrWsMsg == "reseting")
    {
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg.left(6) == "moveOk" || QStrWsMsg.left(7) == "badMove")
    {
        if (QStrWsMsg.right(7) == "promote" || QStrWsMsg.left(7) == "badMove")
        {
            if (_pChessboard->getWhoseTurn() == WHITE_TURN && this->getPlayerSocket(PT_WHITE) != nullptr)
                this->getPlayerSocket(PT_WHITE)->sendTextMessage(QStrWsMsg);
            else if (_pChessboard->getWhoseTurn() == BLACK_TURN && this->getPlayerSocket(PT_BLACK) != nullptr)
                this->getPlayerSocket(PT_BLACK)->sendTextMessage(QStrWsMsg);
            else
                qDebug() << "ERROR: Websockets::sendMsg: uknown/wrong turn:" << _pChessboard->getWhoseTurn();
        }
        else
        {
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(QStrWsMsg);
        }
    }
    else if (QStrWsMsg == "resetComplited")
    {
        if (this->isGameTableOccupied())
            _pChessboard->startQueueTimer();

        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg + " " + this->getTableDataAsJSON());
    }
    else if (QStrWsMsg == "timeOutStart")
    {
        if (!this->isStartClickedByPlayer(PT_WHITE))
            this->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        if (!this->isStartClickedByPlayer(PT_BLACK))
            this->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);

        _pClientsList->resetPlayersStartConfirmInfo();
         _pChessboard->stopQueueTimer();

        if (this->isGameTableOccupied())
            _pChessboard->startQueueTimer();
    }
    else if (QStrWsMsg == "timeOutWhite" || QStrWsMsg == "timeOutBlack")
    {
        this->playerIsLeavingGame(this->getPlayerSocket(playerTypeFromQStr(QStrWsMsg.right(5))),
                                  ET_TIMEOUT_GAME);
        this->endOfGame(ET_TIMEOUT_GAME);

        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg + " " + this->getTableDataAsJSON());
    }
    else qDebug() << "ERRROR: uknown Websockets::sendMsg() parameter:" << QStrWsMsg;
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    emit this->addTextToConsole("Sending to 'chess' class: " + QsMsgForChessClass + "\n", LOG_WEBSOCKET);
    emit this->MsgFromWebsocketsToChess(QsMsgForChessClass);
}

void Websockets::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) //todo: co to??? czyżby to że sprawdzam czy udało mi się zrzutować obiekt...
        //... z powyższej linjki?
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

void Websockets::cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE chair)
{
    this->clearPlayerType(chair);

    if (this->getQueuedClientsList() != "queueEmpty")
    {
        Clients nextQueuedClient;
        if (this->isClientInList(getNextQueuedClientSocket()))
            nextQueuedClient = this->getClient(this->getNextQueuedClientSocket());
        else return;

        this->removeClientFromQueue(nextQueuedClient.socket);
        this->setPlayerType(nextQueuedClient.socket, chair);
        this->setClientState(nextQueuedClient.socket, false);

        emit this->setBoardDataLabels(this->getPlayerName(chair),
                                      chair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
        qDebug() << "Websockets::cleanChairAndPutThereNextQueuedClientIfExist(): new" <<
                    playerTypeAsQStr(chair) << "player name =" << this->getPlayerName(chair);
        emit this->addTextToConsole("New " + playerTypeAsQStr(chair) + " player: " +
                                    this->getPlayerName(chair) + "\n", LOG_WEBSOCKET);
    }
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

void Websockets::playerIsLeavingGame(QWebSocket *pClient, END_TYPE leavingType)
{
    //todo: reszta stanow tu poki co nie pasuje, bo gracz nei wychodzi z gry, tylko...
    //... jest ona normalnie konczona. jak przeniose to do wlasciwych klas, to...
    //...bedzie to mozna normlanie polaczyc
    if (leavingType != ET_SOCKET_LOST && leavingType != ET_GIVE_UP &&
            leavingType != ET_TIMEOUT_GAME)
    {
        qDebug() << "ERROR: Websockets::playerIsLeavingGame: wrong leavingType val:" <<
                    leavingType;
        return;
    }

    this->endOfGame(leavingType, pClient);

    Q_FOREACH (Clients client, m_clients)
        client.socket->sendTextMessage(endTypeAsQstr(leavingType) +
                                       playerTypeAsQStr(this->getClientType(pClient)) +
                                       " " + this->getTableDataAsJSON());

    emit MsgFromWebsocketsToChess("reset");
}
