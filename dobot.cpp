#include "dobot.h"


//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian tylko kalibracyjnych
//todo4: w całej klasie dobot ie powinna się pojawić ani 1 linia kodu z której by wynikało że...
//...ma onacoś wspólnego z szachami

Dobot::Dobot(ArduinoUsb *pArduinoUsb):
    m_fGripOpened(6.9f),
    m_fGripClosed(7.55f)
{
    _pArduinoUsb = pArduinoUsb;

    _sGrippersItemID = 0;
    
    connectStatus = false;

    m_PtpCmdActualVal.x = 200;
    m_PtpCmdActualVal.y = 0;
    m_PtpCmdActualVal.z = 25;
    m_PtpCmdActualVal.r = 0;

    HomeChess.x = 140;
    HomeChess.y = 0;
    HomeChess.z = 10; //niżej by waliło w szachownicę
    HomeChess.r = 0;

    m_gripperServo.address = 4;
    m_gripperServo.frequency = 50;
    m_gripperServo.dutyCycle = m_fGripOpened;

    ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
    
    m_ullCoreQueuedCmdIndex = 1;
    m_uiQueuedCmdLeftSpace = std::numeric_limits<unsigned int>::max();
    
    firstPosId.index = 0;
    lastPosId.index = 0;
    takenPosId.index = 0;

    middleAboveBoard.x = 260; //mniej wiecej srodek planszy w osi x
    middleAboveBoard.y = HomeChess.y;
    middleAboveBoard.z = _pChessboard->getMinBoardZ() + Piece::dMaxPieceHeight;
    middleAboveBoard.r = 0;

    retreatYPlus.x = 180;
    retreatYPlus.y = HomeChess.y + 100;
    retreatYPlus.z = _pChessboard->getMinBoardZ() + Piece::dMaxPieceHeight;
    retreatYPlus.r = 0;

    retreatYMinus.x = 180;
    retreatYMinus.y = HomeChess.y - 100;
    retreatYMinus.z = _pChessboard->getMinBoardZ() + Piece::dMaxPieceHeight;
    retreatYMinus.r = 0;
}

void Dobot::onPeriodicTaskTimer()
{
    PeriodicTask(); //zapytuj się dobota co tam u niego co chwilę
    QTimer *periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer");
    periodicTaskTimer->start();
}

void Dobot::onGetPoseTimer()
{
    QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
    GetPose(&pose);
    
    emit JointLabelText(QString::number(pose.jointAngle[0]), 1);
    emit JointLabelText(QString::number(pose.jointAngle[1]), 2);
    emit JointLabelText(QString::number(pose.jointAngle[2]), 3);
    emit JointLabelText(QString::number(pose.jointAngle[3]), 4);
    
    emit AxisLabelText(QString::number(pose.x), 'x');
    emit AxisLabelText(QString::number(pose.y), 'y');
    emit AxisLabelText(QString::number(pose.z), 'z');
    emit AxisLabelText(QString::number(pose.r), 'r');

    getPoseTimer->start();
    
    this->QueuedIdList(); //sprawdzaj w kółko czy można wypchać do zakolejkowania dobotowi kolejny ruch
    this->checkPWM();
    //TODO: to tu  powoduje to deziorientację będąc w tym miejscu.
}

