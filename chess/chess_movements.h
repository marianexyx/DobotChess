#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once
#include "chess/vars/sequence_types.h"
#include "piece_controller.h"

class ChessMovements
{
    friend class Chess;

private:
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    PieceController* _pPieceController;

    void copyPiecesToBoard(const Chessboard &source, Chessboard& target);
    bool isPieceSetOnStartFields();
    bool isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2);
    bool isFieldMissingItsStartPiece(Field* pField);
    void findAndReturnPieceToItsStartingField(Field* pField);
    void tryToMovePieceFromHisNotStartingField(Piece* pPiece, Field* pField);
    void tryToPutPieceOnItsStartingField(short sFieldNr);

public:
    ChessMovements(PieceController* pPieceController, Chessboard* pBoardMain,
                   Chessboard* pBoardRemoved);
    ~ChessMovements() {}

    Field* findRookFieldInCastling(Field* pField, VERTICAL_MOVE MoveStage);
    void doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type);
    void regularMoveSequence(Field* pFrom, Field* pTo);
    void removeMoveSequence(Field* pFieldWithPieceToRemove);
    void restoreMoveSequence(Piece* pPieceToRestore);
    void castlingMoveSequence(Field* pFrom, Field* pTo);
    void enpassantMoveSequence(Field* pFrom, Field* pTo);
    void promoteMoveSequence(Field* pFrom, Field* pTo);
    void promoteWithRemoveMoveSequence(Field* pFrom, Field* pTo);
    bool resetPiecePositions();
};

#endif // CHESS_MOVEMENTS_H
