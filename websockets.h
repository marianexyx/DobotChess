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

/*
 //todo: podzielić tą klasę na wielę podklas. zaprojektować klasy tak by tworzyło to małe...
//... klasy na potrzeby tej klasy, żyjące jak organy

// składnik
class CIngredient { / * ... * / };
// obiekt nadrzędny
class CAggregate
{
    private:
    // pole ze składowym składnikiem
        CIngredient* m_pSkladnik;
    public:
    // konstruktor i destruktor
        CAggregate() { m_pSkladnik = new CIngredient; }
        ~CAggregate() { delete m_pSkladnik; }
};
*/

class Websockets: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;

    QWebSocketServer *m_pWebSocketServer;
    QList<Clients> m_clients;

private Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
    void socketDisconnected();

public:
    Websockets(Chessboard *pChessboard, quint16 port, QObject *parent = Q_NULLPTR);

    void sendToChess(QString QsMsgForChessClass);
    void sendMsg(QString QStrWsMsg);

    void endOfGame(END_TYPE EndType, QWebSocket *playerToClear = nullptr);

    //todo: wszystko poniżej powinno by w strukturze Clients
    void newClientSocket(QWebSocket *playerSocket);
    void setClientName(QWebSocket *playerSocket, QString name);
    void setPlayerType(QWebSocket *playerSocket, PLAYERS_TYPES type);
    void clearPlayerType(PLAYERS_TYPES type);
    void setClientState(QWebSocket *playerSocket, bool state);
    void setClientState(PLAYERS_TYPES type, bool state);
    void addClientToQueue(QWebSocket *playerSocket);
    void removeClient(QWebSocket *playerSocket);
    void removeClientFromQueue(QWebSocket *playerSocket);
    void resetPlayersStartConfirmInfo();
    void replaceClientOnChairWithQueuedPlayerIfExist(PLAYERS_TYPES chair, LOG formMsg = LOG_NOTHING);
    void clearBothPlayersStates();

    Clients getClient(QWebSocket *playerSocket);
    QWebSocket *getClientSocket(QString playerName);
    QString getClientName(QWebSocket *playerSocket);
    QWebSocket *getNextQueuedClientSocket();
    QString getQueuedClientsList();
    PLAYERS_TYPES getClientType(QWebSocket *playerSocket);
    bool isPlayerChairEmpty(PLAYERS_TYPES type);
    int64_t getQueuedClient(QString name);
    int64_t getQueuedClient(QWebSocket *playerSocket);
    QWebSocket *getPlayerSocket(PLAYERS_TYPES type);
    bool isStartClickedByPlayer(PLAYERS_TYPES type);
    QString getPlayerName(PLAYERS_TYPES type);
    bool isClientInQueue(QWebSocket *playerSocket);
    bool isClientNameExists(QString name);
    int getAmountOfQueuedClients();

    //void testQueuedClients(); //test jednostkowy

    ~Websockets();

public Q_SLOTS: //Q_SLOTS jest dla mechanizmow "3rd party", ktore chca uzywac slotow
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
