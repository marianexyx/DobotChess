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
    int64_t ID;
    QWebSocket *socket;
    QString name;
    PLAYER_TYPE type;
    bool isStartClickedByPlayer;
    int64_t queue;

    bool operator ==(const struct Client& cl)
    {
        return socket == cl.socket &&
                name == cl.name &&
                type == cl.type &&
                isStartClickedByPlayer == cl.isStartClickedByPlayer &&
                queue == cl.queue;
    }
};

class Clients
{
private:
    QList<Client> _clients;
public:
    Clients(): _clients() {}

    void newClient(QWebSocket* pClientSocket);
    //todo: czy nie powienen był używać samych wskaźników do clientów? sprawdzić w działaniu
    void setClientName(Client client, QString name);
    void setPlayerType(Client client, PLAYER_TYPE type);
    void clearPlayerType(PLAYER_TYPE type);
    void setClientState(Client client, bool state);
    void setClientState(PLAYER_TYPE type, bool state);
    void addClientToQueue(Client client);
    void removeClient(Client client);
    void removeClientFromQueue(Client client);
    void resetPlayersStartConfirmInfo();
    void cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE chair);

    QList<Client> getClientsList() const { return _clients; }
    bool isClientInList(Client client);
    Client* getClient(QWebSocket* pClientSocket);
    Client* getClient(int64_t clientID);
    Client* getPlayer(PLAYER_TYPE type); //todo: typy z dużych? poczytać
    QWebSocket* getClientSocket(QString playerName);
    QString getClientName(Client client);
    Client getNextQueuedClient();
    QString getQueuedClientsList();
    PLAYER_TYPE getClientType(Client client);
    bool isPlayerChairEmpty(PLAYER_TYPE type, bool bErrorLog = false);
    bool isGameTableOccupied();
    int64_t getClientPosInQueue(Client client);
    QWebSocket* getPlayerSocket(PLAYER_TYPE type);
    bool isStartClickedByPlayer(PLAYER_TYPE type);
    bool isStartClickedByBothPlayers();
    QString getPlayerName(PLAYER_TYPE type);
    bool isClientLoggedIn(Client client);
    bool isClientInQueue(Client client);
    bool isClientNameExists(QString name, bool bErrorLog = false);
    int getAmountOfQueuedClients();
    bool isClientAPlayer(Client client, bool bErrorLog = false);
    bool isClientIDExists(int64_t ID);
    int64_t getClientID(Client client);
    int64_t getNextAvailableClientID();

    //void testQueuedClients(); //test jednostkowy

public slots:
    void showClientsInForm();

signals:
    void addTextToLogPTE(QString, LOG);
    void showClientsList(QList<Client>);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
};

#endif // CLIENT_H
