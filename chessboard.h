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
//todo: przemyśleć dziedziczenie z field
class Chessboard2 //todo: bez makra QOBCJECT. ta szachownica ma by w 100% zalezna
{
//info: szachownica nie powinna być tworzona a z bierkami- to...
//...gra o tym decyduje gdzie i jakie są

private:
    BOARD m_boardType;
    Field* m_pField[64];
    Point3D m_dMinBoard, m_dMaxBoard;

public:
    Chessboard2(BOARD boardType);
    ~Chessboard2();

    QString** FENToBoard(QString FENBoard); //dodatkowo zawsze zwalniac potem pamiec- freeboard
    //void showBoardInDebug(); //nie przerabiac poki niepotrzebne
    QString arrayBoardAsQStr(QString QStrBoard[8][8]); //todo: double pointers + freeboard

    BOARD getBoardType() const { return m_boardType; }
    void setPieceOnField(short sPassedPiece, short sDestFieldNr); //todo: friend dla chwytaka?
    short getPieceOnField(short sField) const { return m_pField[sField]->getPieceOnField(); }
    void clearField(short sNrToClear);
    Point3D getFieldLocation3D(short sFieldNr) const { return m_pField[sFieldNr]->getLocation3D(); }
    bool isPointInLocationLimits(Point3D point);

signals:
    void addTextToConsole(QString);
    void clearFormBoard();
    void showBoardInForm(QString);
};

class Chessboard: public QObject
{
    Q_OBJECT

private:
    QString m_QStrSiteMoveRequest;
    QString m_QStrGameStatus;
    QString m_QStrBoard[8][8];
    WHOSE_TURN m_WhoseTurn;

    QStringList m_legalMoves;
    QStringList m_historyMoves;

    const int m_nMaxPieceHeight;
    Point3D m_dMinBoard, m_dMaxBoard;

    void FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString whoseTurn);

public:
    Chessboard();

    void findBoardPos(QString QStrPiecePositions); //todo: dziwna nazwa. operowanie na globalnych.
    void pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter, //todo: to robi jakoby za...
                           DIGIT digit, SEQUENCE_TYPE Type); //...przenoszenie chwytakiem
    void saveStatusData(QString status);

    bool bIsMoveInAxisRange(float x, float y, float z); //todo:dobot
    void resetBoardData();

    //todo: zamienić można na litery odpowiadające im na szachownicy
    int nGripperPiece;                  // nr bierki znajdującej się aktualnie w chwytaku


    //metody dostępowe
    void setWhoseTurn(WHOSE_TURN Turn)              { m_WhoseTurn = Turn; }
    void setMoveType(SEQUENCE_TYPE Type)            { m_moveType = Type; }
    void setLegalMoves(QStringList legalMoves)      { m_legalMoves = legalMoves;
                                                      emit showLegalMoves(legalMoves); }
    void setHistoryMoves(QStringList historyMoves)  { m_historyMoves = historyMoves;
                                                      emit showHistoryMoves(historyMoves); }
    //short getBoardByItsType(BOARD boardType);
    void setPieceOnBoard(BOARD boardType, short sPieceNr, short sFieldNr);
    void setPieceOnBoard(BOARD boardType, short sPieceNr, PositionOnBoard fieldLines);
    void setPieceOnBoard(BOARD boardType, PositionOnBoard pieceLines, short sFieldNr);
    void setPieceOnBoard(BOARD boardType, PositionOnBoard pieceLines, PositionOnBoard fieldLines);

    void clearLegalMoves()                          { m_legalMoves.clear();
                                                      emit showLegalMoves(m_legalMoves);}
    void clearHistoryMoves()                        { m_historyMoves.clear();
                                                      emit showHistoryMoves(m_historyMoves); }

    QString getSiteMoveRequest()                    { return m_QStrSiteMoveRequest; }
    QString getPiecieFromTo();
    QString getGameStatus()                         { return m_QStrGameStatus; }
    WHOSE_TURN getWhoseTurn()                       { return m_WhoseTurn; }
    QString getStrWhoseTurn();
    SEQUENCE_TYPE getMoveType()                     { return m_moveType; }
    QString getCastlings()                          { return m_QStrCastlings; }
    QString getEnpassant()                          { return m_QStrEnpassant; }
    QStringList getLegalMoves()                     { return m_legalMoves; }
    QStringList getHisotyMoves()                    { return m_historyMoves; }
    QString getHisotyMovesAsQStr()                  { return m_historyMoves.join(" "); }
    double getMinBoardZ()                           { return m_dMinBoard.z; }
    Point3D getMinBoardAxis() const { return m_dMinBoard; }
    Point3D getMaxBoardAxis() const { return m_dMaxBoard; }

    short getPieceOnBoardAsNr(BOARD boardType, short sFieldNr);
    short getPieceOnBoardAsNr(BOARD boardType, PositionOnBoard fieldLines);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, short sFieldNr);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, PositionOnBoard fieldLines);

signals:
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
    void msgFromChessboardToWebsockets(QString); //todo: znowu odwoływanie się w górę do rodzica
};

#endif // CHESSBOARD_H