void Dobot::QueuedIdList()
{
    //m_uiQueuedCmdLeftSpace= GetQueuedCmdLeftSpace; //ile zostało pamięci w dobocie na ruchy. ...
    //... elseTODO: Póki co nie ma w dobocie tej funkcji. Może z czasem dodadzą.
    /*if (m_uiQueuedCmdLeftSpace <= 0) //jeżeli pamięć dobota spadła do zera- została przepełniona
    { //elseTODO: póki dobot nie wklei tego do swojego dll'a to nie mogę tego używać
        qDebug() << "FATAL ERROR: Dobot queue memory  full. data overflown/lost. Stop arm.";
        this->addTextToConsole("FATAL ERROR: Dobot queue memory full."
            "Data leak. Stop arm.\n", 'd');
        SetQueuedCmdForceStopExec(); //zatrzymaj ramię
    }*/

    GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny index dobota


    emit this->QueueLabels(m_uiQueuedCmdLeftSpace, m_ullDobotQueuedCmdIndex,
                           m_ullCoreQueuedCmdIndex, QueuedCmdIndexList.size(),
                           firstPosId.index); //todo: zastrukturyzować

    //servo
    if (!arduinoGripperStateList.isEmpty())
    {
        if (arduinoGripperStateList.first().index <= m_ullDobotQueuedCmdIndex)
        {
            QString QStrServoState = arduinoGripperStateList.first().isGripperOpen ? "Open" : "Close";
            qDebug() << "servo" << QStrServoState << ", servoListLastIndex =" << arduinoGripperStateList.first().index
                     << ", dobotActualIndex =" << m_ullDobotQueuedCmdIndex;
            _pArduinoUsb->sendDataToUsb("servo" + QStrServoState);
            arduinoGripperStateList.removeFirst();
        }
        emit showArduinoGripperStateList(arduinoGripperStateList);
    }

    if (!QueuedCmdIndexList.isEmpty())
    {
        QListIterator<ArmPosForCurrentCmdQueuedIndex> QueuedCmdIndexIter(QueuedCmdIndexList);

        QueuedCmdIndexIter.toFront(); //najstarszy wpis w kontenerze
        if(QueuedCmdIndexIter.hasNext())
        {
            firstPosId = QueuedCmdIndexIter.next(); //najstarszy wpis w kontenerze (najmniejszy ID)

            if(firstPosId.index - m_ullDobotQueuedCmdIndex < 15)
            {
                takenPosId = QueuedCmdIndexList.takeFirst(); //wypchnij na dobota kolejny id...
                //...ruchu z kontenera. najstarszy wpis w kontenerze znajduje się na jego początku.

                switch(takenPosId.move)
                {
                case DM_TO_POINT:
                    //todo: zabronic ruchow w 3 plaszczyznach na raz
                    //todo: zabronic na ruch 'x' i 'y', jezeli 'z' jest zbyt nisko
                    ptpCmd.x = takenPosId.x;
                    ptpCmd.y = takenPosId.y;
                    ptpCmd.z = takenPosId.z;
                    ptpCmd.r = takenPosId.r;
                    SetPTPCmd(&ptpCmd, true, &takenPosId.index);
                    break;
                case DM_WAIT:
                    SetWAITCmd(&gripperMoveDelay, true, &takenPosId.index);
                    break;
                case DM_OPEN:
                {
                    m_gripperServo.dutyCycle = m_fGripOpened;
                    int openGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (openGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", LOG_DOBOT);
                    }
                }
                    break;
                case DM_CLOSE:
                {
                    m_gripperServo.dutyCycle = m_fGripClosed;
                    int closeGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (closeGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", LOG_DOBOT);
                    }
                }
                    break;
                case DM_HOME:
                {
                    this->addTextToConsole("HomeCmd: recalibrating arm...\n", LOG_DOBOT);
                    HOMECmd HOMEChess;
                    HOMEChess.reserved = 1; //todo: jak się to ma do innych indexów?
                    int result = SetHOMECmd(&HOMEChess, true, &takenPosId.index);
                    if (result != DobotCommunicate_NoError)
                        qDebug() << "ERROR: SetHOMECmd result != DobotCommunicate_NoError";
                }
                    break;
                case DM_UP:
                case DM_DOWN:
                default:
                    qDebug() << "ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:" << takenPosId.sequence;
                    this->addTextToConsole("ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:"
                                           + takenPosId.sequence, LOG_DOBOT);
                    break;
                }
            } 
        emit showActualDobotQueuedCmdIndexList(QueuedCmdIndexList);
        }
    }

    if (m_ullRetreatIndex <= m_ullDobotQueuedCmdIndex)
    {
        qDebug() << "Dobot::QueuedIdList(): retreat";
        PtpCmdActualVal retreatId;
        retreatId = (pose.y >= middleAboveBoard.y) ?  retreatYPlus : retreatYMinus;
        addCmdToList(DM_TO_POINT, retreatId.x, retreatId.y, retreatId.z);
        m_ullRetreatIndex = std::numeric_limits<int64_t>::max();
    }
}

