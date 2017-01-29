#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "QString"
#include "QObject"
#include "qdebug.h"

//TODO: *klasa chessboard powinna być dzieckiem klasy 'chess', jako że będzie używać wszystkiego stamtąd.
//jednak nie wiem jak to zrobić, bo później klasa 'chess' używa wskaźnika na klasę 'chessboard' by
//używać jego operacji, wiec tak się chyba nie da. wymyślić zatem coś podobnego

struct ArmPosition
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float r = 0.0;
};

class Chessboard: public QObject
{
    Q_OBJECT

public:
    Chessboard();

    void findDobotXYZVals(QString QS_msg);
    void findBoardPos(QString QsPiecePositions);
    int findPieceLetterPos(QString QsLetter);
    bool removeStatements();
    bool castlingStatements(QString QsPossibleCastlingVal);

    //TODO: jeżeli zrobię poniższe dane jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    ArmPosition PieceFrom, PieceTo, PieceActualPos/*, ArmUp, ArmDown*/;

    QString QsPiecieFromTo;                // f.e. "e2e4"

    int nCyfraPolaFrom;
    int nLiteraPolaFrom;
    QString QsLiteraPolaFrom;
    QString QsPieceFrom;

    int nCyfraPolaTo;
    int nLiteraPolaTo;
    QString QsLiteraPolaTo;
    QString QsPieceTo;

    QString QsPieceToReject;               //zmienna: bierka która ma być zbita

    //zmienne do roszady:
    //TODO: zastąpić to prostą matematyką typu +1, -1 jeżeli się da
    QString QsKingPosF;                     // skąd najpierw idzie król
    QString QsKingPosT;                     // dokąd najpierw idzie król
    QString QsRookPosF;                     // skąd później idzie wieża
    QString QsRookPosT;                     // dokąd później idzie wieża

    bool abBoard[8][8] =                    // plansza jako tablica. jedynki to zajęte pola
    {{1, 1, 0, 0, 0, 0, 1, 1} ,             // TODO: nie liczyć tego tylko zawsze zapytywać TCP...
    {1, 1, 0, 0, 0, 0, 1, 1} ,              //...o stan pola gry.
    {1, 1, 0, 0, 0, 0, 1, 1} ,
    {1, 1, 0, 0, 0, 0, 1, 1} ,
    {1, 1, 0, 0, 0, 0, 1, 1} ,
    {1, 1, 0, 0, 0, 0, 1, 1} ,
    {1, 1, 0, 0, 0, 0, 1, 1} ,
    {1, 1, 0, 0, 0, 0, 1, 1}};

    float afChessboardPositions_x[8][8];
    float afChessboardPositions_y[8][8];
    float afChessboardPositions_z[8][8];



signals:
    void addTextToDobotConsole(QString);
};

#endif // CHESSBOARD_H
