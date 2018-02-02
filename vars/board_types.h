#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H

#pragma once
#include <QString>

enum BOARD { B_MAIN,
             B_CHENARD,
             B_REMOVED };

inline QString boardTypeAsQstr(BOARD board)
{
    QString QStrType;
    switch(board)
    {
    case B_MAIN: QStrType = "mainBoard"; break;
    case B_CHENARD: QStrType = "chenardBoard"; break;
    case B_REMOVED: QStrType = "removedBoard"; break;
    default:
        QStrType = "ERROR: wrong boardTypeAsQstr arg = " + board;
        qDebug() << QStrType;
        break;
    }

    return QStrType;
}

#endif // BOARD_TYPES_H
