#ifndef WEBTABLE_H
#define WEBTABLE_H

#include <QString>
#include <QObject>
#include <QList>
#include <QRegExp>
#include "qdebug.h"
#include "qwebsocket.h"

//TODO: cała ta klasa praktycznie powinna się znaleźć na MySQL, a wszystkie zmienne powinny być tam...
//... zmienianie z poziomu tego serwera, tak bym dwustronny mechanizm websocketów nadal działał.

//TODO: każde zapytanie o ruch powinno by opatrzene identyfikatorem przynajmniej koloru gracza by...
//...kontrolować czy nie robimy ruchu za gracza przeciwnego

struct queuedPlayer
{
    QWebSocket *playerSocket;
    QString playerName;
};

class WebTable: public QObject
{
    Q_OBJECT

private:
    //zamienniki mysqla
    QString m_QStrNameWhite;
    QString m_QStrNameBlack;
    QList<queuedPlayer> m_playersQueue;
    bool m_bIsWhiteStartClicked;
    bool m_bIsBlackStartClicked;

public:
    WebTable();

    void checkWebsocketMsg(QString QstrWebsocketMsg);
    void addPlayerToQueue(queuedPlayer playerToQueue);
    void removePlayerFromQueue(QString playerToRemove);
    //bool isQueueEmpty() { return getQueuedPlayers() == "janusz,bogdan,stefan" ? true : false; }
    bool isQueueEmpty() { return getQueuedPlayers() == "queueEmpty" ? true : false; }

//-----METODY-DOSTĘPOWE-DO-PÓL-----//
    QString getNameWhite() const          { return m_QStrNameWhite;  }
    QString getNameBlack() const          { return m_QStrNameBlack; }
    QString getQueuedPlayers();
    bool getIsWhiteStartClicked() const   { return m_bIsWhiteStartClicked; }
    bool getIsBlackStartClicked() const   { return m_bIsBlackStartClicked; }

    void setNameWhite(QString QStrWhite)  { m_QStrNameWhite = QStrWhite; }
    void setNameBlack(QString QStrBlack)  { m_QStrNameBlack = QStrBlack; }
    void setIsWhiteStartClicked(bool bIsWhiteStartClicked) {  m_bIsWhiteStartClicked = bIsWhiteStartClicked; }
    void setIsBlackStartClicked(bool bIsBlackStartClicked) {  m_bIsBlackStartClicked = bIsBlackStartClicked; }
};

#endif // WEBTABLE_H
