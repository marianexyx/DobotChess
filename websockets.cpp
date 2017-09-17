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
        emit addTextToConsole("WebSocket server listening on port" + port, WEBSOCKET);
    }

    _pChessboard = pChessboard;
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();
    //qDeleteAll(m_clients.begin(), m_clients.end()); //todo: nie wiem jak to usunąć. póki mam
    //jednen obiekt/ramię to nie jest mi to potrzebne
}

void Websockets::onNewConnection() //nowe połączenia
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    this->newClientSocket(pSocket);
    emit addTextToConsole("New connection \n", WEBSOCKET);

    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
}

//TODO: Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
//póki nie zmieniam wersji to może tak zostać
//TODO2: komendy serwisowe do wejdą tutaj- jak to się ma do nich?

void Websockets::resetPlayersStartConfirmInfo()
{
    qDebug() << "Websockets::resetPlayersStartConfirmInfo()";
    if (this->getClientStateByType(PT_WHITE) != CS_CLICKED_START)
           this->setClientStateByType(PT_WHITE, CS_NONE);
    if (this->getClientStateByType(PT_BLACK) != CS_CLICKED_START)
        this->setClientStateByType(PT_BLACK, CS_NONE);
}

QString Websockets::getTableData() //as JSON
{
    //TABLE_DATA{"wplr":"WHITE","bplr":"BLACK","turn":"wt","wtime":345581,"btime":300000,"queue":"empty"}
    QString QStrTableData = "TABLE_DATA{\"wplr\":\"" + this->getPlayerNameByType(PT_WHITE) +
            "\",\"bplr\":\"" + this->getPlayerNameByType(PT_BLACK) +
            "\",\"turn\":\"" + _pChessboard->getStrWhoseTurn() +
            "\",\"wtime\":" + QString::number(_pChessboard->getWhiteTimeLeft())  +
            ",\"btime\":" + QString::number(_pChessboard->getBlackTimeLeft()) +
            ",\"queue\":\"" + this->getQueuedClients();

    if (_pChessboard->isStartTimerRunning())
    {
        QString QStrWhiteClickedStart = (this->getClientStateByType(PT_WHITE) == CS_CLICKED_START) ? "w" : "x";
        QString QStrBlackClickedStart = (this->getClientStateByType(PT_BLACK) == CS_CLICKED_START) ? "b" : "x";
        QStrTableData += "\",\"start\":\"" + QStrWhiteClickedStart + QStrBlackClickedStart +
                QString::number(_pChessboard->getStartTimeLeft());
    }

    QStrTableData += "\"}";

    qDebug() << "Websockets::getTableData(): QStrTableData =" << QStrTableData;
    return QStrTableData;
}

