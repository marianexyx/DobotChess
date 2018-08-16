#ifndef CHESS_MOVEMENTS_H
#define CHESS_MOVEMENTS_H

#pragma once
#include "chess/vars/sequence_types.h"
#include "piece_controller.h"

class ChessMovements
{
    friend class Chess;

private:
    Chessboard* m_pBoardMain;
    Chessboard* m_pBoardRemoved;
    PieceController* m_pPieceController;

    ChessMovements(PieceController* pPieceController, Chessboard* pBoardMain,
                   Chessboard* pBoardRemoved);

    void regularMoveSequence(Field* pFrom, Field* pTo);
    void removeMoveSequence(Field* pFieldWithPieceToRemove);
    void restoreMoveSequence(Piece* pPieceToRestore);
    void castlingMoveSequence(Field* pFrom, Field* pTo);
    Field* findRookFieldInCastling(Field* pField, VERTICAL_MOVE MoveStage);
    void enpassantMoveSequence(Field* pFrom, Field* pTo);
    void promoteMoveSequence(Field* pFrom, Field* pTo);
    void promoteWithRemoveMoveSequence(Field* pFrom, Field* pTo);

    bool isPieceSetOnStartFields();
    void copyPiecesToBoard(const Chessboard &source, Chessboard& target);
    void tryToPutPieceOnItsStartingField(short sFieldNr);
    bool isFieldMissingItsStartPiece(Field* pField);
    void findAndReturnPieceToItsStartingField(Field* pField);
    void tryToMovePieceFromHisNotStartingField(Piece* pPiece, Field* pField);
    bool isPieceSetOnBoardsIdentical(Chessboard& board1, Chessboard& board2);

public:
    void doMoveSequence(PosFromTo PosMove, SEQUENCE_TYPE Type);
    bool resetPiecePositions();
};

#endif // CHESS_MOVEMENTS_H
