#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "QString"
#include "QObject"
#include "qdebug.h"

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
    void findBoardPos(int nFromLetter, int nFromDiggit, int nToLetter, int nToDiggit);
    int findPieceLetterPos(QString QsLetter);
    QString findPieceLetterPos(int nLetter);
    int fieldNrToFieldPos(int nfieldNr, bool bRow);
    bool removeStatements();
    bool castlingStatements();
    void castlingFindRookToMove();
    void actualPos(int nLetter, int nDigit);
    void pieceStateChanged(bool bIsMoveFrom, int nPieceLetter,
                           int nPieceDigit, char chMoveType);
    bool compareArrays(int nArray1[][8], int nArray2[][8]);

    //TODO: jeżeli zrobię poniższe dane (tj. struktury) jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    ArmPosition PieceFrom, PieceTo, PieceActualPos;

    QString QsPiecieFromTo;             // f.e. "e2e4"
    int nGripperPiece;              // nr bierki znajdującej się aktualnie w chwytaku

    bool bTestEnpassant;                //czy wewnętrzny powierzchowny test na enpassant się powiódł

    bool bTestPromotion;                //czy wewnętrzny powierzchowny test na promocję się powiódł
    QString QsFuturePromote;            //zmienna pamiętająca jakie było zapytanie o ruch typu promocja...
    //... Pójdzie drugie zapytanie na WWW i potrzeba będzie później stare skądś odczytać.
    bool bPromotionConfirmed;           //gdy podczas promocji pojawi się zbijanie, to dzięki tej...
    //...zmiennej program wie co i jak zbijać podczas ruchu typu promocja.

    /*const*/ int anStartBoard[8][8];       //do sprawdzania przy odkładaniu bierek na miejsce czy ...
                                        //...szachownica osiągła swoje startowe ułożenie bierek.
    int anBoard[8][8];                  //plansza jako tablica. liczby to zajęte pola.
                                        //każda liczba to inna bierka.
    int anTempBoard[8][8];              //pomocnicza tablica do sprawdzania czy udało się przywrócić...
                                        //...bierki na szachownicy do oryginalnego/pierwotnego stanu.
    int anRemoved[8][4];               //tablica usuniętych

    float afChessboardPositions_x[8][8];
    float afChessboardPositions_y[8][8];
    float afChessboardPositions_z[8][8];

    float afRemovedPiecesPositions_x[8][4];
    float afRemovedPiecesPositions_y[8][4];
    float afRemovedPiecesPositions_z[8][4];

signals:
    void addTextToDobotConsole(QString);
};

#endif // CHESSBOARD_H
