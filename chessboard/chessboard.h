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
    BOARD m_boardType;
    bool m_bBoardIsReal;
    //future: could create struct "4 corners" or smtg
    Point3D  m_a1, m_a8, m_h1, m_h8;
    Point3D m_remWhiteCloserOuter, m_remWhiteCloserInner;
    Point3D m_remWhiteFurtherOuter, m_remWhiteFurtherInner;
    Point3D m_remBlackCloserOuter, m_remBlackCloserInner;
    Point3D m_remBlackFurtherOuter, m_remBlackFurtherInner;
    Field* m_pField[64];

    double calculateFieldCenter(Point3D bottomLeft, Point3D bottomRight, Point3D topLeft,
                                Point3D topRight, int nHorizontalFields, int nVerticalFields,
                                int nHorizontalFieldNr, int nVerticalFieldNr, AXIS axis);
    void calculateFieldsPointsOnMainBoard(Point3D A1, Point3D A8, Point3D H1, Point3D H8);
    void calculateFieldsPointsOnRemovedBoard(Point3D whiteCloserOuter,
          Point3D whiteCloserInner, Point3D whiteFutherOuter, Point3D whiteFutherInner,
          Point3D blackCloserOuter, Point3D blackCloserInner, Point3D blackFutherOuter,
          Point3D blackFutherInner);

    void setPieceOnField(Piece* pPiece, Field* pField, bool bDebugLog = false);
    void clearField(Field* pField, bool bErrorLog = false) { pField->clearField(bErrorLog); }

public:
    Chessboard(BOARD BoardType, bool bBoardIsReal = true,
               BoardsRealVars realVars = BoardsRealVars());
    ~Chessboard();

    const double dMaxPieceHeight; //todo: set and use as static?

    bool isBoardReal(bool bErrorLog = false);
    bool isPieceAlreadyExistsOnBoard(Piece* pPiece, bool bErrorLog = false);

    BOARD getBoardType() const { return m_boardType; }
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

