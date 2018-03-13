#include "dobot_queue.h"

DobotQueue::DobotQueue(Dobot *pDobot)
{
    _pDobot = pDobot;
    _pServo = pDobot->getServoPointer();

    _un64CoreQueuedCmdID = 1; //set 1st ID
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max();
    _un64RealTimeDobotActualID = 0;
    _un64RetreatID = 0;
    _un64LastDobotIDShownInUI = 0;
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return; //future
    Dobot::isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                   SHOW_ERRORS);

    if (!_pServo->isServoListEmpty())
        _pServo->moveServoManually();

    this->showLastExecutedArmMoveInUI();

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        this->queuePhysicalMoveOnArm(this->getNextPhysicalMoveToQueueOnArm());
        this->removeOldQueuedMovesFromCore();
    }

    //if (!this->isRetreatLastExecutedArmMoveType)
    //  this->retreat(); //todo: zrobić to po udanych ruchach dobota
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable()
{
    emit _pDobot->queueLabels(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                           _un64CoreQueuedCmdID, _queuedCmdIDList.size(),
                           _lowestIDMoveInList);

    if (!_queuedCmdIDList.isEmpty())
    {
        QListIterator<DobotMove*> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            //todo: ten zapis poniżej to jakiś dramat. zadziałą to wogle?
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext()->ID;
            if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
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
        QListIterator<DobotMove*> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext()->ID;
            if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                return *_queuedCmdIDList.first();
        }
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getNextPhysicalMoveToQueueOnArm(): reached artificially "
                "created anty-error return (shouldn't be possible)";
    return errorMove;
}

void DobotQueue::showLastExecutedArmMoveInUI()
{
    if (_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI)
    {
        DobotMove Move = this->getQueuedMoveInCore(_un64RealTimeDobotActualID);
        emit _pDobot->addTextToLogPTE("Executed move(ID,type): " + QString::number(Move.ID)
                                      + "," + dobotMoveAsQstr(Move.type), LOG_DOBOT);
        _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
    }
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    /* todo: tu jest błąD
    if (!_queuedCmdIDList.removeOne(this->getQueuedMoveInCore(_un64RealTimeDobotActualID - 1)))
        qDebug() << "ERROR: DobotQueue::removeOldQueuedMovesFromCore(): move in list not found";
    */
}

DobotMove DobotQueue::getQueuedMoveInCore(uint64_t un64ID)
{
    Q_FOREACH(DobotMove* Move, _queuedCmdIDList)
    {
        if (Move->ID == un64ID)
            return *Move;
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getQueuedMoveInCore(): move with ID:"
             << un64ID << "doesn't exists";
    return errorMove;
}

/*bool DobotQueue::isRetreatLastExecutedArmMoveType()
{
    if (this->getQueuedMoveInCore(_un64CoreQueuedCmdID).type == )
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
        emit _pDobot->addTextToLogPTE("ERROR: Dobot queue memory full. Stop arm.\n", LOG_DOBOT);
        SetQueuedCmdForceStopExec(); //stop arm
    }
}*/

void DobotQueue::queuePhysicalMoveOnArm(DobotMove move)
{
    switch(move.type)
    {
    case DM_TO_POINT:
    {
        //todo: zabronić oś z jezeli jest zbyt nisko
        PTPCmd moveAsPtpCmd;
        moveAsPtpCmd.ptpMode = PTPMOVLXYZMode; //ruch typu kartezjański liniowy
        //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
        moveAsPtpCmd.x = move.xyz.x;
        moveAsPtpCmd.y = move.xyz.y;
        moveAsPtpCmd.z = move.xyz.z;
        Dobot::isArmReceivedCorrectCmd(SetPTPCmd(&moveAsPtpCmd, true, &move.ID), SHOW_ERRORS);
        break;
    }
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
        Dobot::isArmReceivedCorrectCmd(SetHOMECmd(&HOME, true, &move.ID), SHOW_ERRORS);
    }
        break;
    case DM_UP:
    case DM_DOWN:
        _pServo->addServoMoveToGripperStatesList(move.type);
        break;
    default:
        qDebug() << "ERROR: Dobot::queuePhysicalMoveOnArm(): wrong move type:" << move.type;
    }
}

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    _un64CoreQueuedCmdID += 1;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedCmdIDList << &cmdToQueue;
}

void DobotQueue::saveIDFromConnectedDobot()
{
   if (Dobot::isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                      SHOW_ERRORS))
   {
       emit _pDobot->queueLabels(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                              _un64CoreQueuedCmdID, _queuedCmdIDList.size(),
                              _lowestIDMoveInList);
   }

    _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}