void Websockets::receivedMsg(QString QStrWsMsg)
{
    if (QStrWsMsg != "keepConnected")
    {
        qDebug() << "Websockets::receivedMsg (from site):" << QStrWsMsg;
        emit addTextToConsole("received: " + QStrWsMsg + "\n", WEBSOCKET);
    }

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (QStrWsMsg == "keepConnected")
    {  //todo: używam tego kodu już 2gi raz takiego samego- zapakować w funkcję, ...
        //...poza tym ten kod tu istieje by było cokolwiek w tej opcji. może return dać jakoś?
        std::string s = std::to_string(m_clients.size());
        char const *pchar = s.c_str();
        emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
    }
    else if (QStrWsMsg == "newGame")
    {
        QString QStrWhoSent = "";
        if (pClient == this->getPlayerSocket(PT_WHITE))  QStrWhoSent = "WHITE";
        else if (pClient == this->getPlayerSocket(PT_BLACK)) QStrWhoSent = "BLACK";

        this->sendToChess(QStrWsMsg + " " + QStrWhoSent);
    }
    else if (QStrWsMsg.left(4) == "move") { this->sendToChess(QStrWsMsg); }
    //todo: kto decyduje o tym komunikacie (reset)? oby nei gracze
    else if (QStrWsMsg == "reset") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "getTableData") pClient->sendTextMessage(this->getTableData());
    else if (QStrWsMsg == "giveUp")
    {
        if (pClient) //todo: zrozumieć to i dać w razie czego więcej tych warunków tam gdzie są...
            //...QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        {
            //todo: praktycznie jest to powtórzenie tego co jest w socketDisconnected
            if (pClient == this->getPlayerSocket(PT_WHITE))
            {
                qDebug() << "Websockets::receivedMsg(): giveUp white player";
                this->addTextToConsole("Player has logouted. New white: WHITE\n", WEBSOCKET);
                this->endOfGame(ET_GIVE_UP, pClient);
                emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

                Q_FOREACH (Clients client, m_clients)
                    client.socket->sendTextMessage("givedUpWhite " + this->getTableData());
                //todo: tu się wyślą wszystkie ifnromacje, ale nie opcja startu gry- czy to dobrze?

                emit MsgFromWebsocketsToChess("reset");
            }
            else if (pClient == this->getPlayerSocket(PT_BLACK))
            {
                qDebug() << "Websockets::receivedMsg(): logout black player";
                this->addTextToConsole("Player has logouted. New black: BLACK\n", WEBSOCKET);
                this->endOfGame(ET_GIVE_UP, pClient);
                emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

                Q_FOREACH (Clients client, m_clients)
                    client.socket->sendTextMessage("givedUpBlack " + this->getTableData());

                emit MsgFromWebsocketsToChess("reset");
            }
            else qDebug() << "ERROR: Websockets::receivedMsg(): non-player tried to logout (hacker?)";
        }
        else qDebug() << "ERROR: Websockets::receivedMsg(): giveUp: !isset pClient";
    }
    else if (QStrWsMsg.left(5) == "sitOn")
    {
        if (this->getClientNameBySocket(pClient) == "-1")
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): sitOn: client not loggedIn";
            return;
        }

        QString QStrPlayerType = QStrWsMsg.mid(5);
        PLAYERS_TYPES playerChair;
        if (QStrPlayerType == "White") playerChair = PT_WHITE;
        else if (QStrPlayerType == "Black") playerChair = PT_BLACK;
        else
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): sitOn: wrong sufix:" << QStrPlayerType;
            return;
        }

        _pChessboard->setWhoseTurn(NO_TURN);
        if (!this->isPlayerChairEmpty(playerChair))
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): client wanted to sit on occupied chair. client name:"
                     << this->getClientNameBySocket(pClient) << ", player name:" <<
                        this->getPlayerNameByType(playerChair);
            return;
        }
        else
            this->setPlayerType(pClient, playerChair);

        //todo: funkcje informacyjne powinny wykonywać się wewnątrz funkcji zmieniającej nazwy
        qDebug() << "Websockets::receivedMsg(): new" << QStrPlayerType << "player:" <<
                    this->getPlayerNameByType(playerChair);
        emit this->addTextToConsole("New white player: " + this->getPlayerNameByType(playerChair) +
                                    "\n", WEBSOCKET);
        emit setBoardDataLabels(this->getPlayerNameByType(playerChair),
                                playerChair == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();

        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg == "standUp")
    {
        if (pClient == this->getPlayerSocket(PT_WHITE) || pClient == this->getPlayerSocket(PT_BLACK))
        {
            PLAYERS_TYPES chairToClear = (pClient == this->getPlayerSocket(PT_WHITE)) ? PT_WHITE : PT_BLACK;

            //funckje są w tym endtype nieczytelne
            this->endOfGame(ET_STAND_UP, pClient);

            QString QStrPlayer = (chairToClear == PT_WHITE) ? "white" : "black";
            qDebug() << "Websockets::receivedMsg(): new" << QStrPlayer << "player:" <<
                        this->getPlayerNameByType(chairToClear);
            emit this->addTextToConsole("New " + QStrPlayer + " player: " + this->getPlayerNameByType(chairToClear) +
                                        "\n", WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerNameByType(chairToClear),
                                    chairToClear == PT_WHITE ? BDL_WHITE_NAME : BDL_BLACK_NAME);

            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(this->getTableData());

            emit MsgFromWebsocketsToChess("reset");
        }
        else
        {
            qDebug() << "ERROR: Websockets::receivedMsg(): non-player client wanted to leave chair. client name:"
                     << this->getClientNameBySocket(pClient);
            return;
        }
    }
    else if (QStrWsMsg.left(3) == "im ")
    {
        //todo: jeżeli pojawi się nick, który już jest w liście, to tego co jest wyrzuć i wszystko zresetuj
        this->setClientName(pClient, QStrWsMsg.mid(3));
        pClient->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg.left(9) == "promoteTo") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "queueMe")
    {
        this->addClientToQueue(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg == "leaveQueue")
    {
        this->removeClientFromQueueBySocket(pClient);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else  qDebug() << "ERROR: Websockets::receivedMsg(): unknown parameter:" << QStrWsMsg;
}

void Websockets::sendMsg(QString QStrWsMsg)
{
    qDebug() << "Websockets::sendMsg() received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", WEBSOCKET);

    if (QStrWsMsg == "newOk")
    {
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage("newOk");
    }
    else if (QStrWsMsg.left(6) == "moveOk" || QStrWsMsg.left(7) == "badMove")
    {
        if (QStrWsMsg.left(7) == "badMove" || QStrWsMsg.right(7) == "promote")
        {
            if (_pChessboard->getWhoseTurn() == WHITE_TURN && this->getPlayerSocket(PT_WHITE) != nullptr)
            {
                qDebug() << "Sending to site" << QStrWsMsg;
                this->getPlayerSocket(PT_WHITE)->sendTextMessage(QStrWsMsg);
            }
            else if (_pChessboard->getWhoseTurn() == BLACK_TURN && this->getPlayerSocket(PT_BLACK) != nullptr)
            {
                qDebug() << "Sending to site" << QStrWsMsg;
                this->getPlayerSocket(PT_BLACK)->sendTextMessage(QStrWsMsg);
            }
            else
                qDebug() << "ERROR: Websockets::sendMsg: uknown/wrong turn:" << _pChessboard->getWhoseTurn();
        }
        else
        {
            Q_FOREACH (Clients client, m_clients)
                client.socket->sendTextMessage(QStrWsMsg);
        }
    }
    else if (QStrWsMsg.left(8) == "promoted" || QStrWsMsg == "reseting" || QStrWsMsg == "resetComplited")
    {
        if (QStrWsMsg == "resetComplited")
        {
            //todo: muszę tu chyba znowu pamiętać o tym, żeby zmieniać nazwy graczom w formie. ...
            //...niech to się dzieje jakoś automatycznie

            if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
                _pChessboard->startQueueTimer(); //todo: to też
            QStrWsMsg = QStrWsMsg + " " + this->getTableData();
        }
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg == "timeOutWhite" || QStrWsMsg == "timeOutBlack")
    {
        //todo: ogarnąć w końcu funkcję czyszczenia danych przy końcach...
        //...gry- w sumie Websockets::endOfGame daje chyba radę- ogarnąć na spokojnie
        this->endOfGame(QStrWsMsg.right(5) == "White" ? ET_WHIE_WON : ET_BLACK_WON);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else if (QStrWsMsg == "timeOutStartQueue" )
    {
        this->endOfGame(ET_TIMEOUT_START);
        Q_FOREACH (Clients client, m_clients)
            client.socket->sendTextMessage(this->getTableData());
    }
    else qDebug() << "ERRROR: uknown Websockets::sendMsg() parameter:" << QStrWsMsg;
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    emit this->addTextToConsole("Sending to 'chess' class: " + QsMsgForChessClass + "\n", WEBSOCKET);
    emit this->MsgFromWebsocketsToChess(QsMsgForChessClass);
}

void Websockets::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) //todo: co to??? czyżby to że sprawdzam czy udało mi się zrzutować obiekt...
        //... z powyższej linjki?
    {
        if (pClient == this->getPlayerSocket(PT_WHITE))
        {
            qDebug() << "Websockets::socketDisconnected(): white player disconnected";
            this->endOfGame(ET_SOCKET_LOST, pClient);
            qDebug() << "Websockets::socketDisconnected(): setBoardDataLabels new white name ="
                     << this->getPlayerNameByType(PT_BLACK);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);

            Q_FOREACH (Clients client, m_clients)
            {
                qDebug() << "Websockets::socketDisconnected(): send new tableData to each client";
                client.socket->sendTextMessage(this->getTableData());
            }

            qDebug() << "Websockets::socketDisconnected(): newWhite: WHITE";
            this->addTextToConsole("New white player: WHITE\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        else if (pClient == this->getPlayerSocket(PT_BLACK))
        {
            qDebug() << "Websockets::socketDisconnected(): black player disconnected";
            this->endOfGame(ET_SOCKET_LOST, pClient);
            qDebug() << "Websockets::socketDisconnected(): setBoardDataLabels new black name ="
                     << this->getPlayerNameByType(PT_BLACK);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);

            Q_FOREACH (Clients client, m_clients)
            {
                qDebug() << "Websockets::socketDisconnected(): send new tableData to each client";
                client.socket->sendTextMessage(this->getTableData());
            }

            qDebug() << "Websockets::socketDisconnected(): newBlack: BLACK";
            this->addTextToConsole("New black player: BLACK\n", WEBSOCKET);

            emit MsgFromWebsocketsToChess("reset");
        }
        //todo: wyświetla mi się disconnect w dziwnym miejscu
        else
        {
            qDebug() << "non-player disconnected";
            emit addTextToConsole("non-player disconnected\n", WEBSOCKET);
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
    std::string s = std::to_string(m_clients.size());
    char const *pchar = s.c_str();
    emit setBoardDataLabels(pchar, BDL_SOCKETS_ONLINE);
}

void Websockets::msgFromChessboardToWebsockets(QString QStrWsMsg) //todo: niepotrzebne to przejście
{
    this->sendMsg(QStrWsMsg);
}

void Websockets::moveNextClientFromQueueToTableIfExists(PLAYERS_TYPES chair)
{
    qDebug() << "inside Websockets::moveNextClientFromQueueToTableIfExists()";
    if (this->getQueuedClients() != "queueEmpty")
    {
        qDebug() << "Websockets::moveNextClientFromQueueToTableIfExists(): "
                    "next queued client exists";

        Clients nextQueuedClient = this->getClientBySocket(this->getNextQueuedClientsSocket());
        qDebug() << "Websockets::moveNextClientFromQueueToTableIfExists(): move client:" <<
                     nextQueuedClient.name << "from queue to table";

        this->removeClientFromQueueBySocket(nextQueuedClient.socket);

        this->setPlayerType(nextQueuedClient.socket, chair);
        QString QStrType = (nextQueuedClient.type == PT_WHITE) ? "white" : "black";
        qDebug() << "Websockets::moveNextClientFromQueueToTableIfExists(): "
                    "client from queue new type:" << QStrType;

        this->setClientStateBySocket(nextQueuedClient.socket, CS_NONE);
    }
}

//todo: funkcja w sumie nic nie mówi o tym co dokładnie robi
void Websockets::endOfGame(END_TYPE EndType, QWebSocket *clientToClear)
{
    //todo: zastanowić się na spokojnie jakie czyszczenia jeszcze tu upchać
    this->resetPlayersStartConfirmInfo();
    _pChessboard->stopQueueTimer();
    _pChessboard->stopBoardTimers();
    this->clearBothPlayersStates();
    _pChessboard->setWhoseTurn(NO_TURN);

    switch(EndType)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        qDebug() << "Websockets::endOfGame(): ET_DRAW: clearing both players type";
        this->clearPlayerType(PT_WHITE); //todo: to się pewnie musi dziać gdzieś jeszcze. ...
        this->clearPlayerType(PT_BLACK); //...sprawdzić czy nie dubluję tego
        this->moveNextClientFromQueueToTableIfExists(PT_WHITE);
        this->moveNextClientFromQueueToTableIfExists(PT_BLACK);
        break;

    case ET_STAND_UP:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (this->getClientTypeBySocket(clientToClear) == PT_WHITE)
        {
            this->clearPlayerType(PT_WHITE);
            this->moveNextClientFromQueueToTableIfExists(PT_WHITE);
        }
        else if (this->getClientTypeBySocket(clientToClear) == PT_BLACK)
        {
            this->clearPlayerType(PT_BLACK);
            this->moveNextClientFromQueueToTableIfExists(PT_BLACK);
        }
        else qDebug() << "ERROR: Websockets::endOfGame: wrong player type:" << EndType;
        break;

    case ET_TIMEOUT_START:
        if (this->getClientStateByType(PT_WHITE) != CS_CLICKED_START)
        {
            this->clearPlayerType(PT_WHITE);
            this->moveNextClientFromQueueToTableIfExists(PT_WHITE);
            //todo: funkcje informacyjne powinny wykonywać się wewnątrz funkcji zmieniającej nazwy
            qDebug() << "Websockets::receivedMsg(): new white player:" <<
                        this->getPlayerNameByType(PT_WHITE);
            emit this->addTextToConsole("New white player: " + this->getPlayerNameByType(PT_WHITE) +
                                        "\n", WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_WHITE), BDL_WHITE_NAME);
        }
        if (this->getClientStateByType(PT_BLACK) != CS_CLICKED_START)
        {
            this->clearPlayerType(PT_BLACK);
            this->moveNextClientFromQueueToTableIfExists(PT_BLACK);
            //todo: funkcje informacyjne powinny wykonywać się wewnątrz funkcji zmieniającej nazwy
            qDebug() << "Websockets::receivedMsg(): new black player:" <<
                        this->getPlayerNameByType(PT_BLACK);
            emit this->addTextToConsole("New white player: " + this->getPlayerNameByType(PT_BLACK) +
                                        "\n", WEBSOCKET);
            emit setBoardDataLabels(this->getPlayerNameByType(PT_BLACK), BDL_BLACK_NAME);
        }

        if (!this->isPlayerChairEmpty(PT_WHITE) && !this->isPlayerChairEmpty(PT_BLACK))
            _pChessboard->startQueueTimer();
        break;

    default:
        qDebug() << "ERROR: Websockets::endOfGame: unknown EndType val=" << EndType;
        break;

        //todo: sprawdzić czy zresetowałem inne dane: zegary, tury, planszę fizyczną/ w pamięci itd
    }
}

