#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "webtable.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


class Websockets: public QObject
{
    Q_OBJECT

public:
    Websockets(WebTable *pWebTable, quint16 port, QObject *parent = Q_NULLPTR);

    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients; //kontener z aktywnymi połączeniami websocketowymi

    void sendToChess(QString QsMsgForChessClass);

    ~Websockets();

public Q_SLOTS:
    void processWebsocketMsg(QString QS_WbstMsgToProcess);
    void onNewConnection();

signals:
    void addTextToWebsocketConsole(QString QStrMsg);
    void MsgFromWebsocketsToChess(QString QStrMsgFromWebsockets);
    void MsgFromWebsocketsToWebtable(QString QStrMsgFromWebsockets);

private:
    WebTable *_pWebTable;

private Q_SLOTS: //czym to się różni od zwykłego private/zwykłego slots?
    //void onNewConnection();
    void socketDisconnected();
};

#endif // Websockets_H
