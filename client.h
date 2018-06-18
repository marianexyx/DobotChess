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
#include "vars/basic_vars.h"
#include "typeinfo"

const QString QUEUE_EMPTY = "queueEmpty";

struct Client //todo: class friend to Clients
{
    int64_t ID;
    QWebSocket *socket;
    QString name;
    PLAYER_TYPE type;
    bool isStartClickedByPlayer;
    int64_t queue;

    bool operator ==(const struct Client& cl) { return ID == cl.ID; }
};

class Clients: public QObject
{
    Q_OBJECT

public:
    QList<Client> _clients;

    Clients(): _clients() {}

    void newClient(QWebSocket& clientSocket);
    void setClientName(const Client& client, QString QStrName);
    void setPlayerType(const Client& client, PLAYER_TYPE Type);
    void clearPlayerType(PLAYER_TYPE Type);
    void setClientStartConfirmation(const Client& client, bool bState);
    void setClientStartConfirmation(PLAYER_TYPE Type, bool bState);
    void addClientToQueue(const Client& client);
    void removeClientFromList(const Client& client);
    void removeClientFromQueue(const Client& client);
    void resetPlayersStartConfirmInfo();
    void putOnChairNextQueuedClientIfExist(PLAYER_TYPE Chair);

    QList<Client> getClientsList() const
    {
        qDebug() << "Clients::getClientsList()";
        return _clients;
    }
    bool isClientInList(const Client& client, bool bErrorLog = false);
    Client getClient(QWebSocket* pClientSocket);
    Client getClient(int64_t n64ClientID);
    Client getClient(QString QStrName);
    Client getPlayer(PLAYER_TYPE Type);
    QWebSocket* getClientSocket(QString QStrPlayerName);
    QString getClientName(const Client& client);
    Client getNextQueuedClient();
    QString getQueuedClientsList();
    bool isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog = false);
    bool isGameTableOccupied();
    int64_t getClientPosInQueue(const Client& client);
    QWebSocket* getPlayerSocket(PLAYER_TYPE Type);
    bool isStartClickedByPlayer(PLAYER_TYPE Type);
    bool isStartClickedByBothPlayers();
    QString getPlayerName(PLAYER_TYPE Type);
    bool isClientLoggedIn(const Client& client);
    bool isClientInQueue(const Client& client);
    bool isClientNameExists(QString QStrName, bool bErrorLog = false);
    int getAmountOfQueuedClients();
    bool isClientAPlayer(const Client& client, bool bErrorLog = false);
    bool isClientIDExists(int64_t n64ID);
    int64_t getClientID(const Client& client);
    int64_t getNextAvailableClientID();

    //void testQueuedClients(); //unit test

public slots:
    void showClientsInUI();

signals:
    void addTextToLogPTE(QString, LOG);
    void showClientsList(QList<Client>);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
};

#endif // CLIENT_H
