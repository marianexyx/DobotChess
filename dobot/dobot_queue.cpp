#include "dobot_queue.h"

DobotQueue::DobotQueue(Dobot *pDobot)
{
    _pDobot = pDobot;
    _pServo = pDobot->getServoPointer();

    _n64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max();
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return;

    GetQueuedCmdCurrentID(&_n64RealTimeDobotActualID);

    if (!_pServo->isServoListEmpty())
        _pServo->moveServoManually();

    if (!_queuedCmdIDList.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.next();
            if(firstMoveInList.ID - _n64RealTimeDobotActualID < 15)
                this->queuePhysicalMoveOnArm(_queuedCmdIDList.takeFirst());

            emit showActualDobot_queuedCmdIDList(_queuedCmdIDList);
        }
    }

    emit this->queueLabels(_unQueuedCmdLeftSpace, _n64RealTimeDobotActualID,
                           _n64CoreQueuedCmdID, _queuedCmdIDList.size(),
                           _lowestIDMoveInList.ID);

    //this->retreat(); //todo: zrobić to po udanych ruchach dobota
}

//todo: if retreat is set from xml
/*void DobotQueue::retreat() //todo
{
    if (_n64RetreatID <= _n64RealTimeDobotActualID)
    {
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): retreat";
        PtpCmdActualVal retreatId;
        retreatId = (_pose.y >= middleAboveBoard.y) ?  retreatYPlus : retreatYMinus;
        addCmdToList(DM_TO_POINT, retreatId.x, retreatId.y, retreatId.z);
        _n64RetreatID = std::numeric_limits<int64_t>::max();
    }
}*/

//future: póki co nie ma w dobocie (w dll) tej funkcji. Może z czasem dodadzą.
/*bool DobotQueue::isDobotCmdsLeftSpaceEmpty()
{
    GetQueuedCmdLeftSpace(&_unQueuedCmdLeftSpace);
    if (_unQueuedCmdLeftSpace <= 0)
    {
        qDebug() << "ERROR: Dobot queue memory  full. data overflown/lost. Stop arm.";
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
    case DM_HOME:
    {
        emit _pDobot->addTextToLogPTE("HOMECmd: recalibrating arm...\n", LOG_DOBOT);

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

void DobotQueue::addCmdToList(DOBOT_MOVE_TYPE Type, Point3D point)
{
    //todo: usuwać z tablicy wiadomości na dobota wiadomości te które już poszły (może minus 1)
    _n64CoreQueuedCmdID += 1; //aktualne id ruchu = +1 większe od ostatniego

    DobotMove cmdToQueue;
    cmdToQueue.ID = _n64CoreQueuedCmdID;
    cmdToQueue.type = Type;
    cmdToQueue.xyz = point;

    _queuedCmdIDList << cmdToQueue;
}

void DobotQueue::saveIDFromConnectedDobot()
{
    //sprawdź aktualny id i zapisz w zmiennej
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