void Websockets::newClientSocket(QWebSocket *playerSocket)
{
    Clients newClient;
    newClient.socket = playerSocket;
    newClient.name.clear();
    newClient.queue = -1;
    newClient.state = CS_NONE;
    newClient.type = PT_NONE;

    //todo: sprawdzać czy któreś dane się w liście nie powtarzają

    m_clients << newClient;
}

void Websockets::setClientName(QWebSocket *playerSocket, QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
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
                "typeid(playerSocket) =" << typeid(playerSocket).name();
}

void Websockets::setPlayerType(QWebSocket *playerSocket, PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.type = type;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setPlayerType: new player type =" <<
                            playerTypeAsQStr(m_clients.at(nClientpos).type);
            }
            else qDebug() << "ERROR: Websockets::setPlayerType: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    Clients client;
    qDebug() << "ERROR: Websockets::setPlayerType: socket not found. m_clients.size() =" <<
                m_clients.size() << ", typeid(client.socket) =" << typeid(client.socket).name() <<
                "typeid(playerSocket) =" << typeid(playerSocket).name();

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
                changedClient.state = CS_NONE;

                int nClientpos = m_clients.indexOf(client);
                if (nClientpos >= 0 && nClientpos < m_clients.size())
                {
                    m_clients.replace(nClientpos, changedClient);
                    qDebug() << "Websockets::clearPlayerType";
                }
                else qDebug() << "ERROR: Websockets::clearPlayerType: iteration error. iter val =" << nClientpos;

                return;
            }
            else qDebug() << "WARNING: Websockets::clearPlayerType- client.type not found";
        }
    }
    else qDebug() << "WARNING: Websockets::clearPlayerType- already clear";
}

