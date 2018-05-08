#include "dobot_queue.h"

DobotQueue::DobotQueue(/*Dobot *pDobot*/)
{
    //_pDobot = pDobot; //todo: używm tylko do emitowaia sygnału
    //_pServo = pDobot->getServoPointer();

    _un64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max(); //future
    _un64RealTimeDobotActualID = 0;
    _un64RetreatID = 0;
    _un64LastDobotIDShownInUI = 0;
    _lowestIDMoveInList = 0;
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return; //future
    isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                            SHOW_ERRORS);

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        emit this->sendMoveToArm(this->getNextMoveToSendToArm());
        this->removeOldQueuedMovesFromCore();
    }

    //if (!this->isRetreatLastExecutedArmMoveType)
    //  this->retreat(); //todo: zrobić to po udanych ruchach dobota
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

            emit this->showActualDobotQueuedCmdIDList(_queuedArmCmds);
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

            if (_lowestIDMoveInList >= _un64RealTimeDobotActualID)
            {
                if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                {
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
    if (_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI
            && _un64LastDobotIDShownInUI > 0)
    {
        QString QStrMoveID = "";
        QString QStrDobotMove = "";

        DobotMove Move = this->getQueuedMove(_executedArmCmds, _un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit this->addTextToLogPTEInUI("Executed move ID = " + QStrMoveID +
                                       " type = " + QStrDobotMove + "\n", LOG_DOBOT);
    }

    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_executedArmCmds.empty() && _un64RealTimeDobotActualID > _executedArmCmds.first().ID)
        _executedArmCmds.removeFirst();
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

/*bool DobotQueue::isRetreatLastExecutedArmMoveType()
{
    if (this->getQueuedMove(_un64CoreQueuedCmdID).type == )
        //future: jednak wprowadzam pojęcie "retreat" do klasy dobota
}*/

//todo: if retreat is set from xml
/*void DobotQueue::retreat() //todo
{
    if (_un64CoreQueuedCmdID <= _un64RealTimeDobotActualID)
    {
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): retreat";
        PtpCmdActualVal retreatId;
        retreatId = (_pose.y >= middleAboveBoard.y) ?  retreatYPlus : retreatYMinus;
        addArmMoveToQueue(DM_TO_POINT, retreatId.x, retreatId.y, retreatId.z);
    }
}*/

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
    qDebug() << "DobotQueue::addArmMoveToQueue(): type =" << dobotMoveAsQstr(Type)
             << ", point =" << point.getAsQStr() << ", ID =" << _un64CoreQueuedCmdID + 1;

    if (!isPointInLimits(point)) return;
    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedArmCmds << cmdToQueue;
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

    _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}
