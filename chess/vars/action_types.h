#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum ACTION_TYPE
{
    AT_NONE,
    AT_NEW_WHITE_PLAYER,
    AT_NEW_BLACK_PLAYER,
    AT_NEW_GAME_STARTED,
    AT_BAD_MOVE,
    AT_RESET_COMPLITED,
    AT_END_GAME, //must be last, cuz END_TYPE is added, when this type is sent to WWW
    AT_ERROR = 99 //big number prevent this type from covering diffrent AT_END_GAME
};

inline QString actionTypeAsQstr(ACTION_TYPE AT)
{
    switch(AT)
    {
    case AT_NONE: return "none";
    case AT_NEW_GAME_STARTED: return "newOk";
    case AT_BAD_MOVE: return "badMove";
    case AT_RESET_COMPLITED: return "resetComplited";
    case AT_END_GAME: return "endGame";
    default:
        qDebug() << "ERROR: actionTypeAsQstr(): wrong arg =" << AT;
        return "";
    }
}

#endif // ACTION_TYPES_H
