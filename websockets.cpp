#include "websockets.h"

Websockets::Websockets(Chessboard *pChessboard, quint16 port, QObject *parent):
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

    _pChessboard = pChessboard;
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

void Websockets::resetPlayersStartConfirmInfo()
{
    qDebug() << "Websockets::resetPlayersStartConfirmInfo()";
    if (this->isStartClickedByPlayer(PT_WHITE))
           this->setClientState(PT_WHITE, false);
    if (this->isStartClickedByPlayer(PT_BLACK))
        this->setClientState(PT_BLACK, false);
}

QString Websockets::getTableDataAsJSON()
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt","wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + this->getPlayerName(PT_WHITE) +
            "\",\"bplr\":\"" + this->getPlayerName(PT_BLACK) +
            "\",\"turn\":\"" + _pChessboard->getStrWhoseTurn() +
            "\",\"wtime\":" + QString::number(_pChessboard->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pChessboard->getBlackTimeLeft()) +
            ",\"queue\":\"" + this->getQueuedClientsList();

    if (_pChessboard->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart = this->isStartClickedByPlayer(PT_WHITE) ? "w" : "x";
        QString QStrBlackClickedStart = this->isStartClickedByPlayer(PT_BLACK) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pChessboard->getStartTimeLeft());
    }

    if (!_pChessboard->getHisotyMoves().isEmpty())
        QStrTableData += "\",\"history\":\"" + _pChessboard->getHisotyMovesAsQStr();

    QStrTableData += "\"}";

    qDebug() << "Websockets::getTableDataAsJSON(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Websockets::receivedMsg(QString QStrWsMsgToProcess)
{    
    if (QStrWsMsgToProcess != "keepConnected")
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWsMsgToProcess;

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (QStrWsMsgToProcess == "keepConnected")
    {
        emit setBoardDataLabels(std::to_string(m_clients.size()).c_str(), BDL_SOCKETS_ONLINE);
    }
    else if (QStrWsMsgToProcess == "newGame" || QStrWsMsgToProcess == "new")
    {
        QString QStrWhoSent = "";
        if (pClient == this->getPlayerSocket(PT_WHITE))  QStrWhoSent = "WHITE";
        else if (pClient == this->getPlayerSocket(PT_BLACK)) QStrWhoSent = "BLACK";

        this->sendToChess(QStrWsMsgToProcess + " " + QStrWhoSent);
    }
    else if (QStrWsMsgToProcess.left(4) == "move") { this->sendToChess(QStrWsMsgToProcess); }
    else if (QStrWsMsgToProcess == "getTableDataAsJSON") pClient->sendTextMessage(this->getTableDataAsJSON());
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

        PLAYERS_TYPES playerChair = playerTypeFromQStr(QStrWsMsgToProcess.mid(5));

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

        this->resetPlayersStartConfirmInfo(); //todo: to jest  bardziej zmienna stolu, czy clienta?
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

void Websockets::msgFromChessboardToWebsockets(QString QStrWsMsg)
{
    //todo: to niepotrzebne przejście zniknie przy polaczeniu klas
    this->sendMsg(QStrWsMsg);
}

void Websockets::cleanChairAndPutThereNextQueuedClientIfExist(PLAYERS_TYPES chair)
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

void Websockets::newClientSocket(QWebSocket *clientSocket)
{    
    Clients newClient;
    newClient.socket = clientSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.isStartClickedByPlayer = false;
    newClient.type = PT_NONE;

    m_clients << newClient;
}

void Websockets::setClientName(QWebSocket *clientSocket, QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
        {
            qDebug() << "ERROR: Websockets::setClientName: name" << name << "already exists.";
            return;
        }
    }

    if (this->getClientName(clientSocket) == name)

    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.name = name;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientName: new name =" << m_clients.at(nClientpos).name;
            }
            else qDebug() << "ERROR: Websockets::setClientName: iteration error. iter val =" << nClientpos;

            return;
        }
    }

    Clients client;
    qDebug() << "ERROR: Websockets::setClientName: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(clientSocket) =" << typeid(clientSocket).name();
}

void Websockets::setPlayerType(QWebSocket *clientSocket, PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (type != PT_NONE && client.queue > 0)
            {
                qDebug() << "ERROR: Websockets::setPlayerType: client in queue can not sit on chair";
                return;
            }

            Clients changedClient = client;
            changedClient.type = type;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setPlayerType: new" <<
                            playerTypeAsQStr(m_clients.at(nClientpos).type) << "player:" <<
                            this->getPlayerName(type) ;
                emit this->addTextToConsole("New " + playerTypeAsQStr(m_clients.at(nClientpos).type)
                                            + " player: " + this->getPlayerName(type) +
                                            "\n", LOG_WEBSOCKET);
                emit setBoardDataLabels(this->getPlayerName(type),
                                        type == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);
            }
            else
                qDebug() << "ERROR: Websockets::setPlayerType: iteration error. iter val ="
                         << nClientpos;

            return;
        }
    }
    Clients client;
    qDebug() << "ERROR: Websockets::setPlayerType: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(clientSocket) =" << typeid(clientSocket).name();

}

