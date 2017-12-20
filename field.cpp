#include "field.h"

Field::Field(short sFieldNr)
{
    if (Field::isInRange(sFieldNr))
        _sNr = sFieldNr;
    else return;

    _sPieceOnField = 0;

    _sStartPieceNrOnField = Field::startPieceNrOnField(sFieldNr);

    _Location3D.x = -1;
    _Location3D.y = -1;
    _Location3D.z = -1;
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

static PosOnBoard Field::Pos(short sFieldNr)
{
    PosOnBoard FieldLines;
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

static short Field::nr(PosOnBoard FieldLines)
{
    short sFieldNr = static_cast<short>(FieldLines.Letter + 1) +
            static_cast<short>(FieldLines.Digit)*8;

    if (Field::isInRange(sFieldNr)) return sFieldNr;
    else return -1;
}

static QString Field::nrAsQStr(short sFieldNr)
{
    PosOnBoard PosOnBoard = Field::Pos(sFieldNr);
    QString PosAsQstr = pieceLetterPosAsQStr(PosOnBoard.Letter) +
            QString::number(PosOnBoard.Digit);
    return PosAsQstr;
}

static short Field::startPieceNrOnField(short sFieldNr)
{
    if (!Field::isInRange(sFieldNr)) return -1;

    if (sFieldNr <= 16) return sFieldNr;
    else if (sFieldNr >= 48) return sFieldNr - 32;
    else return 0;
}

static short startPieceNrOnField(PosOnBoard FieldLines)
{
    short sFieldID = Field::nr(FieldLines);
    return Field::startPieceNrOnField(sFieldID);
}

void Field::setPieceOnField(short sPieceNr)
{
    if (!Piece::isInRange(sPieceNr)) return;
    else _sPieceOnField = sPieceNr;
}

void Field::clearField()
{
    if (_sPieceOnField >= 0)
        qDebug() << "ERROR: Field::clearField: field is already clear. field ="
                 << Field::nrAsQStr(_sNr) << ", piece nr on it =" << _sPieceOnField;
    _sPieceOnField = 0;
}
bool Field::isFieldOccupied(bool bErrorLog = false)
{
    if (bErrorLog)
        qDebug() << "ERROR: Field::isFieldOccupied: field is already occupied by"
                    " another piece, field =" << _sNr << ", piece =" << _sPieceOnField;

    return _sPieceOnField > 0 ? true : false;
}
