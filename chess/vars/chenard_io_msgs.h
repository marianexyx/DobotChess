#ifndef CHENARD_IO_MSGS_H
#define CHENARD_IO_MSGS_H

#pragma once
#include <QString>

//https://github.com/cosinekitty/chenard/wiki/ChenServer#complete-command-reference

enum CHENARD_MSG_TYPE
{
    CMT_ERROR,
    CMT_NEW,
    CMT_MOVE,
    CMT_STATUS,
    CMT_HISTORY,
    CMT_LEGAL
};

inline CHENARD_MSG_TYPE ChenardMsgType(QString QStrMsgType)
{
    if (QStrMsgType == "new") return CMT_NEW;
    else if (QStrMsgType == "status") return CMT_STATUS;
    else if (QStrMsgType == "legal") return CMT_LEGAL;
    else if (QStrMsgType == "history pgn") return CMT_HISTORY;
    else if (QStrMsgType.left(5) == "move ") return CMT_MOVE;
    else
    {
        qCritical() << "unknown QStrMsgType type:" << QStrMsgType;
        return CMT_ERROR;
    }
}

inline QString chenardMsgTypeAsQStr(CHENARD_MSG_TYPE CMT)
{
    switch(CMT)
    {
    case CMT_NEW: return "new";
    case CMT_STATUS: return "status";
    case CMT_LEGAL: return "legal";
    case CMT_HISTORY: return "history pgn";
    case CMT_MOVE: return "move ";
    default:
        qCritical() << "wrong arg =" << QString::number(CMT);
        return "";
    }
}

inline bool isChenardAnswerCorrect(CHENARD_MSG_TYPE Command, QString QStrAns, bool bErrorLog = false)
{
    bool bReturn;

    switch(Command)
    {
    case CMT_NEW:
    case CMT_HISTORY:
        bReturn = (QStrAns.left(2) == "OK") ? true : false; //"OK" and "OK\n"
        break;
    case CMT_STATUS:
    case CMT_LEGAL:
        bReturn = true; //always ok
        break;
    case CMT_MOVE:
        //BAD_FORMAT answer it this case is an error, because it shouldn't be possible
        bReturn = (QStrAns.left(4) == "OK 1") ? true : false; //"OK 1" and "OK 1\n"
        break;
    default:
        qCritical() << "wrong arg =" << QString::number(Command);
        bReturn = false;
    }

    if (!bReturn && bErrorLog)
        qCritical() << "wrong answer:" << QStrAns << "for"
                    << chenardMsgTypeAsQStr(CMT_NEW) << "command";

    return bReturn;
}

#endif // CHENARD_IO_MSGS_H
