#ifndef FIELD_H
#define FIELD_H

#pragma once

#include "QString"
#include "piece.h"
#include "vars/basic_vars.h"
#include "vars/posotion_on_board.h"

//todo: udałoby się dziedziczyć chessboard po field'zie. settery mogłyby by być wtedy...
//... protected, a gettery public (sprawdzić)
class Field
{
private:
    short _sNr;
    short _sPieceOnField;
    short _sStartPieceNrOnField;
    Point3D _Location3D;

public:
    Field(short sFieldID);
    ~Field();

    static double dSquareWidht = 21.f;

    static bool isInRange(short sFieldNr);
    static PosOnBoard Pos(short sFieldNr);
    static short nr(PosOnBoard FieldLines);
    static QString nrAsQStr(short sFieldNr);
    static short startPieceNrOnField(short sFieldNr);
    static short startPieceNrOnField(PosOnBoard FieldLines);

    void setField3DLocation(Point3D Point) { _Location3D = Point; }
    void setPieceOnField(short sPieceNr);
    void clearField();

    Point3D getLocation3D() const { return _Location3D; }
    bool isFieldOccupied(bool bErrorLog = false);
    short getNr() const { return _sNr; }
    QString getNrAsQStr() const { return Field::nrAsQStr(_sNr); }
    short getPieceNrOnField() const { return _sPieceOnField; }
    short getStartPieceNrOnField() const { return _sStartPieceNrOnField; }
};

#endif // FIELD_H
