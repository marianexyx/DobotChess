#include "piece.h"

Piece::Piece(ushort usPieceID)
{
    if (Piece::isInRange(usPieceID))
        m_pieceType = Piece::Type(usPieceID);
    else return;

    m_usPieceID = usPieceID;
    m_PieceColor = Piece::Color(usPieceID);
    m_usStartFieldID = Piece::startFieldNr(usPieceID);
    m_PromotedType = P_ERROR;
}

/*static*/ bool Piece::isInRange(ushort usPieceNr)
{
    if (usPieceNr < 1 || usPieceNr > 32)
    {
        qCritical() << "pieceNr out of range 1-32:" << QString::number(usPieceNr);
        return false;
    }
    else return true;
}

/*static*/ PLAYER_TYPE Piece::Color(char chFENSign)
{
    switch(chFENSign)
    {
    case 'p': case 'r': case 'n': case 'b': case 'k': case 'q':
        return PT_BLACK;
    case 'P': case 'R': case 'N': case 'B': case 'K': case 'Q':
        return PT_WHITE;
    default:
        qCritical() << "unknown FENSign val =" << QString::number(chFENSign);
        return PT_NONE;
    }
}

/*static*/ PLAYER_TYPE Piece::Color(ushort usPieceNr)
{
    if (!Piece::isInRange(usPieceNr)) return PT_NONE;

    if (usPieceNr <= 16) return PT_WHITE;
    else return PT_BLACK;
}

/*static*/ PIECE_TYPE Piece::Type(char chFENSign)
{
    if (Piece::Color(chFENSign) == PT_NONE) return P_ERROR;

    switch(chFENSign)
    {
    case 'p': case 'P': return P_PAWN;
    case 'r': case 'R': return P_ROOK;
    case 'n': case 'N': return P_KNIGHT;
    case 'b': case 'B': return P_BISHOP;
    case 'k': case 'K': return P_KING;
    case 'q': case 'Q': return P_QUEEN;
    default:
        qCritical() << "unknown chFENSign =" << QString::number(chFENSign);
        return P_ERROR;
    }
}

/*static*/ PIECE_TYPE Piece::Type(QString QStrFENSign)
{
    if (QStrFENSign.length() != 1)
    {
        qCritical() << "QStrFENSign.length != 1. QStrFENSign =" << QStrFENSign;
        return P_ERROR;
    }

    if (QStrFENSign == "p" || QStrFENSign == "P") return P_PAWN;
    else if (QStrFENSign == "r" || QStrFENSign == "R") return P_ROOK;
    else if (QStrFENSign == "n" || QStrFENSign == "N") return P_KNIGHT;
    else if (QStrFENSign == "b" || QStrFENSign == "B") return P_BISHOP;
    else if (QStrFENSign == "k" || QStrFENSign == "K") return P_KING;
    else if (QStrFENSign == "q" || QStrFENSign == "Q") return P_QUEEN;
    else
    {
        qCritical() << "unknown QStrFENSign =" << QStrFENSign;
        return P_ERROR;
    }
}

/*static*/ PIECE_TYPE Piece::Type(ushort usPieceNr)
{
    if (!Piece::isInRange(usPieceNr)) return P_ERROR;

    switch(usPieceNr)
    {
    case 1: case 8: case 25: case 32: return P_ROOK;
    case 2: case 7: case 26: case 31: return P_KNIGHT;
    case 3: case 6: case 27: case 30: return P_BISHOP;
    case 4: case 28: return P_QUEEN;
    case 5: case 29: return P_KING;
    default: return P_PAWN;
    }
}

/*static*/ ushort Piece::nr(PosOnBoard pieceLines)
{
    short sPieceNr = static_cast<short>(pieceLines.Letter) +
            static_cast<short>(pieceLines.Digit - 1) * 8;

    if (Piece::isInRange(sPieceNr)) return sPieceNr;
    else return 0;
}

/*static*/ ushort Piece::startFieldNr(ushort usPieceNr)
{
    if (!Piece::isInRange(usPieceNr)) return 0;

    if (usPieceNr <= 16) return usPieceNr;
    else return usPieceNr + 32;
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
    if (m_pieceType != P_PAWN)
    {
        qCritical() << "only pawn can be promoted. (this is '"
                    << QString::number(m_pieceType) << "' piece char type)";
        return;
    }

    if (promotedType == P_QUEEN || promotedType == P_KNIGHT ||
            promotedType == P_ROOK || promotedType == P_BISHOP)
        m_PromotedType = promotedType;
    else qCritical() << "wrong promotion type:" << QString::number(promotedType);
}

QString Piece::getName()
{
    QString QStrName = this->getAsFENSign();

    switch(m_usPieceID)
    {
    case 1: case 2: case 3: case 25: case 26: case 27:
        QStrName += "1"; break;
    case 6: case 7: case 8: case 30: case 31: case 32:
        QStrName += "2"; break;
    case 4: case 5: case 28: case 29:
        break;
    default:
        QStrName += QString::number(m_usPieceID - static_cast<int>(m_PieceColor)*8);
    }

    return QStrName;
}

QString Piece::getAsFENSign()
{
    QString QStrFENSign = m_PromotedType == P_ERROR
            ? QChar::fromLatin1(m_pieceType) : QChar::fromLatin1(m_PromotedType);

    if (m_PieceColor == PT_WHITE)
        QStrFENSign = QStrFENSign.toUpper();

    return QStrFENSign;
}

QString Piece::dumpAllData()
{
    return "[piece.h]: m_usPieceID: " + QString::number(m_usPieceID)
            + ", m_pieceType: " + QString::number(m_pieceType)
            + ", m_PieceColor: " + playerTypeAsQStr(m_PieceColor)
            + ", m_usStartFieldID: " + QString::number(m_usStartFieldID)
            + ", m_PromotedType: " + QString::number(m_PromotedType);
}
