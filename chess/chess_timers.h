#ifndef CHESS_TIMERS_H
#define CHESS_TIMERS_H

#pragma once
#include "chess.h"
#include <QTimer>

class ChessTimers
{
    friend class Chess;

private:
    Chess* _pChess;
    ChessResets* _pResets;
    Clients* _pClientsList;

    QTimer* _whiteTimer;
    QTimer* _blackTimer;
    QTimer* _updateLabelTimer;
    QTimer* _startQueueTimer;
    int _nRemainingWhiteTime;
    int _nRemainingBlackTime;
    const long _lTimersStartTime;
    const long _lTimersStartQueue;

    void stopBoardTimers();

private slots:
    void playerTimeOut(PLAYER_TYPE Player);
    void updateTimeLabels();
    void timeOutStartQueue();

//public:
    ChessTimers(Chess* pChess);

    void startGameTimer();
    void resetGameTimers();
    QString milisecToClockTime(long lMilis);
    void switchPlayersTimers();
    void startQueueTimer();
    void stopQueueTimer();

    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft() { return _startQueueTimer->remainingTime(); }
    bool isStartTimerRunning() { return _startQueueTimer->isActive(); }
};

#endif // CHESS_TIMERS_H
