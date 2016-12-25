#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "QString"
#include "QObject"
#include "qdebug.h"

//TODO: *klasa chessboard powinna być dzieckiem klasy 'chess', jako że będzie używać wszystkiego stamtąd.
//jednak nie wiem jak to zrobić, bo później klasa 'chess' używa wskaźnika na klasę 'chessboard' by
//używać jego operacji, wiec tak się nie da. wymyślić zatem coś podobnego

class Chessboard: public QObject
{
    Q_OBJECT

public:
    Chessboard();

    void findDobotXYZVals(QString QS_msg);

signals:
    void addTextToDobotConsole(QString);
};

#endif // CHESSBOARD_H
