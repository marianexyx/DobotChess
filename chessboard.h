#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once

#include <QtMath>
#include "QString"
#include "QObject"
#include "qdebug.h"
#include <vector>
#include <typeinfo>
#include <limits>
#include "field.h"
#include "piece.h"
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"
#include "vars/sequence_types.h"
#include "vars/board_types.h"
#include "vars/turn_types.h"
#include "vars/players_types.h"
#include "vars/board_types.h"
#include "vars/posotion_on_board.h"
#include "vars/piece_type.h" //todo: includowac caly folder

//todo: chessboard powinno być tworzone tylko wewnątrz partii szachów?
//info: bez makra QOBCJECT. ta szachownica ma by w 100% zalezna
//info: szachownica nie powinna być tworzona z bierkami- to...
//...gra o tym decyduje gdzie i jakie są
class Chessboard
{
private:
    BOARD _BoardType;
    Field* _pField[64];
    Point3D _MinBoard, _MaxBoard;

public:
    Chessboard(BOARD boardType);
    ~Chessboard();

    BOARD getBoardType() const { return _BoardType; }
    void setPieceOnField(short sPassedPiece, Field *pField); //todo: friend dla chwytaka?
    void clearField(Field* pField);
    bool isPointInLocationLimits(Point3D point);
    bool isPieceExistsOnBoard(short sPieceID, bool bErrorLog = false);
    Field* getField(short sFieldNr) const { return _pField[sFieldNr]; }
    Field* getField(PosOnBoard Pos) const { return _pField[Field::nr(Pos)]; }
    short getFieldNrWithGivenPieceNrIfExists(short sPieceNr);
    Point3D getMinBoardAxis() const { return _MinBoard; }
    Point3D getMaxBoardAxis() const { return _MaxBoard; }

    //void showBoardInDebug(); //nie przerabiac poki niepotrzebne
    //todo: double pointers + freeboard:
    static QString arrayBoardAsQStr(QString QStrBoard[8][8]);

signals:
    void addTextToConsole(QString);
    void showBoardInForm(QString);
    void clearFormBoard();
    void setBoardDataLabels(QString, BOARD_DATA_LABELS); //todo
};

#endif // CHESSBOARD_H
