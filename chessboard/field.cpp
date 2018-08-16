#include "field.h"

Field::Field(ushort usFieldID)
{
    if (Field::isInRange(usFieldID))
        m_usNr = usFieldID;
    else return;

    m_pPieceOnField = nullptr;
    m_usStartPieceNrOnField = Field::startPieceNrOnField(usFieldID);
}


/*static*/ bool Field::isInRange(ushort usFieldNr)
{
    if (usFieldNr < 1 || usFieldNr > 64)
    {
        qCritical() << " out of range. nr =" << QString::number(usFieldNr);
        return false;
    }
    else return true;
}

/*static*/ PosOnBoard Field::Pos(ushort usFieldNr)
{
    PosOnBoard FieldLines;
    if (!Field::isInRange(usFieldNr)) return FieldLines;

    if (usFieldNr % 8 != 0)
    {
        FieldLines.Letter  = static_cast<LETTER>(usFieldNr % 8);
        FieldLines.Digit = static_cast<DIGIT>((usFieldNr / 8) + 1);
    }
    else
    {
        FieldLines.Letter = L_H;
        FieldLines.Digit = static_cast<DIGIT>((usFieldNr / 8));
    }

    if (!Field::isInRange(Field::nr(FieldLines)))
        qCritical() << "field" << Field::nrAsQStr(Field::nr(FieldLines))
                    << "isn't in range after convertation";

    return FieldLines;
}

/*static*/ ushort Field::nr(PosOnBoard fieldLines)
{
    ushort usFieldNr = static_cast<short>(fieldLines.Letter) +
            static_cast<short>(fieldLines.Digit - 1) * 8;

    if (Field::isInRange(usFieldNr)) return usFieldNr;
    else return 0;
}

/*static*/ ushort Field::nr(LETTER L, DIGIT D)
{
    PosOnBoard FieldLines(L, D);
    return Field::nr(FieldLines);
}

/*static*/ QString Field::nrAsQStr(ushort usFieldNr)
{
    PosOnBoard PosOnBoard = Field::Pos(usFieldNr);
    QString PosAsQstr = pieceLetterPosAsQStr(PosOnBoard.Letter) +
            QString::number(PosOnBoard.Digit);

    return PosAsQstr;
}

/*static*/ ushort Field::startPieceNrOnField(ushort usFieldNr)
{
    if (!Field::isInRange(usFieldNr)) return 0;

    if (usFieldNr <= 16) return usFieldNr;
    else if (usFieldNr >= 49) return usFieldNr - 32;
    else return 0;
}

/*static*/ ushort Field::startPieceNrOnField(PosOnBoard fieldLines)
{
    ushort usFieldID = Field::nr(fieldLines);
    return Field::startPieceNrOnField(usFieldID);
}

void Field::setPieceOnField(Piece *pPiece)
{
    if (pPiece == nullptr)
    {
        qCritical() << "piece can't be nullptr";
        return;
    }
    else m_pPieceOnField = pPiece;
}

bool Field::isFieldOccupied(bool bErrorLog /*= false*/)
{
    if (m_pPieceOnField != nullptr)
    {
        return true;
        if (bErrorLog)
            qCritical() << "field is already occupied by another piece, field ="
                        << Field::nrAsQStr(m_usNr) << ", piece ="
                        << QString::number(m_pPieceOnField->getNr());
    }
    else return false;
}

void Field::clearField(bool bErrorLog /*= false*/)
{
    if (bErrorLog && m_pPieceOnField == nullptr)
        qCritical() << "field is already clear (=0). field =" << Field::nrAsQStr(m_usNr);
    m_pPieceOnField = nullptr;
}

Piece* Field::getPieceOnField(bool bErrorLog /*= false*/) const
{
    if (bErrorLog && m_pPieceOnField == nullptr)
        qCritical() << "piece == nullptr";

    return m_pPieceOnField;
}

Point3D Field::getLocation3D()
{
    if (!m_location3D.isPointSet())
        qCritical() << "m_location3D not set";

    return m_location3D;
}

QString Field::dumpAllData()
{
    QString QStrData;

    QStrData = "[field.h] m_usNr: " + QString::number(m_usNr);
    QString QStrPieceOnField = m_pPieceOnField == nullptr ?
                "0" : QString::number(m_pPieceOnField->getNr());
    QStrData += ", m_pPieceOnField->nr: " + QStrPieceOnField;
    QStrData += ", m_usStartPieceNrOnField: " + QString::number(m_usStartPieceNrOnField);
    QStrData += ", m_location3D: " + m_location3D.getAsQStr();

    return QStrData;
}
