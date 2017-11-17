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
    PLAYER_TYPE type;
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
    void playerIsLeavingGame(QWebSocket *pClient, END_TYPE leavingType);

    //todo: wszystko poniżej powinno by w strukturze Clients
    void newClientSocket(QWebSocket *clientSocket);
    void setClientName(QWebSocket *clientSocket, QString name);
    void setPlayerType(QWebSocket *clientSocket, PLAYER_TYPE type);
    void clearPlayerType(PLAYER_TYPE type);
    void setClientState(QWebSocket *clientSocket, bool state);
    void setClientState(PLAYER_TYPE type, bool state);
    void addClientToQueue(QWebSocket *clientSocket);
    void removeClient(QWebSocket *clientSocket);
    void removeClientFromQueue(QWebSocket *clientSocket);
    void resetPlayersStartConfirmInfo();
    void cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE chair);

    bool isClientInList(QWebSocket *clientSocket);
    Clients getClient(QWebSocket *clientSocket);
    QWebSocket *getClientSocket(QString playerName);
    QString getClientName(QWebSocket *clientSocket);
    QWebSocket *getNextQueuedClientSocket();
    QString getQueuedClientsList();
    PLAYER_TYPE getClientType(QWebSocket *clientSocket);
    bool isPlayerChairEmpty(PLAYER_TYPE type);
    bool isGameTableOccupied();
    int64_t getQueuedClient(QString name);
    int64_t getQueuedClient(QWebSocket *clientSocket);
    QWebSocket *getPlayerSocket(PLAYER_TYPE type);
    bool isStartClickedByPlayer(PLAYER_TYPE type);
    QString getPlayerName(PLAYER_TYPE type);
    bool isClientInQueue(QWebSocket *clientSocket);
    bool isClientNameExists(QString name);
    int getAmountOfQueuedClients();
    bool isClientAPlayer(QWebSocket *clientSocket);

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