void Websockets::clearPlayerType(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                Clients changedClient = client;
                changedClient.type = PT_NONE;
                changedClient.isStartClickedByPlayer = false;

                int nClientpos = m_clients.indexOf(client);
                if (nClientpos >= 0 && nClientpos < m_clients.size())
                {
                    qDebug() << "Websockets::clearPlayerType:" << playerTypeAsQStr(type);
                    m_clients.replace(nClientpos, changedClient);
                }
                else qDebug() << "ERROR: Websockets::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
        }
        qDebug() << "WARNING: Websockets::clearPlayerType- client.type not found";
    }
    else qDebug() << "WARNING: Websockets::clearPlayerType- already clear";
}

void Websockets::setClientState(QWebSocket *clientSocket, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Websockets::setClientState(QWebSocket *clientSocket, "
                            "bool state)- client has already clicked start";

            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Websockets::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientState- client not found";
}

void Websockets::setClientState(PLAYERS_TYPES type, bool state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            if(state && client.isStartClickedByPlayer == state)
                qDebug() << "WARNING: Websockets::setClientState(PLAYERS_TYPES type, "
                            "bool state)- client has already clicked start";

            Clients changedClient = client;
            changedClient.isStartClickedByPlayer = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientState: new state =" <<
                            m_clients.at(nClientpos).isStartClickedByPlayer;
            }
            else qDebug() << "ERROR: Websockets::setClientState: iteration error. iter val =" <<
                             nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientState- client not found";
}

void Websockets::addClientToQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue > 0)
            {
                qDebug() << "ERROR: Websockets::addClientToQueue- "
                            "client already in queue:" << client.queue;
                return;
            }
            if (client.type != PT_NONE)
            {
                qDebug() << "ERROR: Websockets::addClientToQueue- client is" <<
                            playerTypeAsQStr(client.type);
                return;
            }
        }
    }

    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue > maxQueue)
            maxQueue = client.queue;
    }
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.queue = (maxQueue == 0) ? 1 : maxQueue + 1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::addClientToQueue: new queue =" << m_clients.at(nClientpos).queue;
            }
            else qDebug() << "ERROR: Websockets::addClientToQueue: iteration error. iter val =" << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

void Websockets::removeClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if(!m_clients.removeOne(client))
                qDebug() << "ERROR: Websockets::removeClient- client not found";;
            return;
        }
    }
}

void Websockets::removeClientFromQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            Clients changedClient = client;
            changedClient.queue = -1;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
                m_clients.replace(nClientpos, changedClient);
            else qDebug() << "ERROR: Websockets::removeClientFromQueue: iteration error. iter val ="
                          << nClientpos;

            emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
            return;
        }
    }
    emit setBoardDataLabels(QString::number(getAmountOfQueuedClients()), BDL_QUEUE_PLAYERS);
}

bool Websockets::isClientInList(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return true;
    }
    qDebug() << "ERROR: Websockets::isClientInList(): client not found";
    return false;
}

Clients Websockets::getClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client;
    }
    qDebug() << "ERROR: Websockets::getClient(): client not found";
    Clients errorClient;
    return errorClient;
}

QWebSocket *Websockets::getClientSocket(QString playerName)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == playerName)
            return client.socket;
    }
    return nullptr;
}

QString Websockets::getClientName(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.name;
    }
    qDebug() << "ERROR: Websockets::getClientName(): client not found";
    return "";
}

QWebSocket *Websockets::getNextQueuedClientSocket()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue >= 0)
            minQueue = client.queue;
    }
    qDebug() << "Websockets::getNextQueuedClientSocket(): minQueue =" << minQueue;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue == minQueue)
        {
            qDebug() << "Websockets::getNextQueuedClientSocket(): client.name =" <<
                        client.name;
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getNextQueuedClientSocket(): client not found";
    return nullptr;
}

