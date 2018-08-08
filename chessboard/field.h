#ifndef FIELD_H
#define FIELD_H

#pragma once
#include "chessboard/piece.h"
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
    ~Field() {}

    static bool isInRange(short sFieldNr);
    static PosOnBoard Pos(short sFieldNr);
    static short nr(PosOnBoard fieldLines);
    static short nr(LETTER L, DIGIT D);
    static QString nrAsQStr(short sFieldNr);
    static short startPieceNrOnField(short sFieldNr);
    static short startPieceNrOnField(PosOnBoard fieldLines);

    void set3DLocation(Point3D p) {_location3D = p; }
    void setPieceOnField(Piece* pPiece);
    void clearField(bool bErrorLog = false);

    Point3D getLocation3D();
    bool isFieldOccupied(bool bErrorLog = false);
    short getNr() const { return _sNr; }
    PosOnBoard getPos() const { return Field::Pos(_sNr); }
    QString getNrAsQStr() const { return Field::nrAsQStr(_sNr); }
    Piece* getPieceOnField(bool bErrorLog = false) const;
    short getStartPieceNrOnField() const { return _sStartPieceNrOnField; }
};

#endif // FIELD_H
