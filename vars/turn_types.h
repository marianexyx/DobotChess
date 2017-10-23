#ifndef TURN_TYPES_H
#define TURN_TYPES_H

#pragma once
#include <QString>

enum WHOSE_TURN { NO_TURN,
                  WHITE_TURN,
                  BLACK_TURN };

inline QString turnTypeAsQstr(WHOSE_TURN turn)
{
    QString QStrType;
    switch(turn)
    {
    case NO_TURN: QStrType = "noTurn"; break;
    case WHITE_TURN: QStrType = "whiteTurn"; break;
    case BLACK_TURN: QStrType = "blackTurn"; break;
    default:
        QStrType = "ERROR: wrong turnTypeAsQstr arg = " + turn;
        qDebug() << QStrType;
        break;
    }

    return QStrType;
}

#endif // TURN_TYPES_H
