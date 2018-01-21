#include "dobot.h"

//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian tylko kalibracyjnych
//todo4: w całej klasie dobot ie powinna się pojawić ani 1 linia kodu z której by wynikało że...
//...ma onacoś wspólnego z szachami

Dobot::Dobot(ArduinoUsb *pArduinoUsb, DobotQueue pQueue)
{
    _pQueue = pQueue;
    _pArduinoUsb = pArduinoUsb;

    _sGrippersItemID = 0;
    
    _connectStatus = false;

    _lastGivenPoint(200,0,25);

    _Home.x = 140;
    _Home.y = 0;
    _Home.z = 10;
    _Home.r = 0;

    _gripperServo.address = 4;
    _gripperServo.frequency = 50;
    _gripperServo.dutyCycle = _fGripOpened;
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
    GetPose(&_pose);
    
    emit JointLabelText(QString::number(_pose.jointAngle[0]), 1);
    emit JointLabelText(QString::number(_pose.jointAngle[1]), 2);
    emit JointLabelText(QString::number(_pose.jointAngle[2]), 3);
    emit JointLabelText(QString::number(_pose.jointAngle[3]), 4);
    
    emit AxisLabelText(QString::number(_pose.x), 'x');
    emit AxisLabelText(QString::number(_pose.y), 'y');
    emit AxisLabelText(QString::number(_pose.z), 'z');
    emit AxisLabelText(QString::number(_pose.r), 'r');

    getPoseTimer->start();
    
    _pQueue->queuedIDList();
    this->checkPWM();
    //TODO: to tu  powoduje to deziorientację będąc w tym miejscu.
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

Point3D Dobot::getHomePos()
{
    Point3D home(_Home); //todo: home ciągnąć z xml
    return home;
}

void Dobot::onConnectDobot()
{
    if (!_connectStatus)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        _connectStatus = true;
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
        
        int result = GetQueuedCmdCurrentID(&_n64RealTimeDobotActualID); //sprawdź aktualny id i zapisz w zmiennej
        if (result == DobotCommunicate_NoError)
        {
            emit this->QueueLabels(_unQueuedCmdLeftSpace, _n64RealTimeDobotActualID,
                                   _n64CoreQueuedCmdID, QueuedCmdIDList.size(),
                                   firstPosId.index);
        }
        else
        {
            qDebug() << "ERROR: Dobot::onConnectDobot(): GetQueuedCmdCurrentID gone wrong";
            this->addTextToConsole("ERROR: Dobot::onConnectDobot(): "
                                   "GetQueuedCmdCurrentID gone wrong \n", LOG_DOBOT);
        }

        _n64CoreQueuedCmdID = _n64RealTimeDobotActualID; //jeśli dobot przed aktualnym uruchomieniem programu...
        //...wykonywał jakieś ruchy, to startowy index na core byłby normalnie mniejszy od aktualnego
    }
    else
    {
        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
        _connectStatus = false;
        refreshBtn();
        DisconnectDobot();
    }
}

void Dobot::refreshBtn() //niby button, ale używamy póki co tylko jako funkcja
{
    if(_connectStatus) emit RefreshDobotButtonsStates(false);
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
    
    SetHOMEParams(&_Home, false, NULL); //todo: NULL- pewnie dlatego mi się wykrzacza ID
}

//TODO: tu nie zadziała to
void Dobot::gripperAngle(float fDutyCycle)
{
    //uwaga: wykonuje się to polecenie bez kolejki
    if (fDutyCycle != 0) _gripperServo.dutyCycle = fDutyCycle;
    qDebug() << "_gripperServo.dutyCycle = " << fDutyCycle;
    SetIOPWM(&_gripperServo, false, NULL);
}

