#include "dobot.h"

//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian...
//...tylko kalibracyjnych

Dobot::Dobot(ArduinoUsb *pUsb, RealVars gameConfigVars):
    _ARM_MAX_VELOCITY(300), //todo: ile jest max? 200? 300?
    _ARM_MAX_ACCELERATION(300)
{
    _pUsb = pUsb;

    _pQueue = new DobotQueue(this);
    _pServo = new DobotServo(this, gameConfigVars.fGripperOpened,
                             gameConfigVars.fGripperClosed);

    _sItemIDInGripper = 0;
    
    _bConnectedToDobot = false;

    Point3D fakeMid(200,0,25); //todo: liczyć
    _lastGivenPoint = fakeMid; //todo: pierwszy punkt jako middle above

    /*_Home.x = 140;
    _Home.y = 0;
    _Home.z = 10;*/
    _Home.x = gameConfigVars.home.x;
    _Home.y = gameConfigVars.home.y;
    _Home.z = gameConfigVars.home.z;
    _Home.r = 0;

    _homeToMiddleAbove = gameConfigVars.homeToMiddleAbove;
}

Dobot::~Dobot()
{
    delete _pQueue;
    delete _pServo;
}

void Dobot::onPeriodicTaskTimer() //todo zmienić nazwy timerów
{
    PeriodicTask(); //start arm task loop. non-return funcion
    QTimer* periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer"); //find timer by name
    periodicTaskTimer->start(); //auto restart timer
}

void Dobot::onGetPoseTimer()
{
    QTimer* getPoseTimer = findChild<QTimer *>("getPoseTimer"); //find timer by name

    this->saveActualDobotPosition();
    _pQueue->parseNextMoveToArmIfPossible();

    getPoseTimer->start(); //auto restart timer
}

/*static*/ bool Dobot::isArmReceivedCorrectCmd(int nResult, bool bErrorLog /*= false*/)
{
    if (nResult == DobotCommunicate_NoError)
        return true;
    else
    {
        if (bErrorLog)
        {
            if (nResult == DobotCommunicate_BufferFull)
                qDebug() << "ERROR: Dobot::isArmReceivedCorrectCmd(): dobot buffer is full";
            else if (nResult == DobotCommunicate_Timeout)
                qDebug() << "ERROR: Dobot::isArmReceivedCorrectCmd(): cmd timeout";
            else
                qDebug() << "ERROR: Dobot::isArmReceivedCorrectCmd(): unknown error:" << nResult;
        }

        return false;
    }
}

void Dobot::saveActualDobotPosition()
{
    Pose pose;
    GetPose(&pose); //pose from arm

    _realTimePoint.x = pose.x;
    _realTimePoint.y = pose.y;
    _realTimePoint.z = pose.z;

    emit JointLabelText(QString::number(pose.jointAngle[0]), 1);
    emit JointLabelText(QString::number(pose.jointAngle[1]), 2);
    emit JointLabelText(QString::number(pose.jointAngle[2]), 3);
    emit JointLabelText(QString::number(pose.jointAngle[3]), 4);

    emit AxisLabelText(QString::number(pose.x), 'x');
    emit AxisLabelText(QString::number(pose.y), 'y');
    emit AxisLabelText(QString::number(pose.z), 'z');
    emit AxisLabelText(QString::number(pose.r), 'r');
}

bool Dobot::bIsMoveInAxisRange(Point3D point)
{
    if (point.x > 300) qDebug() << "todo: ogarnac blokady";
    //todo: zrobic jeszcze prewencyjnie wewnetrzny kod blokad
    //...max ciągnąć z xml
    //...i pouswstawiać to tam gdzie trzeba
    return true;
}

void Dobot::setItemInGripper(short sGrippersItemID)
{
    if (!this->isGripperEmpty())
    {
        qDebug() << "ERROR: Dobot::isGripperEmpty(): it isn't. item nr in gripper: "
                 << _sItemIDInGripper << ". passed item nr as par:" << sGrippersItemID;
        return;
    }

    _sItemIDInGripper = sGrippersItemID;
}

void Dobot::clearGripper()
{
    if (this->isGripperEmpty())
    {
        qDebug() << "ERROR: Dobot::clearGripper(): gripper is already empty";
        return;
    }

    _sItemIDInGripper = 0;
}

Point3D Dobot::getHomePos() //todo:
{
    Point3D home(_Home.x, _Home.y, _Home.z);
    return home;
}

