#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once
#include <QtMath>
#include "QObject"
#include <vector>
#include <typeinfo>
#include <climits>
#include "xml_reader.h"
#include "vars/board_data_labels.h"
#include "vars/intermediate_points.h"
#include "chessboard/field.h"
#include "chessboard/vars/board_axis.h"
#include "chessboard/vars/board_types.h"

class Chessboard: public QObject
{
    Q_OBJECT
    friend class PieceController;
    friend class ChessMovements;

private:
    BOARD _boardType;
    bool _bBoardIsReal;
    Point3D  _a1, _a8, _h1, _h8;
    Point3D _remWhiteCloserOuter, _remWhiteFurtherInner;
    Point3D _remBlackCloserOuter, _remBlackFurtherInner;
    double _dSquareWidth;
    Field* _pField[64];

    void calculateFieldsPointsOnMainBoard(Point3D A1, Point3D A8, Point3D H1, Point3D H8);
    void calculateFieldsPointsOnRemovedBoard(Point3D whiteCloserOuter,
          Point3D whiteFutherInner, Point3D blackCloserOuter, Point3D blackFutherInner);

    void setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog = false);
    void clearField(Field* pField, bool bErrorLog = false) { pField->clearField(bErrorLog); }

public:
    Chessboard(BOARD BoardType, bool bBoardIsReal = true,
               RealVars gameConfigVars = RealVars());
    ~Chessboard();

    const float fMaxPieceHeight;

    IntermediatePoints calculateIntermediatePoints(RealVars RV, Chessboard *pBoard);

    bool isBoardReal(bool bErrorLog = false);
    bool isPieceAlreadyExistsOnBoard(Piece* pPiece, bool bErrorLog = false);

    BOARD getBoardType() const { return _boardType; }
    Field* getField(short sFieldNr) const;
    Field* getField(PosOnBoard Pos) const;
    Field* getFieldWithGivenPieceIfExists(Piece* pPiece, bool bErrorLog = false);
    QString dumpAllData();

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showImaginaryBoardInUI(QString);
    void clearBoardInUI();
};

#endif // CHESSBOARD_H

