#ifndef CHESS_STATUS_H
#define CHESS_STATUS_H

#pragma once
#include "chess/vars/sequence_types.h"
#include "chess/vars/end_of_game_types.h"
#include "chess/vars/turn_types.h"
#include "client.h"
#include "piece_controller.h"

class ChessStatus: public QObject
{
    Q_OBJECT
    friend class Chess;

private:
    Chessboard* m_pBoardMain;
    Clients* m_pClientsList;
    PieceController* m_pPieceController;

    WHOSE_TURN m_WhoseTurn;
    PosFromTo m_PosMove;
    SEQUENCE_TYPE m_MoveType;
    QStringList m_legalMoves;
    QStringList m_historyMoves;
    QString m_QStrCastlings;
    QString m_QStrEnpassant;
    END_TYPE m_FENGameState;

    ChessStatus(PieceController* pPieceController, Chessboard* pBoardMain, Clients* pClientsList);

    void setMove(QString QStrMove);
    SEQUENCE_TYPE findMoveType(QString QStrMove);
    bool isMoveLegal(QString QStrMove) { return m_legalMoves.contains(QStrMove)? true : false; }
    bool isMoveEnpassant(QString QStrMoveToTest);
    bool isMoveCastling(QString QStrMoveToTest);
    bool isMovePromotionWithRemoving(QString QStrMoveToTest);
    static bool isMovePromotion(QString QStrMove, bool bErrorLog = false);
    bool isMoveRemoving(QString QStrMoveToTest);

    void saveStatusData(QString QStrStatus);
    WHOSE_TURN whoseTurnFromFENStatus(QString QStrWhoseTurn);
    void resetStatusData();
    void promotePawn(PosOnBoard posOfPawnToPromote, QString QStrPromoType);
    QString dumpAllData();

    void setLegalMoves(QString QStrMsg);
    void setLegalMoves(QStringList moves);
    void setHistoryMoves(QString QStrMsg);
    void setHistoryMoves(QStringList moves);
    void clearMove() { m_PosMove.clear(); }
    void clearLegalMoves();
    void clearHistoryMoves();

public:
    static bool isMoveInProperFormat(QString QStrMove, bool bErrorLog = false);
    bool isMoveARequestForPromotion(QString QStrMove)
    { return m_legalMoves.contains(QStrMove + "q")? true : false; }
    static bool isSignProperPromotionType(QString QStrSign, bool bErrorLog = false);

    WHOSE_TURN getWhoseTurn() const { return m_WhoseTurn; }
    PLAYER_TYPE getActivePlayerType();
    QString getHistoryMovesAsQStr() const
    { return (m_historyMoves.size() ? m_historyMoves.join(" ") : "0"); }

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
};

#endif // CHESS_STATUS_H