bool Dobot::bIsMoveInAxisRange(Point3D point)
{
    //todo: zrobic jeszcze prewencyjnie wewnetrzny kod blokad
    return true;
}

void Dobot::setItemInGripper(short sGrippersItemID)
{
    if (!this->isGripperEmpty())
    {
        qDebug() << "ERROR: Dobot::isGripperEmpty(): it isn't. item nr in gripper: "
                 << _sGrippersItemID << ". passed item nr as par:" << sGrippersItemID;
        return;
    }

    _sGrippersItemID = sGrippersItemID;
}

void Dobot::clearGripper()
{
    if (!this->isGripperEmpty())
    {
        qDebug() << "ERROR: Dobot::isGripperEmpty(): it isn't. item nr in gripper: "
                 << _sGrippersItemID << ". passed item nr as par:" << sGrippersItemID;
        return;
    }

    _sGrippersItemID = 0;
}

float Dobot::getHomePos(char ch)
{
    if (ch == 'x') return HomeChess.x;
    else if (ch == 'y') return HomeChess.y;
    else if (ch == 'z') return HomeChess.z;
    else return HomeChess.r;
}

float Dobot::getmiddleAboveBoardPos(char ch)
{
    if (ch == 'x') return middleAboveBoard.x;
    else if (ch == 'y') return middleAboveBoard.y;
    else if (ch == 'z') return middleAboveBoard.z;
    else return middleAboveBoard.r;
}

void Dobot::onConnectDobot()
{
    if (!connectStatus)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        connectStatus = true;
        qDebug() << "Dobot connection success";
        this->addTextToConsole("Dobot connected \n", LOG_DOBOT);

        QTimer *periodicTaskTimer = new QTimer(this);
        periodicTaskTimer->setObjectName("periodicTaskTimer");
        connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
        periodicTaskTimer->setSingleShot(true);
        periodicTaskTimer->start(5);
        //ciągle dostaję błąd: QObject::startTimer: Timers can only be used with threads started with QThread

        QTimer *getPoseTimer = new QTimer(this);
        getPoseTimer->setObjectName("getPoseTimer");
        connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
        getPoseTimer->setSingleShot(true);
        getPoseTimer->start(200);

        this->refreshBtn();
        this->initDobot();
        
        int result = GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny id i zapisz w zmiennej
        if (result == DobotCommunicate_NoError)
        {
            emit this->QueueLabels(m_uiQueuedCmdLeftSpace, m_ullDobotQueuedCmdIndex,
                                   m_ullCoreQueuedCmdIndex, QueuedCmdIndexList.size(),
                                   firstPosId.index);
        }
        else
        {
            qDebug() << "ERROR: Dobot::onConnectDobot(): GetQueuedCmdCurrentIndex gone wrong";
            this->addTextToConsole("ERROR: Dobot::onConnectDobot(): "
                                   "GetQueuedCmdCurrentIndex gone wrong \n", LOG_DOBOT);
        }

        m_ullCoreQueuedCmdIndex = m_ullDobotQueuedCmdIndex; //jeśli dobot przed aktualnym uruchomieniem programu...
        //...wykonywał jakieś ruchy, to startowy index na core byłby normalnie mniejszy od aktualnego
    }
    else
    {
        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
        connectStatus = false;
        refreshBtn();
        DisconnectDobot();
    }
}

void Dobot::refreshBtn() //niby button, ale używamy póki co tylko jako funkcja
{
    if(connectStatus) emit RefreshDobotButtonsStates(false);
    else emit RefreshDobotButtonsStates(true);
}

