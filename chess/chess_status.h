#ifndef CHESS_STATUS_H
#define CHESS_STATUS_H

#pragma once
#include "chess/end_of_game_types.h"
#include "chess/turn_types.h"

enum MOVES_LISTS { ML_HISTORY, ML_LEGAL };

class Chess;

class ChessStatus
{
    friend class Chess;

private:
    Chess* _pChess;
    Chessboard* _pBoardMain;
    Clients* _pClientsList;

    END_TYPE _FENGameState;
    WHOSE_TURN _WhoseTurn;
    QString _QStrCastlings;
    QString _QStrEnpassant;
    QStringList _legalMoves;
    QStringList _historyMoves;

    WHOSE_TURN whoseTurn(QString QStrWhoseTurn);

//public:
    ChessStatus(Chess* pChess);
    ~ChessStatus() {}

    void saveStatusData(QString QStrStatus);
    void resetStatusData();

    //todo: settery jako friend dla chess? sprawdzić czy większośc metod dostepowych nie jest...
    //...używanych tylko w tej klasie
    void setGameStatus(QString QStrStatus) { _FENGameState = FENGameState(QStrStatus); }
    void setWhoseTurn(WHOSE_TURN Turn) { _WhoseTurn = Turn; }
    void setLegalMoves(QString QStrMsg);
    void setLegalMoves(QStringList moves);
    void setHistoryMoves(QString QStrMsg);
    void setHistoryMoves(QStringList moves);

    void clearLegalMoves();
    void clearHistoryMoves();

public:
    static bool isSignProperPromotionType(QString QStrSign, bool bErrorLog = false);
    static bool isMovePromotion(QString QStrMove, bool bErrorLog = false);
    static bool isMoveInProperFormat(QString QStrMove, bool bErrorLog = false);

    bool isMoveLegal(QString QStrMove) { return _legalMoves.contains(QStrMove)? true : false; }
    bool isMoveARequestForPromotion(QString QStrMove){
        return _legalMoves.contains(QStrMove + "q")? true : false; }
    bool isMoveRemoving();
    bool isMoveCastling(QString QStrMoveToTest);
    bool isMoveEnpassant(QString QStrMoveToTest);

    END_TYPE getFENGameState() const { return _FENGameState; }
    WHOSE_TURN getWhoseTurn() const { return _WhoseTurn; }
    QStringList getLegalMoves() const { return _legalMoves; }
    QStringList getHistoryMoves() const { return _historyMoves; }
    QString getHistoryMovesAsQStr() const { return _historyMoves.join(" "); }
    QString getCastlings() const { return _QStrCastlings; }
    QString getEnpassant() const { return _QStrEnpassant; }
    PLAYER_TYPE getActivePlayerType();
};

#endif // CHESS_STATUS_H
