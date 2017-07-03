#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "chessboard.h"
#include "webtable.h"
#include "vars/log.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


class Websockets: public QObject
{
    Q_OBJECT

private:
    WebTable *_pWebTable;
    Chessboard *_pChessboard;

    QWebSocket *m_pWhitePiecesSocket;
    QWebSocket *m_pBlackPiecesSocket;

private Q_SLOTS: //TODO: czym to się różni od zwykłego private/zwykłego slots?
    void socketDisconnected();

public:
    Websockets(Chessboard *pChessboard, WebTable *pWebTable, quint16 port, QObject *parent = Q_NULLPTR);

    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients; //kontener z aktywnymi połączeniami websocketowymi

    void sendToChess(QString QsMsgForChessClass);
    void sendMsg(QString QStrWsMsg);

    ~Websockets();

public Q_SLOTS:
    void receivedMsg(QString QS_WbstMsgToProcess);
    void onNewConnection();

signals:
    void addTextToConsole(QString, LOG);
    void MsgFromWebsocketsToChess(QString QStrMsgFromWebsockets);
};

#endif // Websockets_H
