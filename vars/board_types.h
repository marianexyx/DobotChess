#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H

#pragma once
#include <QString>

enum BOARD { B_MAIN,
             B_CHENARD,
             B_REMOVED };

inline QString boardTypeAsQstr(BOARD board)
{
    switch(board)
    {
    case B_MAIN: return "mainBoard";
    case B_CHENARD: return "chenardBoard";
    case B_REMOVED: return "removedBoard";
    default:
        qDebug() << "ERROR: boardTypeAsQstr(): wrong arg = " << board;
        return "";
    }
}

#endif // BOARD_TYPES_H