void Websockets::setClientStateBySocket(QWebSocket *playerSocket, CLIENT_STATE state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.state = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientStateBySocket: new state =" << m_clients.at(nClientpos).state;
            }
            else qDebug() << "ERROR: Websockets::setClientStateBySocket: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientStateBySocket- client not found";
}

void Websockets::setClientStateByType(PLAYERS_TYPES type, CLIENT_STATE state)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            Clients changedClient = client;
            changedClient.state = state;

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
                qDebug() << "Websockets::setClientStateByType: new state =" << m_clients.at(nClientpos).state;
            }
            else qDebug() << "ERROR: Websockets::setClientStateByType: iteration error. iter val =" << nClientpos;

            return;
        }
    }
    qDebug() << "ERROR: Websockets::setClientStateByType- client not found";
}

void Websockets::addClientToQueue(QWebSocket *playerSocket)
{
    int64_t maxQueue = 0;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue > maxQueue)
            maxQueue = client.queue;
    }
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
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

            return;
        }
    }
}

void Websockets::removeClient(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            if(!m_clients.removeOne(client))
                qDebug() << "ERROR: Websockets::removeClient- client not found";;
            return;
        }
    }
}

void Websockets::removeClientFromQueueBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            Clients changedClient = client;
            changedClient.queue = -1; //todo: poza zakresem unsigned

            int nClientpos = m_clients.indexOf(client);
            if (nClientpos >= 0 && nClientpos < m_clients.size())
            {
                m_clients.replace(nClientpos, changedClient);
            }
            else qDebug() << "ERROR: Websockets::removeClientFromQueueBySocket: iteration error. iter val ="
                          << nClientpos;

            return;
        }
    }
}

