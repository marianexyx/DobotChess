#ifndef TABLE_DATA_TYPES_H
#define TABLE_DATA_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum TABLE_DATA_TYPE
{
    TD_NONE,
    TD_ACTION,
    TD_WHITE_PLAYER,
    TD_BLACK_PLAYER,
    TD_GAME_STATE,
    TD_WHITE_TIME,
    TD_BLACK_TYPE,
    TD_QUEUE,
    TD_START_TIME,
    TD_HISTORY,
    TD_PROMOTIONS,
    TD_ERROR
};

inline QString tableDataTypeAsQstr(TABLE_DATA_TYPE TD)
{
    switch(TD)
    {
    case TD_NONE: return "none";
    case TD_ACTION: return "action";
    case TD_WHITE_PLAYER: return "whitePlayer";
    case TD_BLACK_PLAYER: return "blackPlayer";
    case TD_GAME_STATE: return "gameState";
    case TD_WHITE_TIME: return "whiteTime";
    case TD_BLACK_TYPE: return "blackTime";
    case TD_QUEUE: return "queue";
    case TD_START_TIME: return "startTime";
    case TD_HISTORY: return "history";
    case TD_PROMOTIONS: return "promotions";
    default:
        qDebug() << "ERROR: tableDataTypeAsQstr(): wrong arg =" << TD;
        return "";
    }
}

#endif // TABLE_DATA_TYPES_H