void Dobot::doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump)
{
    if (this->isPointTotallyDiffrent(dest3D)) return;

    //todo: przesunąć wyświetlanie wszystkich komunikatów do czasu rzeczywistego
    if (VertMove == VM_GRAB)
        this->gripperState(DM_OPEN);

    dest3D.z += dJump;
    this->addCmdToList(DM_TO_POINT, dest3D);

    if (VertMove == VM_NONE) return;

    if (!this->isPointDiffrentOnlyInZAxis(dest3D)) return;
    this->armUpDown(DM_DOWN, dest3D - dJump);

    if (VertMove == VM_PUT)
        this->gripperState(DM_OPEN);
    else if (VertMove == VM_GRAB)
        this->gripperState(DM_CLOSE);

    this->armUpDown(DM_UP, dest3D);
}

bool Dobot::isPointTotallyDiffrent(Point3D point)
{
    if (point.x != _lastGivenPoint.x && point.y != _lastGivenPoint.y
            && point.z != _lastGivenPoint.z)
    {
        qDebug() << "ERROR: Dobot::isPointTotallyDiffrent(): moves in 3 axis at once "
                    "are forbidden. point =" << point.x << point.y << point.z
                 << "_lastGivenPoint =" << _lastGivenPoint.x << _lastGivenPoint.y
                 << _lastGivenPoint.z;
        return true;
    }
    else return false;
}

bool Dobot::isPointDiffrentOnlyInZAxis(Point3D point)
{
    if (point.x != _lastGivenPoint.x || point.y != _lastGivenPoint.y)
    {
        qDebug() << "ERROR: Dobot::isPointDiffrentOnlyInZAxis(): tried to move Z axis"
                    " with X or Y axis. point xy =" << point.x << point.y <<
                    "_lastGivenPoint xy =" << _lastGivenPoint.x << _lastGivenPoint.y;
        return false;
    }
    else return true;
}

void Dobot::gripperState(DOBOT_MOVE State)
{
    qDebug() << "Dobot::gripperState:" << (State == DM_OPEN ? "open" : "close");
    this->addCmdToList(State);
    this->writeMoveTypeInConsole(State);

    if (State == DM_CLOSE)
        this->wait(400);
}

void Dobot::wait(int nMs) //for motors
{
    _gripperMoveDelay.timeout = nMs;
    this->addCmdToList(DM_WAIT);
    this->writeMoveTypeInConsole(DM_WAIT);
}

void Dobot::addCmdToList(DOBOT_MOVE Move, Point3D point = _lastGivenPoint)
{
    _lastGivenPoint = point;

    _pQueue->addCmdToList(Move, point);
}

void Dobot::armUpDown(DOBOT_MOVE ArmDestination, double dHeight)
{
    if (ArmDestination != DM_UP && ArmDestination != DM_DOWN)
    {
        qDebug() << "ERROR: Dobot::armUpDown(): wrong armDestination val ="
                 << dobotMoveAsQstr(ArmDestination);
        return;
    }

    //todo: uporządkować kod zabezpieczeń

    Point3D dest3D = _lastGivenPoint;
    dest3D.z = dHeight;

    this->addCmdToList(ArmDestination, dest3D);
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
        if (gripperControl.dutyCycle != _fGripOpened && gripperControl.dutyCycle != _fGripClosed)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" << gripperControl.dutyCycle;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" +
                                   QString::number(gripperControl.dutyCycle) + "\n", LOG_DOBOT);
        }
    }
}

void Dobot::writeMoveTypeInConsole(DOBOT_MOVE MoveState)
{
    QString QStrMsg;

    switch(MoveState)
    {
    case DM_TO_POINT: QStrMsg = ""; break;
    case DM_WAIT: QStrMsg = "wait " + QString::number(_gripperMoveDelay.timeout) +
                " ms\n"; break;
    case DM_OPEN: QStrMsg = "gripper opened\n"; break;
    case DM_CLOSE: QStrMsg = "gripper closed\n"; break;
    case DM_UP: QStrMsg = "arm up\n"; break;
    case DM_DOWN: QStrMsg = "arm down\n"; break;
    default: QStrMsg = "ERROR: Dobot::writeMoveTypeInConsole(): wrong movement state: "
                + dobotMoveAsQstr(MoveState) + "\n"; break;
    }

    emit this->addTextToConsole(QStrMsg, LOG_DOBOT);
}

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
