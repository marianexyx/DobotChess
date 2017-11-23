#include "field.h"

Field::Field()
{
    if (Field::isInRange(sFieldID))
        m_sNr = sFieldID;
    else return;

    m_sPieceOnField = 0;
}


static bool Field::isInRange(short sFieldNr)
{
    if (sFieldNr < 1 || sFieldNr > 64)
    {
        qDebug() << "ERROR: Field: fieldNr out of range 1-64:" << sFieldNr;
        return false;
    }
    else return true;
}

static PositionOnBoard Field::Pos(short sFieldNr)
{
    PositionOnBoard FieldLines;
    if (!Field::isInRange(sFieldNr)) return FieldLines;

    if (sFieldNr % 8 != 0)
    {
        FieldLines.Digit = sFieldNr / 8;
        FieldLines.Letter  = (sFieldNr - 1) - (FieldLines.Digit * 8);
    }
    else
    {
        FieldLines.Digit = (sFieldNr / 8) - 1;
        FieldLines.Letter = 7;
    }

    return FieldLines;
}

static short Field::nr(PositionOnBoard FieldLines)
{
    short sFieldNr = static_cast<short>(FieldLines.Letter + 1) +
            static_cast<short>(FieldLines.Digit)*8;

    if (Field::isInRange(sFieldNr)) return sFieldNr;
    else return -1;
}

static QString Field::nrAsQStr(short sFieldNr)
{
    PositionOnBoard PosOnBoard = Field::Pos(sFieldNr);
    QString PosAsQstr = pieceLetterPosAsQStr(PosOnBoard.Letter) +
            QString::number(PosOnBoard.Digit);
    return PosAsQstr;
}

void Field::setPieceOnField(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return;
    else m_sPieceOnField = sPieceNr;
}

void Field::clear()
{
    if (m_sPieceOnField >= 0)
        qDebug() << "ERROR: Field::clearField: field is already clear. field ="
                 << Field::nrAsQStr(m_sNr) << ", piece nr on it =" << m_sPieceOnField;
    m_sPieceOnField = 0;
}
