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

    void handleMove(QString move); //?
    void regularMoveSequence(); //todo: napisac
    void removeMoveSequence(); //todo: napisac
    void restoreMoveSequence(); //todo: napisac
    void castlingMoveSequence();
    void enpassantMoveSequence();

    void goToSafeRemovedField(DIGIT digitTo, SEQUENCE_TYPE sequence); //?
    //void BadMove(QString msg);
    void PromoteToWhat(QString moveForFuturePromote);


};

#endif // CHESS_MOVEMENTS_H
