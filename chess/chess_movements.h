#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once

#include <QString>
#include "chess.h"
#include "chessboard.h"
#include "websockets.h" //todo: to i inne dziedziczyć?
#include "vars/sequence_types.h"
#include "vars/board_axis.h"
#include "vars/posotion_on_board.h"
#include "vars/board_types.h"
#include "vars/dobot_moves.h"

//todo: sprawdzic klasę z założeniami w "przebudowa podobiektow w klasie chess.txt"
class ChessMovements
{
private:
    Chess* _pChess;

    Websockets* _pWebsockets;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;

    SEQUENCE_TYPE _MoveType; //todo: raczej lepiej by to było w statusie
    PosFromTo _PosMove;

public:
    ChessMovements(Chess* pChess, Websockets* pWebsockets,
                   Chessboard* pBoardMain, Chessboard* pBoardRemoved);
    ~ChessMovements();

    PosOnBoard findKingPosInCastling(PosOnBoard FieldDest);
    PosOnBoard findRookPosInCastling(PosOnBoard FieldDest);
    void goToSafeRemovedFieldIfNeeded(Chess *pChess);
    SEQUENCE_TYPE findMoveType(QString move);

    //todo: wszystkie ruchy powinny byc jako private/friend
    void regularMoveSequence(Chess* pChess);
    void removeMoveSequence(Chess* pChess);
    void restoreMoveSequence(Chess* pChess, short sPieceToRestore);
    void castlingMoveSequence(Chess *pChess);
    void enpassantMoveSequence(Chess* pChess);
    void promoteMoveSequence(Chess* pChess);

    void setMove(QString QStrMove) { _PosMove = PosFromTo::fromQStr(QStrMove); }
    void setMoveType(SEQUENCE_TYPE Type) { _MoveType = Type; }
    PosFromTo getMove() const { return _PosMove; }
    SEQUENCE_TYPE getMoveType() const { return _MoveType; }
    void clearMove() { _PosMove.clear(); } //todo: sprawdzić gdzie to implementować w całym kodzie
};

#endif // CHESS_MOVEMENTS_H
