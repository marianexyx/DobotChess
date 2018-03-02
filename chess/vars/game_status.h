#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#pragma once
#include <QString>
#include "turn_types.h"

enum GAME_STATUS
{
    GS_ERROR,
    GS_TURN_NONE_WAITING_FOR_PLAYERS,
    GS_TURN_NONE_WAITING_FOR_START_CONFIRMS,
    GS_TURN_NONE_RESETING,
    GS_TURN_WHITE,
    GS_TURN_WHITE_FIRST_TURN,
    GS_TURN_WHITE_PROMOTE,
    GS_TURN_BLACK,
    GS_TURN_BLACK_PROMOTE
};

inline QString gameStatusAsQStr(GAME_STATUS GS)
{
    switch(GS)
    {
    case GS_TURN_NONE_WAITING_FOR_PLAYERS: return "noTurn_waitingForPlayers";
    case GS_TURN_NONE_WAITING_FOR_START_CONFIRMS: return "noTurn_waitingStartConfirms";
    case GS_TURN_NONE_RESETING: return "noTurn_reseting";
    case GS_TURN_WHITE: return "noTurn_white";
    case GS_TURN_WHITE_FIRST_TURN: return "noTurn_white_firtsTurn";
    case GS_TURN_WHITE_PROMOTE: return "noTurn_white_promote";
    case GS_TURN_BLACK: return "noTurn_black";
    case GS_TURN_BLACK_PROMOTE: return "noTurn_black_promote";
    case GS_ERROR:
    default:
        qDebug() << "ERROR: gameStatusAsQStr(): wrong param:" << GS;
        return "";
    }
}

inline WHOSE_TURN whoseTurnFromGameStatus(GAME_STATUS GS)
{
    switch(GS)
    {
    case GS_ERROR: return NO_TURN;
    case GS_TURN_NONE_WAITING_FOR_PLAYERS: return NO_TURN;
    case GS_TURN_NONE_WAITING_FOR_START_CONFIRMS: return NO_TURN;
    case GS_TURN_NONE_RESETING: return NO_TURN;
    case GS_TURN_WHITE: return WHITE_TURN;
    case GS_TURN_WHITE_FIRST_TURN: return WHITE_TURN;
    case GS_TURN_WHITE_PROMOTE: return WHITE_TURN;
    case GS_TURN_BLACK: return BLACK_TURN;
    case GS_TURN_BLACK_PROMOTE: return BLACK_TURN;
    default:
        qDebug() << "ERROR: whoseTurnFromGameStatus(): unknwon GAME_STATUS:" << GS;
        return NO_TURN;
    }
}

#endif // GAME_STATUS_H
