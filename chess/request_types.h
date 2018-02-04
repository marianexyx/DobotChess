#ifndef REQUEST_TYPES_H
#define REQUEST_TYPES_H

#pragma once
#include <QString>

enum REQUEST_TYPE
{
    RT_NONE,
    RT_NEW_GAME,
    RT_MOVE,
    RT_GET_TABLE_DATA,
    RT_GIVE_UP,
    RT_SIT_ON,
    RT_STAND_UP,
    RT_IM,
    RT_PROMOTE_TO,
    RT_QUEUE_ME,
    RT_LEAVE_QUEUE,
    RT_CLIENT_LEFT
};

inline REQUEST_TYPE requestType(QString QStrRequest, bool bErrorLog = false)
{   
    if (QStrRequest == "newGame") return RT_NEW_GAME;
    else if (QStrRequest.left(4) == "move") return RT_MOVE;
    else if (QStrRequest == "getTableDataAsJSON") return RT_GET_TABLE_DATA;
    else if (QStrRequest == "giveUp") return RT_GIVE_UP;
    else if (QStrRequest.left(5) == "sitOn") return RT_SIT_ON;
    else if (QStrRequest.left(7) == "standUp") return RT_STAND_UP;
    else if (QStrRequest.left(2) == "im") return RT_IM;
    else if (QStrRequest.left(9) == "promoteTo") return RT_PROMOTE_TO;
    else if (QStrRequest == "queueMe") return RT_QUEUE_ME;
    else if (QStrRequest == "leaveQueue") return RT_LEAVE_QUEUE;
    else if (QStrRequest == "clientLeft") return RT_CLIENT_LEFT;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR: requestType(): not a valid type. QStrRequest =" << QStrRequest;

        return RT_NONE;
    }
}

inline QString requestTypeAsQStr(REQUEST_TYPE RT)
{
    switch(RT)
    {
    case RT_NEW_GAME: return "newGame";
    case RT_MOVE: return "move";
    case RT_GET_TABLE_DATA: return "getTableDataAsJSON";
    case RT_GIVE_UP: return "giveUp";
    case RT_SIT_ON: return "sitOn";
    case RT_STAND_UP: return"standUp";
    case RT_IM: return "im";
    case RT_PROMOTE_TO: return "promoteTo";
    case RT_QUEUE_ME: return "queueMe";
    case RT_LEAVE_QUEUE: return "leaveQueue";
    case RT_CLIENT_LEFT: return "clientLeft";
    default:
        qDebug() << "ERROR: requestTypeAsQStr(): wrong arg =" << RT;
        return "";
    }
}

#endif // REQUEST_TYPES_H
