#ifndef DOBOT_MOVES_H
#define DOBOT_MOVES_H

#pragma once
#include <QString>
#include "vars/basic_vars.h"
#include "DobotDll.h"
#include "DobotType.h"

enum DOBOT_MOVE_TYPE
{
    DM_NONE,
    DM_TO_POINT,
    DM_CALIBRATE,
    DM_OPEN,
    DM_CLOSE,
    DM_UP,
    DM_DOWN,
    DM_WAIT
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
    case DM_WAIT: return "wait";
    default:
        qDebug() << "ERROR: dobotMoveAsQstr(): wrong arg =" << DM;
        return "";
    }
}

struct DobotMove
{
    uint64_t ID;
    DOBOT_MOVE_TYPE type;
    Point3D xyz;

    DobotMove() :ID(0), type(DM_NONE), xyz(0,0,0) {}
    DobotMove(uint64_t id, DOBOT_MOVE_TYPE MT, Point3D p): ID(id), type(MT), xyz(p) {}
};

inline bool isArmReceivedCorrectCmd(int nResult, bool bErrorLog = false)
{
    if (nResult == DobotCommunicate_NoError)
        return true;
    else
    {
        if (bErrorLog)
        {
            if (nResult == DobotCommunicate_BufferFull)
                qDebug() << "ERROR: isArmReceivedCorrectCmd(): dobot buffer is full";
            else if (nResult == DobotCommunicate_Timeout)
                qDebug() << "ERROR: isArmReceivedCorrectCmd(): cmd timeout";
            else
                qDebug() << "ERROR: isArmReceivedCorrectCmd(): unknown error:"
                         << nResult;
        }

        return false;
    }
}

#endif // DOBOT_MOVES_H
