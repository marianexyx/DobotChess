#ifndef END_OF_GAME_TYPES_H
#define END_OF_GAME_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum END_TYPE
{
    ET_NONE,
    ET_WHIE_WON,
    ET_BLACK_WON,
    ET_DRAW,
    ET_GIVE_UP,
    ET_SOCKET_LOST,
    ET_TIMEOUT_GAME
};

inline QString endTypeAsQstr(END_TYPE ET)
{
    //future: change strings for endian node for cumminicating with site
    switch(ET)
    {
    case ET_WHIE_WON: return "wWon";
    case ET_BLACK_WON: return "bWon";
    case ET_DRAW: return "draw";
    case ET_GIVE_UP: return "giveUp";
    case ET_SOCKET_LOST: return "socketLost";
    case ET_TIMEOUT_GAME: return "timeOut";
    default:
        qDebug() << "ERROR: endTypeAsQstr(): wrong arg =" << ET;
        return "";
    }
}

inline END_TYPE FENGameState(QString QStrFENGameState)
{
    if (QStrFENGameState == "*") return ET_NONE;
    else if (QStrFENGameState == "1-0") return ET_WHIE_WON;
    else if (QStrFENGameState == "0-1") return ET_BLACK_WON;
    else if (QStrFENGameState == "1/2-1/2") return ET_DRAW;
    else
    {
        qDebug() << "ERROR: FENGameState(): unknown val:" << QStrFENGameState;
        return ET_NONE;
    }
}

#endif // END_OF_GAME_TYPES_H
