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
inline QString clientIDAsQStr(CLIENT_ID CID) { return CID == CID_CORE ? "coreID" : "sqlID"; }

class Client
{
    friend class Clients;

private:
    uint64_t m_ID;
    uint64_t m_sqlID;
    QWebSocket *m_socket;
    PLAYER_TYPE m_Type;
    bool m_isStartClickedByPlayer;
    uint64_t m_queue;
    QString m_name;

public:
    bool operator ==(const struct Client& cl) { return m_ID == cl.m_ID; }

    uint64_t ID() const { return m_ID; }
    uint64_t sqlID() const { return m_sqlID; }
    QWebSocket *socket() const { return m_socket; }
    PLAYER_TYPE type() const { return m_Type; }
    bool isStartClickedByPlayer() const { return m_isStartClickedByPlayer; }
    uint64_t queue() const { return m_queue; }
    QString name(bool bShowErrors = true) const
    {
        if (bShowErrors && m_sqlID == 0)
            qCritical() << "tried to access client's name without setting his sqlID 1st";
        return m_name;
    }
};

class Clients: public QObject
{
    Q_OBJECT

public:
    QList<Client> m_clients;

    Clients(): m_clients() {}

    void newClient(QWebSocket* const clientSocket);
    void putOnChairNextQueuedClient(PLAYER_TYPE Chair);
    void addClientToQueue(const Client& client);

    bool isClientInList(const Client& client, bool bErrorLog = false);
    bool isClientInList(QWebSocket* pClientSocket, bool bErrorLog = false);
    bool isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog = false);
    bool isWholeGameTableEmpty();
    bool isWholeGameTableOccupied();
    bool isClientIDExists(uint64_t un64ID);
    bool isClientSqlIDExists(uint64_t un64ID, bool bErrorLog = false);
    bool isClientAPlayer(const Client& client, bool bErrorLog = false);
    bool isStartClickedByPlayer(PLAYER_TYPE Type);
    bool isStartClickedByBothPlayers();
    bool isClientInQueue(const Client& client);

    void clearClientSqlID(const Client& client);
    void clearPlayerType(PLAYER_TYPE Type);
    void resetPlayersStartConfirmInfo();
    void removeClientFromList(const Client& client);
    void removeClientFromQueue(const Client& client);

    void setClientSqlIDAndName(const Client& client, uint64_t un64SqlID);
    void setPlayerType(const Client& client, PLAYER_TYPE Type);
    void setClientStartConfirmation(const Client& client, bool bState);
    void setClientStartConfirmation(PLAYER_TYPE Type, bool bState);

    QList<Client> getClientsList() const { return m_clients; }
    Client getClient(QWebSocket* pClientSocket);
    Client getClient(uint64_t un64ClientID, CLIENT_ID IdType = CID_CORE);
    Client getPlayer(PLAYER_TYPE Type);
    Client getNextQueuedClient();
    QString getQueuedClientsSqlIDsList();
    QString getQueuedClientsNamesList();
    uint64_t getClientPosInQueue(const Client& client);
    QWebSocket* getPlayerSocket(PLAYER_TYPE Type);
    QString getPlayerName(PLAYER_TYPE Type);
    uint getAmountOfQueuedClients();
    uint64_t getClientID(const Client& client);
    uint64_t getNextAvailableClientID();

    QString dumpAllData();

public slots:
    void showClientsInUI();

signals:
    void addTextToLogPTE(QString, LOG);
    void showClientsListInUI(QList<Client>);
    void showQueuedClientsListInUI(QString);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
};

#endif // CLIENT_H
