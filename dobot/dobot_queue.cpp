#include "dobot_queue.h"

DobotQueue::DobotQueue():
    _fGripOpened(6.9f), //future: ciągnąć z xml
    _fGripClosed(7.55f)
{
    _n64CoreQueuedCmdID = 1;
    _unQueuedCmdLeftSpace = std::numeric_limits<uint>::max();

    _firstPosId.ID = 0;
    _lastPosId.ID = 0; //todo: nie widzę abym tego używał
    _takenPosId.ID = 0;

    _ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
}

//sprawdzaj w kółko czy można wypchać do zakolejkowania dobotowi kolejny ruch
void DobotQueue::queuedIDList() //todo: nazwa
{
    //_unQueuedCmdLeftSpace= GetQueuedCmdLeftSpace; //ile zostało pamięci w dobocie na ruchy. ...
    //... future: Póki co nie ma w dobocie tej funkcji. Może z czasem dodadzą.
    /*if (_unQueuedCmdLeftSpace <= 0) //jeżeli pamięć dobota spadła do zera- została przepełniona
    { //future: póki dobot nie wklei tego do swojego dll'a to nie mogę tego używać
        qDebug() << "FATAL ERROR: Dobot queue memory  full. data overflown/lost. Stop arm.";
        this->addTextToConsole("FATAL ERROR: Dobot queue memory full."
            "Data leak. Stop arm.\n", 'd');
        SetQueuedCmdForceStopExec(); //zatrzymaj ramię
    }*/

    GetQueuedCmdCurrentID(&_n64RealTimeDobotActualID);


    emit this->QueueLabels(_unQueuedCmdLeftSpace, _n64RealTimeDobotActualID,
                           _n64CoreQueuedCmdID, _QueuedCmdIDList.size(),
                           _firstPosId.ID); //todo: zastrukturyzować?

    //servo
    if (!_arduinoGripperStates.isEmpty())
    {
        if (_arduinoGripperStates.first().ID <= _n64RealTimeDobotActualID)
        {
            QString QStrServoState =
                    _arduinoGripperStates.first().isGripperOpen ? "Open" : "Close";
            qDebug() << "servo" << QStrServoState << ", servoListLastIndex ="
                     << _arduinoGripperStates.first().ID << ", dobotActualIndex ="
                     << _n64RealTimeDobotActualID;
            _pArduinoUsb->sendDataToUsb("servo" + QStrServoState);
            _arduinoGripperStates.removeFirst();
        }
        emit showArduinoGripperStateList(_arduinoGripperStates);
    }

    if (!_QueuedCmdIDList.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(_QueuedCmdIDList);

        QueuedCmdIDIter.toFront(); //najstarszy wpis w kontenerze
        if(QueuedCmdIDIter.hasNext())
        {
            //najstarszy wpis w kontenerze (najmniejszy ID):
            _firstPosId = QueuedCmdIDIter.next();

            if(_firstPosId.ID - _n64RealTimeDobotActualID < 15)
            {
                //wypchnij na dobota kolejny id ruchu z kontenera. najstarszy...
                //...wpis w kontenerze znajduje się na jego początku.
                _takenPosId = _QueuedCmdIDList.takeFirst();

                this->queuePhysicalMoveOnArm(_takenPosId.move);

            }

        emit showActualDobot_QueuedCmdIDList(_QueuedCmdIDList);
        }
    }

    //this->retreat(); //todo: zrobić to po udanych ruchach dobota
}

//todo: if retreat is set from xml
/*void DobotQueue::retreat() //todo
{
    if (_n64RetreatID <= _n64RealTimeDobotActualID)
    {
        qDebug() << "DobotQueue::queuedIDList(): retreat";
        PtpCmdActualVal retreatId;
        retreatId = (_pose.y >= middleAboveBoard.y) ?  retreatYPlus : retreatYMinus;
        addCmdToList(DM_TO_POINT, retreatId.x, retreatId.y, retreatId.z);
        _n64RetreatID = std::numeric_limits<int64_t>::max();
    }
}*/

void DobotQueue::queuePhysicalMoveOnArm(DOBOT_MOVE Move)
{
    switch(Move)
    {
    case DM_TO_POINT:
        //todo: zabronić oś z jezeli jest zbyt nisko
        _ptpCmd.x = _takenPosId.xyz.x;
        _ptpCmd.y = _takenPosId.xyz.y;
        _ptpCmd.z = _takenPosId.xyz.z;
        SetPTPCmd(&_ptpCmd, true, &_takenPosId.ID);
        break;
    case DM_WAIT:
        SetWAITCmd(&_gripperMoveDelay, true, &_takenPosId.ID);
        break;
    case DM_OPEN:
    {
        _gripperServo.dutyCycle = _fGripOpened;
        int openGripResult = SetIOPWM(&_gripperServo, true, &_takenPosId.ID);
        if (openGripResult != DobotCommunicate_NoError)
            qDebug() << "ERROR: DobotQueue::queuePhysicalMoveOnArm(): "
                        "SetIOPWM gone wrong";
    }
        break;
    case DM_CLOSE:
    {
        _gripperServo.dutyCycle = _fGripClosed;
        int closeGripResult = SetIOPWM(&_gripperServo, true, &_takenPosId.ID);
        if (closeGripResult != DobotCommunicate_NoError)
            qDebug() << "ERROR: DobotQueue::queuePhysicalMoveOnArm(): "
                        "SetIOPWM gone wrong";
    }
        break;
    case DM_HOME:
    {
        this->addTextToConsole("HomeCmd: recalibrating arm...\n", LOG_DOBOT);
        HOMECmd HOME;
        HOME.reserved = 1; //todo: o co tutah dokładnie chodzi z tym indexem?
        int result = SetHOMECmd(&HOME, true, &_takenPosId.ID);
        if (result != DobotCommunicate_NoError)
            qDebug() << "ERROR: DobotQueue::queuePhysicalMoveOnArm(): "
                        "SetHOMECmd result != DobotCommunicate_NoError";
    }
        break;
    case DM_UP:
    case DM_DOWN: //todo: ??
    default:
        qDebug() << "ERROR: Dobot::queuePhysicalMoveOnArm(): wrong _takenPosId.type:"
                 << _takenPosId.sequence;
    }
}

//todo: jak wyjebie stąd servo, to DOBOT_MOVE będzie zbędny?
void DobotQueue::addCmdToList(DOBOT_MOVE Move, Point3D point)
{
    //todo: usuwać z tablicy wiadomości na dobota wiadomości te które już poszły (może minus 1)
    _n64CoreQueuedCmdID += 1; //aktualne id ruchu = +1 większe od ostatniego

    _cmd.ID = _n64CoreQueuedCmdID;
    _cmd.move = Move;
    _cmd.xyz = point;

    _QueuedCmdIDList << _cmd;

    //todo:
    if (Move == DM_OPEN || Move == DM_CLOSE)
    {
        ServoArduino servoState;
        servoState.ID = _n64CoreQueuedCmdID;
        servoState.isGripperOpen = (Move == DM_OPEN) ? true : false;
        qDebug() << "DobotQueue::addCmdToList():" << dobotMoveAsQstr(Move);

        _arduinoGripperStates << servoState;
    }
}
