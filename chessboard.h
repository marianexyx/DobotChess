#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once

#include <QTimer>
#include <QtMath>
#include "QString"
#include "QObject"
#include "qdebug.h"
#include <vector>
#include <typeinfo>
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

//todo: przemyśleć dziedziczenie z field
class Chessboard2 //todo: bez makra QOBCJECT. ta szachownica ma by w 100% zalezna
{
//info: szachownica nie powinna być tworzona a z bierkami- to gra o tym decyduje gdzie i jakie są

private:
    Field* m_pField[64];

public:
    Chessboard2()
    {
        for (int i=1; i>=64; ++i)
            *m_pField[i] = new Field(i);
    }

    ~Chessboard2()
    {
        for (int i=1; i >=64; ++i)
        {
            delete m_pField[i];
            m_pField[i] = nullptr;
        }
    }

    //<--todo
    Point3D m_dMinBoard, m_dMaxBoard;
    void FENToBoard(QString FENBoard);
    void showBoardInDebug();
    QString arrayBoardAsQStr(QString QStrBoard[8][8]);

signals:
    void addTextToConsole(QString);
    void clearBoard(); //clearFormBoard
    void showBoard(QString); //showForm
    //todo --/>

    void setPieceOnField(short sPassedPiece, short sDestFieldNr) //todo: friend dla chwytaka?
    {
        if (!Piece::isInRange(sPassedPiece)) return;
        if (m_pField[sDestFieldNr]->isFieldOccupied())
        {
            qDebug() << "ERROR: Chessboard2::setPiecePosition: field is already occupied"
                        " by another piece. nPiece =" << sPassedPiece << ", field =" <<
                        m_pField[sDestFieldNr];
            return;
        }
        for (int i = 1; i >=64; ++i)
        {
            if (m_pField[i]->getPieceOnField() == sPassedPiece)
            {
                qDebug() << "ERROR: Chessboard2::setPiecePosition: this piece "
                            "already exist on board. piece =" << sPassedPiece <<
                            "on field =" << m_pField[i]->getNrAsQStr();
                return;
            }
        }

        m_pField[nDestinationNr].setPieceOnField(sPassedPiece);
        qDebug() << "Chessboard2::setPieceOnField- new pieceNr:" << sPassedPiece
                 << "on fieldNr:" << nDestinationNr;
    }

    short getPieceOnField(short sField) const { return m_pField[sField]->getPieceOnField(); }

    void clearField(short sNrToClear)
    {
        qDebug() << "Chessboard2::clearField: clearing field:" << Field::nrAsQStr(nNrToClear) <<
                    ". old piece =" << m_pField[sNrToClear]->getPieceOnField() <<
                    ", now it will be == 0";
        m_pField[sNrToClear]->clear();
    }
};

class Chessboard: public QObject
{
    Q_OBJECT

private:
    QString m_QStrSiteMoveRequest;
    QString m_QStrGameStatus;
    QString m_QStrBoard[8][8];
    WHOSE_TURN m_WhoseTurn;
    QString m_QStrCastlings;
    QString m_QStrEnpassant;
    QStringList m_legalMoves;
    QStringList m_historyMoves;
    SEQUENCE_TYPE m_moveType;

    QTimer *m_whiteTimer;
    QTimer *m_blackTimer;
    QTimer *m_updateLabelTimer;
    QTimer *m_startQueueTimer;
    int m_nRemainingWhiteTime;
    int m_nRemainingBlackTime;

    const int m_nMaxPieceHeight;
    Point3D m_dMinBoard, m_dMaxBoard;

    void changeWindowTitle(); //todo: to wyrzucenia

    void FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString whoseTurn);
    const long m_lTimersStartTime;
    const long m_lTimersStartQueue;

public:
    Chessboard();

    void findBoardPos(QString QStrPiecePositions); //todo: dziwna nazwa. operowanie na globalnych.
    short PositionOnBoardToFieldNr(PositionOnBoard fieldLines);
    PositionOnBoard fieldNrToPositionOnBoard(short sFieldNr);
    bool isMoveRemoving();
    bool isMoveCastling(QString moveToTest);
    bool isMoveEnpassant(QString moveToTest);
    void castlingFindRookToMove();
    void pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter, //todo: to robi jakoby za...
                           DIGIT digit, SEQUENCE_TYPE Type); //...przenoszenie chwytakiem
    bool compareArrays(short nArray1[][8], short nArray2[][8]);
    void saveStatusData(QString status);
    void showBoardInDebug();
    QString arrayBoardToQStr(QString QStrBoard[8][8]);

    void startGameTimer();
    void resetGameTimers();
    QString milisecToClockTime(long lMilis);
    void stopBoardTimers();
    void switchPlayersTimers();
    void startQueueTimer();
    void stopQueueTimer();
    bool bIsMoveInAxisRange(float x, float y, float z); //todo:dobot
    void resetBoardData();

    //todo: mam problemy z zwracaiem tablic do funckyj. nie marnować na...
    //...to teraz czasu i dac jako public
    short m_asBoardMain[8][8];
    /*todo: const*/ short m_anBoardStart[8][8];
    short m_asBoardTemp[8][8];
    short m_asBoardRemoved[8][4];

    //todo: na razie niech to będzie w public
    double m_adChessboardPositions_x[8][8];
    double m_adChessboardPositions_y[8][8];
    double m_adChessboardPositions_z[8][8];
    double m_adRemovedPiecesPositions_x[8][4];
    double m_adRemovedPiecesPositions_y[8][4];
    double m_adRemovedPiecesPositions_z[8][4];

    //TODO: jeżeli zrobię poniższe dane (tj. struktury) jako private, to jak się potem do...
    //...nich dobrać metodami dostępowymi?
    //TODO: używanie tych zmiennych globalnych powoduje że gubię się w kodzie
    PositionOnBoard PieceFrom, PieceTo, PieceActualPos;

    QString QStrAIPiecieFromTo;           //zapamiętany kolejny ruch bota czekający na wywołanie

    //todo: zamienić można na litery odpowiadające im na szachownicy
    int nGripperPiece;                  // nr bierki znajdującej się aktualnie w chwytaku

    QString QStrFuturePromote;

    //metody dostępowe
    void setWhoseTurn(WHOSE_TURN Turn)              { m_WhoseTurn = Turn;
                                                      this->changeWindowTitle(); } //todo: zabrać to?
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
    int getMaxPieceHeight() const                   { return m_nMaxPieceHeight; }
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
    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft()                      { return m_startQueueTimer->remainingTime(); }
    bool isStartTimerRunning()                  { return m_startQueueTimer->isActive(); }
    short getPieceOnBoardAsNr(BOARD boardType, short sFieldNr);
    short getPieceOnBoardAsNr(BOARD boardType, PositionOnBoard fieldLines);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, short sFieldNr);
    PositionOnBoard getPieceOnBoardAsLines(BOARD boardType, PositionOnBoard fieldLines);

private slots:
    void timeOutWhite();
    void timeOutBlack();
    void updateTimeLabels();
    void timeOutStartQueue();

signals:
    void addTextToConsole(QString);
    void changeWindowTitle(QString);
    void clearBoard();
    void showBoard(QString);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
    void msgFromChessboardToWebsockets(QString); //todo: znowu odwoływanie się w górę do rodzica
};

#endif // CHESSBOARD_H
