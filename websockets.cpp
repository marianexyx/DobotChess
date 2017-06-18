#include "websockets.h"

Websockets::Websockets(Chessboard *pChessboard, WebTable *pWebTable, quint16 port, QObject *parent):
    QObject(parent),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this); //nowy serwer websocket
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) //niech ws serwer nasłuchuje na tym porcie
    {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Websockets::onNewConnection);

        qDebug() << "WebSocket server listening on port" << port;
        emit addTextToConsole("WebSocket server listening on port" + port, WEBSOCKET);
    }

    _pWebTable = pWebTable;
    _pChessboard = pChessboard;

    m_pWhitePiecesSocket = nullptr;
    m_pBlackPiecesSocket = nullptr;
}

Websockets::~Websockets()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Websockets::onNewConnection() //nowe połączenia
{
    qDebug() << "Websockets::onNewConnection()";
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Websockets::receivedMsg);
    connect(pSocket, &QWebSocket::disconnected, this, &Websockets::socketDisconnected);
    m_clients << pSocket;
    emit addTextToConsole("New connection \n", WEBSOCKET);
}

//TODO: Q_FOREACH (QWebSocket *pNextClient, m_clients) ma być depreciated
//na korzyść: "for (QWebSocket *pClient : qAsConst(m_clients))"
//TODO2: komendy serwisowe do wejdą tutaj- jak to się ma do nich?

