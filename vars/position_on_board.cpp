#include "position_on_board.h"

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
        qDebug() << "ERROR: PosOnBoard(int nL, int nD): parameters out of range: nL ="
                 << nL << ", nD =" << nD;
    }
}

PosOnBoard::PosOnBoard(QString QStrL, DIGIT D)
{
    Letter = pieceLetterPos(QStrL);
    Digit = D;
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PosOnBoard(QString QStrL, DIGIT D): parameter is X: Letter ="
                 << Letter << ", Digit =" << Digit;
}

PosOnBoard::PosOnBoard(QString QStrL, int nD)
{
    Letter = pieceLetterPos(QStrL);
    Digit = static_cast<DIGIT>(nD);
    if (Letter == L_X || Digit == D_X)
        qDebug() << "WARNING: PosOnBoard(QString QStrL, int nD): parameter is X: Letter ="
                 << Letter << ", Digit =" << Digit;
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
        qDebug() << "WARNING: PosOnBoard(QString QStrBoardPos): parameter is X: Letter ="
                 << Letter << ", Digit =" << Digit;
}

//PosFromTo:
PosFromTo::PosFromTo()
{
    from.Letter = L_X;
    from.Digit = D_X;
    to.Letter = L_X;
    to.Digit = D_X;
}

PosFromTo::PosFromTo(LETTER FromL, DIGIT FromD, LETTER ToL, DIGIT ToD)
{
    from.Letter = FromL;
    from.Digit = FromD;
    to.Letter = ToL;
    to.Digit = ToD;
}

PosFromTo::PosFromTo(QString QStrMoveFromTo)
{
    if (!PosFromTo::isMoveInProperFormat(QStrMoveFromTo)) return;

    from.Letter = pieceLetterPos(QStrMoveFromTo.left(1));
    from.Digit = static_cast<DIGIT>(QStrMoveFromTo.mid(1,1).toInt());
    to.Letter = pieceLetterPos(QStrMoveFromTo.mid(2,1));
    to.Digit = static_cast<DIGIT>(QStrMoveFromTo.mid(3,1).toInt());
    //QString::right(1) function will crash if move is promotion (last 5th...
    //...char != int
}

/*static*/ bool PosFromTo::isMoveInProperFormat(QString QStrMoveFromTo)
{
    if  (QStrMoveFromTo.length() == 4)
    {
        if (pieceLetterPos(QStrMoveFromTo.left(1)) == L_X)
            return false;

        if (static_cast<DIGIT>(QStrMoveFromTo.mid(1,1).toInt()) < D_1 ||
                static_cast<DIGIT>(QStrMoveFromTo.mid(1,1).toInt()) > D_8)
        {
            qDebug() << "ERROR: PosFromTo::isMoveInProperFormat():"
                        " pieceFromDigit is out of range <1, 8>. it's ="
                     << static_cast<DIGIT>(QStrMoveFromTo.mid(1,1).toInt())
                     << ", QStrMoveFromTo.mid(2,1) =" << QStrMoveFromTo.mid(1,1);
            return false;
        }

        if (pieceLetterPos(QStrMoveFromTo.mid(2,1)) == L_X)
            return false;

        if (static_cast<DIGIT>(QStrMoveFromTo.right(1).toInt()) < D_1 ||
                static_cast<DIGIT>(QStrMoveFromTo.right(1).toInt()) > D_8)
        {
            qDebug() << "ERROR: PosFromTo::isMoveInProperFormat():"
                        " pieceToDigit is out of range <1, 8>. it's ="
                     << static_cast<DIGIT>(QStrMoveFromTo.right(1).toInt())
                     << ", QStrMoveFromTo.right(1) =" << QStrMoveFromTo.right(1);
            return false;
        }
    }
    else
    {
        qDebug() << "ERROR: PosFromTo::isMoveInProperFormat(): wrong length:"
                 << QStrMoveFromTo.length() << ". full string =" << QStrMoveFromTo;
        return false;
    }

    return true;
}

/*static*/ PosFromTo PosFromTo::fromQStr(QString QStrMoveFromTo)
{
    PosFromTo move;

    if (!PosFromTo::isMoveInProperFormat(QStrMoveFromTo)) return move;

    move.from.Letter = pieceLetterPos(QStrMoveFromTo.left(1));
    move.from.Digit = static_cast<DIGIT>(QStrMoveFromTo.mid(1,1).toInt());
    move.to.Letter = pieceLetterPos(QStrMoveFromTo.mid(2,1));
    move.to.Digit = static_cast<DIGIT>(QStrMoveFromTo.mid(3,1).toInt());
    //QString::right(1) function will crash if move is promotion (last 5th...
    //...char != int

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
