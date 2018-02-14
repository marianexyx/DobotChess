#ifndef CHESS_RESETS_H
#define CHESS_RESETS_H

#pragma once
#include "chess.h"

class Clients;
class ChessStatus;
class ChessTimers;
class ChessMovements;

class ChessResets
{
    friend class Chess;

private:
    Chess* _pChess;
    Clients* _pClientsList;
    ChessTimers* _pTimers;
    ChessStatus* _pStatus;
    ChessMovements* _pMovements;

    bool isPieceSetOnStartFields();
    void sendEndGameMsgToAllClients(END_TYPE WhoWon, Client* pPlayerToClear = nullptr);
    void changePlayersOnChairs(END_TYPE WhoWon, Client* pPlayerToClear);
    bool isPieceSetOnBoardsIdentical(Chessboard* pBoard1, Chessboard* pBoard2);

public:
    ChessResets(Chess* pChess);

    void restartGame(END_TYPE WhoWon, Client* pPlayerToClear = nullptr);
    void resetPiecePositions();
};

#endif // CHESS_RESETS_H
