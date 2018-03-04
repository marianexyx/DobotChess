#ifndef CHESS_RESETS_H
#define CHESS_RESETS_H

#pragma once
#include "chess/vars/end_of_game_types.h"
#include "client.h"
#include "chessboard.h"

class ChessResets
{
    friend class Chess;

private:
    Clients* _pClientsList;
    Chessboard* _pBoardCoreMain;

    bool isPieceSetOnStartFields(); //todo: do chessboardu?
    QString getEndGameMsg(END_TYPE WhoWon, QString QStrTableData, PosFromTo* pMove = nullptr,
                          Client* pPlayerToClear = nullptr);
    void changePlayersOnChairs(END_TYPE WhoWon, Client* pPlayerToClear);
    bool isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2);

public:
    ChessResets(Clients* pClientsList, Chessboard* pBoardCoreMain);
    void resetPiecePositions();

};

#endif // CHESS_RESETS_H
