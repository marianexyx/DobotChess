#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once

#include "chess.h"

class ChessMovements
{
private:
    Chess* _pChess;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;

    SEQUENCE_TYPE _MoveType;
    PosFromTo _PosMove; //todo: gdzieś po wszystkim ...
    //...tą zmienną trzeba prewencyjnie zerować. gdzie?

public:
    ChessMovements(Chess* pChess);
    ~ChessMovements();

    Field *findKingFieldInCastling(Field *pField);
    Field *findRookFieldInCastling(Field *pField);
    void goToSafeRemovedFieldIfNeeded(Field *pFieldDest);
    SEQUENCE_TYPE findMoveType(QString move);

    void doMoveSequence(SEQUENCE_TYPE MoveType, PosFromTo Move);
    void regularMoveSequence(Field *pFrom, Field *pTo);
    void removeMoveSequence(Field* pFieldWithPieceToRemove);
    void restoreMoveSequence(Piece* pPieceToRestore);
    void castlingMoveSequence(Field *pFrom, Field *pTo);
    void enpassantMoveSequence(Field *pFrom, Field *pTo);
    void promoteMoveSequence(Field *pFrom, Field *pTo);

    void setMove(QString QStrMove) { _PosMove = PosFromTo::fromQStr(QStrMove); }
    void setMoveType(SEQUENCE_TYPE Type) { _MoveType = Type; }
    PosFromTo getMove() const { return _PosMove; }
    SEQUENCE_TYPE getMoveType() const { return _MoveType; }
    void clearMove() { _PosMove.clear(); } //todo: sprawdzić gdzie to implementować w całym kodzie
};

#endif // CHESS_MOVEMENTS_H
