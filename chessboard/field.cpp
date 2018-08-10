#include "field.h"

Field::Field(short sFieldNr)
{
    if (Field::isInRange(sFieldNr))
        _sNr = sFieldNr;
    else return;

    _pPieceOnField = nullptr;
    _sStartPieceNrOnField = Field::startPieceNrOnField(sFieldNr);
}


/*static*/ bool Field::isInRange(short sFieldNr)
{
    if (sFieldNr < 1 || sFieldNr > 64)
    {
        qCritical() << " out of range. nr =" << sFieldNr;
        return false;
    }
    else return true;
}

/*static*/ PosOnBoard Field::Pos(short sFieldNr)
{
    PosOnBoard FieldLines;
    if (!Field::isInRange(sFieldNr)) return FieldLines;

    if (sFieldNr % 8 != 0)
    {
        FieldLines.Letter  = static_cast<LETTER>(sFieldNr % 8);
        FieldLines.Digit = static_cast<DIGIT>((sFieldNr / 8) + 1);
    }
    else
    {
        FieldLines.Letter = L_H;
        FieldLines.Digit = static_cast<DIGIT>((sFieldNr / 8));
    }

    if (!Field::isInRange(Field::nr(FieldLines)))
        qCritical() << "field" << Field::nr(FieldLines) << "isn't in range after convertation";

    return FieldLines;
}

/*static*/ short Field::nr(PosOnBoard fieldLines)
{
    short sFieldNr = static_cast<short>(fieldLines.Letter) +
            static_cast<short>(fieldLines.Digit - 1) * 8;

    if (Field::isInRange(sFieldNr)) return sFieldNr;
    else return -1;
}

/*static*/ short Field::nr(LETTER L, DIGIT D)
{
    PosOnBoard FieldLines(L, D);
    return Field::nr(FieldLines);
}

/*static*/ QString Field::nrAsQStr(short sFieldNr)
{
    PosOnBoard PosOnBoard = Field::Pos(sFieldNr);
    QString PosAsQstr = pieceLetterPosAsQStr(PosOnBoard.Letter) +
            QString::number(PosOnBoard.Digit);

    return PosAsQstr;
}

/*static*/ short Field::startPieceNrOnField(short sFieldNr)
{
    if (!Field::isInRange(sFieldNr)) return -1;

    if (sFieldNr <= 16) return sFieldNr;
    else if (sFieldNr >= 49) return sFieldNr - 32;
    else return 0;
}

/*static*/ short Field::startPieceNrOnField(PosOnBoard fieldLines)
{
    short sFieldID = Field::nr(fieldLines);
    return Field::startPieceNrOnField(sFieldID);
}

void Field::setPieceOnField(Piece *pPiece)
{
    if (pPiece == nullptr)
    {
        qCritical() << "piece can't be nullptr";
        return;
    }
    else _pPieceOnField = pPiece;
}

void Field::clearField(bool bErrorLog /*= false*/)
{
    if (bErrorLog && _pPieceOnField == nullptr)
        qCritical() << "field is already clear (=0). field =" << Field::nrAsQStr(_sNr);
    _pPieceOnField = nullptr;
}

Point3D Field::getLocation3D()
{
    if (!_location3D.isPointSet())
        qCritical() << "_location3D not set";

    return _location3D;
}

bool Field::isFieldOccupied(bool bErrorLog /*= false*/)
{
    if (_pPieceOnField != nullptr)
    {
        return true;
        if (bErrorLog)
            qCritical() << "field is already occupied by another piece, field ="
                        << _sNr << ", piece =" << _pPieceOnField->getNr();
    }
    else return false;
}

Piece* Field::getPieceOnField(bool bErrorLog /*= false*/) const
{
    if (bErrorLog && _pPieceOnField == nullptr)
        qCritical() << "piece == nullptr";

    return _pPieceOnField;
}

QString Field::dumpAllData()
{
    return "[field.h]: _sNr: " + QString::number(_sNr)
            //todo: can i check _pPieceOnField->getNr() against nullptr?
            + ", _pPieceOnField->nr: " + _pPieceOnField->getNr()
            + ", _sStartPieceNrOnField: " + QString::number(_sStartPieceNrOnField)
            + ", _location3D: " + _location3D.getAsQStr();
}
