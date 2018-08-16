#ifndef POSITION_ON_BOARD_H
#define POSITION_ON_BOARD_H

#pragma once
#include "QString"
#include "chessboard/vars/board_axis.h"

struct PosOnBoard
{
    LETTER Letter;
    DIGIT Digit;

    PosOnBoard(): Letter(L_X), Digit(D_X) {}
    PosOnBoard(LETTER L, DIGIT D): Letter(L), Digit(D) {}
    PosOnBoard(int nL, int nD);
    PosOnBoard(QString QStrL, DIGIT D);
    PosOnBoard(QString QStrL, int nD);
    PosOnBoard(QString QStrBoardPos);

    static QString posAsQStr(PosOnBoard Pos)
    { return pieceLetterPosAsQStr(Pos.Letter) + QString::number(Pos.Digit); }
    bool isPosSet(bool bErrorLog = false);
    void setLetterFromQStr(QString QStrL) { Letter = pieceLetterPos(QStrL); }
    QString getAsQStr() { return pieceLetterPosAsQStr(Letter) + QString::number(Digit); }
};

struct PosFromTo
{
    PosOnBoard from;
    PosOnBoard to;

    PosFromTo(): from(), to() {}
    PosFromTo(PosOnBoard PosFrom, PosOnBoard PosTo) { from = PosFrom, to = PosTo; }
    PosFromTo(LETTER FromL, DIGIT FromD, LETTER ToL, DIGIT ToD);
    PosFromTo(QString QStrMoveFromTo);

    static bool isMoveInProperFormat(QString QStrMoveFromTo);
    static PosFromTo fromQStr(QString QStrMoveFromTo);
    QString asQStr();
    void clear();
};

#endif // POSITION_ON_BOARD_H
