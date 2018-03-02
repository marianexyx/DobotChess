#ifndef CHESS_TIMERS_H
#define CHESS_TIMERS_H

#pragma once
#include <QTimer>
#include "client.h"

class ChessTimers: public QObject
{
    Q_OBJECT

    friend class Chess;

private:
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

public:
    ChessTimers(Clients* pClientsList);

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
