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
    _bFirstMoveIsDone = false;

    _home.x = gameConfigVars.home.x;
    _home.y = gameConfigVars.home.y;
    _home.z = gameConfigVars.home.z;
    _home.r = 0;

    _homeToMiddleAbove = gameConfigVars.homeToMiddleAbove;

    //future: this below shows that we r using chessboard. it may be wiped, only if...
    //...it's passed, but it's not efficient. better wasy might be passing whole object.
    _dSafeAxisZ = qMin(qMin(gameConfigVars.A1.z, gameConfigVars.A8.z),
                       qMin(gameConfigVars.H1.z, gameConfigVars.H8.z)) +
            gameConfigVars.fPieceHeight;

    connect(_pQueue, SIGNAL(sendMoveToArm(DobotMove)),
            this, SLOT(sendMoveToArm(DobotMove)));
    connect(_pQueue, SIGNAL(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)),
            this, SLOT(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)));
    connect(_pQueue, SIGNAL(addTextToLogPTEInUI(QString, LOG)),
            this, SLOT(addTextToLogPTEInUI(QString, LOG)));
    connect(_pQueue, SIGNAL(showQueuedArmCmdsOnCore()),
            this, SLOT(showQueuedArmCmdsOnCorePTE()));
    connect(_pQueue, SIGNAL(showSentArmCmdsToDobot()),
            this, SLOT(showSentArmCmdsToDobotPTE()));
}

Dobot::~Dobot()
{
    delete _pQueue;
    delete _pServo;
}

void Dobot::onPeriodicTaskTimer()
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
        if (_pQueue->isArmCoveringGame() && _bFirstMoveIsDone)
            _pQueue->retreat(_lastGivenPoint);
    }

    getPoseTimer->start(); //auto restart timer
}

void Dobot::showQueueLabelsInUI(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                                int nCoreIdLeft, uint64_t un64CoreNextId)
{
    emit this->queueLabels(unSpace, un64DobotId, un64CoreMaxId, nCoreIdLeft, un64CoreNextId);
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

        this->createAndStartPeriodicTimer();
        this->createAndStartPoseTimer();
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

    emit setDobotButtonsStates(_bConnectedToDobot);
}

void Dobot::createAndStartPeriodicTimer()
{
    //create dobot periodic timer
    QTimer *periodicTaskTimer = new QTimer(this);
    periodicTaskTimer->setObjectName("periodicTaskTimer");
    connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
    periodicTaskTimer->setSingleShot(true);
    periodicTaskTimer->start(5);
    //future: i'm still getting warning in debug: "QObject::startTimer: Timers...
    //...can onlybe used with threads started with QThread", yet it still works

}

void Dobot::createAndStartPoseTimer()
{
    QTimer *getPoseTimer = new QTimer(this);
    getPoseTimer->setObjectName("getPoseTimer");
    connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
    getPoseTimer->setSingleShot(true);
    getPoseTimer->start(200);
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
    
    SetHOMEParams(&_home, false, NULL);

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
        //todo: block "Z" axis, if it is too low
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
    case DM_CALIBRATE: //todo: this is home or calibrate? mess name
    {
        emit this->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd HOME;
        HOME.reserved = 1; //todo: i dont get this "1" ID. seen somewhere propably. ask of forum
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
    if (!_bConnectedToDobot)
    {
        qDebug() << "ERROR: Dobot::queueMoveSequence(): dobot not connected";
        return;
    }

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

bool Dobot::isMoveSafe(Point3D point)
{
    if (point.z <= _dSafeAxisZ && point.x != _lastGivenPoint.x &&
            point.y != _lastGivenPoint.y)
    {
        qDebug() << "ERROR: Dobot::isMoveSafe(): it's not. given point =" <<
                    point.getAsQStr() << ", _lastGivenPoint =" <<
                    _lastGivenPoint.getAsQStr() << ", _dSafeAxisZ =" << _dSafeAxisZ;
        return false;
    }
    else return true;
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
    if (!_bFirstMoveIsDone && (Type == DM_TO_POINT || Type == DM_UP || Type == DM_DOWN))
    {
        qDebug() << "WARNING: Dobot::addArmMoveToQueue(): move type =" <<
                    dobotMoveAsQstr(Type) << "cannot be the first arm move";
        return;
    }

    Point3D point = _lastGivenPoint;
    _pQueue->addArmMoveToQueue(Type, point);
}

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    if (!this->isMoveSafe(point)) return;

    if (!_bFirstMoveIsDone) _bFirstMoveIsDone = true;

    _lastGivenPoint = point;
    _pQueue->addArmMoveToQueue(Type, point);
}

void Dobot::armUpDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight)
{
    if (!_bFirstMoveIsDone)
    {
        qDebug() << "WARNING: Dobot::armUpDown(): first move cannot be up/down type";
        return;
    }

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
