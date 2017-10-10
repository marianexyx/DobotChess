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
    case PT_WHITE: QStrType = "WHITE"; break;
    case PT_BLACK: QStrType = "BLACK"; break;
    default:
        qDebug() << "ERROR: playerTypeAsQStr: unknwown parameter:" << PT;
        break;
    }
    return QStrType;
}

#endif // PLAYERS_TYPES_H
