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
    int Letter = -1;
    int Digit = -1;
};

class Chessboard: public QObject
{
    Q_OBJECT

public:
    Chessboard();

    void findBoardPos(QString QsPiecePositions);
    int findPieceLetterPos(QString QsLetter);
    bool removeStatements();
    bool castlingStatements(QString QsPossibleCastlingVal);
    void actualPos(int nLetter, int nDigit);

    //TODO: jeżeli zrobię poniższe dane (tj. struktury) jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    ArmPosition PieceFrom, PieceTo, PieceActualPos/*, ArmUp, ArmDown*/;

    QString QsPiecieFromTo;                // f.e. "e2e4"

    //int nCyfraPolaFrom;
    //int nLiteraPolaFrom;
    //QString QsLiteraPolaFrom;
    QString QsPieceFrom;                   // f.e. "e2"

    //int nCyfraPolaTo;
    //int nLiteraPolaTo;
    //QString QsLiteraPolaTo;
    QString QsPieceTo;                     // f.e. "e4"

    QString QsPieceToReject;               //zmienna: bierka która ma być zbita

    //zmienne do roszady:
    //TODO: zastąpić to prostą matematyką typu +1, -1 jeżeli się da
    //QString QsKingPosF;                     // skąd najpierw idzie król
    //QString QsKingPosT;                     // dokąd najpierw idzie król
    QString QsRookPosF;                     // skąd później idzie wieża
    QString QsRookPosT;                     // dokąd później idzie wieża

    int nTransferredPiece;

    int anBoard[8][8] =                     // plansza jako tablica. liczby to zajęte pola.
    {{1, 9, 0, 0, 0, 0, 17, 25} ,           // każda liczba to inna bierka.
    {2, 10, 0, 0, 0, 0, 18, 26} ,           // TODO: nie liczyć tego tylko zawsze zapytywać TCP...
    {3, 11, 0, 0, 0, 0, 19, 27} ,           //...o stan pola gry.
    {4, 12, 0, 0, 0, 0, 20, 28} ,
    {5, 13, 0, 0, 0, 0, 21, 29} ,
    {6, 14, 0, 0, 0, 0, 22, 30} ,
    {7, 15, 0, 0, 0, 0, 23, 31} ,
    {8, 16, 0, 0, 0, 0, 24, 32}};

    bool abRemoved[4][8] =                  //tablica usuniętych
    {{0, 0, 0, 0, 0, 0, 0, 0} ,
    {0, 0, 0, 0, 0, 0, 0, 0} ,
    {0, 0, 0, 0, 0, 0, 0, 0} ,
    {0, 0, 0, 0, 0, 0, 0, 0}};

    int nRemovingRowPos;
    int nRemovingColumnPos;

    float afChessboardPositions_x[8][8];
    float afChessboardPositions_y[8][8];
    float afChessboardPositions_z[8][8];

    float afRemovedPiecesPositions_x[4][8];
    float afRemovedPiecesPositions_y[4][8];
    float afRemovedPiecesPositions_z[4][8];

signals:
    void addTextToDobotConsole(QString);
};

#endif // CHESSBOARD_H
