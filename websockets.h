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

private slots:
    void socketDisconnected();

public:
    Websockets(QObject* parent = Q_NULLPTR);
    ~Websockets();

    void listenOnPort(quint16 port);
    static QString toJSON(QString QStrData);
    Clients* getClientsListPointer() const { return m_pClientsList; }

public slots:
    void onNewConnection();
    void receivedMsg(QString QStrMsg);
    void sendMsgToClient(QString QStrMsg, uint64_t un64ReceiverID);
    void sendMsgToAllClients(QString QStrMsg);

signals:
    void addTextToLogPTE(QString, LOG);
    void msgFromWebsocketsToChess(QString QStrMsg, uint64_t un64SenderID);
};

#endif // Websockets_H
