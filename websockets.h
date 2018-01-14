#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#pragma once

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "client.h"
#include "vars/log.h"
#include "vars/board_data_labels.h"
#include "vars/players_types.h"
#include "vars/end_of_game_types.h"
#include "typeinfo"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Websockets: public QObject
{
    Q_OBJECT

private:
    QWebSocketServer *_pWebSocketServer;
    Clients* _pClients;

private Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void socketDisconnected();

public:
    Websockets(Clients* pClients, quint16 port, QObject* parent = Q_NULLPTR);
    ~Websockets();

public Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void receivedMsg(QString QStrMsg);
    void onNewConnection();

public slots:
    void sendMsgToClient(QString QStrMsg, Client* pClient);
    void sendMsgToAllClients(QString QStrMsg);

signals:
    void addTextToConsole(QString, LOG);
    void msgFromWebsocketsToChess(QString QStrMsg, Client* pClient);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
};

#endif // Websockets_H
