#ifndef PIECE_CONTROLLER_H
#define PIECE_CONTROLLER_H

#pragma once
#include "vars/log.h"
#include "dobot.h"
#include "piece_set.h"
#include "chessboard.h"
#include "ctype.h"

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
    PieceController(Dobot* pDobot, Chessboard *pBoardMain, Chessboard *pBoardRemoved);

    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE VertMove = VM_NONE, bool bRetreat = false);
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece, bool bErrorLog = false);
    bool isMoveSet();
    bool isAnyPawnPromoted();
    Field* searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece);

    void clearLastPos() { _lastPos.Letter = L_X; _lastPos.Digit = D_X; }
    void clearPawnsPromotions();
    PosOnBoard getLastPos() const { return _lastPos; }
    QString getPromotedPawnsPositions();
    Piece* getPiece(short sPieceNr) const { return _pPieceSet->getPiece(sPieceNr); }

    Dobot* getDobotPointer() const { return _pDobot; }
    Chessboard* getBoardMainPointer() const { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() const { return _pBoardRemoved; }

signals:
    void addTextToLogPTE(QString, LOG);
    void showRealBoardInUI();
};

#endif // PIECE_CONTROLLER_H
