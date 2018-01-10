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

    SEQUENCE_TYPE _MoveType; //todo: raczej lepiej by to było w statusie
    PosFromTo _PosMove;

public:
    ChessMovements(Chess* pChess);
    ~ChessMovements();

    PosOnBoard findKingPosInCastling(PosOnBoard FieldDest);
    PosOnBoard findRookPosInCastling(PosOnBoard FieldDest);
    void goToSafeRemovedFieldIfNeeded();
    SEQUENCE_TYPE findMoveType(QString move);

    //todo: wszystkie ruchy powinny byc jako friend dla chess
    void regularMoveSequence();
    void removeMoveSequence();
    void restoreMoveSequence(short sPieceToRestore);
    void castlingMoveSequence();
    void enpassantMoveSequence();
    void promoteMoveSequence();

    void setMove(QString QStrMove) { _PosMove = PosFromTo::fromQStr(QStrMove); }
    void setMoveType(SEQUENCE_TYPE Type) { _MoveType = Type; }
    PosFromTo getMove() const { return _PosMove; }
    SEQUENCE_TYPE getMoveType() const { return _MoveType; }
    void clearMove() { _PosMove.clear(); } //todo: sprawdzić gdzie to implementować w całym kodzie
};

#endif // CHESS_MOVEMENTS_H
