#ifndef DOBOT_MOVES_H
#define DOBOT_MOVES_H

#pragma once
#include <QString>

//todo: przenieść do folderu dobot (bez vertical move)
enum DOBOT_MOVE_TYPE { DM_NONE,
                       DM_TO_POINT,
                       DM_CALIBRATE,
                       DM_OPEN,
                       DM_CLOSE,
                       DM_UP,
                       DM_DOWN
                     };

enum VERTICAL_MOVE { VM_NONE, VM_GRAB, VM_PUT };

inline QString dobotMoveAsQstr(DOBOT_MOVE_TYPE DM)
{
    QString QStrMove;
    switch(DM)
    {
    case DM_TO_POINT: QStrMove = "toPoint" ; break;
    case DM_CALIBRATE: QStrMove = "calibrate" ; break;
    case DM_OPEN: QStrMove = "open" ; break;
    case DM_CLOSE: QStrMove = "close" ; break;
    case DM_UP: QStrMove = "up" ; break;
    case DM_DOWN: QStrMove = "down" ; break;
    default:
        qDebug() << "ERROR: dobotMoveAsQstr(): wrong arg =" << DM;
        break;
    }

    return QStrMove;
}

#endif // DOBOT_MOVES_H
