#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once
#include <QtMath>
#include "QObject"
#include <vector>
#include <typeinfo>
#include <limits>
#include "field.h"
#include "piece.h"
#include "vars/board_axis.h"
#include "vars/board_data_labels.h"
#include "vars/board_types.h"

//info: bez makra QOBCJECT. ta szachownica ma by w 100% zalezna
//info: szachownica nie powinna być tworzona z bierkami- to...
//...gra o tym decyduje gdzie i jakie są
enum BOARD_POINTS { BP_MIN, BP_MAX, BP_MIDDLE, BP_RETREAT_LEFT, BP_RETREAT_RIGHT };

//future: ta klasa może mieć swój podfolder razem z klasami piece, field i enumami
class Chessboard
{
private:
    BOARD _BoardType;
    Field* _pField[64];
    Point3D  _A1, _A8, _H1, _H8;
    Point3D _remWhiteCloserOuter, _remWhiteFurtherInner;
    Point3D _remBlackCloserOuter, _remBlackFurtherInner;
    Point3D _MinBoard, _MaxBoard, _middleAbove, _retreatLeft, _retreatRight;
    double _dSquareWidth;

    void calculateFields3DLocationsOnMainBoard(Point3D A1, Point3D A8, Point3D H1, Point3D H8);
    void calculateFields3DLocationsOnRemovedBoard(Point3D whiteCloserOuter,
          Point3D whiteFutherInner, Point3D blackCloserOuter, Point3D blackFutherInner);
    void calculateMarginal3DValues();
    void calculateMiddleAbovePoint();
    void calculateRetreatPoints();

public:
    Chessboard(BOARD boardType); //todo: friend dla chess
    ~Chessboard();

    bool isPointInLocationLimits(Point3D point);
    bool isPieceExistsOnBoard(Piece* pPiece, bool bErrorLog = false);
    void setPieceOnField(Piece* pPiece, Field* pField); //todo: friend dla chwytaka?
    BOARD getBoardType() const { return _BoardType; }
    Point3D getBoardPoint3D(BOARD_POINTS BP) const;
    Field* getField(short sFieldNr) const { return _pField[sFieldNr]; }
    Field* getField(PosOnBoard Pos) const { return _pField[Field::nr(Pos)]; }
    Field* getFieldWithGivenPieceIfExists(Piece* pPiece);
    void clearField(Field* pField);

    static bool isBoardReal(BOARD boardType, bool bErrorLog = false);

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void clearBoardInUI();
    void showBoardInUI(QString, BOARD);
};

#endif // CHESSBOARD_H
