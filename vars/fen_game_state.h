#ifndef FEN_GAME_STATE_H
#define FEN_GAME_STATE_H

#pragma once
#include "qstring.h"

enum FEN_GAME_STATE
{
    FGS_NONE,
    FGS_IN_PROGRESS,
    FGS_WHITE_WON,
    FGS_BLACK_WON,
    FGS_DRAW
};

inline FEN_GAME_STATE FENGameState(QString QStrFENGameState)
{
    if (QStrFENGameState == "*") return FGS_IN_PROGRESS;
    else if (QStrFENGameState == "1-0") return FGS_WHITE_WON;
    else if (QStrFENGameState == "0-1") return FGS_BLACK_WON;
    else if (QStrFENGameState == "1/2-1/2") return FGS_DRAW;
    else
    {
        qDebug() << "ERROR: FENGameState: unknown par:" << QStrFENGameState;
        return FGS_NONE;
    }
}

#endif // FEN_GAME_STATE_H
