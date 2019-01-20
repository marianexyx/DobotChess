#ifndef PIECE_CONTROLLER_H
#define PIECE_CONTROLLER_H

#pragma once
#include "ctype.h"
#include "vars/log.h"
#include "dobot/dobot.h"
#include "piece_set.h"
#include "chessboard/chessboard.h"
#include "vars/vertical_move.h"

//future: I don't control where pieces are in real time- information is lost when...
//...converting to Dobot list of 3D points. Chessboards calculate end position ...
//...immadietely assuming that real moves has been done in no time. Make app knows...
//...the actual position of pieces in real time (lot of work)
class PieceController: public QObject
{
    Q_OBJECT

private:
    Dobot* m_pDobot;
    PieceSet* m_pPieceSet;
    Chessboard* m_pBoardMain;
    Chessboard* m_pBoardRemoved;
    PosOnBoard m_lastPos;
    BOARD m_lastBoardType;

    bool isPieceSetOk();
    bool isAnyPawnPromoted();

public:
    PieceController(Dobot* pDobot, Chessboard *pBoardMain, Chessboard *pBoardRemoved);

    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE VertMove = VM_NONE, bool bRetreat = false);
    Field* searchForPieceActualFieldOnBoard(Chessboard* pBoard, Piece* pPiece);
    bool isPieceStayOnItsStartingField(Piece* pPiece, bool bErrorLog = false);
    bool isMoveSet();
    void clearLastPos() { m_lastPos.Letter = L_X; m_lastPos.Digit = D_X; }
    void clearPawnsPromotions();

    PosOnBoard getLastPos() const { return m_lastPos; }
    BOARD getLastBoardType() const { return m_lastBoardType; }
    QString getPromotedPawnsPositions();
    Piece* getPiece(short sPieceNr) const { return m_pPieceSet->getPiece(sPieceNr); }
    QString dumpAllData();

    Dobot* getDobotPointer() const { return m_pDobot; }
    PieceSet* getPieceSetPointer() const { return m_pPieceSet; }
    Chessboard* getBoardMainPointer() const { return m_pBoardMain; }
    Chessboard* getBoardRemovedPointer() const { return m_pBoardRemoved; }

signals:
    void addTextToLogPTE(QString, LOG);
    void showRealBoardInUI();
};

#endif // PIECE_CONTROLLER_H
