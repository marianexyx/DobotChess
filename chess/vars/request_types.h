#ifndef REQUEST_TYPES_H
#define REQUEST_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum REQUEST_TYPE
{
    RT_NONE,
    RT_GET_TABLE_DATA,
    RT_IM,
    RT_SIT_ON,
    RT_NEW_GAME,
    RT_MOVE,
    RT_PROMOTE_TO,
    RT_STAND_UP,
    RT_QUEUE_ME,
    RT_LEAVE_QUEUE,
    RT_CLIENT_LEFT,
    RT_LOGOUT
};

QString requestTypeAsQStr(REQUEST_TYPE RT);

struct clientRequest
{
    static uint64_t counter;
    uint64_t clientID;
    REQUEST_TYPE type;
    QString param;
    bool service;

    clientRequest();
    clientRequest(uint64_t i, REQUEST_TYPE R, QString p, bool s = false);
    void clear();
    QString dumpAllData();
};

inline REQUEST_TYPE requestTypeFromQStr(QString QStrRequest, bool bErrorLog = false)
{   

    if (QStrRequest == "getTableData") return RT_GET_TABLE_DATA;
    else if (QStrRequest.left(2) == "im") return RT_IM;
    else if (QStrRequest.left(5) == "sitOn") return RT_SIT_ON;
    else if (QStrRequest == "newGame") return RT_NEW_GAME;
    else if (QStrRequest.left(4) == "move") return RT_MOVE;
    else if (QStrRequest.left(9) == "promoteTo") return RT_PROMOTE_TO;
    else if (QStrRequest.left(7) == "standUp") return RT_STAND_UP;
    else if (QStrRequest == "queueMe") return RT_QUEUE_ME;
    else if (QStrRequest == "leaveQueue") return RT_LEAVE_QUEUE;
    else if (QStrRequest == "clientLeft") return RT_CLIENT_LEFT;
    else if (QStrRequest == "logout") return RT_LOGOUT;
    else
    {
        if (bErrorLog)
            qWarning() << "not a valid type. QStrRequest =" << QStrRequest;

        return RT_NONE;
    }
}

inline QString requestTypeAsQStr(REQUEST_TYPE RT)
{
    switch(RT)
    {
    case RT_NONE: return "none";
    case RT_GET_TABLE_DATA: return "getTableData";
    case RT_IM: return "im";
    case RT_SIT_ON: return "sitOn";
    case RT_NEW_GAME: return "newGame";
    case RT_MOVE: return "move";
    case RT_PROMOTE_TO: return "promoteTo";
    case RT_STAND_UP: return"standUp";
    case RT_QUEUE_ME: return "queueMe";
    case RT_LEAVE_QUEUE: return "leaveQueue";
    case RT_CLIENT_LEFT: return "clientLeft";
    case RT_LOGOUT: return "logout";
    default:
        qCritical() << "wrong arg =" << QString::number(RT);
        return "";
    }
}

enum REJECTED_REQUEST_REACTION
{
    RRR_NONE,
    RRR_RESEND_TABLE_DATA,
    RRR_DOUBLE_LOGIN,
    RRR_REMOVE_AND_REFRESH_CLIENT
};

inline QString rejectedRequestReactionAsQStr(REJECTED_REQUEST_REACTION RRR)
{
    switch(RRR)
    {
    case RRR_NONE: return "none";
    case RRR_RESEND_TABLE_DATA: return "resendTableData";
    case RRR_DOUBLE_LOGIN: return "doubleLogin";
    case RRR_REMOVE_AND_REFRESH_CLIENT: return "removeAndRefreshClient";
    default:
        qCritical() << "wrong arg =" << QString::number(RRR);
        return "";
    }
}

#endif // REQUEST_TYPES_H