void Websockets::receivedMsg(QString QStrWsMsg)
{
    qDebug() << "Websockets::receivedMsg (from site): " << QStrWsMsg;
    emit addTextToConsole("received: " + QStrWsMsg + "\n", WEBSOCKET);

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender()); //TODO: czy to jest potrzbne? czy to coś robi?

    if (QStrWsMsg == "keepConnected") { pClient->sendTextMessage("connectionOnline"); }
    else if (QStrWsMsg == "newGame") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg.left(4) == "move") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg == "reset") { this->sendToChess(QStrWsMsg); }
    else if (QStrWsMsg.left(5) == "check")
    {
        QStrWsMsg = QStrWsMsg.mid(6);
        if (QStrWsMsg == "whitePlayer")
        {
            qDebug() << "Websockets received: checkedWhite " << _pWebTable->getNameWhite();
            emit addTextToConsole("sent: checkedWhite " + _pWebTable->getNameWhite() + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedWhite " + _pWebTable->getNameWhite());
        }
        else if (QStrWsMsg == "blackPlayer")
        {
            qDebug() << "Websockets received: checkedBlack " << _pWebTable->getNameBlack();
            emit addTextToConsole("sent: checkedBlack " + _pWebTable->getNameBlack() + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedBlack " + _pWebTable->getNameBlack());
        }
        else if (QStrWsMsg == "whoseTurn")
        {
            qDebug() << "Websockets received: checkedTurn " << _pChessboard->getStrWhoseTurn();
            emit addTextToConsole("sent: checkedTurn " + _pChessboard->getStrWhoseTurn() + "\n", WEBSOCKET);
            pClient->sendTextMessage("checkedTurn " + _pChessboard->getWhoseTurn() );
        }
        else  qDebug() << "ERROR: Websockets::receivedMsg(): wrong check parameter: " << QStrWsMsg;
    }
    else if (QStrWsMsg.left(6) == "change")
    {
        QStrWsMsg = QStrWsMsg.mid(7);
        QString QStrPlayerType = QStrWsMsg.left(11); qDebug() << "QStrPlayerType =" << QStrPlayerType;
        QString QStrPlayerName = QStrWsMsg.mid(12); qDebug() << "QStrPlayerName =" << QStrPlayerName;

        if (QStrPlayerType == "whitePlayer")
        {
            emit MsgFromWebsocketsToWebtable("newWhite " + QStrPlayerName);

            if (QStrPlayerName == "White") m_pWhitePiecesSocket = nullptr; //todo: zastąpić white/black stałą string
            else m_pWhitePiecesSocket = pClient; //skojarzenie siadającego gracza białego z gniazdem bierek białych

            Q_FOREACH (QWebSocket *pNextClient, m_clients)
                pNextClient->sendTextMessage("newWhite " + _pWebTable->getNameWhite());

            qDebug() << "newWhite:" << _pWebTable->getNameWhite();
            this->addTextToConsole("New white player: " + _pWebTable->getNameWhite() + "\n", WEBSOCKET);
        }
        else if (QStrPlayerType.left(11) == "blackPlayer")
        {
            emit MsgFromWebsocketsToWebtable("newBlack " + QStrPlayerName);

            if (QStrPlayerName == "Black") m_pBlackPiecesSocket = nullptr;
            else m_pBlackPiecesSocket = pClient; //skojarzenie siadającego gracza czarnego z gniazdem bierek czarnych

            Q_FOREACH (QWebSocket *pNextClient, m_clients)
                pNextClient->sendTextMessage("newBlack " + _pWebTable->getNameBlack());

            qDebug() << "newBlack:" << _pWebTable->getNameWhite();
            this->addTextToConsole("New black player: " + _pWebTable->getNameBlack() + "\n", WEBSOCKET);
        }
        else  qDebug() << "ERROR: Websockets::receivedMsg(): wrong change parameter: " << QStrWsMsg;
    }
    else if (QStrWsMsg.left(9) == "promoteTo") { this->sendToChess(QStrWsMsg); }
    else  qDebug() << "ERROR: Websockets::receivedMsg(): unknown parameter:" << QStrWsMsg;
}

void Websockets::sendMsg(QString QStrWsMsg)
{
    qDebug() << "Websockets::sendMsg (to site) received:" << QStrWsMsg;
    emit addTextToConsole("sent: " + QStrWsMsg + "\n", WEBSOCKET);

    if (QStrWsMsg == "newOk")
    {
        Q_FOREACH (QWebSocket *pNextClient, m_clients)
            pNextClient->sendTextMessage("newOk");
    }
    else if (QStrWsMsg.left(6) == "moveOk" || QStrWsMsg.left(7) == "badMove")
    {
        if (QStrWsMsg.left(7) == "badMove" || QStrWsMsg.right(7) == "promote")
        {
            if (_pChessboard->getWhoseTurn() == WHITE_TURN) m_pWhitePiecesSocket->sendTextMessage(QStrWsMsg);
            else if (_pChessboard->getWhoseTurn() == BLACK_TURN) m_pBlackPiecesSocket->sendTextMessage(QStrWsMsg);
            else qDebug() << "ERROR: Websockets::sendMsg: uknown/wrong turn:" << _pChessboard->getWhoseTurn();
        }
        else
        {
            Q_FOREACH (QWebSocket *pNextClient, m_clients)
                pNextClient->sendTextMessage(QStrWsMsg);
        }
    }
    else if (QStrWsMsg == "ready")
    {
        m_pWhitePiecesSocket->sendTextMessage(QStrWsMsg);
        m_pBlackPiecesSocket->sendTextMessage(QStrWsMsg);
    }
    else if (QStrWsMsg.left(8) == "promoted")
    {
        Q_FOREACH (QWebSocket *pNextClient, m_clients)
            pNextClient->sendTextMessage(QStrWsMsg);
    }
    else qDebug() << "ERRROR: uknown Websockets::sendMsg() parameter";
}

void Websockets::sendToChess(QString QsMsgForChessClass)
{
    qDebug() << "Sending to 'chess' class: " << QsMsgForChessClass;
    emit this->addTextToConsole("Sending to 'chess' class: " + QsMsgForChessClass + "\n", WEBSOCKET);
    emit this->MsgFromWebsocketsToChess(QsMsgForChessClass);
}

void Websockets::socketDisconnected() //rozłączanie websocketa
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
        emit addTextToConsole("Disconnected\n", WEBSOCKET);
    }
}
