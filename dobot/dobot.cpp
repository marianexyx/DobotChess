#include "dobot.h"

Dobot::Dobot(RealVars gameConfigVars, IntermediatePoints intermediatePoints):
    _ARM_MAX_VELOCITY(300), //todo: what's the max val? 200? 300?
    _ARM_MAX_ACCELERATION(300)
{
    _pQueue = new DobotQueue(intermediatePoints);
    _pGripper = new DobotGripper(gameConfigVars.fGripperOpened, gameConfigVars.fGripperClosed);

    _sItemIDInGripper = 0;
    _bConnectedToDobot = false;
    _bFirstMoveIsDone = false;

    _home.x = gameConfigVars.home.x;
    _home.y = gameConfigVars.home.y;
    _home.z = gameConfigVars.home.z;
    _home.r = 0;

    connect(_pQueue, SIGNAL(sendMoveToArm(DobotMove)),
            this, SLOT(sendMoveToArm(DobotMove)));
    connect(_pQueue, SIGNAL(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)),
            this, SLOT(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)));
    connect(_pQueue, SIGNAL(addTextToLogPTEInUI(QString, LOG)),
            this, SLOT(addTextToLogPTEInUI(QString, LOG)));
    connect(_pQueue, SIGNAL(showQueuedArmCmdsOnCore(QString)),
            this, SLOT(showQueuedArmCmdsOnCorePTE(QString)));
    connect(_pQueue, SIGNAL(showSentArmCmdsToDobot(QString)),
            this, SLOT(showSentArmCmdsToDobotPTE(QString)));
}

Dobot::~Dobot()
{
    delete _pQueue;
    delete _pGripper;
}

void Dobot::onPeriodicTaskTimer()
{
    PeriodicTask(); //start arm task loop. non-return funcion
    QTimer* periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer"); //find timer by name
    periodicTaskTimer->start(); //auto restart timer
}

