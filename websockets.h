#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "webtable.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)


class Websockets: public QObject
{
    Q_OBJECT

public:
    Websockets(WebTable *pWebTable, quint16 port, QObject *parent = Q_NULLPTR);

    ~Websockets();

public Q_SLOTS:
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
    void onNewConnection();
    void socketDisconnected();
};

#endif // Websockets_H
