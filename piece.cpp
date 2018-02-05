#include "piece.h"

Piece::Piece(short sPieceID)
{
    if (Piece::isInRange(sPieceID))
        _PieceType = Piece::Type(sPieceID);
    else return;

    _sPieceID = sPieceID;
    _PieceColor = Piece::Color(sPieceID);
    _sStartFieldID = Piece::startFieldNr(sPieceID);
}

static bool Piece::isInRange(short sPieceNr)
{
    if (sPieceNr < 1 || sPieceNr > 32)
    {
        qDebug() << "ERROR: Piece: pieceNr out of range 1-32:" << sPieceNr;
        return false;
    }
    else return true;
}

static PLAYER_TYPE Piece::Color(char chFENSign)
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

static PLAYER_TYPE Piece::Color(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return -1;

    if (sPieceNr <= 16) return PT_WHITE;
    else return PT_BLACK;
}

static PIECE_TYPE Piece::Type(char chFENSign)
{
    if (Piece::Color(chFENSign) == PT_NONE) return PT_NONE;

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

static PIECE_TYPE Piece::Type(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return -1;

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

static QString Piece::name(short sPieceNr)
{
    QString QStrName = static_cast<char>(Piece::Type(sPieceNr));
    if (_PieceColor == PT_WHITE) QStrName.toUpper();

    switch(sPieceNr)
    {
    case 1: case 2: case 3: case 25: case 26: case 27:
        QStrName += "1"; break;
    case 6: case 7: case 8: case 30: case 31: case 32:
        QStrName += "2"; break;
    default:
        QStrName += QString::number(sPieceNr - static_cast<int>(_PieceColor)*8);
    }

    return QStrName;
}

static short Piece::nr(PosOnBoard pieceLines)
{
    short sPieceNr = static_cast<short>(pieceLines.Letter) +
            static_cast<short>(pieceLines.Digit - 1) * 8;

    if (Piece::isInRange(sPieceNr)) return sPieceNr;
    else return -1;
}

static PosOnBoard Pos(short sPieceNr);

static short Piece::startFieldNr(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return -1;

    if (sPieceNr <= 16) return sPieceNr;
    else return sPieceNr + 32;
}

static PosOnBoard Piece::startFieldPos(short sPieceNr)
{
    PosOnBoard PieceLines;
    if (!Piece::isInRange(sPieceNr)) return PieceLines;

    if (sPiecedNr % 8 != 0)
    {
        PieceLines.Digit = (sPiecedNr / 8) + 1;
        PieceLines.Letter  = sPiecedNr - (PieceLines.Digit * 8);
    }
    else
    {
        PieceLines.Digit = (sPiecedNr / 8);
        PieceLines.Letter = 8;
    }

    if (PieceLines.Letter < 1 || PieceLines.Letter > 8 ||
            PieceLines.Digit < 1 || PieceLines.Digit > 8)
        qDebug() << "ERROR: Field::startFieldPos(): PieceLines out of range: letter ="
                 << PieceLines.Letter << ", Digit =" << PieceLines.Digit;

    return PieceLines;
}
