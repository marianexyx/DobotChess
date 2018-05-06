#ifndef TURN_TYPES_H
#define TURN_TYPES_H

#pragma once
#include <QString>

enum WHOSE_TURN
{
    NO_TURN,
    WHITE_TURN,
    BLACK_TURN
};

inline QString turnTypeAsQstr(WHOSE_TURN turn)
{
    switch(turn)
    {
    case NO_TURN: return "noTurn";
    case WHITE_TURN: return "whiteTurn";
    case BLACK_TURN: return "blackTurn";
    default:
        qDebug() << "ERROR: wrong turnTypeAsQstr arg =" << turn;
        return "ERROR";
    }
}

inline QString turnTypeAsBriefQstr(WHOSE_TURN turn)
{
    switch(turn)
    {
    case NO_TURN: return "nt";
    case WHITE_TURN: return "wt";
    case BLACK_TURN: return "bt";
    default:
        qDebug() << "ERROR: wrong turnTypeAsBriefQstr arg =" << turn;
        return "ER";
    }
}

#endif // TURN_TYPES_H
