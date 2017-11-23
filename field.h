#ifndef FIELD_H
#define FIELD_H

#pragma once

#include "QString"
#include "piece.h"
#include "vars/basic_vars.h"
#include "vars/posotion_on_board.h"

class Field
{
private:
    short m_sNr;
    short m_sPieceOnField;
    Point3D m_Location3D;

public:
    Field(short sFieldID); //todo: friend dla chessboard
    ~Field(); //todo: friend dla chessboard

    static double dSquareWidht = 21.f;

    static bool isInRange(short sFieldNr);
    static PositionOnBoard Pos(short sFieldNr);
    static short nr(PositionOnBoard FieldLines);
    static QString nrAsQStr(short sFieldNr);

    void setLocation3D(Point3D Point) { m_Location3D = Point; } //todo: friend dla chessboard
    void setPieceOnField(short sPieceNr); //todo: friend dla chessboard
    void clear();

    Point3D getLocation3D() const { return m_Location3D; }
    bool isFieldOccupied() const { return m_sPieceOnField > 0 ? true : false; }
    short getNr() const { return m_sNr; }
    QString getNrAsQStr() const { return Field::nrAsQStr(m_sNr); }
    short getPieceOnField() const { return m_sPieceOnField; }
};

#endif // FIELD_H
