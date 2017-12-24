#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#pragma once

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "chessboard.h"
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
    QWebSocketServer *m_pWebSocketServer;

private Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void socketDisconnected();

public:
    Websockets(quint16 port, QObject *parent = Q_NULLPTR);

    void sendToChess(QString QsMsgForChessClass);

    void endOfGame(END_TYPE EndType, QWebSocket *playerToClear = nullptr);
    void playerIsLeavingGame(QWebSocket *pClient, END_TYPE leavingType);

    ~Websockets();

public Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void receivedMsg(QString QStrWbstMsgToProcess);
    void onNewConnection();

public slots:
    void sendMsg(QString QStrWsMsg);

signals:
    void addTextToConsole(QString, LOG);
    void MsgFromWebsocketsToChess(QString QStrMsgFromWebsockets);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);

};

#endif // Websockets_H
