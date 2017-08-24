#ifndef PLAYERS_TYPES_H
#define PLAYERS_TYPES_H

#include <QString>

enum PLAYERS_TYPES
{
    PT_NONE,
    PT_WHITE,
    PT_BLACK
};

//TODO: includować to wszędzie gdzie używam zmiennych biały/czarny czy white/black

inline QString playerTypeAsQStr(PLAYERS_TYPES type)
{
    QString str;
    switch(type)
    {
    case PT_NONE:
        str = "";
        break;
    case PT_WHITE:
        str = "WHITE";
        break;
    case PT_BLACK:
        str = "BLACK";
        break;
    default:
        qDebug() << "ERROR: playerTypeAsQStr: unknwown parameter:" << type;
        str = "";
        break;
    }
    return str;
}

#endif // PLAYERS_TYPES_H