void Dobot::initDobot()
{
    SetCmdTimeout(3000); //Command timeout
    SetQueuedCmdClear(); //clear old commands
    SetQueuedCmdStartExec(); //set the queued command running
    
    char deviceSN[64];
    GetDeviceSN(deviceSN, sizeof(deviceSN));
    
    char deviceName[64];
    GetDeviceName(deviceName, sizeof(deviceName));
    
    uint8_t majorVersion, minorVersion, revision;
    GetDeviceVersion(&majorVersion, &minorVersion, &revision);
    
    
    emit deviceLabels(deviceSN, deviceName, QString::number(majorVersion) + "." +
                      QString::number(minorVersion) + "." +
                      QString::number(revision));
    
    //set the end effector parameters
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 71.6f; //59.7f; TODO: wcześniej była ta wartość. co
    //ona oznacza w praktyce?
    SetEndEffectorParams(&endEffectorParams, false, NULL);
    
    JOGJointParams jogJointParams;
    for (int i = 0; i < 4; i++)
    {
        jogJointParams.velocity[i] = 100;
        jogJointParams.acceleration[i] = 100;
    }
    SetJOGJointParams(&jogJointParams, false, NULL);
    
    JOGCoordinateParams jogCoordinateParams;
    for (int i = 0; i < 4; i++)
    {
        jogCoordinateParams.velocity[i] = 100;
        jogCoordinateParams.acceleration[i] = 100;
    }
    SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);
    
    //todo: predkosci
    JOGCommonParams jogCommonParams;
    jogCommonParams.velocityRatio = 100;
    jogCommonParams.accelerationRatio = 100;
    SetJOGCommonParams(&jogCommonParams, false, NULL);
    
    PTPJointParams ptpJointParams;
    for (int i = 0; i < 4; i++)
    {
        ptpJointParams.velocity[i] = 200;
        ptpJointParams.acceleration[i] = 200;
    }
    SetPTPJointParams(&ptpJointParams, false, NULL);
    
    PTPCoordinateParams ptpCoordinateParams;
    ptpCoordinateParams.xyzVelocity = 200;
    ptpCoordinateParams.xyzAcceleration = 200;
    ptpCoordinateParams.rVelocity = 200;
    ptpCoordinateParams.rAcceleration = 200;
    SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);
    
    PTPJumpParams ptpJumpParams;
    ptpJumpParams.jumpHeight = 20;
    ptpJumpParams.zLimit = 150;
    SetPTPJumpParams(&ptpJumpParams, false, NULL);
    
    SetHOMEParams(&HomeChess, false, NULL); //todo: NULL- pewnie dlatego mi się wykrzacza ID
}

//TODO: tu nie zadziała to
void Dobot::gripperAngle(float fDutyCycle)
{
    //uwaga: wykonuje się to polecenie bez kolejki
    if (fDutyCycle != 0) m_gripperServo.dutyCycle = fDutyCycle;
    qDebug() << "m_gripperServo.dutyCycle = " << fDutyCycle;
    SetIOPWM(&m_gripperServo, false, NULL);
}

void Dobot::doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump)
{
    //todo: przesunąć wyświetlanie wszystkich komunikatów do czasu rzeczywistego
    if (VertMove == VM_GRAB)
        this->gripperState(DM_OPEN);

    dest3D.z += dJump;
    this->addCmdToList(DM_TO_POINT, dest3D);

    if (VertMove == VM_NONE) return;

    this->armUpDown(DM_DOWN, dest3D - dJump); //todo: usunąć "DOBOT_MOVE"

    if (VertMove == VM_PUT)
        this->gripperState(DM_OPEN);
    else if (VertMove == VM_GRAB)
        this->gripperState(DM_CLOSE);

    this->armUpDown(DM_UP, );
}

void Dobot::gripperState(DOBOT_MOVE State)
{
    qDebug() << "Dobot::gripperState:" << (State == DM_OPEN ? "open" : "close");
    this->addCmdToList(State);
    this->writeMoveTypeInConsole(State);
}

//todo: niech wait zawsze samo się odpala jak zamykam chwytak
void Dobot::wait(int nMs) //todo: wait for motors
{
    gripperMoveDelay.timeout = nMs;
    this->addCmdToList(DM_WAIT);
    this->writeMoveTypeInConsole(DM_WAIT);
}

