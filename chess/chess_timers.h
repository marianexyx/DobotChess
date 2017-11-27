#ifndef CHESS_TIMERS_H
#define CHESS_TIMERS_H

#pragma once

#include <QString>
#include <QTimer>

class ChessTimers
{
private:
    QTimer *m_whiteTimer;
    QTimer *m_blackTimer;
    QTimer *m_updateLabelTimer;
    QTimer *m_startQueueTimer;
    int m_nRemainingWhiteTime;
    int m_nRemainingBlackTime;
    const long m_lTimersStartTime;
    const long m_lTimersStartQueue;

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
    int getStartTimeLeft()                      { return m_startQueueTimer->remainingTime(); }
    bool isStartTimerRunning()                  { return m_startQueueTimer->isActive(); }
};

#endif // CHESS_TIMERS_H
