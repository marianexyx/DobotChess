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

struct Clients
{
    QWebSocket *socket;
    QString name;
    PLAYERS_TYPES type;
    bool isStartClickedByPlayer;
    int64_t queue;

    //operator potrzebny do usuwania struktur z list
    bool operator ==(const struct Clients& st)
    {
        return socket == st.socket &&
                name == st.name &&
                type == st.type &&
                isStartClickedByPlayer == st.isStartClickedByPlayer &&
                queue == st.queue;
    }
};

class Websockets: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;

    QWebSocketServer *m_pWebSocketServer;
    QList<Clients> m_clients;

private Q_SLOTS: //TODO: czym to się różni od zwykłego private/zwykłego slots?
    void socketDisconnected();

public:
    Websockets(Chessboard *pChessboard, quint16 port, QObject *parent = Q_NULLPTR);

    void sendToChess(QString QsMsgForChessClass);
    void sendMsg(QString QStrWsMsg);

    void endOfGame(END_TYPE EndType, QWebSocket *playerToClear = nullptr);

    //todo: wszystko poniżej jako prywatne?
    //todo: nie trzeba dopisywać "byType"- funkcje mogą mieć takie same nazwy i przyjmować...
    //...różne parametry
    void newClientSocket(QWebSocket *playerSocket);
    void setClientName(QWebSocket *playerSocket, QString name);
    void setPlayerType(QWebSocket *playerSocket, PLAYERS_TYPES type);
    void clearPlayerType(PLAYERS_TYPES type);
    void setClientStateBySocket(QWebSocket *playerSocket, bool state);
    void setClientStateByType(PLAYERS_TYPES type, bool state);
    void addClientToQueue(QWebSocket *playerSocket);
    void removeClient(QWebSocket *playerSocket);
    void removeClientFromQueueBySocket(QWebSocket *playerSocket);
    void resetPlayersStartConfirmInfo();
    void moveNextClientFromQueueToTableIfExists(PLAYERS_TYPES chair);
    void clearBothPlayersStates();

    Clients getClientBySocket(QWebSocket *playerSocket);
    QWebSocket *getClientSocketByName(QString playerName);
    QString getClientNameBySocket(QWebSocket *playerSocket);
    QWebSocket *getNextQueuedClientsSocket();
    QString getQueuedClientsListAsQStr();
    PLAYERS_TYPES getClientTypeBySocket(QWebSocket *playerSocket);
    bool isPlayerChairEmpty(PLAYERS_TYPES type);
    int64_t getQueuedClientByName(QString name);
    int64_t getQueuedClientBySocket(QWebSocket *playerSocket);
    QWebSocket *getPlayerSocket(PLAYERS_TYPES type);
    bool isStartClickedByPlayerType(PLAYERS_TYPES type);
    QString getPlayerNameByType(PLAYERS_TYPES type);
    bool isClientInQueue(QWebSocket *playerSocket);
    bool isClientNameExists(QString name);
    int getAmountOfQueuedClients();

    //void testQueuedClients(); //test jednostkowy

    ~Websockets();

public Q_SLOTS: //todo: jaka jest różnica między zwykłym slots?
    void receivedMsg(QString QStrWbstMsgToProcess);
    void onNewConnection();

public slots:
    void msgFromChessboardToWebsockets(QString QStrWsMsg);
    QString getTableDataAsJSON();

signals:
    void addTextToConsole(QString, LOG);
    void MsgFromWebsocketsToChess(QString QStrMsgFromWebsockets);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showClientsList(QList<Clients>);
};

#endif // Websockets_H
