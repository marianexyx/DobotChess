#ifndef PLAYER_TYPE_H
#define PLAYER_TYPE_H

#pragma once
#include <QString>

enum PLAYER_TYPE
{
    PT_ERROR,
    PT_NONE,
    PT_WHITE,
    PT_BLACK
};

inline QString playerTypeAsQStr(PLAYER_TYPE PT)
{
    switch(PT)
    {
    case PT_ERROR: return "";
    case PT_NONE: return "None";
    case PT_WHITE: return "White";
    case PT_BLACK: return "Black";
    default:
        qCritical() << "unknwown parameter:" << QString::number(PT);
        return "";
    }
}

inline PLAYER_TYPE playerTypeFromQStr(QString QStrPlayer, bool bShowErrors = false)
{
    //future: func could check regardless of upper strings
    if (QStrPlayer == "White") return PT_WHITE;
    else if (QStrPlayer == "Black") return PT_BLACK;
    else if (QStrPlayer == "None") return PT_NONE;
    else
    {
        if (bShowErrors)
            qCritical() << "unknwown parameter:" << QStrPlayer;
        return PT_ERROR;
    }
}

#endif // PLAYER_TYPE_H
