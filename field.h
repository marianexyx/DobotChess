#ifndef FIELD_H
#define FIELD_H

#pragma once
#include "piece.h"
#include "vars/basic_vars.h"

class Field
{
private:
    short _sNr;
    Piece* _pPieceOnField;
    short _sStartPieceNrOnField;
    Point3D _location3D;

public:
    Field(short sFieldID);
    ~Field();

    static bool isInRange(short sFieldNr);
    static PosOnBoard Pos(short sFieldNr);
    static short nr(PosOnBoard fieldLines);
    static short nr(LETTER L, DIGIT D);
    static QString nrAsQStr(short sFieldNr);
    static short startPieceNrOnField(short sFieldNr);
    static short startPieceNrOnField(PosOnBoard fieldLines);

    void setField3DLocation(Point3D point) { _location3D = point; }
    void setPieceOnField(Piece* pPiece);
    void clearField();

    Point3D getLocation3D() const { return _location3D; }
    bool isFieldOccupied(bool bErrorLog = false);
    short getNr() const { return _sNr; }
    PosOnBoard getPos() const { return Field::Pos(_sNr); }
    QString getNrAsQStr() const { return Field::nrAsQStr(_sNr); }
    Piece* getPieceOnField(bool bErrorLog = false) const;
    short getStartPieceNrOnField() const { return _sStartPieceNrOnField; }
};

#endif // FIELD_H
