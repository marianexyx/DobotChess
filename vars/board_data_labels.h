#ifndef BOARD_DATA_LABELS_H
#define BOARD_DATA_LABELS_H

#pragma once
#include <QString>

enum BOARD_DATA_LABELS
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

inline QString boardDataLabelsAsQstr(BOARD_DATA_LABELS BDL)
{
    QString QStrBoardLbl;
    switch(BDL)
    {
    case BDL_SOCKETS_ONLINE: QStrBoardLbl = "socketsOnline" ; break;
    case BDL_WHITE_NAME: QStrBoardLbl = "whiteName" ; break;
    case BDL_BLACK_NAME: QStrBoardLbl = "blackName" ; break;
    case BDL_TURN: QStrBoardLbl = "turn" ; break;
    case BDL_GAME_STATUS: QStrBoardLbl = "gameStatus" ; break;
    case BDL_MOVES: QStrBoardLbl = "moves" ; break;
    case BDL_CASTLINGS: QStrBoardLbl = "castlings" ; break;
    case BDL_ENPASSANT: QStrBoardLbl = "enapssant" ; break;
    case BDL_WHITE_TIME: QStrBoardLbl = "whiteTime" ; break;
    case BDL_BLACK_TIME: QStrBoardLbl = "blackTime" ; break;
    case BDL_QUEUE_PLAYERS: QStrBoardLbl = "queuePlayers" ; break;
    case BDL_QUEUE_TIME: QStrBoardLbl = "queueTime" ; break;
    default:
        QStrBoardLbl = "ERROR: wrong boardDataLabelsAsQstr arg = " + BDL;
        qDebug() << QStrBoardLbl;
        break;
    }

    return QStrBoardLbl;
}

#endif // BOARD_DATA_LABELS_H
