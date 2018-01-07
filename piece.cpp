#include "piece.h"

Piece::Piece(short sPieceID)
{
    if (Piece::isInRange(sPieceID))
        m_PieceType = Piece::Type(sPieceID);
    else return;

    m_sPieceID = sPieceID;
    m_PieceColor = Piece::Color(sPieceID);
    m_sStartFieldID = Piece::StartFieldNr(sPieceID);
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

static PLAYER_TYPE Piece::Color(char cFENSign)
{
    PLAYER_TYPE PlayerType;

    switch(cFENSign)
    {
    case 'p': case 'r': case 'n': case 'b': case 'k': case 'q':
        PlayerType = PT_BLACK; break;
    case 'P': case 'R': case 'N': case 'B': case 'K': case 'Q':
        PlayerType = PT_WHITE; break;
    default:
        qDebug() << "ERROR: Piece::player unknown FENSign val =" << cFENSign;
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

static PIECE_TYPE Piece::Type(char cFENSign)
{
    if (Piece::Color(cFENSign) == PT_NONE) return PT_NONE;

    PIECE_TYPE Type;

    switch(cFENSign)
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

static short Piece::nr(PosOnBoard PieceLines)
{
    short sPieceNr = static_cast<short>(PieceLines.Letter + 1) +
            static_cast<short>(PieceLines.Digit)*8;

    if (Piece::isInRange(sPieceNr)) return sPieceNr;
    else return -1;
}

static PosOnBoard Pos(short sPieceNr);

static short Piece::StartFieldNr(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return -1;

    if (sPieceNr <= 16) return sPieceNr;
    else return sPieceNr + 32;
}

static PosOnBoard Piece::StartFieldPos(short sPieceNr)
{
    PosOnBoard PieceLines;

    if (!Piece::isInRange(sPieceNr)) return PieceLines;

    if (sPiecedNr % 8 != 0)
    {
        PieceLines.Digit = sPiecedNr / 8;
        PieceLines.Letter  = (sPiecedNr - 1) - (PieceLines.Digit * 8);
    }
    else
    {
        PieceLines.Digit = (sPiecedNr / 8) - 1;
        PieceLines.Letter = 7;
    }

    return PieceLines;
}
