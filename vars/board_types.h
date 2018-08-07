#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H

#pragma once
#include <QString>

enum BOARD
{
    B_MAIN,
    B_REMOVED
};

inline QString boardTypeAsQstr(BOARD Board)
{
    switch(Board)
    {
    case B_MAIN: return "mainBoard";
    case B_REMOVED: return "removedBoard";
    default:
        qCritical() << "wrong arg = " << Board;
        return "";
    }
}

#endif // BOARD_TYPES_H
