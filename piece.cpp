#include "piece.h"

Piece::Piece(short sPieceID)
{
    if (Piece::isInRange(sPieceID))
        _pieceType = Piece::Type(sPieceID);
    else return;

    _sPieceID = sPieceID;
    _pieceColor = Piece::Color(sPieceID);
    _sStartFieldID = Piece::startFieldNr(sPieceID);
    _promotedType = P_ERROR;
}

/*static*/ bool Piece::isInRange(short sPieceNr)
{
    if (sPieceNr < 1 || sPieceNr > 32)
    {
        qDebug() << "ERROR: Piece::isInRange(): pieceNr out of range 1-32:" << sPieceNr;
        return false;
    }
    else return true;
}

/*static*/ PLAYER_TYPE Piece::Color(char chFENSign)
{
    PLAYER_TYPE PlayerType;

    switch(chFENSign)
    {
    case 'p': case 'r': case 'n': case 'b': case 'k': case 'q':
        PlayerType = PT_BLACK; break;
    case 'P': case 'R': case 'N': case 'B': case 'K': case 'Q':
        PlayerType = PT_WHITE; break;
    default:
        qDebug() << "ERROR: Piece::player unknown FENSign val =" << chFENSign;
        PlayerType = PT_NONE;
        break;
    }

    return PlayerType;
}

/*static*/ PLAYER_TYPE Piece::Color(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return PT_NONE;

    if (sPieceNr <= 16) return PT_WHITE;
    else return PT_BLACK;
}

/*static*/ PIECE_TYPE Piece::Type(char chFENSign)
{
    if (Piece::Color(chFENSign) == PT_NONE) return P_ERROR;

    PIECE_TYPE Type;

    switch(chFENSign)
    {
    case 'p': case 'P': Type = P_PAWN; break;
    case 'r': case 'R': Type = P_ROOK; break;
    case 'n': case 'N': Type = P_KNIGHT; break;
    case 'b': case 'B': Type = P_BISHOP; break;
    case 'k': case 'K': Type = P_KING; break;
    case 'q': case 'Q': Type = P_QUEEN; break;
    default: break;
    }

    return Type;
}

/*static*/ PIECE_TYPE Piece::Type(QString QStrFENSign)
{
    if (QStrFENSign.length() != 1)
    {
        qDebug() << "ERROR: PIECE_TYPE Type(QString): QStrFENSign.length != 1. "
                    "QStrFENSign =" << QStrFENSign;
        return P_ERROR;
    }

    PIECE_TYPE Type;
    if (QStrFENSign == "p" || QStrFENSign == "P") Type = P_PAWN;
    else if (QStrFENSign == "r" || QStrFENSign == "R") Type = P_ROOK;
    else if (QStrFENSign == "n" || QStrFENSign == "N")  Type = P_KNIGHT;
    else if (QStrFENSign == "b" || QStrFENSign == "B")  Type = P_BISHOP;
    else if (QStrFENSign == "k" || QStrFENSign == "K")  Type = P_KING;
    else if (QStrFENSign == "q" || QStrFENSign == "Q")  Type = P_QUEEN;
    else
    {
        qDebug() << "ERROR: PIECE_TYPE Type(QString): unknown QStrFENSign ="
                 << QStrFENSign;
        Type = P_ERROR;
    }

    return Type;
}

/*static*/ PIECE_TYPE Piece::Type(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr))
    {
        qDebug() << "ERROR: Piece::Type:";
        return P_ERROR;
    }

    PIECE_TYPE PieceType;

    switch(sPieceNr)
    {
    case 1: case 8: case 25: case 32:
        PieceType = P_ROOK; break;
    case 2: case 7: case 26: case 31:
        PieceType = P_KNIGHT; break;
    case 3: case 6: case 27: case 30:
        PieceType = P_BISHOP; break;
    case 4: case 28:
        PieceType = P_QUEEN; break;
    case 5: case 29:
        PieceType = P_KING; break;
    default:
        PieceType = P_PAWN; break;
    }

    return PieceType;
}

/*static*/ QString Piece::FENSign(short sPieceNr)
{
    QString QStrFENSign = QChar::fromLatin1(Piece::Type(sPieceNr));
    PLAYER_TYPE PieceColor = Piece::Color(sPieceNr);
    if (PieceColor == PT_WHITE) QStrFENSign.toUpper();
    return QStrFENSign;
}

/*static*/ QString Piece::name(short sPieceNr) //todo: źle zwraca nazwę
{  
    PLAYER_TYPE PieceColor = Piece::Color(sPieceNr);
    QString QStrName = Piece::FENSign(sPieceNr);

    switch(sPieceNr)
    {
    case 1: case 2: case 3: case 25: case 26: case 27:
        QStrName += "1"; break;
    case 6: case 7: case 8: case 30: case 31: case 32:
        QStrName += "2"; break;
    default:
        QStrName += QString::number(sPieceNr - static_cast<int>(PieceColor)*8);
    }

    return QStrName;
}

/*static*/ short Piece::nr(PosOnBoard pieceLines)
{
    short sPieceNr = static_cast<short>(pieceLines.Letter) +
            static_cast<short>(pieceLines.Digit - 1) * 8;

    if (Piece::isInRange(sPieceNr)) return sPieceNr;
    else return -1;
}

/*static*/ PosOnBoard Pos(short sPieceNr); //todo: co to??

/*static*/ short Piece::startFieldNr(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr))
    {
        qDebug() << "ERROR: Piece::startFieldNr:";
        return -1;
    }

    if (sPieceNr <= 16) return sPieceNr;
    else return sPieceNr + 32;
}

/*static*/ PosOnBoard Piece::startFieldPos(short sPieceNr)
{
    PosOnBoard PieceLines;
    if (!Piece::isInRange(sPieceNr)) return PieceLines;

    if (sPieceNr % 8 != 0)
    {
        PieceLines.Letter = static_cast<LETTER>(sPieceNr % 8);
        PieceLines.Digit = static_cast<DIGIT>((sPieceNr / 8) + 1);
    }
    else
    {
        PieceLines.Letter = L_H;
        PieceLines.Digit = static_cast<DIGIT>((sPieceNr / 8));
    }
    if ((int)PieceLines.Digit > 2)
        PieceLines.Digit = static_cast<DIGIT>((short)PieceLines.Digit + 4);

    return PieceLines;
}

void Piece::setPromotedType(PIECE_TYPE promotedType)
{
    if (_pieceType != P_PAWN)
    {
        qDebug() << "ERROR: Piece::setPromotedType(): only pawn can be promoted"
                    " . (this is '" <<  (char)_pieceType << "' piece char type)";
        return;
    }

    if (promotedType == P_QUEEN || promotedType == P_KNIGHT ||
            promotedType == P_ROOK || promotedType == P_BISHOP)
        _promotedType = promotedType;
    else qDebug() << "ERROR: Piece::setPromotedType(): wrong promotion type:"
                  << (char)promotedType;
}