void Websockets::clearBothPlayersStates()
{
    this->setClientStateByType(PT_WHITE, CS_NONE);
    this->setClientStateByType(PT_BLACK, CS_NONE);
}

Clients Websockets::getClientBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client;
        }
    }
    qDebug() << "ERROR: Websockets::getClientBySocket(): client not found";
    Clients errorClient;
    return errorClient; //todo: oszukiwanie kompilatora- powinny tu być wyjątki
}

QWebSocket *Websockets::getClientSocketByName(QString playerName)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == playerName)
        {
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getClientSocketByName(): client not found";
    return nullptr;
}

QString Websockets::getClientNameBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.name;
        }
    }
    qDebug() << "ERROR: Websockets::getClientNameBySocket(): client not found";
    return "";
}

QWebSocket *Websockets::getNextQueuedClientsSocket()
{
    int64_t minQueue = std::numeric_limits<int64_t>::max();
    Q_FOREACH (Clients client, m_clients)
    {
        if (minQueue > client.queue && client.queue >= 0)
            minQueue = client.queue;
    }
    qDebug() << "Websockets::getNextQueuedClientsSocket(): minQueue =" << minQueue;
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.queue == minQueue)
        {
            qDebug() << "Websockets::getNextQueuedClientsSocket(): client.name =" <<
                        client.name;
            return client.socket;
        }
    }
    qDebug() << "ERROR: Websockets::getNextQueuedClientsSocket(): client not found";
    return nullptr;
}

