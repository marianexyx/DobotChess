#include "dobot.h"

Dobot::Dobot(ArduinoUsb *pUsb, RealVars gameConfigVars,
             Point3D retreatLeft, Point3D retreatRight):
    _ARM_MAX_VELOCITY(300), //todo: ile jest max? 200? 300?
    _ARM_MAX_ACCELERATION(300)
{
    _pUsb = pUsb;

    _pQueue = new DobotQueue(retreatLeft, retreatRight);
    _pServo = new DobotServo(gameConfigVars.fGripperOpened, gameConfigVars.fGripperClosed);

    _sItemIDInGripper = 0;
    
    _bConnectedToDobot = false;

    Point3D fakeMid(200,0,25); //todo: its old? useless?
    _lastGivenPoint = fakeMid; //todo: make first point as a middle above

    _home.x = gameConfigVars.home.x;
    _home.y = gameConfigVars.home.y;
    _home.z = gameConfigVars.home.z;
    _home.r = 0;

    _homeToMiddleAbove = gameConfigVars.homeToMiddleAbove;

    connect(_pQueue, SIGNAL(sendMoveToArm(DobotMove)),
            this, SLOT(sendMoveToArm(DobotMove)));
    connect(_pQueue, SIGNAL(showQueueLabelsInUI(int, int, int, int, int)),
            this, SLOT(showQueueLabelsInUI(int, int, int, int, int)));
    connect(_pQueue, SIGNAL(addTextToLogPTEInUI(QString, LOG)),
            this, SLOT(addTextToLogPTEInUI(QString, LOG)));
    connect(_pQueue, SIGNAL(showActualDobotQueuedCmdIDList(QList<DobotMove>)),
            this, SLOT(showActualDobotQueuedCmdIDListPTE(QList<DobotMove>)));
    connect(_pQueue, SIGNAL(showOnDobotQueuedCmdsList(QList<DobotMove>)),
            this, SLOT(showOnDobotQueuedCmdsListPTE(QList<DobotMove>)));
}

Dobot::~Dobot()
{
    delete _pQueue;
    delete _pServo;
}

void Dobot::onPeriodicTaskTimer() //todo: change timers names
{
    PeriodicTask(); //start arm task loop. non-return funcion
    //find timer by name:
    QTimer* periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer");
    periodicTaskTimer->start(); //auto restart timer
}

void Dobot::onGetPoseTimer()
{
    QTimer* getPoseTimer = findChild<QTimer *>("getPoseTimer"); //find timer by name

    if (_bConnectedToDobot)
    {
        this->saveActualDobotPosition();
        _pQueue->parseNextMoveToArmIfPossible();
        _pQueue->showLastExecutedArmMoveInUI();
        if (_pQueue->isArmCoveringGame()) _pQueue->retreat(_lastGivenPoint);
    }

    getPoseTimer->start(); //auto restart timer
}

void Dobot::showQueueLabelsInUI(int nSpace, int nDobotId, int nCoreMaxId,
                                int nCoreIdLeft, int nCoreNextId)
{
    emit this->queueLabels(nSpace, nDobotId, nCoreMaxId, nCoreIdLeft, nCoreNextId);
}

void Dobot::addTextToLogPTEInUI(QString QStrTxt, LOG log)
{
    emit this->addTextToLogPTE(QStrTxt, log);
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
    if (point.x > 300) qDebug() << "todo: comprehend locks";
    //todo: get them from xml
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

Point3D Dobot::getHomePos()
{
    Point3D home(_home.x, _home.y, _home.z);
    return home;
}

void Dobot::onConnectDobot()
{
    if (!_bConnectedToDobot)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError)
            emit DobotErrorMsgBox();

        emit this->addTextToLogPTE("Dobot connected \n", LOG_DOBOT);

        //todo: make timers in seperate functions
        //create dobot periodic timer
        QTimer *periodicTaskTimer = new QTimer(this);
        periodicTaskTimer->setObjectName("periodicTaskTimer");
        connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
        periodicTaskTimer->setSingleShot(true);
        periodicTaskTimer->start(5);
        //future: i'm still getting warning in debug: "QObject::startTimer: Timers...
        //...can onlybe used with threads started with QThread", yet it still works

        //create dobot pose timer
        QTimer *getPoseTimer = new QTimer(this);
        getPoseTimer->setObjectName("getPoseTimer");
        connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
        getPoseTimer->setSingleShot(true);
        getPoseTimer->start(200);

        this->initDobot();
        _pQueue->saveIDFromConnectedDobot(); //1st check

        _bConnectedToDobot = true;
    }
    else
    {
        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
        _bConnectedToDobot = false;
        DisconnectDobot();
    }

    emit RefreshDobotButtonsStates(_bConnectedToDobot); //todo: change name
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
    
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 40.f; //determined experimentally for gripper
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
    
    SetHOMEParams(&_home, false, NULL); //todo: NULL??

    IOMultiplexing iom;
    iom.address = 4;
    iom.multiplex = IOFunctionPWM;
    int iomResult = SetIOMultiplexing(&iom, false, NULL);
    if (iomResult != DobotCommunicate_NoError)
        qDebug() << "SetIOMultiplexing error";
}

void Dobot::sendMoveToArm(DobotMove move)
{
    qDebug() << "Dobot::sendMoveToArm(): move type =" << dobotMoveAsQstr(move.type)
             << ", ID =" << move.ID << ", xyz =" << move.xyz.getAsQStr();

    switch(move.type)
    {
    case DM_TO_POINT:
    case DM_UP:
    case DM_DOWN:
    {
        //todo: zabronić oś z jezeli jest zbyt nisko
        PTPCmd moveAsPtpCmd;
        moveAsPtpCmd.ptpMode = PTPMOVLXYZMode; //move type is Cartesian linear
        //future: dobot may have better way of movemenst. maybe CPAbsoluteMode?
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
    case DM_CALIBRATE: //future: this is home or calibrate? mess name
    {
        emit this->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd HOME;
        HOME.reserved = 1; //todo: i dont get this "1" ID. seen somewhere propably
        isArmReceivedCorrectCmd(SetHOMECmd(&HOME, true, &move.ID), SHOW_ERRORS);
    }
        break;
    default:
        qDebug() << "ERROR: Dobot::sendMoveToArm(): wrong move type:" << move.type;
    }
}

void Dobot::queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove
                              /*= VM_NONE*/, bool bRetreat /*= false*/)
{
    //todo: set back 3rd axis secure
    //if (_bConnectedToDobot && this->isPointTotallyDiffrent(dest3D)) return;

    if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_OPEN);

    dest3D.z += dJump;
    this->addArmMoveToQueue(DM_TO_POINT, dest3D);

    _pQueue->setRetreat(bRetreat);

    if (VertMove == VM_NONE) return;

    if (!this->isPointDiffrentOnlyInZAxis(dest3D)) return;
    this->armUpDown(DM_DOWN, dest3D.z - dJump);

    if (VertMove == VM_PUT)
        this->addArmMoveToQueue(DM_OPEN);
    else if (VertMove == VM_GRAB)
    {
        this->addArmMoveToQueue(DM_CLOSE);
        this->addArmMoveToQueue(DM_WAIT);
    }

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