QString Websockets::getQueuedClientsList()
{
    QString QStrQueuedClients;
    QStrQueuedClients.clear();
    int64_t maxQueue = 0;
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    int64_t lastBigggestQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue > 0)
            minQueue = client.queue;
    }
    int nNumberOFClients = m_clients.size();
    int nLoopBreakingCounter = 0;
    if (minQueue != 0 && minQueue != std::numeric_limits<int64_t>::max())
    {
        do
        {
            Q_FOREACH (Clients client, m_clients)
            {
                if (client.queue > maxQueue && client.queue < lastBigggestQueue)
                    maxQueue = client.queue;
            }

            lastBigggestQueue = maxQueue;

            Q_FOREACH (Clients client, m_clients)
            {
                if (client.queue == maxQueue)
                {
                    QStrQueuedClients = client.name + "," + QStrQueuedClients;
                    maxQueue = 0;
                }
            }
            if (nLoopBreakingCounter > nNumberOFClients + 1)
            {
                qDebug() << "ERROR: Websockets::getQueuedClientsList: incorrect loop break. nLoopBreakingCounter ="
                         << nLoopBreakingCounter << ", nNumberOFClients =" << nNumberOFClients+1;
                break;
            }
            nLoopBreakingCounter++;
        } while (lastBigggestQueue != minQueue);
    }
    if (QStrQueuedClients.isEmpty()) QStrQueuedClients = "queueEmpty";
    else QStrQueuedClients = QStrQueuedClients.left(QStrQueuedClients.length() - 1); //usuń ostatni przecinek

    return QStrQueuedClients;
}

/*void Websockets::testQueuedClients()
{
    //for (int i=1; i<10; i++)
    for (int i=10; i>1; i--)
    {
        Clients testClient;
        testClient.name = "test" + QString::number(i);
        testClient.queue = i;
        qDebug() << "client" << testClient.name << "queue =" << testClient.queue;
        testClient.socket = nullptr;
        testClient.isStartClickedByPlayer = false;
        testClient.type = PT_NONE;

        m_clients << testClient;
    }

    qDebug() << "Websockets::testQueuedClients(): m_clients.size =" << m_clients.size();
    QString QStrQueuedClients = this->getQueuedClientsList();
    qDebug() << "Websockets::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

PLAYERS_TYPES Websockets::getClientType(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.type;
    }
    qDebug() << "ERROR: Websockets::getClientType(): socket not found";
    return PT_NONE;
}

bool Websockets::isPlayerChairEmpty(PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
            return false;
    }
    return true;
}

bool Websockets::isGameTableOccupied()
{
    if (this->isPlayerChairEmpty(PT_WHITE) || this->isPlayerChairEmpty(PT_BLACK))
        return false;
    else return true;
}

int64_t Websockets::getQueuedClient(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return client.queue;
    }
    qDebug() << "ERROR: Websockets::getQueuedClient(): client not found";
    return -1;
}

int64_t Websockets::getQueuedClient(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
            return client.queue;
    }
    qDebug() << "ERROR: Websockets::getQueuedClient(): client not found";
    return -1;
}

QWebSocket *Websockets::getPlayerSocket(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.socket;
        }
        return nullptr;
    }
    else
    {
        qDebug() << "ERROR: Websockets::getPlayerSocket- wrong parameter:"
                  << type;
        return nullptr;
    }
}

bool Websockets::isStartClickedByPlayer(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.isStartClickedByPlayer;
        }
        return false;
    }
    else
    {
        qDebug() << "ERROR: Websockets::isStartClickedByPlayer- wrong parameter:"
                  << type;
        return false;
    }
}

QString Websockets::getPlayerName(PLAYERS_TYPES type)
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE))
        return "White";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK))
        return "Black";
    else
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
                return client.name;
        }
        qDebug() << "ERROR: Websockets::getPlayerName- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Websockets::isClientInQueue(QWebSocket *clientSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == clientSocket)
        {
            if (client.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Websockets::isClientInQueue(): client not found";
    return false;
}

bool Websockets::isClientNameExists(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
            return true;
    }
    return false;
}

int Websockets::getAmountOfQueuedClients()
{
    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (maxQueue < client.queue && client.queue > 0)
            maxQueue = client.queue;
    }

    if (maxQueue > 0)
    {
        int64_t minQueue = std::numeric_limits<int64_t>::max();
        Q_FOREACH (Clients client, m_clients)
        {
            if (minQueue > client.queue && client.queue > 0)
                minQueue = client.queue;
        }
        int nAmountOfQueuedClients = 1 + maxQueue - minQueue;

        return nAmountOfQueuedClients;
    }
    else return 0;
}

bool Websockets::isClientAPlayer(QWebSocket *clientSocket)
{
    if (this->getClientType(clientSocket) == PT_WHITE ||
            this->getClientType(clientSocket) == PT_BLACK )
        return true;
    else return false;

}
