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
    void setClientName(Client client, QString QStrName);
    void setPlayerType(Client client, PLAYER_TYPE Type);
    void clearPlayerType(PLAYER_TYPE Type);
    void setClientStartConfirmation(Client client, bool bState);
    void setClientStartConfirmation(PLAYER_TYPE Type, bool bState);
    void addClientToQueue(Client client);
    void removeClient(Client client);
    void removeClientFromQueue(Client client);
    void resetPlayersStartConfirmInfo();
    void cleanChairAndPutThereNextQueuedClientIfExist(PLAYER_TYPE Chair);

    QList<Client> getClientsList() const { return _clients; }
    bool isClientInList(Client client);
    Client* getClient(QWebSocket* pClientSocket);
    Client* getClient(int64_t n64ClientID);
    Client* getPlayer(PLAYER_TYPE Type);
    QWebSocket* getClientSocket(QString QStrPlayerName);
    QString getClientName(Client client);
    Client getNextQueuedClient();
    QString getQueuedClientsList();
    PLAYER_TYPE getClientType(Client client);
    bool isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog = false);
    bool isGameTableOccupied();
    int64_t getClientPosInQueue(Client client);
    QWebSocket* getPlayerSocket(PLAYER_TYPE Type);
    bool isStartClickedByPlayer(PLAYER_TYPE Type);
    bool isStartClickedByBothPlayers();
    QString getPlayerName(PLAYER_TYPE Type);
    bool isClientLoggedIn(Client client);
    bool isClientInQueue(Client client);
    bool isClientNameExists(QString QStrName, bool bErrorLog = false);
    int getAmountOfQueuedClients();
    bool isClientAPlayer(Client client, bool bErrorLog = false);
    bool isClientIDExists(int64_t n64ID);
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
