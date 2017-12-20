#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once

#include <QString>
#include "chess.h"
#include "chessboard.h"
#include "vars/sequence_types.h"
#include "vars/board_axis.h"
#include "vars/posotion_on_board.h"
#include "vars/board_types.h"
#include "vars/dobot_moves.h"

//todo: sprawdzic klasę z założeniami w "przebudowa podobiektow w klasie chess.txt"
class ChessMovements
{
private:
    Chessboard2* _pChessboard2Main;
    Chessboard2* _pChessboard2Removed;

    SEQUENCE_TYPE _moveType;
    PosFromTo _PosMove;

public:
    ChessMovements(Chessboard2* pChessboard2Main, Chessboard2* pChessboard2Removed);
    ~ChessMovements();

    PosOnBoard findKingPosInCastling(PosOnBoard FieldDest);
    PosOnBoard findRookPosInCastling(PosOnBoard FieldDest);

    //todo: wszystkie ruchy powinny byc jako private/friend
    void regularMoveSequence(Chess* pChess);
    void removeMoveSequence(Chess* pChess);
    void restoreMoveSequence(Chess* pChess, short sPieceToRestore);
    void castlingMoveSequence(Chess *pChess);
    void enpassantMoveSequence(Chess* pChess);
    void promoteMoveSequence(Chess* pChess);

    void goToSafeRemovedFieldIfNeeded(Chess *pChess);
    //void BadMove(QString msg);
    void promoteToWhat(QString QStrMoveForFuturePromote);

    void setMove(QString QStrMove) { _PosMove = PosFromTo::fromQStr(QStrMove); }
    PosFromTo getMove() const { return _PosMove; }
    void clearMove(); //todo: na koniec sprawdzić gdzie to implementować
};

#endif // CHESS_MOVEMENTS_H
