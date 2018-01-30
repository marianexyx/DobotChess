#ifndef CHESS_STATUS_H
#define CHESS_STATUS_H

#pragma once
#include "chess.h"

class ChessStatus
{

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

public:
    ChessStatus(Chess* pChess);
    ~ChessStatus() {}

    bool isMoveLegal(QString QStrMove) { return _legalMoves.contains(QStrMove)? true : false; }
    bool isMoveARequestForPromotion(QString QStrMove){
        return _legalMoves.contains(QStrMove + "q")? true : false; }
    bool isMoveRemoving();
    bool isMoveCastling(QString QStrMoveToTest);
    bool isMoveEnpassant(QString QStrMoveToTest);
    static bool isSignProperPromotionType(QString QStrSign, bool bErrorLog = false);
    static bool isMovePromotion(QString QStrMove, bool bErrorLog = false);
    void saveStatusData(QString status);
    void resetStatusData();

    //todo: settery jako friend dla chess? sprawdzić czy większośc metod dostepowych nie jest...
    //...używanych tylko w tej klasie
    void setGameStatus(QString QStrStatus) { _FENGameState = FENGameState(QStrStatus); }
    void setWhoseTurn(WHOSE_TURN Turn) { _WhoseTurn = Turn; }
    void setLegalMoves(QString msg);
    void setLegalMoves(QStringList moves) { _legalMoves = moves;
                                            emit _pChess->showLegalMoves(_legalMoves); }
    void setHistoryMoves(QString msg);
    void setHistoryMoves(QStringList moves) { _historyMoves = moves;
                                              emit _pChess->showHistoryMoves(_historyMoves); }

    END_TYPE getFENGameState() const { return _FENGameState; }
    WHOSE_TURN getWhoseTurn() const { return _WhoseTurn; }
    QStringList getLegalMoves() const { return _legalMoves; }
    QStringList getHisotyMoves() const { return _historyMoves; }
    QString getHisotyMovesAsQStr() const { return _historyMoves.join(" "); }
    QString getCastlings() const { return _QStrCastlings; }
    QString getEnpassant() const { return _QStrEnpassant; }
    PLAYER_TYPE getActivePlayerType();

    void clearLegalMoves() { _legalMoves.clear();
                             emit _pChess->showLegalMoves(_legalMoves);}
    void clearHistoryMoves() { _historyMoves.clear();
                               emit _pChess->showHistoryMoves(_historyMoves); }
};

#endif // CHESS_STATUS_H