void Dobot::armUpDown(DOBOT_MOVE ArmDestination, double dHeight)
{
    if (ArmDestination != DM_UP && ArmDestination != DM_DOWN)
    {
        qDebug() << "ERROR: Dobot::armUpDown(): wrong armDestination val ="
                 << dobotMoveAsQstr(ArmDestination);
        return;
    }

    //qDebug() << "ERROR: Dobot::armUpDown: tried to move Z axis with X or Y axis";

    //todo3: uporządkować kod zabezpieczeń

    //todo: m_PtpCmdActualVal zmienic nazwe punktu (lub dodac kolejny) na cos w stylu...
    //..."lastDest3D", i odrozniac to gdzie jest ramie, od tego gdzie bylo ostatnio

    Point3D dest3D = m_PtpCmdActualVal;
    dest3D.z = dHeight;

    this->addCmdToList(ArmDestination, dest3D);
}

//todo: przyjmowanie punktu jako ruchu
//todo: jak wyjebie stąd servo, to DOBOT_MOVE będzie zbędny?
void Dobot::addCmdToList(DOBOT_MOVE Move, Point3D point = _lastGivenPoint)
{
    //todo: usuwać z tablicy wiadomości na dobota wiadomości te które już poszły (może minus 1)
    m_ullCoreQueuedCmdIndex += 1; //aktualne id ruchu = +1 większe od ostatniego
    m_posIdx.index = m_ullCoreQueuedCmdIndex;
    m_posIdx.sequence = sequence;
    m_posIdx.move = Move;
    m_posIdx.x = (x != ACTUAL_POS) ? x : m_PtpCmdActualVal.x;
    m_posIdx.y = (y != ACTUAL_POS) ? y : m_PtpCmdActualVal.y;
    m_posIdx.z = (z != ACTUAL_POS) ? z : m_PtpCmdActualVal.z;
    m_posIdx.r = (r != ACTUAL_POS) ? r : m_PtpCmdActualVal.r;
    QueuedCmdIndexList << m_posIdx;

    if (move == DM_OPEN || move == DM_CLOSE)
    {
        ServoArduino servoState;
        servoState.index = m_ullCoreQueuedCmdIndex;
        servoState.isGripperOpen = (move == DM_OPEN) ? true : false;
        qDebug() << "add command to arduinoGripperStateList:" << dobotMoveAsQstr(move);
        arduinoGripperStateList << servoState;
    }
}

void Dobot::checkPWM()
{
    IOPWM gripperControl;
    if (gripperControl.address != 0 && gripperControl.frequency != 0 && gripperControl.dutyCycle != 0)
    {
        GetIOPWM(&gripperControl);

        if (gripperControl.address!= 4 )
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.address!= 4. val =" << gripperControl.address;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.address!= 4. val =" +
                                   QString::number(gripperControl.address) + "\n", LOG_DOBOT);
        }
        if (gripperControl.frequency != 50)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" << gripperControl.address;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" +
                                   QString::number(gripperControl.frequency) + "\n", LOG_DOBOT);
        }
        if (gripperControl.dutyCycle != m_fGripOpened && gripperControl.dutyCycle != m_fGripClosed)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" << gripperControl.dutyCycle;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" +
                                   QString::number(gripperControl.dutyCycle) + "\n", LOG_DOBOT);
        }
    }
}

//todo: do chess wrzucic to
void Dobot::writeMoveTypeInConsole(DOBOT_MOVE MoveState)
{
    QString QStrMsg;

    switch(MoveState)
    {
    case DM_TO_POINT: QStrMsg = ""; break;
    case DM_WAIT: QStrMsg = "wait " + QString::number(gripperMoveDelay.timeout) +
                " ms\n"; break;
    case DM_OPEN: QStrMsg = "gripper opened\n"; break;
    case DM_CLOSE: QStrMsg = "gripper closed\n"; break;
    case DM_UP: QStrMsg = "arm up\n"; break;
    case DM_DOWN: QStrMsg = "arm down\n"; break;
    default: QStrMsg = "ERROR: Dobot::writeMoveTypeInConsole(): wrong movement state: " +
                dobotMoveAsQstr(moveState) + "\n"; break;
    }

    emit this->addTextToConsole(QStrMsg, LOG_DOBOT);
}


void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
