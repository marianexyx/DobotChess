#include "chess_timers.h"

ChessTimers::ChessTimers(Clients* pClientsList):
    _lTimersStartTime(1000*60*30), //30min
    _lTimersStartQueue(1000*60*2) //2min
{
    _pClientsList = pClientsList;

    _whiteTimer = new QTimer();
    _blackTimer = new QTimer();
    _updateLabelTimer = new QTimer();
    _startQueueTimer = new QTimer();
    _whiteTimer->setInterval(_lTimersStartTime);
    _blackTimer->setInterval(_lTimersStartTime);
    _updateLabelTimer->setInterval(1000);
    _startQueueTimer->setInterval(_lTimersStartQueue);
    _whiteTimer->setSingleShot(true);
    _blackTimer->setSingleShot(true);
    _updateLabelTimer->setSingleShot(false);
    _startQueueTimer->setSingleShot(true);
    connect(_whiteTimer, SIGNAL(timeout()), this, SLOT(playerTimeOut(PT_WHITE)));
    connect(_blackTimer, SIGNAL(timeout()), this, SLOT(playerTimeOut(PT_BLACK)));
    connect(_updateLabelTimer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    connect(_startQueueTimer, SIGNAL(timeout()), this, SLOT(startTimeOut()));
    _nRemainingWhiteTime = _lTimersStartTime;
    _nRemainingBlackTime = _lTimersStartTime;
}

void ChessTimers::updateTimeLabels()
{
    if (_whiteTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(_whiteTimer->remainingTime()),
                                       BDL_WHITE_TIME);
    else if (_blackTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(_blackTimer->remainingTime()),
                                       BDL_BLACK_TIME);

    if (_startQueueTimer->isActive())
        emit this->setBoardDataLabel(this->milisecToClockTime(_startQueueTimer->remainingTime()),
                                       BDL_QUEUE_TIME);

    emit this->setBoardDataLabel(this->milisecToClockTime(_lTimersStartQueue), BDL_QUEUE_TIME);
}

void ChessTimers::startTimeOut()
{    
    this->stopQueueTimer();
    emit this->timeOutStart();
}

void ChessTimers::startGameTimer()
{
    _whiteTimer->start();
    _updateLabelTimer->start();
}

void ChessTimers::stopBoardTimers()
{
    _whiteTimer->stop();
    _blackTimer->stop();
    _updateLabelTimer->stop();
}

void ChessTimers::resetGameTimers()
{
    this->stopBoardTimers();
    _whiteTimer->setInterval(_lTimersStartTime);
    _blackTimer->setInterval(_lTimersStartTime);
    _nRemainingWhiteTime = _whiteTimer->interval();
    _nRemainingBlackTime = _blackTimer->interval();
    emit this->setBoardDataLabel(this->milisecToClockTime(_lTimersStartTime), BDL_WHITE_TIME);
    emit this->setBoardDataLabel(this->milisecToClockTime(_lTimersStartTime), BDL_BLACK_TIME);
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

void ChessTimers::switchPlayersTimers(WHOSE_TURN Turn)
{
    if (Turn == WHITE_TURN)
    {
        _nRemainingBlackTime = _blackTimer->remainingTime();
        _blackTimer->stop();

        _whiteTimer->setInterval(_nRemainingWhiteTime);
        _whiteTimer->start();
    }
    else if (Turn == BLACK_TURN)
    {
        _nRemainingWhiteTime = _whiteTimer->remainingTime();
        _whiteTimer->stop();

        _blackTimer->setInterval(_nRemainingBlackTime);
        _blackTimer->start();
    }
    else qDebug() << "ERROR: ChessTimers::switchPlayersTimers(): getWhoseTurn isn't "
                     "white or black.  it's' ="
                  << turnTypeAsQstr(Turn);
}

GAME_STATUS ChessTimers::startQueueTimer()
{
    qDebug() << "ChessTimers::startQueueTimer()";
    _startQueueTimer->stop();
    _startQueueTimer->setInterval(_lTimersStartQueue);
    emit this->setBoardDataLabel(this->milisecToClockTime(_startQueueTimer->remainingTime()),
                                   BDL_QUEUE_TIME);
    _startQueueTimer->start();
    _updateLabelTimer->start();

    return GS_TURN_NONE_WAITING_FOR_START_CONFIRMS;
}

void ChessTimers::stopQueueTimer() //stop == reset
{
    _startQueueTimer->stop();
    _updateLabelTimer->stop();
    this->updateTimeLabels();
}

//get/set:

int ChessTimers::getWhiteTimeLeft()
{
    if (_whiteTimer->remainingTime() != -1)
        return _whiteTimer->remainingTime();
    else return _nRemainingWhiteTime;
}

int ChessTimers::getBlackTimeLeft()
{
    if (_blackTimer->remainingTime() != -1)
        return _blackTimer->remainingTime();
    else return _nRemainingBlackTime;
}
