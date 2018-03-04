#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once
#include "chess/vars/sequence_types.h"
#include "chessboard.h"
#include "piece_controller.h"

class ChessMovements
{
    friend class Chess;

private:
    Chessboard* _pBoardCoreMain;
    Chessboard* _pBoardCoreRemoved;
    PieceController* _pPieceController;

public:
    ChessMovements(PieceController* pPieceController, Chessboard* pBoardCoreMain,
                   Chessboard* pBoardCoreRemoved);
    ~ChessMovements();

    Field* findKingFieldInCastling(Field* pField);
    Field* findRookFieldInCastling(Field* pField);
    void goToSafeRemovedFieldIfNeeded(Field* pFieldDest);
    void doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type);
    void regularMoveSequence(Field* pFrom, Field* pTo);
    void removeMoveSequence(Field* pFieldWithPieceToRemove);
    void restoreMoveSequence(Piece* pPieceToRestore);
    void castlingMoveSequence(Field* pFrom, Field* pTo);
    void enpassantMoveSequence(Field* pFrom, Field* pTo);
    void promoteMoveSequence(Field* pFrom, Field* pTo);
};

#endif // CHESS_MOVEMENTS_H
