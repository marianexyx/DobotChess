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

    bool isPieceSetOnStartFields();
    void sendEndGameMsgToAllClients(END_TYPE WhoWon, Client* pPlayerToClear = nullptr);
    void changePlayersOnChairs(END_TYPE WhoWon, Client* pPlayerToClear);
    bool isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2);

public:
    ChessResets(Clients* pClientsList);

    void restartGame(END_TYPE WhoWon, Client* pPlayerToClear = nullptr);
    void resetPiecePositions();
};

#endif // CHESS_RESETS_H
