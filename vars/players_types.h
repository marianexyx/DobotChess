#ifndef PLAYER_TYPE_H
#define PLAYER_TYPE_H

#pragma once
#include <QString>

enum PLAYER_TYPE
{
    PT_NONE,
    PT_WHITE,
    PT_BLACK
};

inline QString playerTypeAsQStr(PLAYER_TYPE PT)
{
    switch(PT)
    {
    case PT_NONE: return "";
    case PT_WHITE: return "White";
    case PT_BLACK: return "Black";
    default:
        qCritical() << "unknwown parameter:" << QString::number(PT);
        return "";
    }
}

inline PLAYER_TYPE playerTypeFromQStr(QString QStrPlayer)
{
    PLAYER_TYPE playerType = PT_NONE;

    if (QStrPlayer == "White") playerType = PT_WHITE;
    else if (QStrPlayer == "Black") playerType = PT_BLACK;
    else qCritical() << "unknwown parameter:" << QStrPlayer;

    return playerType;
}

#endif // PLAYER_TYPE_H
