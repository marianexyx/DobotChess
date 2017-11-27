#include "chess_timers.h"

ChessTimers::ChessTimers():
    m_lTimersStartTime(1000*60*30), //1000ms (1s) * 60s * 30min
    m_lTimersStartQueue(1000*60*2)
{
    m_whiteTimer = new QTimer();
    m_blackTimer = new QTimer();
    m_updateLabelTimer = new QTimer();
    m_startQueueTimer = new QTimer();
    m_whiteTimer->setInterval(m_lTimersStartTime);
    m_blackTimer->setInterval(m_lTimersStartTime);
    m_updateLabelTimer->setInterval(1000);
    m_startQueueTimer->setInterval(m_lTimersStartQueue);
    m_whiteTimer->setSingleShot(true);
    m_blackTimer->setSingleShot(true);
    m_updateLabelTimer->setSingleShot(false);
    m_startQueueTimer->setSingleShot(true);
    connect(m_whiteTimer, SIGNAL(timeout()), this, SLOT(timeOutWhite()));
    connect(m_blackTimer, SIGNAL(timeout()), this, SLOT(timeOutBlack()));
    connect(m_updateLabelTimer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    connect(m_startQueueTimer, SIGNAL(timeout()), this, SLOT(timeOutStartQueue()));
    m_nRemainingWhiteTime = m_lTimersStartTime;
    m_nRemainingBlackTime = m_lTimersStartTime;
}
//private slots:

void ChessTimers::timeOutWhite()
{
    this->resetGameTimers();
    emit msgFromChessboardToWebsockets("timeOutWhite");
}

void ChessTimers::timeOutBlack()
{
    this->resetGameTimers();
    emit msgFromChessboardToWebsockets("timeOutBlack");
}

void ChessTimers::updateTimeLabels()
{
    if (m_whiteTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_whiteTimer->remainingTime()), BDL_WHITE_TIME);
    else if (m_blackTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_blackTimer->remainingTime()), BDL_BLACK_TIME);

    if (m_startQueueTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    else setBoardDataLabels(milisecToClockTime(m_lTimersStartQueue), BDL_QUEUE_TIME);
}

void ChessTimers::timeOutStartQueue()
{
    this->stopQueueTimer();
    emit msgFromChessboardToWebsockets("timeOutStartQueue");
}

//public:

void ChessTimers::startGameTimer()
{
    m_whiteTimer->start();
    m_updateLabelTimer->start();
}

void ChessTimers::stopBoardTimers()
{
    m_whiteTimer->stop();
    m_blackTimer->stop();
    m_updateLabelTimer->stop();
}

void ChessTimers::resetGameTimers()
{
    this->stopBoardTimers();
    m_whiteTimer->setInterval(m_lTimersStartTime);
    m_blackTimer->setInterval(m_lTimersStartTime);
    m_nRemainingWhiteTime = m_whiteTimer->interval();
    m_nRemainingBlackTime = m_blackTimer->interval();
    emit setBoardDataLabels(milisecToClockTime(m_lTimersStartTime), BDL_WHITE_TIME);
    emit setBoardDataLabels(milisecToClockTime(m_lTimersStartTime), BDL_BLACK_TIME);
}

QString ChessTimers::milisecToClockTime(long lMilis)
{
    if (lMilis > 0)
    {
        int nAllSecs = lMilis / 1000;
        int nMins = qFloor(nAllSecs / 60);
        int nSecs = qFloor(nAllSecs % 60);
        QString QStrMinsPrefix, QStrSecsPrefix;
        nMins >= 10 ? QStrMinsPrefix = "" : QStrMinsPrefix = "0";
        nSecs >= 10 ? QStrSecsPrefix = "" : QStrSecsPrefix = "0";
        return QStrMinsPrefix + QString::number(nMins) + ":" + QStrSecsPrefix + QString::number(nSecs);
    }
    else return "00:00";
}

void ChessTimers::switchPlayersTimers()
{
    if (getWhoseTurn() == WHITE_TURN)
    {
        m_nRemainingBlackTime = m_blackTimer->remainingTime();
        m_blackTimer->stop();

        m_whiteTimer->setInterval(m_nRemainingWhiteTime);
        m_whiteTimer->start();
    }
    else if (getWhoseTurn() == BLACK_TURN)
    {
        m_nRemainingWhiteTime = m_whiteTimer->remainingTime();
        m_whiteTimer->stop();

        m_blackTimer->setInterval(m_nRemainingBlackTime);
        m_blackTimer->start();
    }
    else qDebug() << "ERROR: Chessboard::switchPlayersTimers(): getWhoseTurn isn't "
                     "white or black.  it's' ==" << getWhoseTurn();
}

void ChessTimers::startQueueTimer()
{
    qDebug() << "Chessboard::startQueueTimer()";
    m_startQueueTimer->stop();
    m_startQueueTimer->setInterval(m_lTimersStartQueue);
    emit setBoardDataLabels(milisecToClockTime(m_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    m_startQueueTimer->start();
    m_updateLabelTimer->start();
}

void ChessTimers::stopQueueTimer() //stop == reset
{
    m_startQueueTimer->stop();
    m_updateLabelTimer->stop();
    this->updateTimeLabels();
}

//get/set:

int ChessTimers::getWhiteTimeLeft()
{
    if (m_whiteTimer->remainingTime() != -1)
        return m_whiteTimer->remainingTime();
    else return m_nRemainingWhiteTime;
}

int ChessTimers::getBlackTimeLeft()
{
    if (m_blackTimer->remainingTime() != -1)
        return m_blackTimer->remainingTime();
    else return m_nRemainingBlackTime;
}
