#ifndef WEBTABLE_H
#define WEBTABLE_H

#include <QString>
#include <QObject>
#include "qdebug.h"

//TODO: cała ta klasa praktycznie powinna się znaleźć na MySQL, a wszystkie zmienne powinny być tam...
//... zmienianie z poziomu tego serwera, tak bym dwustronny mechanizm websocketów nadal działał.

//TODO: każde zapytanie o ruch powinno by opatrzene identyfikatorem przynajmniej koloru gracza by...
//...kontrolować czy nie robimy ruchu za gracza przeciwnego

class WebTable: public QObject
{
    Q_OBJECT

private:
    //zamienniki mysqla
    QString m_QStrNameWhite; // nazwa białego- startowo "Biały"
    QString m_QStrNameBlack; // nazwa czarnego- startowo "Czarny"

public:
    WebTable();

    void checkWebsocketMsg(QString QstrWebsocketMsg);

//-----METODY-DOSTĘPOWE-DO-PÓL-----//
    QString getNameWhite() const          { return m_QStrNameWhite; }
    QString getNameBlack() const          { return m_QStrNameBlack; }

    void setNameWhite(QString QStrWhite)  { m_QStrNameWhite = QStrWhite; }
    void setNameBlack(QString QStrBlack)  { m_QStrNameBlack = QStrBlack; }
};

#endif // WEBTABLE_H
