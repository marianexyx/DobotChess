#include "dobot_queue.h"

DobotQueue::DobotQueue(IntermediatePoints intermediatePoints)
{
    _un64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max(); //future:
    _un64RealTimeDobotActualID = 0;
    _un64EscapeID = 0;
    _un64LastDobotIDShownInUI = 0;
    _lowestIDMoveInCoreList = 0;
    _escapeLeft = intermediatePoints.retreatLeft;
    _escapeRight = intermediatePoints.retreatRight;
    _bEscape = false;
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

        emit this->showQueuedArmCmdsOnCore(this->getQueuedArmCmds());
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable()
{
    emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                   _un64CoreQueuedCmdID, _queuedArmCmdsOnCore.size(),
                                   _lowestIDMoveInCoreList);

    if (!_queuedArmCmdsOnCore.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmdsOnCore);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if (QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInCoreList = QueuedCmdIDIter.peekNext().ID;

            if (_lowestIDMoveInCoreList >= _un64RealTimeDobotActualID)
            {
                if (_lowestIDMoveInCoreList - _un64RealTimeDobotActualID < 15)
                    return true;
            }
            else qCritical() << "_lowestIDMoveInCoreList < _un64RealTimeDobotActualID ("
                          << QString::number(_lowestIDMoveInCoreList) << "<"
                          << QString::number(_un64RealTimeDobotActualID) << ")";
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
        if (QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInCoreList = QueuedCmdIDIter.peekNext().ID;
            //future: why those 2 qdebugs below must be here for funtion to work?
            ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
            qInfo() << "new _lowestIDMoveInCoreList =" << QString::number(_lowestIDMoveInCoreList);

            if (_lowestIDMoveInCoreList >= _un64RealTimeDobotActualID)
            {
                //32 moves was dobot max memory capacity, so 15 moves sent will be safe
                if(_lowestIDMoveInCoreList - _un64RealTimeDobotActualID < 15)
                {
                    ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
                    qInfo() << "return next queued move. type ="
                             << dobotMoveAsQstr(_queuedArmCmdsOnCore.first().type)
                             << ", ID =" << QString::number(_queuedArmCmdsOnCore.first().ID)
                             << ", point =" << _queuedArmCmdsOnCore.first().xyz.getAsQStr();
                    _sentArmCmdsToDobot << _queuedArmCmdsOnCore.first();

                    return _queuedArmCmdsOnCore.takeFirst();
                }
            }
            else qCritical() << "_lowestIDMoveInCoreList < _un64RealTimeDobotActualID ("
                             << QString::number(_lowestIDMoveInCoreList) << "<"
                             << QString::number(_un64RealTimeDobotActualID) << ")";
        }
    }

    qCritical() << "reached artificially created anty-error return (shouldn't be possible)";
    return *new DobotMove;
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
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }

    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_sentArmCmdsToDobot.empty()
            && _un64RealTimeDobotActualID > _sentArmCmdsToDobot.first().ID)
    {
        _sentArmCmdsToDobot.removeFirst();
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }
}

DobotMove DobotQueue::getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID)
{
    foreach(DobotMove move, cmdsList)
    {
        if (move.ID == un64ID)
            return move;
    }

    qCritical() << "move with ID nr" << QString::number(un64ID) << "doesn't exists";
    return *new DobotMove;
}

bool DobotQueue::isArmCoveringView()
{
    if (_bEscape && _un64RealTimeDobotActualID > _un64EscapeID &&
            _un64RealTimeDobotActualID >= _un64CoreQueuedCmdID && _queuedArmCmdsOnCore.isEmpty())
        return true;
    else return false;
}

void DobotQueue::escape(Point3D lastPoint)
{
    if (qAbs(lastPoint.y - _escapeLeft.y) > qAbs(lastPoint.y - _escapeRight.y))
        this->addArmMoveToQueue(DM_TO_POINT, _escapeRight);
    else this->addArmMoveToQueue(DM_TO_POINT, _escapeLeft);

    _un64EscapeID = _un64CoreQueuedCmdID;
    _bEscape = false; //prevent unwanted retreats
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
        qCritical() << "isPointInLimits == false. move type =" << dobotMoveAsQstr(Type);
        return;
    }

    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedArmCmdsOnCore << cmdToQueue;

    emit this->showQueuedArmCmdsOnCore(this->getQueuedArmCmds());
}

void DobotQueue::saveIDFromConnectedDobot()
{
    if (isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                SHOW_ERRORS))
   {
       emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                      _un64CoreQueuedCmdID, _queuedArmCmdsOnCore.size(),
                                      _lowestIDMoveInCoreList);
   }

    _un64EscapeID = _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}

QString DobotQueue::dumpAllData()
{
    QString QStrData;

    QStrData = "[dobot_queue.h]\n";
    QStrData += "_un64CoreQueuedCmdID: " + QString::number(_un64CoreQueuedCmdID) + "\n";
    QStrData += "_un64RealTimeDobotActualID: "
            + QString::number(_un64RealTimeDobotActualID) + "\n";
    QStrData += "_unQueuedCmdLeftSpace: " + QString::number(_unQueuedCmdLeftSpace) + "\n";
    QStrData += "_queuedArmCmdsOnCore: " + this->getQueuedArmCmds() + "\n";
    QStrData += "_sentArmCmdsToDobot: " + this->getSentArmCmds() + "\n";
    QStrData += "_lowestIDMoveInCoreList: " + QString::number(_lowestIDMoveInCoreList) + "\n";
    QStrData += "_un64LastDobotIDShownInUI: "
            + QString::number(_un64LastDobotIDShownInUI) + "\n";
    QStrData += "_un64EscapeID: " + QString::number(_un64EscapeID) + "\n";
    QStrData += "_bEscape: " + QString::number(_bEscape) + "\n";

    return QStrData;
}

QString DobotQueue::getQueuedArmCmds()
{
    QString QStrQueuedList;
    DobotMove item;

    for(int i=0; i<_queuedArmCmdsOnCore.count(); ++i)
    {
       item = _queuedArmCmdsOnCore.at(i);
       QStrQueuedList += QString::number(item.ID) + ". " + dobotMoveAsQstr(item.type)
               + ": " + item.xyz.getAsQStr() + "\n";
    }

    return QStrQueuedList;
}

QString DobotQueue::getSentArmCmds()
{
    QString QStrQueuedList;
    DobotMove item;

    for(int i=0; i<_sentArmCmdsToDobot.count(); ++i)
    {
       item = _sentArmCmdsToDobot.at(i);
       if (_un64RealTimeDobotActualID == item.ID)
           QStrQueuedList += ">";
       QStrQueuedList += QString::number(item.ID) + ". " +  dobotMoveAsQstr(item.type)
               + ": " + item.xyz.getAsQStr() + "\n";
    }

    return QStrQueuedList;
}