void Dobot::onGetPoseTimer()
{
    //find timer by name:
    QTimer* getPoseTimer = findChild<QTimer *>("getPoseTimer");

    if (_bConnectedToDobot)
    {
        this->saveActualDobotPosition();
        _pQueue->parseNextMoveToArmIfPossible();
        _pQueue->showLastExecutedArmMoveInUI();
        if (_pQueue->isArmCoveringView() && _bFirstMoveIsDone)
            _pQueue->escape(_lastGivenPoint);
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

void Dobot::setItemInGripper(short sGrippersItemID)
{
    if (!this->isGripperEmpty())
    {
        qCritical() << "gripper isn't empty. item nr in it:" << QString::number(_sItemIDInGripper)
                    << ". given item nr:" << QString::number(sGrippersItemID);
        return;
    }

    _sItemIDInGripper = sGrippersItemID;
}

void Dobot::clearGripper()
{
    if (this->isGripperEmpty())
    {
        qCritical() << "gripper is already empty";
        return;
    }

    _sItemIDInGripper = 0;
}

Point3D Dobot::getHomePos()
{
    Point3D home(_home.x, _home.y, _home.z);
    return home;
}

QString Dobot::dumpAllData()
{
    QString QStrData;

    QStrData = "[dobot.h]\n";
    QStrData += "_sItemIDInGripper: " + QString::number(_sItemIDInGripper) + "\n";
    QStrData += "_bConnectedToDobot: " + QString::number(_bConnectedToDobot) + "\n";
    QStrData += "_bFirstMoveIsDone: " + QString::number(_bFirstMoveIsDone) + "\n";
    QStrData += "_realTimePoint: " + _realTimePoint.getAsQStr() + "\n";
    QStrData += "_lastGivenPoint: " + _lastGivenPoint.getAsQStr() + "\n";
    QStrData += "\n";
    QStrData += _pQueue->dumpAllData();

    return QStrData;
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
    //ClearAllAlarmsState() //future:
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
    endEffectorParams.xBias = 40.f; //determined experimentally for my gripper
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
    
    SetHOMEParams(&_home, false, NULL);

    //set PWM adress on servo pin to make gripper work properly
    IOMultiplexing iom;
    iom.address = 4;
    iom.multiplex = IOFunctionPWM;
    isArmReceivedCorrectCmd(SetIOMultiplexing(&iom, false, NULL), SHOW_ERRORS);
}

void Dobot::sendMoveToArm(DobotMove move)
{
    qInfo() << "move type =" << dobotMoveAsQstr(move.type) << ", ID ="
            << QString::number(move.ID) << ", xyz =" << move.xyz.getAsQStr();

    switch(move.type)
    {
    case DM_TO_POINT:
    case DM_UP:
    case DM_DOWN:
    {
        if (!XmlReader::isPointInLimits(move.xyz)) return;
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
        _pGripper->openGripper(move.ID);
        break;
    case DM_CLOSE:
        _pGripper->closeGripper(move.ID);
        break;
    case DM_WAIT:
        _pGripper->wait(move.ID);
        break;
    case DM_CALIBRATE:
    {
        emit this->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd calibrateCmd;
        //todo: I dont get this "1" ID. seen somewhere propably. ask of forum?...
        //... maybe remove it, and see if anything would change (might be useless)
        calibrateCmd.reserved = 1;
        isArmReceivedCorrectCmd(SetHOMECmd(&calibrateCmd, true, &move.ID), SHOW_ERRORS);
    }
        break;
    default: qCritical() << "wrong move type:" << QString::number(move.type);
    }
}

void Dobot::queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove
                              /*= VM_NONE*/, bool bEscape /*= false*/)
{
    if (!_bConnectedToDobot)
    {
        qWarning() << "dobot not connected";
        return;
    }

    if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_OPEN);

    dest3D.z += dJump;
    this->addArmMoveToQueue(DM_TO_POINT, dest3D);

    _pQueue->setescape(bEscape);

    if (VertMove == VM_NONE) return;

    if (!this->isPointDiffrentOnlyInZAxis(dest3D)) return;
    this->moveArmUpOrDown(DM_DOWN, dest3D.z - dJump);

    if (VertMove == VM_PUT)
        this->addArmMoveToQueue(DM_OPEN);
    else if (VertMove == VM_GRAB)
    {
        this->addArmMoveToQueue(DM_CLOSE);
        this->addArmMoveToQueue(DM_WAIT);
    }

    this->moveArmUpOrDown(DM_UP, dest3D.z);
}

bool Dobot::isMoveSafe(Point3D point)
{
    if (point.z <= _intermediatePoints.safeAxisZ.z
            && point.x != _lastGivenPoint.x && point.y != _lastGivenPoint.y)
    {
        qCritical() << "it's not. given point =" << point.getAsQStr()
                    << ", _lastGivenPoint =" << _lastGivenPoint.getAsQStr()
                    << ", _fSafeAxisZ =" << _intermediatePoints.safeAxisZ.z;
        return false;
    }
    else return true;
}

bool Dobot::isPointDiffrentOnlyInZAxis(Point3D point)
{
    if (point.x != _lastGivenPoint.x || point.y != _lastGivenPoint.y)
    {
        qCritical() << "tried to move Z axis with X or Y axis. point xy ="
                    << QString::number(point.x) << QString::number(point.y)
                    << "_lastGivenPoint xy =" << QString::number(_lastGivenPoint.x)
                    << QString::number(_lastGivenPoint.y);
        return false;
    }
    else return true;
}

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type)
{
    if (!_bFirstMoveIsDone && (Type == DM_TO_POINT || Type == DM_UP || Type == DM_DOWN))
    {
        qWarning() << "move type =" << dobotMoveAsQstr(Type) << "cannot be the first arm move";
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

void Dobot::moveArmUpOrDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight)
{
    if (!_bFirstMoveIsDone)
    {
        qWarning() << "first move cannot be up/down type";
        return;
    }

    if (ArmDestination != DM_UP && ArmDestination != DM_DOWN)
    {
        qCritical() << "wrong armDestination val =" << dobotMoveAsQstr(ArmDestination);
        return;
    }

    Point3D dest3D = _lastGivenPoint;
    dest3D.z = dHeight;

    this->addArmMoveToQueue(ArmDestination, dest3D);
}
