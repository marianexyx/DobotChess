#ifndef CHESS_STATUS_H
#define CHESS_STATUS_H

#pragma once

#include <QString>
#include "chessboard.h"

class ChessStatus
{

private:
    QString _QStrGameStatus;
    QString _QStrBoard[8][8]; //todo:?? nazwać to jakiś tempem, albo używać obiektów
    WHOSE_TURN _WhoseTurn;
    QString _QStrCastlings;
    QString _QStrEnpassant;
    QStringList _legalMoves;
    QStringList _historyMoves;
    PosFromTo _FuturePromoteMove;

    //todo: dodatkowo zawsze zwalniac potem pamiec- freeboard
    QString** FENToBoard(QString FENBoard);
    WHOSE_TURN whoseTurn(QString QStrWhoseTurn);

public:
    ChessStatus();
    ~ChessStatus() {}

    bool isMoveRemoving();
    bool isMoveCastling(QString moveToTest);
    bool isMoveEnpassant(QString moveToTest);
    void saveStatusData(QString status);
    SEQUENCE_TYPE findMoveType(QString move);

    //todo: settery jako friend dla chess?
    void setWhoseTurn(WHOSE_TURN Turn) { _WhoseTurn = Turn; }
    void setLegalMoves(QString msg);
    void setLegalMoves(QStringList legalMoves) { _legalMoves = legalMoves;
                                                 emit showLegalMoves(_legalMoves); }
    void setHistoryMoves(QString msg);
    void setHistoryMoves(QStringList historyMoves) { _historyMoves = historyMoves;
                                                     emit showHistoryMoves(_historyMoves); }
    //void setFuturePromote(QString QStrMove) { _QStrFuturePromote = QStrMove; }

    QString getGameStatus() { return _QStrGameStatus; }
    WHOSE_TURN getWhoseTurn() { return _WhoseTurn; }
    QStringList getLegalMoves() { return _legalMoves; }
    QStringList getHisotyMoves() { return _historyMoves; }
    QString getHisotyMovesAsQStr() { return _historyMoves.join(" "); }
    QString getCastlings() { return _QStrCastlings; }
    QString getEnpassant() { return _QStrEnpassant; }
    QString getFuturePromoteAsQStr() const { return _FuturePromoteMove.asQStr(); }

    void clearLegalMoves() { _legalMoves.clear(); emit showLegalMoves(_legalMoves);}
    void clearHistoryMoves() { _historyMoves.clear(); emit showHistoryMoves(_historyMoves); }
    void clearFuturePromote() { _FuturePromoteMove.clear(); }

signals: //todo: dziedziczyć jakoś?
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
};

#endif // CHESS_STATUS_H
