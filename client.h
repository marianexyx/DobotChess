#ifndef CLIENT_H
#define CLIENT_H

#pragma once
#include "QtWebSockets/QWebSocket"
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "vars/players_types.h"
#include "vars/log.h"
#include "vars/board_data_labels.h"
#include "typeinfo"

struct Client //todo: class friend to Clients
{
    //TODO: ogarnąć ID- posługiwać się ID do przekazywania pomiędzy klasami info o kliencie...
    //...o którym mowa (np. w websocketach każde polecenie od klienta wysyłać z IDkiem do...
    //...klasy chess, gdzie tam dopiero sprawdzać czy dany gracz mógł to polecenie wysłać
    unsigned long long ullId;
    QWebSocket *socket;
    QString name;
    PLAYER_TYPE type;
    bool isStartClickedByPlayer;
    int64_t queue;

    bool operator ==(const struct Client& st)
    {
        return socket == st.socket &&
                name == st.name &&
                type == st.type &&
                isStartClickedByPlayer == st.isStartClickedByPlayer &&
                queue == st.queue;
    }
};

class Clients
{
private:
    QList<Client> _clients;
public:
    Clients() {}

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

signals:
    void showClientsList(QList<Client>);
};

#endif // CLIENT_H
