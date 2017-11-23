#ifndef POSOTION_ON_BOARD_H
#define POSOTION_ON_BOARD_H

#pragma once

#include "QString"
#include "vars/board_axis.h"

struct PositionOnBoard
{
    LETTER Letter;
    DIGIT Digit;

    PositionOnBoard(): Letter(L_X), Digit(D_X) {}
    PositionOnBoard(LETTER L, DIGIT D): Letter(L), Digit(D) {}
    PositionOnBoard(int nL, int nD);
    PositionOnBoard(QString QStrL, DIGIT D);
    PositionOnBoard(QString QStrL, int nD);
    PositionOnBoard(QString QStrBoardPos);

    static QString posAsQStr(PositionOnBoard Pos)
    { return pieceLetterPosAsQStr(Pos.Letter) + QString::number(Pos.Digit + 1); }

    void setLetterFromQStr(QString QStrL) { Letter = pieceLetterPos(QStrL); }
    QString getAsQStr() { return pieceLetterPosAsQStr(Letter) + QString::number(Digit+1); }
};


PositionOnBoard::PositionOnBoard(int nL, int nD)
{
    if ((nL >= L_A && nL <= L_H && nD >= D_1 && nD <= D_8) || (nL == L_X && nD == D_X))
    {
        Letter = static_cast<LETTER>(nL);
        Digit = static_cast<DIGIT>(nD);
    }
    else
    {
        Letter = L_X;
        Digit = D_X;
        qDebug() << "ERROR: PositionOnBoard: parameters out of range: nL =" << nL
                 << ", nD =" << nD;
    }
}

PositionOnBoard::PositionOnBoard(QString QStrL, DIGIT D)
{
    Letter = pieceLetterPos(QStrL);
    Digit = D;
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PositionOnBoard(QString QStrL, DIGIT D): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}

PositionOnBoard::PositionOnBoard(QString QStrL, int nD)
{
    Letter = pieceLetterPos(QStrL);
    Digit = static_cast<DIGIT>(nD);
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PositionOnBoard(QString QStrL, int nD): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}
PositionOnBoard::PositionOnBoard(QString QStrBoardPos)
{
    if (QStrBoardPos.length() != 2)
    {
        qDebug() << "ERROR: PositionOnBoard(QString QStrBoardPos): QStrBoardPos.length() != 2)";
        Letter = L_X;
        Digit = D_X;
    }
    else
    {
        Letter = pieceLetterPos(QStrBoardPos.left(1));
        Digit = static_cast<DIGIT>(QStrBoardPos.right(1).toInt() - 1);
    }
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PositionOnBoard(QString QStrBoardPos): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}

#endif // POSOTION_ON_BOARD_H
