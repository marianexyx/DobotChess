#include "chess_timers.h"

ChessTimers::ChessTimers(Clients* pClientsList):
    m_ulTimersStartTime(1000*60*30), //30min
    m_ulTimersStartQueue(1000*60*2) //2min
{
    m_pClientsList = pClientsList;

    m_whiteTimer = new QTimer();
    m_blackTimer = new QTimer();
    m_updateLabelTimer = new QTimer();
    m_startQueueTimer = new QTimer();
    m_whiteTimer->setInterval(m_ulTimersStartTime);
    m_blackTimer->setInterval(m_ulTimersStartTime);
    m_updateLabelTimer->setInterval(1000);
    m_startQueueTimer->setInterval(m_ulTimersStartQueue);
    m_whiteTimer->setSingleShot(true);
    m_blackTimer->setSingleShot(true);
    m_updateLabelTimer->setSingleShot(false);
    m_startQueueTimer->setSingleShot(true);
    connect(m_whiteTimer, SIGNAL(timeout()), this, SLOT(whiteTimeOut()));
    connect(m_blackTimer, SIGNAL(timeout()), this, SLOT(blackTimeOut()));
    connect(m_updateLabelTimer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    connect(m_startQueueTimer, SIGNAL(timeout()), this, SLOT(startTimeOut()));
    m_unRemainingWhiteTime = m_ulTimersStartTime;
    m_unRemainingBlackTime = m_ulTimersStartTime;
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
        return QStrMinsPrefix + QString::number(nMins) + ":" + QStrSecsPrefix
                + QString::number(nSecs);
    }
    else return "00:00";
}

void ChessTimers::startGameTimer()
{
    m_whiteTimer->start();
    m_updateLabelTimer->start();
}

void ChessTimers::switchPlayersTimers(WHOSE_TURN Turn)
{
    if (Turn == WHITE_TURN)
    {
        m_unRemainingBlackTime = m_blackTimer->remainingTime();
        m_blackTimer->stop();
        m_whiteTimer->setInterval(m_unRemainingWhiteTime);
        m_whiteTimer->start();
    }
    else if (Turn == BLACK_TURN)
    {
        m_unRemainingWhiteTime = m_whiteTimer->remainingTime();
        m_whiteTimer->stop();
        m_blackTimer->setInterval(m_unRemainingBlackTime);
        m_blackTimer->start();
    }
    else qCritical() << "getWhoseTurn isn't white or black. it's =" << turnTypeAsQstr(Turn);
}

GAME_STATUS ChessTimers::startQueueTimer()
{
    m_startQueueTimer->stop();
    m_startQueueTimer->setInterval(m_ulTimersStartQueue);
    emit this->setBoardDataLabel(this->milisecToClockTime(m_startQueueTimer->remainingTime()),
                                   BDL_QUEUE_TIME);

    m_startQueueTimer->start();
    m_updateLabelTimer->start();

    return GS_TURN_NONE_WAITING_FOR_START_CONFIRMS;
}

void ChessTimers::stopQueueTimer() //stop == reset
{
    m_startQueueTimer->stop();
    m_updateLabelTimer->stop();
    this->updateTimeLabels();
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
    m_whiteTimer->setInterval(m_ulTimersStartTime);
    m_blackTimer->setInterval(m_ulTimersStartTime);
    m_unRemainingWhiteTime = m_whiteTimer->interval();
    m_unRemainingBlackTime = m_blackTimer->interval();
    emit this->setBoardDataLabel(this->milisecToClockTime(m_ulTimersStartTime), BDL_WHITE_TIME);
    emit this->setBoardDataLabel(this->milisecToClockTime(m_ulTimersStartTime), BDL_BLACK_TIME);
}

QString ChessTimers::dumpAllData()
{
    QString QStrData;

    QStrData = "[chess_timers.h]\n";
    QStrData += "m_unRemainingWhiteTime: " + QString::number(this->getWhiteTimeLeft()) + "\n";
    QStrData += "m_unRemainingBlackTime: " + QString::number(this->getBlackTimeLeft()) + "\n";
    QStrData += "m_unRemainingBlackTime: " + QString::number(this->getStartTimeLeft()) + "\n";

    return QStrData;
}

uint ChessTimers::getWhiteTimeLeft(bool bSeconds /*= false*/)
{
    int nReturnTime;
    if (m_whiteTimer->remainingTime() != -1)
        nReturnTime = m_whiteTimer->remainingTime();
    else nReturnTime = m_unRemainingWhiteTime;
    if (bSeconds == true) nReturnTime /= 1000;
    return nReturnTime;
}

uint ChessTimers::getBlackTimeLeft(bool bSeconds /*= false*/)
{
    int nReturnTime;
    if (m_blackTimer->remainingTime() != -1)
        nReturnTime = m_blackTimer->remainingTime();
    else nReturnTime = m_unRemainingBlackTime;
    if (bSeconds == true) nReturnTime /= 1000;
    return nReturnTime;
}

uint ChessTimers::getStartTimeLeft(bool bSeconds /*= false*/)
{
    int nReturnTime;
    nReturnTime = m_startQueueTimer->remainingTime();
    if (bSeconds == true) nReturnTime /= 1000;
    return nReturnTime;
}

void ChessTimers::startTimeOut()
{
    this->stopQueueTimer();
    emit this->timeOutStart();
}

void ChessTimers::updateTimeLabels()
{
    if (m_whiteTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(m_whiteTimer->remainingTime()),
                                       BDL_WHITE_TIME);
    else if (m_blackTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(m_blackTimer->remainingTime()),
                                       BDL_BLACK_TIME);

    if (m_startQueueTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(m_startQueueTimer->remainingTime()),
                                       BDL_QUEUE_TIME);

    emit this->setBoardDataLabel(this->milisecToClockTime(m_ulTimersStartQueue), BDL_QUEUE_TIME);
}
