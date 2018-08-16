#ifndef FIELD_H
#define FIELD_H

#pragma once
#include "chessboard/piece.h"
#include "vars/const_flags.h"
#include "vars/point3d.h"

class Field
{
private:
    ushort m_usNr;
    Piece* m_pPieceOnField;
    ushort m_usStartPieceNrOnField;
    Point3D m_location3D;

public:
    Field(ushort usFieldID);
    ~Field() {}

    static bool isInRange(ushort usFieldNr);
    static PosOnBoard Pos(ushort usFieldNr);
    static ushort nr(PosOnBoard fieldLines);
    static ushort nr(LETTER L, DIGIT D);
    static QString nrAsQStr(ushort usFieldNr);
    static ushort startPieceNrOnField(ushort usFieldNr);
    static ushort startPieceNrOnField(PosOnBoard fieldLines);

    void set3DLocation(Point3D p) { m_location3D = p; }
    void setPieceOnField(Piece* pPiece);
    bool isFieldOccupied(bool bErrorLog = false);
    void clearField(bool bErrorLog = false);

    ushort getNr() const { return m_usNr; }
    PosOnBoard getPos() const { return Field::Pos(m_usNr); }
    QString getNrAsQStr() const { return Field::nrAsQStr(m_usNr); }
    Piece* getPieceOnField(bool bErrorLog = false) const;
    ushort getStartPieceNrOnField() const { return m_usStartPieceNrOnField; }
    Point3D getLocation3D();
    QString dumpAllData();
};

#endif // FIELD_H
