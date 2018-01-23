#ifndef POSOTION_ON_BOARD_H
#define POSOTION_ON_BOARD_H

#pragma once

#include "QString"
#include "vars/board_axis.h"

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
    { return pieceLetterPosAsQStr(Pos.Letter) + QString::number(Pos.Digit+1); }

    void setLetterFromQStr(QString QStrL) { Letter = pieceLetterPos(QStrL); }
    QString getAsQStr() { return pieceLetterPosAsQStr(Letter) + QString::number(Digit+1); }
};

struct PosFromTo
{
    PosOnBoard from;
    PosOnBoard to;

    PosFromTo();
    PosFromTo(PosOnBoard PosFrom, PosOnBoard PosTo) { from = PosFrom, to = PosTo;}
    PosFromTo(LETTER fromL, DIGIT fromD, LETTER toL, DIGIT toD);
    PosFromTo(QString QStrMoveFromTo);

    static bool isMoveInProperFormat(QString QStrMoveFromTo);
    static PosFromTo fromQStr(QString QStrMoveFromTo);

    QString asQStr();
    void clear();
};





//PosOnBoard:
PosOnBoard::PosOnBoard(int nL, int nD)
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
        qDebug() << "ERROR: PosOnBoard(int nL, int nD): parameters out of range: nL =" << nL
                 << ", nD =" << nD;
    }
}

PosOnBoard::PosOnBoard(QString QStrL, DIGIT D)
{
    Letter = pieceLetterPos(QStrL);
    Digit = D;
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PosOnBoard(QString QStrL, DIGIT D): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}

PosOnBoard::PosOnBoard(QString QStrL, int nD)
{
    Letter = pieceLetterPos(QStrL);
    Digit = static_cast<DIGIT>(nD);
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PosOnBoard(QString QStrL, int nD): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}
PosOnBoard::PosOnBoard(QString QStrBoardPos)
{
    if (QStrBoardPos.length() != 2)
    {
        qDebug() << "ERROR: PosOnBoard(QString QStrBoardPos): QStrBoardPos.length() != 2)";
        Letter = L_X;
        Digit = D_X;
    }
    else
    {
        Letter = pieceLetterPos(QStrBoardPos.left(1));
        Digit = static_cast<DIGIT>(QStrBoardPos.right(1).toInt());
    }
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PosOnBoard(QString QStrBoardPos): parameter is X: "
                    "Letter =" << Letter << ", Digit =" << Digit;
}




//PosFromTo:
PosFromTo::PosFromTo()
{
    from.Letter = L_X;
    from.Digit = D_X;
    to.Letter = L_X;
    to.Digit = D_X;
}

PosFromTo::PosFromTo(LETTER fromL, DIGIT fromD, LETTER toL, DIGIT toD)
{
    from.Letter = fromL;
    from.Digit = fromD;
    to.Letter = toL;
    to.Digit = toD;
}

PosFromTo::PosFromTo(QString QStrMoveFromTo)
{
    if (!PosFromTo::isMoveInProperFormat(QStrMoveFromTo)) return;

    from.Letter = pieceLetterPos(QStrMoveFromTo.left(1));
    from.Digit = QString::number(QStrMoveFromTo.mid(2,1));
    to.Letter = pieceLetterPos(QStrMoveFromTo.mid(3,1));
    to.Digit = QString::number(QStrMoveFromTo.right(1));
}

static bool PosFromTo::isMoveInProperFormat(QString QStrMoveFromTo)
{
    if (QStrMoveFromTo.length() != 4 && QStrMoveFromTo.length() != 5)
    {
        qDebug() << "ERROR: ChessMovements::isMoveInProperFormat(): wrong length:"
                 << QStrMoveFromTo.length();
        return false;
    }

    if (pieceLetterPos(QStrMoveFromTo.left(1)) == L_X)
        return false;

    if (QString::number(QStrMoveFromTo.mid(2,1)) < 1 ||
            QString::number(QStrMoveFromTo.mid(2,1)) > 8)
    {
        qDebug() << "ERROR: ChessMovements::isMoveInProperFormat(): pieceFromDigit is out"
                    " of range <1, 8>. it ==" << QString::number(QStrMoveFromTo.mid(2,1));
        return false;
    }

    if (pieceLetterPos(QStrMoveFromTo.mid(3,1)) == L_X)
        return false;

    if (QString::number(QStrMoveFromTo.mid(4,1)) < 1 ||
            QString::number(QStrMoveFromTo.mid(4,1)) > 8)
    {
        qDebug() << "ERROR: ChessMovements::isMoveInProperFormat(): pieceToDigit is out"
                    " of range <1, 8>. it ==" << QString::number(QStrMoveFromTo.right(1));
        return false;
    }

    if ((QStrMoveFromTo.right(1) == "q" || QStrMoveFromTo.right(1) == "Q" ||
         QStrMoveFromTo.right(1) == "r" || QStrMoveFromTo.right(1) == "R" ||
         QStrMoveFromTo.right(1) == "b" || QStrMoveFromTo.right(1) == "B" ||
         QStrMoveFromTo.right(1) == "k" || QStrMoveFromTo.right(1) == "K")
            && QStrMoveFromTo.length() == 5)
    {
        qDebug() << "ERROR: ChessMovements::isMoveInProperFormat(): last char isn't "
                    " promotion char:" << QStrMoveFromTo.right(1);
        return false;
    }

    return true;
}

static PosFromTo PosFromTo::fromQStr(QString QStrMoveFromTo)
{
    if (!PosFromTo::isMoveInProperFormat(QStrMoveFromTo)) return;

    PosFromTo move;
    move.from.Letter = pieceLetterPos(QStrMoveFromTo.left(1));
    move.from.Digit = QString::number(QStrMoveFromTo.mid(2,1));
    move.to.Letter = pieceLetterPos(QStrMoveFromTo.mid(3,1));
    move.to.Digit = QString::number(QStrMoveFromTo.right(1));

    return move;
}

QString PosFromTo::asQStr()
{
    return pieceLetterPosAsQStr(from.Letter) + QString::number(from.Digit)
            + pieceLetterPosAsQStr(to.Letter) + QString::number(to.Digit);
}

void PosFromTo::clear()
{
    from.Letter = L_X;
    from.Digit = D_X;
    to.Letter = L_X;
    to.Digit = D_X;
}

#endif // POSOTION_ON_BOARD_H
