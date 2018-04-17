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

    /*if (!_pServo->isServoListEmpty())
        _pServo->moveServoManually();*/

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): "
                    "isNextPhysicalMoveToQueueOnArmAvailable = true. proceed to "
                    "sendMoveToArm(getNextMoveToSendToArm())";
        emit this->sendMoveToArm(this->getNextMoveToSendToArm());
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): "
                    "proceed to removeOldQueuedMovesFromCore()";
        this->removeOldQueuedMovesFromCore();
        qDebug() << "DobotQueue::parseNextMoveToArmIfPossible(): end of if statement";
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
        qDebug() << "DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable(): list of "
                    "moves for dobot isn't empty. size =" << _queuedArmCmds.size();

        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmds);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;

            if (_lowestIDMoveInList >= _un64RealTimeDobotActualID)
            {
                qDebug() << "DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable(): "
                            "_lowestIDMoveInList - _un64RealTimeDobotActualID:"
                         << _lowestIDMoveInList << "-" << _un64RealTimeDobotActualID
                         << "=" << _lowestIDMoveInList - _un64RealTimeDobotActualID
                         << ". return true for below 15";

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
    qDebug() << "DobotQueue::getNextMoveToSendToArm(): _queuedArmCmds.size() ="
             << _queuedArmCmds.size();
    if (!_queuedArmCmds.isEmpty()) //2nd security checks is mandatory
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_queuedArmCmds);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if(QueuedCmdIDIter.hasNext())
        {
            _lowestIDMoveInList = QueuedCmdIDIter.peekNext().ID;
            qDebug() << "DobotQueue::getNextMoveToSendToArm(): "
                        "new _lowestIDMoveInList =" << _lowestIDMoveInList;

            if (_lowestIDMoveInList >= _un64RealTimeDobotActualID)
            {
                if(_lowestIDMoveInList - _un64RealTimeDobotActualID < 15)
                {
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
    if (_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI
            && _un64LastDobotIDShownInUI > 0)
    {
        qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): "
                    "_un64RealTimeDobotActualID > _un64LastDobotIDShownInUI ("
                 << _un64RealTimeDobotActualID << ">"
                 << _un64LastDobotIDShownInUI << ")";
        QString QStrMoveID = "";
        QString QStrDobotMove = "";

        qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): "
                    "getQueuedMove(_executedArmCmds)";
        DobotMove Move = this->getQueuedMove(_executedArmCmds, _un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit this->addTextToLogPTEInUI("Executed move ID = " + QStrMoveID +
                                       " type = " + QStrDobotMove + "\n", LOG_DOBOT);

        qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): enf of if statement";
    }

    /*qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): "
                "_un64LastDobotIDShownInUI =" << _un64LastDobotIDShownInUI
             << ", _un64RealTimeDobotActualID =" << _un64RealTimeDobotActualID;*/

    if (_un64LastDobotIDShownInUI != _un64RealTimeDobotActualID)
        qDebug() << "DobotQueue::showLastExecutedArmMoveInUI(): _un64LastDobotIDShownInUI != "
                    "_un64RealTimeDobotActualID (" << _un64LastDobotIDShownInUI << "!="
                 << _un64RealTimeDobotActualID << "). make _un64LastDobotIDShownInUI = "
                                                  "_un64RealTimeDobotActualID";

    _un64LastDobotIDShownInUI = _un64RealTimeDobotActualID;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!_executedArmCmds.empty() && _un64RealTimeDobotActualID > _executedArmCmds.first().ID)
        _executedArmCmds.removeFirst();
}

DobotMove DobotQueue::getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID)
{
    qDebug() << "inside DobotQueue::getQueuedMove(): ID =" << un64ID;

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

/*void DobotQueue::sendMoveToArm(DobotMove move)
{
    qDebug() << "DobotQueue::sendMoveToArm(): move type =" << dobotMoveAsQstr(move.type)
             << ", ID =" << move.ID;

    switch(move.type)
    {
    case DM_TO_POINT:
    case DM_UP: //todo: up/down tutaj ok?
    case DM_DOWN:
    {
        //todo: zabronić oś z jezeli jest zbyt nisko
        PTPCmd moveAsPtpCmd;
        moveAsPtpCmd.ptpMode = PTPMOVLXYZMode; //ruch typu kartezjański liniowy
        //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
        moveAsPtpCmd.x = move.xyz.x;
        moveAsPtpCmd.y = move.xyz.y;
        moveAsPtpCmd.z = move.xyz.z;
        isArmReceivedCorrectCmd(SetPTPCmd(&moveAsPtpCmd, true, &move.ID), SHOW_ERRORS);
        break;
    }
    case DM_OPEN:
        _pServo->openGripper(move.ID);
        break;
    case DM_CLOSE:
        _pServo->closeGripper(move.ID);
        break;
    case DM_WAIT:
        _pServo->wait(move.ID);
        break;
    case DM_CALIBRATE:
    {
        emit showQueueLabelsInUI->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd HOME;
        HOME.reserved = 1; //todo: o co tutaj dokładnie chodzi z tym indexem?
        isArmReceivedCorrectCmd(SetHOMECmd(&HOME, true, &move.ID), SHOW_ERRORS);
    }
        break;
        //_pServo->addServoMoveToGripperStatesList(move.type); //todo: ????
        //break;
    default:
        qDebug() << "ERROR: Dobot::sendMoveToArm(): wrong move type:" << move.type;
    }

    qDebug() << "end of DobotQueue::sendMoveToArm()";
}*/

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    qDebug() << "DobotQueue::addArmMoveToQueue(): type =" << dobotMoveAsQstr(Type)
             << ", point =" << point.getAsQStr() << ", _un64CoreQueuedCmdID ="
             << _un64CoreQueuedCmdID + 1 << ". add dobot move to queue";
    _un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(_un64CoreQueuedCmdID, Type, point);
    _queuedArmCmds << cmdToQueue;
}

void DobotQueue::saveIDFromConnectedDobot()
{
    if (isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&_un64RealTimeDobotActualID),
                                SHOW_ERRORS))
   {
       qDebug() << "DobotQueue::saveIDFromConnectedDobot(): "
                   "coreNextID =" << _lowestIDMoveInList;
       emit this->showQueueLabelsInUI(_unQueuedCmdLeftSpace, _un64RealTimeDobotActualID,
                                      _un64CoreQueuedCmdID, _queuedArmCmds.size(),
                                      _lowestIDMoveInList);
   }

    _un64CoreQueuedCmdID = _un64RealTimeDobotActualID;
}
