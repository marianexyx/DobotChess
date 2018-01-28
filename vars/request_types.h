#ifndef REQUEST_TYPES_H
#define REQUEST_TYPES_H

#pragma once
#include <QString>

//todo: folder chess?
enum REQUEST_TYPE { RT_NONE,
                    RT_NEW_GAME,
                    RT_MOVE,
                    RT_GET_TABLE_DATA,
                    RT_GIVE_UP,
                    RT_SIT_ON,
                    RT_STAND_UP,
                    RT_IM,
                    RT_PROMOTE_TO,
                    RT_QUEUE_ME,
                    RT_LEAVE_QUEUE
                  };

inline REQUEST_TYPE requestType(QString QStrRequest, bool bErrorLog = false)
{
    if (QStrRequest == "newGame") _request.request = RT_NEW_GAME;
    else if (QStrRequest.left(4) == "move") _request.request = RT_MOVE;
    else if (QStrRequest == "getTableDataAsJSON") _request.request = RT_GET_TABLE_DATA;
    else if (QStrRequest == "giveUp") _request.request = RT_GIVE_UP;
    else if (QStrRequest.left(5) == "sitOn") _request.request = RT_SIT_ON;
    else if (QStrRequest.left(7) == "standUp") _request.request = RT_STAND_UP;
    else if (QStrRequest.left(2) == "im") _request.request = RT_IM;
    else if (QStrRequest.left(9) == "promoteTo") _request.request = RT_PROMOTE_TO;
    else if (QStrRequest == "queueMe") _request.request = RT_QUEUE_ME;
    else if (QStrRequest == "leaveQueue") _request.request = RT_LEAVE_QUEUE;
    else
    {
        if (bErrorLog)
            qDebug() << "ERROR: requestType(): not a valid type. QStrRequest =" << QStrRequest;

        return RT_NONE;
    }
}

inline QString requestTypeAsQStr(REQUEST_TYPE RT)
{
    QString QStrRequest;
    switch(RT)
    {
    case RT_NEW_GAME: QStrRequest = "newGame"; break;
    case RT_MOVE: QStrRequest = "move"; break;
    case RT_GET_TABLE_DATA: QStrRequest = "getTableDataAsJSON"; break;
    case RT_GIVE_UP: QStrRequest = "giveUp"; break;
    case RT_SIT_ON: QStrRequest = "sitOn"; break;
    case RT_STAND_UP: QStrRequest = "standUp"; break;
    case RT_IM: QStrRequest = "im"; break;
    case RT_PROMOTE_TO: QStrRequest = "promoteTo"; break;
    case RT_QUEUE_ME: QStrRequest = "queueMe"; break;
    case RT_LEAVE_QUEUE: QStrRequest = "leaveQueue"; break;
    default:
        qDebug() << "ERROR: requestTypeAsQStr(): wrong arg =" << RT;
        break;
    }

    return QStrRequest;
}

#endif // REQUEST_TYPES_H
