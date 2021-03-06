#ifndef INTERMEDIATE_POINTS_H
#define INTERMEDIATE_POINTS_H

#pragma once
#include "QDebug"
#include "vars/point3d.h"

struct IntermediatePoints
{
    Point3D minGame;
    Point3D maxGame;
    Point3D middleAbove;
    Point3D cournerBelow;
    Point3D cournerAbove;
    Point3D retreatLeft;
    Point3D retreatRight;
    Point3D safeAxisZ; //only Z axis is used

    QString dumpAllData()
    {
        QString QStrData;

        QStrData = "[intermediate_points_calc.h]\n";
        QStrData += "minGame: " + minGame.getAsQStr() + "\n";
        QStrData += "maxGame: " + maxGame.getAsQStr() + "\n";
        QStrData += "middleAbove: " + middleAbove.getAsQStr() + "\n";
        QStrData += "cournerBelow: " + cournerBelow.getAsQStr() + "\n";
        QStrData += "cournerAbove: " + cournerAbove.getAsQStr() + "\n";
        QStrData += "retreatLeft: " + retreatLeft.getAsQStr() + "\n";
        QStrData += "retreatRight: " + retreatRight.getAsQStr() + "\n";
        QStrData += "safeAxisZ: " + safeAxisZ.getAsQStr() + "\n";

        return QStrData;
    }
};

enum INTERMEDIATE_POINTS
{
    IP_MIN_GAME,
    IP_MAX_GAME,
    IP_MIDDLE_ABOVE,
    IP_COURNER_BELOW,
    IP_COURNER_ABOVE,
    IP_RETREAT_LEFT,
    IP_RETREAT_RIGHT,
    IP_SAFE_AXIS_Z
};

inline QString intermediatePointTypeAsQstr(INTERMEDIATE_POINTS IP)
{
    switch(IP)
    {
    case IP_MIN_GAME: return "minGame";
    case IP_MAX_GAME: return "maxGame";
    case IP_MIDDLE_ABOVE: return "middleAbove";
    case IP_COURNER_BELOW: return "cournerBelow";
    case IP_COURNER_ABOVE: return "cournerAbove";
    case IP_RETREAT_LEFT: return "retreatLeft";
    case IP_RETREAT_RIGHT: return "retreatRight";
    case IP_SAFE_AXIS_Z: return "safeAxisZ";
    default:
        qCritical() << "wrong arg =" << QString::number(IP);
        return "";
    }
}

#endif // INTERMEDIATE_POINTS_H
