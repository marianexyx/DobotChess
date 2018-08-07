#ifndef BOARD_DATA_LABELS_H
#define BOARD_DATA_LABELS_H

#pragma once
#include <QString>

enum BOARD_DATA_LABEL
{
    BDL_SOCKETS_ONLINE,
    BDL_WHITE_NAME,
    BDL_BLACK_NAME,
    BDL_TURN,
    BDL_GAME_STATUS,
    BDL_MOVES,
    BDL_CASTLINGS,
    BDL_ENPASSANT,
    BDL_WHITE_TIME,
    BDL_BLACK_TIME,
    BDL_QUEUE_PLAYERS,
    BDL_QUEUE_TIME
};

inline QString boardDataLabelsAsQstr(BOARD_DATA_LABEL BDL)
{
    switch(BDL)
    {
    case BDL_SOCKETS_ONLINE: return "socketsOnline";
    case BDL_WHITE_NAME: return "whiteName";
    case BDL_BLACK_NAME: return "blackName";
    case BDL_TURN: return "turn";
    case BDL_GAME_STATUS: return "gameStatus";
    case BDL_MOVES: return "moves";
    case BDL_CASTLINGS: return "castlings";
    case BDL_ENPASSANT: return "enapssant";
    case BDL_WHITE_TIME: return "whiteTime";
    case BDL_BLACK_TIME: return "blackTime";
    case BDL_QUEUE_PLAYERS: return "queuePlayers";
    case BDL_QUEUE_TIME: return "queueTime";
    default:
        qCritical() << "wrong arg =" << BDL;
        break;
    }
}

#endif // BOARD_DATA_LABELS_H
