#ifndef DOBOT_MOVES_H
#define DOBOT_MOVES_H

#pragma once
#include <QString>

enum DOBOT_MOVE { DM_TO_POINT,
                  DM_HOME,
                  DM_WAIT,
                  DM_OPEN,
                  DM_CLOSE,
                  DM_UP,
                  DM_DOWN
                };

enum VERTICAL_MOVE { VM_NONE, VM_GRAB, VM_PUT };

inline QString dobotMoveAsQstr(DOBOT_MOVE DM)
{
    QString QStrMove;
    switch(DM)
    {
    case DM_TO_POINT: QStrMove = "toPoint" ; break;
    case DM_HOME: QStrMove = "home" ; break;
    case DM_WAIT: QStrMove = "wait" ; break;
    case DM_OPEN: QStrMove = "open" ; break;
    case DM_CLOSE: QStrMove = "close" ; break;
    case DM_UP: QStrMove = "up" ; break;
    case DM_DOWN: QStrMove = "down" ; break;
    default:
        QStrMove = "ERROR: wrong dobotMoveAsQstr arg = " + DM;
        qDebug() << QStrMove;
        break;
    }

    return QStrMove;
}

#endif // DOBOT_MOVES_H
