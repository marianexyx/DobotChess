#ifndef CHESS_RESETS_H
#define CHESS_RESETS_H

#pragma once
#include "chess.h"

class ChessResets
{
private:
    Chess* _pChess;
    Clients* _pClients;
    ChessTimers* _pTimers;
    ChessStatus* _pStatus;

    bool isPieceSetOnStartFields();
    short searchForMissingPieceActualFieldOnMainBoad(Piece* piece);

public:
    ChessResets(Chess* pChess);

    void restartGame(END_TYPE WhoWon, Client* PlayerToClear = nullptr);
    void changePlayersOnChairs(END_TYPE WhoWon, Client* PlayerToClear); //todo: private?
    void sendEndGameMsgToAllClients(END_TYPE WhoWon); //todo: private?
    void resetPiecePositions();

};

#endif // CHESS_RESETS_H
