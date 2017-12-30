#ifndef CHESS_TIMERS_H
#define CHESS_TIMERS_H

#pragma once

#include <QString>
#include <QTimer>
#include "client.h" //todo: ogarnąć to...

class ChessTimers
{
private:
    QTimer *_whiteTimer;
    QTimer *_blackTimer;
    QTimer *_updateLabelTimer;
    QTimer *_startQueueTimer;
    int _nRemainingWhiteTime;
    int _nRemainingBlackTime;
    const long _lTimersStartTime;
    const long _lTimersStartQueue;

private slots:
    void timeOutWhite();
    void timeOutBlack();
    void updateTimeLabels();
    void timeOutStartQueue();

public:
    ChessTimers();

    void startGameTimer();
    void stopBoardTimers();
    void resetGameTimers();
    QString milisecToClockTime(long lMilis);
    void switchPlayersTimers();
    void startQueueTimer();
    void stopQueueTimer();

    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft() { return _startQueueTimer->remainingTime(); }
    bool isStartTimerRunning() { return _startQueueTimer->isActive(); }

signals:
    void sendMsgToPlayer(QString QStrMsgForPlayer);
};

#endif // CHESS_TIMERS_H
