#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once
#include "chess.h"

class ChessMovements
{
    friend class Chess;

private:
    Chess* _pChess;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;

    SEQUENCE_TYPE _MoveType;
    PosFromTo _PosMove;

    void setMoveType(SEQUENCE_TYPE Type) { _MoveType = Type; }

//public:
    ChessMovements(Chess* pChess);
    ~ChessMovements();

    Field* findKingFieldInCastling(Field* pField);
    Field* findRookFieldInCastling(Field* pField);
    void goToSafeRemovedFieldIfNeeded(Field* pFieldDest);
    SEQUENCE_TYPE findMoveType(QString QStrMove);

    void doMoveSequence();
    void regularMoveSequence(Field* pFrom, Field* pTo);
    void removeMoveSequence(Field* pFieldWithPieceToRemove);
    void restoreMoveSequence(Piece* pPieceToRestore);
    void castlingMoveSequence(Field* pFrom, Field* pTo);
    void enpassantMoveSequence(Field* pFrom, Field* pTo);
    void promoteMoveSequence(Field* pFrom, Field* pTo);

    bool isMoveSet();
    void setMove(QString QStrMove);
    PosFromTo getMove() const { return _PosMove; }
    SEQUENCE_TYPE getMoveType() const { return _MoveType; }
    void clearMove() { _PosMove.clear(); }
};

#endif // CHESS_MOVEMENTS_H