QString Websockets::getQueuedClients()
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
    //qDebug() << "minQueue =" << minQueue;
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
            //qDebug() << "new lastBigggestQueue =" << lastBigggestQueue;

            Q_FOREACH (Clients client, m_clients)
            {
                if (client.queue == maxQueue)
                {
                    QStrQueuedClients = client.name + "," + QStrQueuedClients;
                    /*qDebug() << "found last queued client:" << client.queue
                         << ", new queue list =" << QStrQueuedClients;*/
                    maxQueue = 0;
                }
            }
            if (nLoopBreakingCounter > nNumberOFClients + 1)
            {
                qDebug() << "ERROR: Websockets::getQueuedClients: incorrect loop break. nLoopBreakingCounter ="
                         << nLoopBreakingCounter << ", nNumberOFClients =" << nNumberOFClients+1;
                break;
            }
            nLoopBreakingCounter++;
            //qDebug() << "lastBigggestQueue =" << lastBigggestQueue << ", minQueue =" << minQueue;
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
        testClient.state = CS_NONE;
        testClient.type = PT_NONE;

        m_clients << testClient;
    }

    qDebug() << "Websockets::testQueuedClients(): m_clients.size =" << m_clients.size();
    QString QStrQueuedClients = this->getQueuedClients();
    qDebug() << "Websockets::testQueuedClients(): QStrQueuedClients =" << QStrQueuedClients;
}*/

