#include "dobot_queue.h"

DobotQueue::DobotQueue(Point3D escape1, Point3D escape2)
{
    _un64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max(); //future:
    _un64RealTimeDobotActualID = 0;
    _un64RetreatID = 0;
    _un64LastDobotIDShownInUI = 0;
    _lowestIDMoveInList = 0;
    _escape1 = escape1;
    _escape2 = escape2;
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
        emit this->showQueuedArmCmdsOnCore();
        emit this->showSentArmCmdsToDobot();
    }
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable()
{
    emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                   _un64CoreQueuedCmdID, _queuedArmCmdsOnCore.size(),
                                   _lowestIDMoveInList);

    if (!_queuedArmCmdsOnCore.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmdsOnCore);
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
    if (!_queuedArmCmdsOnCore.isEmpty()) //2nd security checks is mandatory
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmdsOnCore);
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
                             << dobotMoveAsQstr(_queuedArmCmdsOnCore.first().type) << ", ID ="
                             << _queuedArmCmdsOnCore.first().ID << ", point ="
                             << _queuedArmCmdsOnCore.first().xyz.getAsQStr();
                    _sentArmCmdsToDobot << _queuedArmCmdsOnCore.first();
                    return _queuedArmCmdsOnCore.takeFirst();
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

        DobotMove Move = this->getQueuedMove(_sentArmCmdsToDobot, _un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit this->addTextToLogPTEInUI("Executed move ID = " + QStrMoveID +
                                       ", type = " + QStrDobotMove + "\n", LOG_DOBOT);
        emit this->showSentArmCmdsToDobot();
    }

    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_sentArmCmdsToDobot.empty() && _un64RealTimeDobotActualID > _sentArmCmdsToDobot.first().ID)
    {
        _sentArmCmdsToDobot.removeFirst();
        emit this->showSentArmCmdsToDobot();
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
            _un64RealTimeDobotActualID >= _un64CoreQueuedCmdID && _queuedArmCmdsOnCore.isEmpty())
        return true;
    else return false;
}

void DobotQueue::retreat(Point3D lastPoint)
{
    if (qAbs(lastPoint.y - _escape1.y) > qAbs(lastPoint.y - _escape2.y))
        this->addArmMoveToQueue(DM_TO_POINT, _escape2);
    else this->addArmMoveToQueue(DM_TO_POINT, _escape1);

    _un64RetreatID = _un64CoreQueuedCmdID;
    _bRetreat = false; //prevent unwanted retreats
}

//future: dobot wanted to introduce this function in dll, but till now it isn't
/*bool DobotQueue::isDobotCmdsLeftSpaceEmpty()
{
    GetQueuedCmdLeftSpace(&_unQueuedCmdLeftSpace);
    if (_unQueuedCmdLeftSpace <= 0)
    {
        emit showQueueLabelsInUI->addTextToLogPTE("ERROR: Dobot queue memory full. "
            "Stop arm.\n", LOG_DOBOT);
        SetQueuedCmdForceStopExec(); //stop arm
    }
}*/

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{    
    if (Type != DM_OPEN && Type != DM_CLOSE && !XmlReader::isPointInLimits(point))
    {
        qDebug() << "ERROR: DobotQueue::addArmMoveToQueue(): isPointInLimits == "
                    "false. move type =" << dobotMoveAsQstr(Type);
        return;
    }

    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedArmCmdsOnCore << cmdToQueue;
    emit this->showQueuedArmCmdsOnCore();
}

void DobotQueue::saveIDFromConnectedDobot()
{
    if (isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                SHOW_ERRORS))
   {
       emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                      _un64CoreQueuedCmdID, _queuedArmCmdsOnCore.size(),
                                      _lowestIDMoveInList);
   }

    _un64RetreatID = _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}
