#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once
#include <QtMath>
#include "QObject"
#include <vector>
#include <typeinfo>
#include <climits>
#include "field.h"
#include "vars/board_axis.h"
#include "vars/board_data_labels.h"
#include "vars/board_types.h"
#include "xml_reader.h"

enum BOARD_POINTS { BP_MIN, BP_MAX, BP_MIDDLE, BP_RETREAT_LEFT, BP_RETREAT_RIGHT };

//future: this class can have its own dir amongfull classes like...
//...piece, field; also enums. this dir should be inside chess dir?

//class PieceController;

class Chessboard: public QObject
{
    Q_OBJECT
    friend class PieceController;
    friend class ChessMovements;

private:
    BOARD _boardType;
    bool _bBoardIsReal;
    Field* _pField[64];
    Point3D  _a1, _a8, _h1, _h8;
    Point3D _remWhiteCloserOuter, _remWhiteFurtherInner;
    Point3D _remBlackCloserOuter, _remBlackFurtherInner;
    Point3D _minBoard, _maxBoard, _middleAbove, _retreatLeft, _retreatRight;
    double _dSquareWidth;

    void calculateFields3DLocationsOnMainBoard(Point3D A1, Point3D A8, Point3D H1, Point3D H8);
    void calculateFields3DLocationsOnRemovedBoard(Point3D whiteCloserOuter,
          Point3D whiteFutherInner, Point3D blackCloserOuter, Point3D blackFutherInner);
    void calculateMarginal3DValues();
    void calculateMiddleAbovePoint();
    void calculateRetreatPoints(RealVars RV);

    void setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog = false);
    void clearField(Field* pField, bool bErrorLog = false);

public:
    Chessboard(BOARD BoardType, bool bBoardIsReal = true,
               RealVars gameConfigVars = RealVars());
    ~Chessboard();

    const float fMaxPieceHeight;

    bool isBoardReal(bool bErrorLog = false);
    bool isPieceAlreadyExistsOnBoard(Piece* pPiece, bool bErrorLog = false);

    BOARD getBoardType() const { return _boardType; }
    Point3D getBoardPoint3D(BOARD_POINTS BP) const;
    Field* getField(short sFieldNr) const;
    Field* getField(PosOnBoard Pos) const;
    Field* getFieldWithGivenPieceIfExists(Piece* pPiece, bool bErrorLog = false);

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showImaginaryBoardInUI(QString);
    void clearBoardInUI();
};

#endif // CHESSBOARD_H

