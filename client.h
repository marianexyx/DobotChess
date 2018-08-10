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
#include "vars/const_flags.h"
#include "typeinfo"
#include "sql.h"

enum CLIENT_ID { CID_CORE, CID_SQL };

class Client
{
    friend class Clients;

private:
    int64_t _ID;
    int64_t _sqlID;
    QWebSocket *_socket;
    PLAYER_TYPE _type;
    bool _isStartClickedByPlayer;
    int64_t _queue;

public:
    bool operator ==(const struct Client& cl) { return _ID == cl._ID; }

    int64_t ID() const { return _ID; }
    int64_t sqlID() const { return _sqlID; }
    QWebSocket *socket() const { return _socket; }
    PLAYER_TYPE type() const { return _type; }
    bool isStartClickedByPlayer() const { return _isStartClickedByPlayer; }
    int64_t queue() const { return _queue; }
    QString name() const { return Sql::getClientName(_sqlID); }
};

class Clients: public QObject
{
    Q_OBJECT

public:
    QList<Client> _clients;

    Clients(): _clients() {}

    void newClient(QWebSocket* const clientSocket);
    void setClientSqlID(const Client& client, int64_t sqlID);
    void clearClientSqlID(const Client& client);
    void setPlayerType(const Client& client, PLAYER_TYPE Type);
    void clearPlayerType(PLAYER_TYPE Type);
    void setClientStartConfirmation(const Client& client, bool bState);
    void setClientStartConfirmation(PLAYER_TYPE Type, bool bState);
    void addClientToQueue(const Client& client);
    void removeClientFromList(const Client& client);
    void removeClientFromQueue(const Client& client);
    void resetPlayersStartConfirmInfo();
    void putOnChairNextQueuedClient(PLAYER_TYPE Chair);

    QList<Client> getClientsList() const { return _clients; }
    bool isClientInList(const Client& client, bool bErrorLog = false);
    bool isClientInList(QWebSocket* pClientSocket, bool bErrorLog = false);
    Client getClient(QWebSocket* pClientSocket);
    Client getClient(int64_t n64ClientID, CLIENT_ID IdType = CID_CORE);
    Client getPlayer(PLAYER_TYPE Type);
    Client getNextQueuedClient();
    QString getQueuedClientsList();
    bool isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog = false);
    bool isWholeGameTableEmpty();
    bool isWholeGameTableOccupied();
    int64_t getClientPosInQueue(const Client& client);
    QWebSocket* getPlayerSocket(PLAYER_TYPE Type);
    bool isStartClickedByPlayer(PLAYER_TYPE Type);
    bool isStartClickedByBothPlayers();
    QString getPlayerName(PLAYER_TYPE Type);
    bool isClientInQueue(const Client& client);
    bool isClientSqlIDExists(int64_t n64ID, bool bErrorLog = false);
    int getAmountOfQueuedClients();
    bool isClientAPlayer(const Client& client, bool bErrorLog = false);
    bool isClientIDExists(int64_t n64ID);
    int64_t getClientID(const Client& client);
    int64_t getNextAvailableClientID();
    QString dumpAllData();

public slots:
    void showClientsInUI();

signals:
    void addTextToLogPTE(QString, LOG);
    void showClientsListInUI(QList<Client>);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
};

#endif // CLIENT_H
