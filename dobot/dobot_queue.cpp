#include "dobot_queue.h"

DobotQueue::DobotQueue(Point3D retreatLeft, Point3D retreatRight)
{
    _un64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max(); //future:
    _un64RealTimeDobotActualID = 0;
    _un64RetreatID = 0;
    _un64LastDobotIDShownInUI = 0;
    _lowestIDMoveInList = 0;
    _retreatLeft = retreatLeft;
    _retreatRight = retreatRight;
    _bRetreat = false;
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return; //future:
    isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                            SHOW_ERRORS);

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        emit this->sendMoveToArm(this->getNextMoveToSendToArm());
        this->removeOldQueuedMovesFromCore();
        emit this->showActualDobotQueuedCmdIDList(_queuedArmCmds);
        emit this->showOnDobotQueuedCmdsList(_executedArmCmds);
    }
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable()
{
    emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                   _un64CoreQueuedCmdID, _queuedArmCmds.size(),
                                   _lowestIDMoveInList);

    if (!_queuedArmCmds.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmds);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;

            if (_lowestIDMoveInList >= _un64RealTimeDobotActualID)
            {
                if (_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                    return true;
            }
            else qDebug() << "ERROR: DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable(): "
                             "_lowestIDMoveInList < _un64RealTimeDobotActualID ("
                          << _lowestIDMoveInList << "<" << _un64RealTimeDobotActualID << ")";
        }
    }

    return false;
}

DobotMove DobotQueue::getNextMoveToSendToArm()
{
    if (!_queuedArmCmds.isEmpty()) //2nd security checks is mandatory
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmds);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;
            //future: why those 2 qdebugs below must be here for funtion to work?
            ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
            qDebug() << "DobotQueue::getNextMoveToSendToArm(): "
                        "new _lowestIDMoveInList =" << _lowestIDMoveInList;

            if (_lowestIDMoveInList >= _un64RealTimeDobotActualID)
            {
                if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                {
                    ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
                    qDebug() << "DobotQueue: return next queued move. type ="
                             << dobotMoveAsQstr(_queuedArmCmds.first().type) << ", ID ="
                             << _queuedArmCmds.first().ID << ", point ="
                             << _queuedArmCmds.first().xyz.getAsQStr();
                    _executedArmCmds << _queuedArmCmds.first();
                    return _queuedArmCmds.takeFirst();
                }
            }
            else qDebug() << "ERROR: DobotQueue::getNextMoveToSendToArm(): "
                             "_lowestIDMoveInList < _un64RealTimeDobotActualID ("
                          << _lowestIDMoveInList << "<"
                          << _un64RealTimeDobotActualID << ")";
        }
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getNextMoveToSendToArm(): reached artificially "
                "created anty-error return (shouldn't be possible)";
    return errorMove;
}

void DobotQueue::showLastExecutedArmMoveInUI()
{
    if (_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI && _un64LastDobotIDShownInUI > 0)
    {
        QString QStrMoveID = "";
        QString QStrDobotMove = "";

        DobotMove Move = this->getQueuedMove(_executedArmCmds, _un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit this->addTextToLogPTEInUI("Executed move ID = " + QStrMoveID +
                                       " type = " + QStrDobotMove + "\n", LOG_DOBOT);
        emit this->showOnDobotQueuedCmdsList(_executedArmCmds);
    }

    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_executedArmCmds.empty() && _un64RealTimeDobotActualID > _executedArmCmds.first().ID)
    {
        _executedArmCmds.removeFirst();
        emit this->showOnDobotQueuedCmdsList(_executedArmCmds); //todo: automatycznie niech..
        //...wkleja tutaj listę, by nie trzeba było zawsze wpisywać parametru
    }
}

DobotMove DobotQueue::getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID)
{
    Q_FOREACH(DobotMove move, cmdsList)
    {
        if (move.ID == un64ID)
            return move;
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getQueuedMove(): move with ID nr"
             << un64ID << "doesn't exists";
    return errorMove;
}

bool DobotQueue::isArmCoveringGame()
{
    if (_bRetreat && _un64RealTimeDobotActualID > _un64RetreatID &&
            _un64RealTimeDobotActualID >= _un64CoreQueuedCmdID && _queuedArmCmds.isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DobotQueue::retreat(Point3D lastPoint)
{
    qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): retreat";

    if (qAbs(lastPoint.y - _retreatLeft.y) > qAbs(lastPoint.y - _retreatRight.y))
        this->addArmMoveToQueue(DM_TO_POINT, _retreatRight);
    else this->addArmMoveToQueue(DM_TO_POINT, _retreatLeft);

    qDebug() << "DobotQueue::retreat(): assign new _un64RetreatID =" << _un64RetreatID;
    _un64RetreatID = _un64CoreQueuedCmdID;
    _bRetreat = false; //prevent unwanted retreats
}

//future: póki co nie ma w dobocie (w dll) tej funkcji. Może z czasem dodadzą.
/*bool DobotQueue::isDobotCmdsLeftSpaceEmpty()
{
    GetQueuedCmdLeftSpace(&_unQueuedCmdLeftSpace);
    if (_unQueuedCmdLeftSpace <= 0)
    {
        emit showQueueLabelsInUI->addTextToLogPTE("ERROR: Dobot queue memory full. Stop arm.\n", LOG_DOBOT);
        SetQueuedCmdForceStopExec(); //stop arm
    }
}*/

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    if (!isPointInLimits(point)) return;
    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedArmCmds << cmdToQueue;
    emit this->showActualDobotQueuedCmdIDList(_queuedArmCmds);
}

void DobotQueue::saveIDFromConnectedDobot()
{
    if (isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                SHOW_ERRORS))
   {
       emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                      _un64CoreQueuedCmdID, _queuedArmCmds.size(),
                                      _lowestIDMoveInList);
   }

    _un64RetreatID = _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
    qDebug() << "DobotQueue::saveIDFromConnectedDobot(): assign new _un64RetreatID ="
             << _un64RetreatID << ", new _un64CoreQueuedCmdID =" << _un64CoreQueuedCmdID
             << ", readed _un64RealTimeDobotActualID =" << _un64RealTimeDobotActualID;
}
