#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#pragma once
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "typeinfo"
#include "client.h"
#include "vars/log.h"
#include "vars/players_types.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Websockets: public QObject
{
    Q_OBJECT

private:
    QWebSocketServer* m_pWebsocketserver;
    Clients* m_pClientsList;

private Q_SLOTS: //Q_SLOTS is for 3rd party mechanisms, that want use slots
    void socketDisconnected();

public:
    Websockets(QObject* parent = Q_NULLPTR);
    ~Websockets();

    void listenOnPort(quint16 port);
    Clients* getClientsListPointer() const { return m_pClientsList; }

public Q_SLOTS:
    void onNewConnection();
    void receivedMsg(QString QStrMsg);

public slots:
    void sendMsgToClient(QString QStrMsg, int64_t n64ReceiverID);
    void sendMsgToAllClients(QString QStrMsg);

signals:
    void addTextToLogPTE(QString, LOG);
    void msgFromWebsocketsToChess(QString QStrMsg, int64_t n64SenderID);
};

#endif // Websockets_H
