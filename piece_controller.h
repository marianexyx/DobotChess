#ifndef PIECE_CONTROLLER_H
#define PIECE_CONTROLLER_H

#pragma once
#include "vars/log.h"
#include "dobot.h"
#include "piece_set.h"
#include "chessboard.h"

class PieceController
{
private:
    Dobot* _pDobot;
    PieceSet* _pPieceSet;
    Chessboard* _pBoardCoreMain;
    Chessboard* _pBoardCoreRemoved;

public:
    PieceController(Dobot* pDobot, Chessboard* pBoardCoreMain, Chessboard* pBoardCoreRemoved);

    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE VertMove = VM_NONE);
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece);
    Field* searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece);

    Piece* getPiece(short sPieceNr) const { return _pPieceSet->getPiece(sPieceNr); }
};

#endif // PIECE_CONTROLLER_H
