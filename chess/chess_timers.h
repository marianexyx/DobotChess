#ifndef CHESS_TIMERS_H
#define CHESS_TIMERS_H

#pragma once
#include <QTimer>
#include <QtMath>
#include "client.h"
#include "chess/vars/turn_types.h"
#include "chess/vars/game_status.h"

class ChessTimers: public QObject
{
    Q_OBJECT
    friend class Chess;

private:
    Clients* m_pClientsList;

    QTimer* m_whiteTimer;
    QTimer* m_blackTimer;
    QTimer* m_turnTimer;
    QTimer* m_updateLabelTimer;
    QTimer* m_startQueueTimer;
    uint m_unRemainingWhiteTime;
    uint m_unRemainingBlackTime;
    const ulong m_ulTimersStartTime;
    const ulong m_ulTimersStartQueue;
    const ulong m_ulTimersTurnTime;

    ChessTimers(Clients* pClientsList);

    QString milisecToClockTime(long lMilis);
    void startGameTimer();
    void switchPlayersTimers(WHOSE_TURN Turn);
    GAME_STATUS startQueueTimer();
    void stopQueueTimer();
    void stopBoardTimers();
    void resetGameTimers();
    QString dumpAllData();

    uint getWhiteTimeLeft(bool bSeconds = false);
    uint getBlackTimeLeft(bool bSeconds = false);
    uint getStartTimeLeft(bool bSeconds = false);
    uint getTurnTimeLeft(bool bSeconds = false);

private slots:
    void startTimeOut();
    void whiteTimeOut() { emit this->timeOutPlayer(PT_WHITE); }
    void blackTimeOut() { emit this->timeOutPlayer(PT_BLACK); }
    void turnTimeOut() { emit this->timeOutTurn(); }
    void updateTimeLabels();

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void timeOutStart();
    void timeOutPlayer(PLAYER_TYPE);
    void timeOutTurn();
};

#endif // CHESS_TIMERS_H
