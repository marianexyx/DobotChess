#include "chess_timers.h"

ChessTimers::ChessTimers(Chess *pChess):
    _lTimersStartTime(1000*60*30), //1000ms (1s) * 60s * 30min
    _lTimersStartQueue(1000*60*2)
{
    _pChess = pChess;

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
    connect(_startQueueTimer, SIGNAL(timeout()), this, SLOT(timeOutStartQueue()));
    _nRemainingWhiteTime = _lTimersStartTime;
    _nRemainingBlackTime = _lTimersStartTime;
}

void ChessTimers::playerTimeOut(PLAYER_TYPE player)
{
    QString QStrMsg;
    switch(player)
    {
    case PT_WHITE: QStrMsg = "timeOutWhite"; break;
    case PT_BLACK: QStrMsg = "timeOutBlack"; break;
    default:
        qDebug() << "ERROR: ChessTimers::playerTimeOut: unknwon PLAYER_TYPE:"
                 << player;
        return;
    }

    _pChess->restartGame(ET_TIMEOUT_GAME, _pChess->getClientsPointer()->getPlayer(player));
    _pChess->sendDataToClient(QStrMsg);
}

void ChessTimers::updateTimeLabels()
{
    if (_whiteTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(_whiteTimer->remainingTime()), BDL_WHITE_TIME);
    else if (_blackTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(_blackTimer->remainingTime()), BDL_BLACK_TIME);

    if (_startQueueTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    else setBoardDataLabels(milisecToClockTime(_lTimersStartQueue), BDL_QUEUE_TIME);
}

void ChessTimers::timeOutStartQueue()
{
    //todo: 2x stopQueueTimer() mamy tu
    this->stopQueueTimer();
    //todo: możnaby zamknać w funkcji clientlistowej (sprawdzić resztę powtórzeń)
    //todo: poza tym z nazwy funkcji wogle nie wiadomo że to tu się dzieje
    //cleanSleepyPlayersChairs
    Clients pClients = _pChess->getClientsPointer();
    if (!pClients.isStartClickedByPlayer(PT_WHITE))
        pClients.cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
    if (!this->isStartClickedByPlayer(PT_BLACK))
        pClients.cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);

    pClients.resetPlayersStartConfirmInfo();
     this->stopQueueTimer();

    if (this->isGameTableOccupied())
        this->startQueueTimer();

    _pChess->sendDataToAllClients("timeOutStartQueue");
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
    emit setBoardDataLabels(milisecToClockTime(_lTimersStartTime), BDL_WHITE_TIME);
    emit setBoardDataLabels(milisecToClockTime(_lTimersStartTime), BDL_BLACK_TIME);
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
        _nRemainingBlackTime = _blackTimer->remainingTime();
        _blackTimer->stop();

        _whiteTimer->setInterval(_nRemainingWhiteTime);
        _whiteTimer->start();
    }
    else if (getWhoseTurn() == BLACK_TURN)
    {
        _nRemainingWhiteTime = _whiteTimer->remainingTime();
        _whiteTimer->stop();

        _blackTimer->setInterval(_nRemainingBlackTime);
        _blackTimer->start();
    }
    else qDebug() << "ERROR: Chessboard::switchPlayersTimers(): getWhoseTurn isn't "
                     "white or black.  it's' ==" << _pChess->getStatusPointer()->getWhoseTurn();
}

void ChessTimers::startQueueTimer()
{
    qDebug() << "Chessboard::startQueueTimer()";
    _startQueueTimer->stop();
    _startQueueTimer->setInterval(_lTimersStartQueue);
    emit setBoardDataLabels(milisecToClockTime(_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    _startQueueTimer->start();
    _updateLabelTimer->start();
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
