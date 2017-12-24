#ifndef CLIENT_H
#define CLIENT_H

#pragma once
#include "QtWebSockets/QWebSocket"
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <limits>
#include "vars/players_types.h"
#include "typeinfo"

class Client
{
private:
    QWebSocket *socket;
    QString name;
    PLAYER_TYPE type;
    bool isStartClickedByPlayer;
    int64_t queue;
public:
    Client();

    bool operator ==(const struct Clients& st);

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
    //todo: void showClientsList(QList<Client>);
};

QList<Client> clients;

#endif // CLIENT_H
