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

class ChessMovements
{
private:
    Chessboard2* _pChessboard2Main;
    Chessboard2* _pChessboard2Removed;

    QString m_QStrCastlings;
    QString m_QStrEnpassant;
    SEQUENCE_TYPE m_moveType;

public:
    ChessMovements(Chessboard2* pChessboard2Main, Chessboard2* pChessboard2Removed);
    ~ChessMovements();

    bool isMoveRemoving();
    bool isMoveCastling(QString moveToTest);
    bool isMoveEnpassant(QString moveToTest);
    PositionOnBoard findKingToMoveInCastling(PositionOnBoard FieldDest);
    PositionOnBoard findRookToMoveInCastling(PositionOnBoard FieldDest);

    //todo: wszystkie ruchy powinny byc jako private/friend
    void regularMoveSequence(Chess* pChess);
    void removeMoveSequence(Chess* pChess);
    void restoreMoveSequence(Chess* pChess, short sPieceToRestore);
    void castlingMoveSequence(Chess *pChess);
    void enpassantMoveSequence(Chess* pChess);
    void promoteMoveSequence(Chess* pChess);

    void goToSafeRemovedFieldIfNeeded(Chess *pChess);
    //void BadMove(QString msg);
    void promoteToWhat(QString moveForFuturePromote);


};

#endif // CHESS_MOVEMENTS_H
