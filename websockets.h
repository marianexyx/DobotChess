#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#pragma once

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "chessboard.h" //todo: tego tu nie powinno być
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
    Websockets(Clients* pClients, quint16 port, QObject *parent = Q_NULLPTR);

    void sendToChess(QString QStrMsg, int64_t clientID);
    void endOfGame(END_TYPE EndType, QWebSocket *playerToClear = nullptr); //todo: przenieść
    void sendMsgToAllClients(QString msg);

    ~Websockets();

public Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void receivedMsg(QString QStrWbstMsgToProcess);
    void onNewConnection();

public slots:
    void sendMsg(QString QStrWsMsg);
    void sendMsg(int64_t ID, QString QStrWsMsg); //todo: można je scalić, albo odróżniać

signals:
    void addTextToConsole(QString, LOG);
    void msgFromWebsocketsToChess(QString QStrMsg, int64_t clientID);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);

};

#endif // Websockets_H
