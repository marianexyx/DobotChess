#ifndef PLAYERS_TYPES_H
#define PLAYERS_TYPES_H

#pragma once
#include <QString>

enum PLAYERS_TYPES
{
    PT_NONE,
    PT_WHITE,
    PT_BLACK
};

//TODO: includować to wszędzie gdzie używam zmiennych biały/czarny czy white/black

inline QString playerTypeAsQStr(PLAYERS_TYPES PT)
{
    QString QStrType;
    switch(PT)
    {
    case PT_NONE: QStrType = ""; break;
    case PT_WHITE: QStrType = "White"; break;
    case PT_BLACK: QStrType = "Black"; break;
    default:
        qDebug() << "ERROR: playerTypeAsQStr: unknwown parameter:" << QString::number(PT);
        break;
    }
    return QStrType;
}

inline PLAYERS_TYPES playerTypeFromQStr(QString QStrPlayer)
{
    PLAYERS_TYPES playerType = PT_NONE;
    if (QStrPlayer == "White") playerType = PT_WHITE;
    else if (QStrPlayer == "Black") playerType = PT_BLACK;
    else qDebug() << "ERROR: playerTypeFromQStr: unknwown parameter:" << QStrPlayer;
    return playerType;
}

#endif // PLAYERS_TYPES_H
