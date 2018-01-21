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
    QString QStrType;
    switch(PT)
    {
    case PT_NONE: QStrType = ""; break;
    case PT_WHITE: QStrType = "White"; break;
    case PT_BLACK: QStrType = "Black"; break;
    default:
        qDebug() << "ERROR: playerTypeAsQStr(): unknwown parameter:" << QString::number(PT);
        break;
    }
    return QStrType;
}

inline PLAYER_TYPE playerTypeFromQStr(QString QStrPlayer)
{
    PLAYER_TYPE playerType = PT_NONE;
    if (QStrPlayer == "White") playerType = PT_WHITE;
    else if (QStrPlayer == "Black") playerType = PT_BLACK;
    else qDebug() << "ERROR: playerTypeFromQStr(): unknwown parameter:" << QStrPlayer;
    return playerType;
}

#endif // PLAYER_TYPE_H
