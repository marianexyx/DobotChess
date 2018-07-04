#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum ACTION_TYPE
{
    AT_NONE,
    AT_NEW_GAME_STARTED,
    AT_MOVE_OK,
    AT_BAD_MOVE,
    AT_PROMOTE_TO_WHAT,
    AT_RESET_COMPLITED,
    AT_END_GAME,
    AT_ERROR
};

//" cont"

inline QString actionTypeAsQstr(ACTION_TYPE AT)
{
    switch(AT)
    {
    case AT_NONE: return "none";
    case AT_NEW_GAME_STARTED: return "newOk";
    case AT_MOVE_OK: return "moveOk";
    case AT_BAD_MOVE: return "badMove";
    case AT_PROMOTE_TO_WHAT: return "promoteToWhat";
    case AT_RESET_COMPLITED: return "resetComplited";
    case AT_END_GAME: return "endGame";
    default:
        qDebug() << "ERROR: actionTypeAsQstr(): wrong arg =" << AT;
        return "";
    }
}

#endif // ACTION_TYPES_H
