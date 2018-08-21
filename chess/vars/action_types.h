#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#pragma once
#include <QString>
#include <QDebug>
#include "chess/vars/request_types.h"

enum ACTION_TYPE
{
    AT_NONE,
    AT_NEW_WHITE_PLAYER,
    AT_NEW_BLACK_PLAYER,
    AT_NEW_GAME_STARTED,
    AT_BAD_MOVE,
    AT_RESET_COMPLITED,
    AT_DOUBLE_LOGIN,
    AT_REMOVE_AND_REFRESH_CLIENT,
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
        qCritical() << "wrong arg =" << QString::number(AT);
        return "";
    }
}

inline ACTION_TYPE rejectedRequestAsActionType(REJECTED_REQUEST_REACTION RRR)
{
    switch(RRR)
    {
    case RRR_NONE:
    case RRR_RESEND_TABLE_DATA:
        return AT_NONE;
    case RRR_DOUBLE_LOGIN:
        return AT_DOUBLE_LOGIN;
    case RRR_REMOVE_AND_REFRESH_CLIENT:
        return AT_REMOVE_AND_REFRESH_CLIENT;
    default:
        qCritical() << "wrong arg =" << QString::number(RRR);
        return AT_ERROR;
    }
}

#endif // ACTION_TYPES_H
