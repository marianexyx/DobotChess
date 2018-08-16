#include "position_on_board.h"

///PosOnBoard:
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
        qCritical() << "parameters out of range: nL =" << QString::number(nL)
                    << ", nD =" << QString::number(nD);
    }
}

PosOnBoard::PosOnBoard(QString QStrL, DIGIT D)
{
    Letter = pieceLetterPos(QStrL);
    Digit = D;
    if (Letter == L_X || Digit == D_X)
        qCritical() << "parameter is X: Letter =" << QString::number(Letter)
                    << ", Digit =" << QString::number(Digit);
}

PosOnBoard::PosOnBoard(QString QStrL, int nD)
{
    Letter = pieceLetterPos(QStrL);
    Digit = static_cast<DIGIT>(nD);
    if (Letter == L_X || Digit == D_X)
        qCritical() << "parameter is X: Letter =" << QString::number(Letter)
                    << ", Digit =" << QString::number(Digit);
}
PosOnBoard::PosOnBoard(QString QStrBoardPos)
{
    if (QStrBoardPos.length() != 2)
    {
        qCritical() << "QStrBoardPos.length() != 2)";
        Letter = L_X;
        Digit = D_X;
    }
    else
    {
        Letter = pieceLetterPos(QStrBoardPos.left(1));
        Digit = static_cast<DIGIT>(QStrBoardPos.right(1).toInt());
    }
    if (Letter == L_X || Digit == D_X)
        qWarning() << "parameter is X: Letter =" << QString::number(Letter)
                   << ", Digit =" << QString::number(Digit);
}

bool PosOnBoard::isPosSet(bool bErrorLog /*= false*/)
{
    if (Letter != L_X && Digit != D_X) return true;
    else
    {
        if (bErrorLog)
            qCritical() << "pos isn't false";
        return false;
    }
}


///PosFromTo:
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
    //don't use QString::right(1) above! function will crash if move...
    //...is promotion (last 5th char != int)
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
            qCritical() << "pieceFromDigit is out of range <1, 8>. it's ="
                        << QStrMoveFromTo.mid(1,1) << ", QStrMoveFromTo.mid(2,1) ="
                        << QStrMoveFromTo.mid(2,1);
            return false;
        }

        if (pieceLetterPos(QStrMoveFromTo.mid(2,1)) == L_X)
            return false;

        if (static_cast<DIGIT>(QStrMoveFromTo.right(1).toInt()) < D_1 ||
                static_cast<DIGIT>(QStrMoveFromTo.right(1).toInt()) > D_8)
        {
            qCritical() << "pieceToDigit is out of range <1, 8>. it's ="
                        << QStrMoveFromTo.right(1) << ", QStrMoveFromTo.right(1) ="
                        << QStrMoveFromTo.right(1);
            return false;
        }
    }
    else
    {
        qCritical() << "wrong length:" << QString::number(QStrMoveFromTo.length())
                    << ". full string =" << QStrMoveFromTo;
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
    //don't use QString::right(1) above! function will crash if move...
    //...is promotion (last 5th char != int)

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
