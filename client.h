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

//future: clients class could has its own dir

enum CLIENT_ID { CID_CORE, CID_SQL };
inline QString clientIDAsQStr(CLIENT_ID CID) { return CID == CID_CORE ? "coreID" : "sqlID"; }

enum SYNCHRONIZED { SY_UNLOGGED_REMOVE_AND_REFRESH_CLIENT = -4,
                    SY_UNLOGGED_DOUBLE_LOGIN,
                    SY_UNLOGGED_LOGOUT,
                    SY_UNLOGGED_LOGIN_FAILED,
                    SY_UNLOGGED,
                    SY_LOGGED_GUEST1,
                    SY_LOGGED_GUEST2
                  };
inline QString synchronizedAsQStr(SYNCHRONIZED SY)
{
    switch(SY)
    {
    case SY_UNLOGGED_REMOVE_AND_REFRESH_CLIENT: return "removeAndRefreshClient";
    case SY_UNLOGGED_DOUBLE_LOGIN: return "doubleLogin";
    case SY_UNLOGGED_LOGOUT: return "logout";
    case SY_UNLOGGED_LOGIN_FAILED: return "loginFailed";
    case SY_UNLOGGED: return "notLogged";
    case SY_LOGGED_GUEST1: return "guest1";
    case SY_LOGGED_GUEST2: return "guest2";
    default:
        qCritical() << "wrong val =" << QString::number(SY);
        return "ERROR";
    }
}
inline QString synchronizedAsQStr(int64_t nSqlId)
{
    if (nSqlId >= SY_UNLOGGED_REMOVE_AND_REFRESH_CLIENT && nSqlId <= SY_LOGGED_GUEST2)
        return synchronizedAsQStr(static_cast<SYNCHRONIZED>(nSqlId));
    else return "client";
}

class Client
{
    friend class Clients;

private:
    uint64_t m_ID;
    int64_t m_sqlID;
    QWebSocket* m_socket;
    PLAYER_TYPE m_Type;
    bool m_isStartClickedByPlayer;
    uint64_t m_queue;
    QString m_name;

public:
    bool operator ==(const struct Client& cl) { return m_ID == cl.m_ID; }
    bool operator !=(const struct Client& cl) { return m_ID != cl.m_ID; }

    //todo: name those as "get..."
    uint64_t ID() const { return m_ID; }
    int64_t sqlID() const { return m_sqlID; }
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
    QString dumpCrucialData()
    {
        return "ID: " + QString::number(m_ID) + ", sqlID:" + QString::number(m_sqlID)
                + ", name:" + m_name + "\n";
    }
    QString dumpAllData()
    {
        return "ID: " + QString::number(m_ID)
                + ", sqlID:" + QString::number(m_sqlID)
                + ", playerType: " + playerTypeAsQStr(m_Type)
                + ", isStartClickedByPlayer: " + QString::number(m_isStartClickedByPlayer)
                + ", queue: " + QString::number(m_queue)
                + ", name:" + m_name + "\n";
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
    bool isClientInList(uint64_t un64ID, bool bErrorLog = false);
    bool isPlayerChairEmpty(PLAYER_TYPE Type, bool bErrorLog = false);
    bool isWholeGameTableEmpty();
    bool isWholeGameTableOccupied();
    bool isClientIDExists(uint64_t un64ID, bool bErrorLog = false);
    bool isClientSqlIDExists(const Client& client, bool bErrorLog = false);
    bool isClientSqlIDExists(int64_t n64sqlID, bool bErrorLog = false);
    bool isClientAPlayer(const Client& client, bool bErrorLog = false);
    bool isStartClickedByPlayer(PLAYER_TYPE Type);
    bool isStartClickedByBothPlayers();
    bool isClientInQueue(const Client& client);
    bool isClientAGuest(const Client& client);
    bool isPlayerAGuest(PLAYER_TYPE Type);

    void clearClientSqlID(const Client& client);
    void clearPlayerType(PLAYER_TYPE Type);
    void resetPlayersStartConfirmInfo();
    void removeClientFromList(const Client& client);
    void removeClientFromQueue(const Client& client);

    void setClientSqlIDAndName(const Client& client, int64_t n64SqlID);
    void setPlayerType(const Client& client, PLAYER_TYPE Type);
    void setClientStartConfirmation(const Client& client, bool bState);
    void setClientStartConfirmation(PLAYER_TYPE Type, bool bState);

    QList<Client> getClientsList() const { return m_clients; }
    Client getClient(QWebSocket* pClientSocket);
    Client getClient(int64_t n64ClientID, CLIENT_ID IdType = CID_CORE);
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
    PLAYER_TYPE getAvailableGuest(bool bErrorLog = false);

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
