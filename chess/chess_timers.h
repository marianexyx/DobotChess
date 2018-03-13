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
    void whiteTimeOut() {emit this->timeOutPlayer(PT_WHITE); }
    void blackTimeOut() {emit this->timeOutPlayer(PT_BLACK); }
    void updateTimeLabels();
    void startTimeOut();

public:
    ChessTimers(Clients* pClientsList);

    void startGameTimer();
    void resetGameTimers();
    QString milisecToClockTime(long lMilis);
    void switchPlayersTimers(WHOSE_TURN Turn);
    GAME_STATUS startQueueTimer();
    void stopQueueTimer();

    int getWhiteTimeLeft();
    int getBlackTimeLeft();
    int getStartTimeLeft() { return _startQueueTimer->remainingTime(); }
    bool isStartTimerRunning() { return _startQueueTimer->isActive(); }

signals:
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void timeOutStart();
    void timeOutPlayer(PLAYER_TYPE);
};

#endif // CHESS_TIMERS_H
