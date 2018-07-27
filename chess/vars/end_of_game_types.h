#ifndef END_OF_GAME_TYPES_H
#define END_OF_GAME_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum END_TYPE
{
    ET_NONE,
    ET_NORMAL_WIN_WHITE,
    ET_NORMAL_WIN_BLACK,
    ET_DRAW,
    ET_GIVE_UP_WHITE,
    ET_GIVE_UP_BLACK,
    ET_SOCKET_LOST_WHITE,
    ET_SOCKET_LOST_BLACK,
    ET_TIMEOUT_GAME_WHITE,
    ET_TIMEOUT_GAME_BLACK,
    ET_ERROR
};

inline QString endTypeAsQstr(END_TYPE ET)
{
    switch(ET)
    {
    case ET_NORMAL_WIN_WHITE: return "normalWinWhite";
    case ET_NORMAL_WIN_BLACK: return "normalWinBlack";
    case ET_DRAW: return "draw";
    case ET_GIVE_UP_WHITE: return "giveUpWhite";
    case ET_GIVE_UP_BLACK: return "giveUpBlack";
    case ET_SOCKET_LOST_WHITE: return "socketLostWhite";
    case ET_SOCKET_LOST_BLACK: return "socketLostBlack";
    case ET_TIMEOUT_GAME_WHITE: return "timeOutWhite";
    case ET_TIMEOUT_GAME_BLACK: return "timeOutBlack";
    default:
        qDebug() << "ERROR: endTypeAsQstr(): wrong arg =" << ET;
        return "";
    }
}

inline END_TYPE FENGameState(QString QStrFENGameState)
{
    if (QStrFENGameState == "*") return ET_NONE;
    else if (QStrFENGameState == "1-0") return ET_NORMAL_WIN_WHITE;
    else if (QStrFENGameState == "0-1") return ET_NORMAL_WIN_BLACK;
    else if (QStrFENGameState == "1/2-1/2") return ET_DRAW;
    else
    {
        qDebug() << "ERROR: FENGameState(): unknown val:" << QStrFENGameState;
        return ET_NONE;
    }
}

#endif // END_OF_GAME_TYPES_H
