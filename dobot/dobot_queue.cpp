#include "dobot_queue.h"

DobotQueue::DobotQueue(Dobot *pDobot)
{
    _pDobot = pDobot;
    _pServo = pDobot->getServoPointer();

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
    Dobot::isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                   SHOW_ERRORS);

    if (!_pServo->isServoListEmpty())
        _pServo->moveServoManually();

    this->showLastExecutedArmMoveInUI();

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): "
                    "isNextPhysicalMoveToQueueOnArmAvailable = true";
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
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            //todo: ten zapis poniżej to jakiś dramat. zadziała to wogle? to...
            //... nie usuwa pobranego wyniku
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;
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
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedCmdIDList);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;
            qDebug() << "DobotQueue::getNextPhysicalMoveToQueueOnArm(): "
                        "new _lowestIDMoveInList =" << _lowestIDMoveInList;
            if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                return _queuedCmdIDList.first();
        }
    }

    DobotMove errorMove;
    qDebug() << "ERROR: DobotQueue::getNextPhysicalMoveToQueueOnArm(): reached artificially "
                "created anty-error return (shouldn't be possible)";
    return errorMove;
}

void DobotQueue::showLastExecutedArmMoveInUI()
{
    if (_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI
            && _un64LastDobotIDShownInUI > 0)
    {
        QString QStrMoveID = 0;
        QString QStrDobotMove = 0;

        DobotMove Move = this->getQueuedMoveInCore(_un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit _pDobot->addTextToLogPTE("Executed move(ID,type): " + QStrMoveID
                                      + "," + QStrDobotMove, LOG_DOBOT);
    }

    /*qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): "
                "_un64LastDobotIDShownInUI =" << _un64LastDobotIDShownInUI
             << ", _un64RealTimeDobotActualID =" << _un64RealTimeDobotActualID;*/
    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    /* todo: tu jest błąd
    if (!_queuedCmdIDList.removeOne(this->getQueuedMoveInCore(_un64RealTimeDobotActualID - 1)))
        qDebug() << "ERROR: DobotQueue::removeOldQueuedMovesFromCore(): move in list not found";
    */
}

DobotMove DobotQueue::getQueuedMoveInCore(uint64_t un64ID)
{
    Q_FOREACH(DobotMove move, _queuedCmdIDList)
    {
        if (move.ID == un64ID)
            return move;
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
    qDebug() << "DobotQueue::addArmMoveToQueue(): type =" << dobotMoveAsQstr(Type)
             << ", point =" << point.getAsQStr() << ", _un64CoreQueuedCmdID ="
             << _un64CoreQueuedCmdID << ", ++ it. add dobot move to queue";
    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedCmdIDList << cmdToQueue;
}

void DobotQueue::saveIDFromConnectedDobot()
{
   if (Dobot::isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                      SHOW_ERRORS))
   {
       qDebug() << "DobotQueue::saveIDFromConnectedDobot(): "
                   "coreNextID =" << _lowestIDMoveInList;
       emit _pDobot->queueLabels(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                              _un64CoreQueuedCmdID, _queuedCmdIDList.size(),
                              _lowestIDMoveInList);
   }

    _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}
