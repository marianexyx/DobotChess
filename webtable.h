#ifndef WEBTABLE_H
#define WEBTABLE_H

#include <QString>
#include <QObject>
#include "qdebug.h"

//TODO: cała ta klasa praktycznie powinna się znaleźć na MySQL, a wszystkie zmienne powinny być tam...
//... zmienianie z poziomu tego serwera, tak bym dwustronny mechanizm websocketów nadal działał.

class WebTable: public QObject
{
    Q_OBJECT

private:
    //zamienniki mysqla
    bool _bGameInProgress;
    QString _QSNameWhite; // nazwa białego- startowo "Biały"
    QString _QSNameBlack; // nazwa czarnego- startowo "Czarny"
    QString _QSWhoseTurn; // czyja tura aktualnie: no_turn, white_turn, black_turn

public:
    WebTable();

//-----METODY-DOSTĘPOWE-DO-PÓL-----//
    QString getNameWhite() const          { return _QSNameWhite; }
    QString getNameBlack() const          { return _QSNameBlack; }
    QString getWhoseTurn() const          { return _QSWhoseTurn; }

    void setNameWhite(QString QSNameWhite)           { _QSNameWhite = QSNameWhite; }
    void setNameBlack(QString QSNameBlack)           { _QSNameBlack = QSNameBlack; }
    void setWhoseTurn(QString QSWhoseTurn)           { _QSWhoseTurn = QSWhoseTurn; }

public slots:
    void checkWebsocketMsg(QString QstrWebsocketMsg);
};

#endif // WEBTABLE_H