void Dobot::onConnectDobot()
{
    if (!_bConnectedToDobot)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError)
            emit DobotErrorMsgBox();

        _bConnectedToDobot = true;
        emit this->addTextToLogPTE("Dobot connected \n", LOG_DOBOT);

        //todo: timery w osobnych funkcjach
        //create dobot periodic timer
        QTimer *periodicTaskTimer = new QTimer(this);
        periodicTaskTimer->setObjectName("periodicTaskTimer");
        connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
        periodicTaskTimer->setSingleShot(true);
        periodicTaskTimer->start(5);
        //future: ciągle dostaję błąd: "QObject::startTimer: Timers can only...
        //...be used with threads started with QThread", jednak ciągle to działa.

        //create dobot pose timer
        QTimer *getPoseTimer = new QTimer(this);
        getPoseTimer->setObjectName("getPoseTimer");
        connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
        getPoseTimer->setSingleShot(true);
        getPoseTimer->start(200);

        this->initDobot();
        _pQueue->saveIDFromConnectedDobot(); //1st check
    }
    else
    {
        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
        _bConnectedToDobot = false;
        DisconnectDobot();
    }

    emit RefreshDobotButtonsStates(_bConnectedToDobot); //todo: nazwa
}

void Dobot::initDobot()
{
    SetCmdTimeout(3000); //command timeout
    SetQueuedCmdClear(); //clear commands queue on arm
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
    
    //todo: ustawić servo tutej
    //set the end effector parameters
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 71.6f; //59.7f; TODO: wcześniej była ta wartość. co...
    //...ona oznacza w praktyce?
    SetEndEffectorParams(&endEffectorParams, false, NULL);
    
    JOGJointParams jogJointParams;
    for (int i=0; i<4; ++i)
    {
        jogJointParams.velocity[i] = _ARM_MAX_VELOCITY;
        jogJointParams.acceleration[i] = _ARM_MAX_ACCELERATION;
    }
    SetJOGJointParams(&jogJointParams, false, NULL);
    
    JOGCoordinateParams jogCoordinateParams;
    for (int i=0; i<4; ++i)
    {
        jogCoordinateParams.velocity[i] = _ARM_MAX_VELOCITY;
        jogCoordinateParams.acceleration[i] = _ARM_MAX_ACCELERATION;
    }
    SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);
    
    JOGCommonParams jogCommonParams;
    jogCommonParams.velocityRatio = _ARM_MAX_VELOCITY;
    jogCommonParams.accelerationRatio = _ARM_MAX_ACCELERATION;
    SetJOGCommonParams(&jogCommonParams, false, NULL);
    
    PTPJointParams ptpJointParams;
    for (int i=0; i<4; ++i)
    {
        ptpJointParams.velocity[i] = _ARM_MAX_VELOCITY;
        ptpJointParams.acceleration[i] = _ARM_MAX_ACCELERATION;
    }
    SetPTPJointParams(&ptpJointParams, false, NULL);
    
    PTPCoordinateParams ptpCoordinateParams;
    ptpCoordinateParams.xyzVelocity = _ARM_MAX_VELOCITY;
    ptpCoordinateParams.xyzAcceleration = _ARM_MAX_ACCELERATION;
    ptpCoordinateParams.rVelocity = _ARM_MAX_VELOCITY;
    ptpCoordinateParams.rAcceleration = _ARM_MAX_ACCELERATION;
    SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);
    
    PTPJumpParams ptpJumpParams;
    ptpJumpParams.jumpHeight = 20;
    ptpJumpParams.zLimit = 150;
    SetPTPJumpParams(&ptpJumpParams, false, NULL);
    
    SetHOMEParams(&_Home, false, NULL); //todo: NULL- pewnie dlatego mi się wykrzacza ID
}

void Dobot::queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove /*= VM_NONE*/)
{
    if (this->isPointTotallyDiffrent(dest3D)) return;

    if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_OPEN);

    dest3D.z += dJump;
    this->addArmMoveToQueue(DM_TO_POINT, dest3D);

    if (VertMove == VM_NONE) return;

    if (!this->isPointDiffrentOnlyInZAxis(dest3D)) return;
    this->armUpDown(DM_DOWN, dest3D.z - dJump);

    if (VertMove == VM_PUT)
        this->addArmMoveToQueue(DM_OPEN);
    else if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_CLOSE);

    this->armUpDown(DM_UP, dest3D.z);
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

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type)
{
    Point3D point = _lastGivenPoint;
    _pQueue->addArmMoveToQueue(Type, point);
}

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    qDebug() << "Dobot::addArmMoveToQueue(): type =" << dobotMoveAsQstr(Type)
             << ", point =" << point.getAsQStr();
    _lastGivenPoint = point;
    _pQueue->addArmMoveToQueue(Type, point);
}

void Dobot::armUpDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight)
{
    if (ArmDestination != DM_UP && ArmDestination != DM_DOWN)
    {
        qDebug() << "ERROR: Dobot::armUpDown(): wrong armDestination val ="
                 << dobotMoveAsQstr(ArmDestination);
        return;
    }

    Point3D dest3D = _lastGivenPoint;
    dest3D.z = dHeight;

    this->addArmMoveToQueue(ArmDestination, dest3D);
}
