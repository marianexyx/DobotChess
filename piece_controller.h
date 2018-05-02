#ifndef PIECE_CONTROLLER_H
#define PIECE_CONTROLLER_H

#pragma once
#include "vars/log.h"
#include "dobot.h"
#include "piece_set.h"
#include "chessboard.h"

class PieceController: public QObject
{
    Q_OBJECT

private:
    Dobot* _pDobot;
    PieceSet* _pPieceSet;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    PosOnBoard _lastPos;

public:
    PieceController(Dobot* pDobot, Chessboard* pBoardMain, Chessboard* pBoardRemoved);

    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE VertMove = VM_NONE);
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece, bool bErrorLog = false);
    bool isMoveSet();
    Field* searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece);

    void clearLastPos() { _lastPos.Letter = L_X; _lastPos.Digit = D_X; }
    PosOnBoard getLastPos() const { return _lastPos; }
    Piece* getPiece(short sPieceNr) const { return _pPieceSet->getPiece(sPieceNr); }

signals:
    void addTextToLogPTE(QString, LOG);
};

#endif // PIECE_CONTROLLER_H
