#include "field.h"

Field::Field(short sFieldNr)
{
    if (Field::isInRange(sFieldNr))
        _sNr = sFieldNr;
    else return;

    _pPieceOnField = nullptr;

    _sStartPieceNrOnField = Field::startPieceNrOnField(sFieldNr);
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
        FieldLines.Digit = (sFieldNr / 8) + 1;
        FieldLines.Letter  = sFieldNr - (FieldLines.Digit * 8);
    }
    else
    {
        FieldLines.Digit = (sFieldNr / 8);
        FieldLines.Letter = 8;
    }

    if (FieldLines.Letter < 1 || FieldLines.Letter > 8 ||
            FieldLines.Digit < 1 || FieldLines.Digit > 8)
        qDebug() << "ERROR: Field::Pos(): FieldLines out of range: letter ="
                 << FieldLines.Letter << ", Digit =" << FieldLines.Digit;

    return FieldLines;
}

static short Field::nr(PosOnBoard FieldLines)
{
    short sFieldNr = static_cast<short>(FieldLines.Letter) +
            static_cast<short>(FieldLines.Digit - 1) * 8;

    if (Field::isInRange(sFieldNr)) return sFieldNr;
    else return -1;
}

static short nr(LETTER let, DIGIT dig)
{
    PosOnBoard FieldLines(let, dig);
    return Field::nr(FieldLines);
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

void Field::setPieceOnField(Piece *pPiece)
{
    if (pPiece != nullptr)
    {
        qDebug() << "ERROR: Field::setPieceOnField(): piece can't be nullptr";
        return;
    }
    else _pPieceOnField = pPiece;
}

void Field::clearField()
{
    if (_pPieceOnField != nullptr)
        qDebug() << "ERROR: Field::clearField(): field is already clear. field ="
                 << Field::nrAsQStr(_sNr) << ", piece nr on it =" << _pPieceOnField->getNr();
    _pPieceOnField = nullptr;
}

bool Field::isFieldOccupied(bool bErrorLog = false)
{
    if (_pPieceOnField != nullptr)
    {
        return true;
        if (bErrorLog)
            qDebug() << "ERROR: Field::isFieldOccupied: field is already occupied by another"
                        " piece, field =" << _sNr << ", piece =" << _pPieceOnField->getNr();
    }
    else return false;
}

Piece* Field::getPieceOnField(bool bErrorLog = false) const
{
    if (bErrorLog && _pPieceOnField == nullptr)
        qDebug() << "ERROR: Field::getPieceOnField(): piece == nullptr";

    return _pPieceOnField;
}
