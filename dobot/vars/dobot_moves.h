#ifndef DOBOT_MOVES_H
#define DOBOT_MOVES_H

#pragma once
#include <QString>

enum DOBOT_MOVE_TYPE
{
    DM_NONE,
    DM_TO_POINT,
    DM_CALIBRATE,
    DM_OPEN,
    DM_CLOSE,
    DM_UP,
    DM_DOWN
};

inline QString dobotMoveAsQstr(DOBOT_MOVE_TYPE DM)
{
    switch(DM)
    {
    case DM_TO_POINT: return "toPoint";
    case DM_CALIBRATE: return "calibrate";
    case DM_OPEN: return "open";
    case DM_CLOSE: return "close";
    case DM_UP: return "up";
    case DM_DOWN: return "down";
    default:
        qDebug() << "ERROR: dobotMoveAsQstr(): wrong arg =" << DM;
        return "";
    }
}

#endif // DOBOT_MOVES_H
