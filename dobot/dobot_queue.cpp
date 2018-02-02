#include "dobot_queue.h"

DobotQueue::DobotQueue(Dobot *pDobot)
{
    _pDobot = pDobot;
    _pServo = pDobot->getServoPointer();

    _n64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max();
    _n64RealTimeDobotActualID = 0;
    _n64RetreatID = 0;
    _n64LastDobotIDShownInUI = 0;
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return; //future

    GetQueuedCmdCurrentID(&_n64RealTimeDobotActualID);

    if (!_pServo->isServoListEmpty())
        _pServo->moveServoManually();

    this->showLastExecutedArmMoveInUI();

    if (this->isNextPhysicalMoveToQueueOnArmCanBeSend)
    {
        this->queuePhysicalMoveOnArm(getNextPhysicalMoveToQueueOnArm);
        this->removeOldQueuedMovesFromCore();
    }

    //if (!this->isRetreatLastExecutedArmMoveType)
    //  this->retreat(); //todo: zrobić to po udanych ruchach dobota
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmCanBeSend()
{
    emit this->queueLabels(_unQueuedCmdLeftSpace, _n64RealTimeDobotActualID,
                           _n64CoreQueuedCmdID, _queuedCmdIDList.size(),
                           _lowestIDMoveInList.ID);

    if (!_queuedCmdIDList.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.next();
            if(_lowestIDMoveInList.ID - _n64RealTimeDobotActualID < 15)
                return true;

            emit showActualDobotQueuedCmdIDList(_queuedCmdIDList);
        }
    }

    return false;
}

DobotMove DobotQueue::getNextPhysicalMoveToQueueOnArm()
{
    if (!_queuedCmdIDList.isEmpty()) //2nd security checks is mandatory
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.next();
            if(_lowestIDMoveInList.ID - _n64RealTimeDobotActualID < 15)
                return _queuedCmdIDList.First();
        }
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getNextPhysicalMoveToQueueOnArm(): reached artificially "
                "created anty-error return (shouldn't be possible)";
    return errorMove;
}

void DobotQueue::showLastExecutedArmMoveInUI()
{
    if (_n64RealTimeDobotActualID > _n64LastDobotIDShownInUI)
    {
        DobotMove Move = this->getQueuedMoveInCore(_n64RealTimeDobotActualID);
        emit _pDobot->addTextToLogPTE("Executed move(ID,type): " + Move.ID + ","
                                      + dobotMoveAsQstr(Move.type), LOG_DOBOT);
        _n64LastDobotIDShownInUI = _n64RealTimeDobotActualID;
    }
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_queuedCmdIDList.removeOne(this->getQueuedMoveInCore(_n64RealTimeDobotActualID - 1)))
        qDebug() << "ERROR: DobotQueue::removeOldQueuedMovesFromCore(): move in list not found";
}

DobotMove DobotQueue::getQueuedMoveInCore(int64_t ID)
{
    Q_FOREACH(DobotMove Move, _queuedCmdIDList)
    {
        if (Move.ID == ID)
            return Move;
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getQueuedMoveInCore(): move with ID:"
             << ID << "doesn't exists";
    return errorMove;
}

/*bool DobotQueue::isRetreatLastExecutedArmMoveType()
{
    if (this->getQueuedMoveInCore(_n64CoreQueuedCmdID).type == )
        //future: jednak wprowadzam pojęcie "retreat" do klasy dobota
}*/

//todo: if retreat is set from xml
/*void DobotQueue::retreat() //todo
{
    if (_n64CoreQueuedCmdID <= _n64RealTimeDobotActualID)
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
        emit _pDobot->addTextToLogPTE("ERROR: Dobot queue memory full. Stop arm.\n", LOG_DOBOT);
        SetQueuedCmdForceStopExec(); //stop arm
    }
}*/

void DobotQueue::queuePhysicalMoveOnArm(DobotMove move)
{
    switch(move.type)
    {
    case DM_TO_POINT:
        //todo: zabronić oś z jezeli jest zbyt nisko
        PTPCmd moveAsPtpCmd;
        moveAsPtpCmd.ptpMode = PTPMOVLXYZMode; //ruch typu kartezjański liniowy
        //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
        moveAsPtpCmd.x = move.xyz.x;
        moveAsPtpCmd.y = move.xyz.y;
        moveAsPtpCmd.z = move.xyz.z;
        SetPTPCmd(&moveAsPtpCmd, true, &move.ID);
        break;
    case DM_OPEN:
        _pServo->openGripper(move.ID);
        break;
    case DM_CLOSE:
        _pServo->closeGripper(move.ID);
        break;
    case DM_CALIBRATE:
    {
        emit _pDobot->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd HOME;
        HOME.reserved = 1; //todo: o co tutaj dokładnie chodzi z tym indexem?
        int result = SetHOMECmd(&HOME, true, &move.ID);
        if (result != DobotCommunicate_NoError)
            qDebug() << "ERROR: DobotQueue::queuePhysicalMoveOnArm(): "
                        "SetHOMECmd result != DobotCommunicate_NoError";
    }
        break;
    case DM_UP:
    case DM_DOWN:
        _pServo->addServoMoveToGripperStatesList(Move);
        break;
    default:
        qDebug() << "ERROR: Dobot::queuePhysicalMoveOnArm(): wrong move type:"
                 << move.type;
    }
}

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    _n64CoreQueuedCmdID += 1;

    DobotMove cmdToQueue;
    cmdToQueue.ID = _n64CoreQueuedCmdID;
    cmdToQueue.type = Type;
    cmdToQueue.xyz = point;

    _queuedCmdIDList << cmdToQueue;
}

void DobotQueue::saveIDFromConnectedDobot()
{
   int result = GetQueuedCmdCurrentID(&_n64RealTimeDobotActualID);
   if (result == DobotCommunicate_NoError)
   {
       emit this->queueLabels(_unQueuedCmdLeftSpace, _n64RealTimeDobotActualID,
                              _n64CoreQueuedCmdID, _queuedCmdIDList.size(),
                              _lowestIDMoveInList.index);
   }
   else
   {
       qDebug() << "ERROR: DobotQueue::onConnectDobot(): saveIDFromConnectedDobot"
                   " gone wrong";
   }

    _n64CoreQueuedCmdID = _n64RealTimeDobotActualID;
}
