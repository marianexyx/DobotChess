#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#pragma once
#include <QString>

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
    QString QStrGameStatus;

    switch(GS)
    {
    case GS_TURN_NONE_WAITING_FOR_PLAYERS: QStrGameStatus = "noTurn_waitingForPlayers"; break;
    case GS_TURN_NONE_WAITING_FOR_START_CONFIRMS:
        QStrGameStatus = "noTurn_waitingStartConfirms"; break;
    case GS_TURN_NONE_RESETING: QStrGameStatus = "noTurn_reseting"; break;
    case GS_TURN_WHITE: QStrGameStatus = "noTurn_white"; break;
    case GS_TURN_WHITE_FIRST_TURN: QStrGameStatus = "noTurn_white_firtsTurn"; break;
    case GS_TURN_WHITE_PROMOTE: QStrGameStatus = "noTurn_white_promote"; break;
    case GS_TURN_BLACK: QStrGameStatus = "noTurn_black"; break;
    case GS_TURN_BLACK_PROMOTE: QStrGameStatus = "noTurn_black_promote"; break;
    case GS_ERROR:
    default: qDebug() << "ERROR: QString gameStatusAsQStr(): wrong param:" << GS;
    }

    return QStrGameStatus;
}

#endif // GAME_STATUS_H
