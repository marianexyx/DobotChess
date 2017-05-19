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
    bool m_bGameInProgress;
    QString m_QStrNameWhite; // nazwa białego- startowo "Biały"
    QString m_QStrNameBlack; // nazwa czarnego- startowo "Czarny"

public:
    WebTable();

//-----METODY-DOSTĘPOWE-DO-PÓL-----//
    QString getNameWhite() const          { return m_QStrNameWhite; }
    QString getNameBlack() const          { return m_QStrNameBlack; }

    void setNameWhite(QString QSNameWhite)           { m_QStrNameWhite = QSNameWhite; }
    void setNameBlack(QString QSNameBlack)           { m_QStrNameBlack = QSNameBlack; }

public slots:
    void checkWebsocketMsg(QString QstrWebsocketMsg);
};

#endif // WEBTABLE_H
