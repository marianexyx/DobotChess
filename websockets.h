#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "webtable.h"


class Websockets: public QObject
{
    Q_OBJECT

public:
    Websockets(quint16 port, WebTable *pWebTable);

    ~Websockets();

    void processWebsocketMsg(QString QS_WbstMsgToProcess);

signals:
    void addTextToWebsocketConsole(QString QStrMsg);
    void MsgFromWebsocketsToChess(QString QStrMsgFromWebsockets);
    void MsgFromWebsocketsToWebtable(QString QStrMsgFromWebsockets);

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;

    WebTable *_pWebTable;

private Q_SLOTS: //czym to się różni od zwykłego private? i zwykłego slots?
    //websockets
    void onNewConnection();
    //void processWebsocketMsg(QString QS_WbstMsgToProcess);
    void socketDisconnected();
};

#endif // Websockets_H