PLAYERS_TYPES Websockets::getClientTypeBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.type;
        }
    }
    qDebug() << "ERROR: Websockets::getClientTypeBySocket(): socket not found";
    return PT_NONE;
}

bool Websockets::isPlayerChairEmpty(PLAYERS_TYPES type)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.type == type)
        {
            return false;
        }
    }
    return true;
}

int64_t Websockets::getQueuedClientByName(QString name)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.name == name)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientByName(): client not found";
    return -1;
}

int64_t Websockets::getQueuedClientBySocket(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            return client.queue;
        }
    }
    qDebug() << "ERROR: Websockets::getQueuedClientBySocket(): client not found";
    return -1;
}

QWebSocket *Websockets::getPlayerSocket(PLAYERS_TYPES type)
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.socket;
            }
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

CLIENT_STATE Websockets::getClientStateByType(PLAYERS_TYPES type) //todo: zamienić na bool
{
    if (type != PT_NONE)
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.state;
            }
        }
        return CS_NONE;
    }
    else
    {
        qDebug() << "ERROR: Websockets::getClientStateByType- wrong parameter:"
                  << type;
        return CS_NONE;
    }
}

QString Websockets::getPlayerNameByType(PLAYERS_TYPES type) //todo: źle ta funkcja jest zrobiona?
{
    if (type == PT_WHITE && isPlayerChairEmpty(PT_WHITE)) return "WHITE";
    else if (type == PT_BLACK && isPlayerChairEmpty(PT_BLACK)) return "BLACK";
    else
    {
        Q_FOREACH (Clients client, m_clients)
        {
            if (client.type == type)
            {
                return client.name;
            }
        }
        qDebug() << "ERROR: Websockets::getPlayerNameByType- player not found:"
                  << playerTypeAsQStr(type);
        return "";
    }
}

bool Websockets::isClientInQueue(QWebSocket *playerSocket)
{
    Q_FOREACH (Clients client, m_clients)
    {
        if (client.socket == playerSocket)
        {
            if (client.queue >= 0)
                return true;
            else return false;
        }
    }
    qDebug() << "ERROR: Websockets::isClientInQueue(): client not found";
    return false;
}
