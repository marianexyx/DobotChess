#include "dobot.h"

Dobot::Dobot(DobotRealVars realVars, IntermediatePoints intermediatePoints):
    m_ARM_MAX_VELOCITY(300), //todo: what's the max val? 200? 300?
    m_ARM_MAX_ACCELERATION(300)
{
    m_pQueue = new DobotQueue(intermediatePoints);
    m_pGripper = new DobotGripper(realVars.fGripperOpened, realVars.fGripperClosed);

    m_usItemIDInGripper = 0;
    m_bConnectedToDobot = false;
    m_bFirstMoveIsDone = false;

    m_intermediatePoints = intermediatePoints;

    m_home.x = realVars.home.x;
    m_home.y = realVars.home.y;
    m_home.z = realVars.home.z;
    m_home.r = 0;

    connect(m_pQueue, SIGNAL(sendMoveToArm(DobotMove)),
            this, SLOT(sendMoveToArm(DobotMove)));
    connect(m_pQueue, SIGNAL(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)),
            this, SLOT(showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t)));
    connect(m_pQueue, SIGNAL(addTextToLogPTEInUI(QString, LOG)),
            this, SLOT(addTextToLogPTEInUI(QString, LOG)));
    connect(m_pQueue, SIGNAL(showQueuedArmCmdsOnCore(QString)),
            this, SLOT(showQueuedArmCmdsOnCorePTE(QString)));
    connect(m_pQueue, SIGNAL(showSentArmCmdsToDobot(QString)),
            this, SLOT(showSentArmCmdsToDobotPTE(QString)));
}

Dobot::~Dobot()
{
    delete m_pQueue;
    delete m_pGripper;
}

void Dobot::onConnectDobot()
{
    if (!m_bConnectedToDobot)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError)
            emit DobotErrorMsgBox();

        emit this->addTextToLogPTE("Dobot connected \n", LOG_DOBOT);

        this->createAndStartPeriodicTimer();
        this->createAndStartPoseTimer();
        this->initDobot();
        m_pQueue->saveIDFromConnectedDobot(); //1st check

        m_bConnectedToDobot = true;
    }
    else
    {
        QTimer* getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
        m_bConnectedToDobot = false;
        DisconnectDobot();
    }

    emit this->setDobotButtonsStates(m_bConnectedToDobot);
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
    //ClearAllAlarmsState() //todo:
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
    endEffectorParams.xBias = 40.f; //distance determined experimentally for my gripper
    SetEndEffectorParams(&endEffectorParams, false, NULL);

    JOGJointParams jogJointParams;
    for (int i=0; i<4; ++i)
    {
        jogJointParams.velocity[i] = m_ARM_MAX_VELOCITY;
        jogJointParams.acceleration[i] = m_ARM_MAX_ACCELERATION;
    }
    SetJOGJointParams(&jogJointParams, false, NULL);

    JOGCoordinateParams jogCoordinateParams;
    for (int i=0; i<4; ++i)
    {
        jogCoordinateParams.velocity[i] = m_ARM_MAX_VELOCITY;
        jogCoordinateParams.acceleration[i] = m_ARM_MAX_ACCELERATION;
    }
    SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);

    JOGCommonParams jogCommonParams;
    jogCommonParams.velocityRatio = m_ARM_MAX_VELOCITY;
    jogCommonParams.accelerationRatio = m_ARM_MAX_ACCELERATION;
    SetJOGCommonParams(&jogCommonParams, false, NULL);

    PTPJointParams ptpJointParams;
    for (int i=0; i<4; ++i)
    {
        ptpJointParams.velocity[i] = m_ARM_MAX_VELOCITY;
        ptpJointParams.acceleration[i] = m_ARM_MAX_ACCELERATION;
    }
    SetPTPJointParams(&ptpJointParams, false, NULL);

    PTPCoordinateParams ptpCoordinateParams;
    ptpCoordinateParams.xyzVelocity = m_ARM_MAX_VELOCITY;
    ptpCoordinateParams.xyzAcceleration = m_ARM_MAX_ACCELERATION;
    ptpCoordinateParams.rVelocity = m_ARM_MAX_VELOCITY;
    ptpCoordinateParams.rAcceleration = m_ARM_MAX_ACCELERATION;
    SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);

    //values for CP copied from here:
    //forum.dobot.cc/t/dobot-magician-taking-pause-when-moving-between-two-points/201/20
    CPParams cpParams;
    cpParams.acc = 100;
    cpParams.juncitionVel = 100;
    cpParams.period = 100; //for nice smoothness
    cpParams.realTimeTrack = false;
    SetCPParams(&cpParams, false, NULL);

    SetHOMEParams(&m_home, false, NULL);

    //set PWM adress on servo pin to make gripper work properly
    IOMultiplexing iom;
    iom.address = 4;
    iom.multiplex = IOFunctionPWM;
    SetIOMultiplexing(&iom, false, NULL);
}

void Dobot::onPeriodicTaskTimer()
{
    PeriodicTask(); //start arm task loop. non-return funcion
    QTimer* periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer"); //find timer by name
    periodicTaskTimer->start(); //auto restart timer
}

void Dobot::onGetPoseTimer()
{
    QTimer* getPoseTimer = findChild<QTimer *>("getPoseTimer"); //find timer by name

    if (m_bConnectedToDobot)
    {
        this->saveActualDobotPosition();
        m_pQueue->parseNextMoveToArmIfPossible();
        m_pQueue->showLastExecutedArmMoveInUI();
        if (m_pQueue->isArmCoveringView() && m_bFirstMoveIsDone)
            m_pQueue->escape(m_lastGivenPoint);
    }

    getPoseTimer->start(); //auto restart timer
}

void Dobot::saveActualDobotPosition()
{
    Pose pose;
    GetPose(&pose); //pose from arm

    m_realTimePoint.x = pose.x;
    m_realTimePoint.y = pose.y;
    m_realTimePoint.z = pose.z;

    emit JointLabelText(QString::number(pose.jointAngle[0], 'f', 2), 1);
    emit JointLabelText(QString::number(pose.jointAngle[1], 'f', 2), 2);
    emit JointLabelText(QString::number(pose.jointAngle[2], 'f', 2), 3);
    emit JointLabelText(QString::number(pose.jointAngle[3], 'f', 2), 4);

    emit AxisLabelText(QString::number(pose.x, 'f', 2), 'x');
    emit AxisLabelText(QString::number(pose.y, 'f', 2), 'y');
    emit AxisLabelText(QString::number(pose.z, 'f', 2), 'z');
    emit AxisLabelText(QString::number(pose.r, 'f', 2), 'r');
}

void Dobot::queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove
                              /*= VM_NONE*/, bool bEscape /*= false*/)
{
    if (!m_bConnectedToDobot)
    {
        //if this will be error debug, then I will be spammed with...
        //...connecting errors all the time, while testing
        qInfo() << "dobot not connected";
        return;
    }

    if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_OPEN);

    dest3D.z += dJump;
    this->addArmMoveToQueue(DM_TO_POINT, dest3D);

    m_pQueue->setEscape(bEscape);

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

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type)
{
    if (!m_bFirstMoveIsDone && (Type == DM_TO_POINT || Type == DM_UP || Type == DM_DOWN))
    {
        qWarning() << "move type =" << dobotMoveAsQstr(Type) << "cannot be the first arm move";
        return;
    }

    Point3D point = m_lastGivenPoint;
    m_pQueue->addArmMoveToQueue(Type, point);
}

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{
    if (!this->isMoveSafe(point)) return;

    if (!m_bFirstMoveIsDone) m_bFirstMoveIsDone = true;

    m_lastGivenPoint = point;
    m_pQueue->addArmMoveToQueue(Type, point);
}

bool Dobot::isMoveSafe(Point3D point)
{
    if (point.z > m_intermediatePoints.safeAxisZ.z
            && point.x != m_lastGivenPoint.x && point.y != m_lastGivenPoint.y)
    {
        qCritical() << "it's not. given point =" << point.getAsQStr()
                    << ", m_lastGivenPoint =" << m_lastGivenPoint.getAsQStr()
                    << ", safe xyz =" << m_intermediatePoints.safeAxisZ.z;
        return false;
    }
    else return true;
}

bool Dobot::isPointDiffrentOnlyInZAxis(Point3D point)
{
    if (point.x != m_lastGivenPoint.x || point.y != m_lastGivenPoint.y)
    {
        qCritical() << "tried to move Z axis with X or Y axis. point xy ="
                    << QString::number(point.x) << QString::number(point.y)
                    << "m_lastGivenPoint xy =" << QString::number(m_lastGivenPoint.x)
                    << QString::number(m_lastGivenPoint.y);
        return false;
    }
    else return true;
}

void Dobot::moveArmUpOrDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight)
{
    if (!m_bFirstMoveIsDone)
    {
        qWarning() << "first move cannot be up/down type";
        return;
    }

    if (ArmDestination != DM_UP && ArmDestination != DM_DOWN)
    {
        qCritical() << "wrong armDestination val =" << dobotMoveAsQstr(ArmDestination);
        return;
    }

    Point3D dest3D = m_lastGivenPoint;
    dest3D.z = dHeight;

    this->addArmMoveToQueue(ArmDestination, dest3D);
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
        moveAsPtpCmd.ptpMode = PTPMOVLXYZMode; //move type is Cartesian-linear
        //future: dobot may have better way of movemenst. maybe CPAbsoluteMode?
        moveAsPtpCmd.x = move.xyz.x;
        moveAsPtpCmd.y = move.xyz.y;
        moveAsPtpCmd.z = move.xyz.z;
        isArmReceivedCorrectCmd(SetPTPCmd(&moveAsPtpCmd, true, &move.ID), SHOW_ERRORS);

        //future: tests. propably i must figure it out from dobot app, or try to reach...
        //...people on forum (search by nicks)
        /*CPCmd cpCmd;
        cpCmd.cpMode = CPAbsoluteMode;
        cpCmd.velocity = 100;
        cpCmd.x = move.xyz.x;
        cpCmd.y = move.xyz.y;
        cpCmd.z = move.xyz.z;
        isArmReceivedCorrectCmd(SetCPCmd(&cpCmd, true, &move.ID), SHOW_ERRORS);*/
        break;
    }
    case DM_OPEN:
        m_pGripper->openGripper(move.ID);
        break;
    case DM_CLOSE:
        m_pGripper->closeGripper(move.ID);
        break;
    case DM_WAIT:
        m_pGripper->wait(move.ID);
        break;
    case DM_CALIBRATE:
    {
        emit this->addTextToLogPTE("HOME Cmd: recalibrating arm...\n", LOG_DOBOT);

        HOMECmd calibrateCmd;
        //future: I dont get this "1" ID. seen somewhere propably. ask of forum?...
        //... maybe remove it, and see if anything would change (might be useless)...
        //...  Info from documentation:
        //queuedCmdIndex: If this command is added to the queue, queuedCmdIndex
        //indicates the index of this command in the queue. Otherwise, it is invalid.
        //...but it looks like this text is everywhere, where queue occurs
        calibrateCmd.reserved = 1;
        isArmReceivedCorrectCmd(SetHOMECmd(&calibrateCmd, true, &move.ID), SHOW_ERRORS);
    }
        break;
    default: qCritical() << "wrong move type:" << QString::number(move.type);
    }
}

void Dobot::setItemInGripper(short sGrippersItemID)
{
    if (!this->isGripperEmpty())
    {
        qCritical() << "gripper isn't empty. item in it:" << QString::number(m_usItemIDInGripper)
                    << ". given item nr:" << QString::number(sGrippersItemID);
        return;
    }

    m_usItemIDInGripper = sGrippersItemID;
}

void Dobot::clearGripper()
{
    if (this->isGripperEmpty())
    {
        qCritical() << "gripper is already empty";
        return;
    }

    m_usItemIDInGripper = 0;
}

void Dobot::forceStopArm() //todo: additionally control with dobot alarms
{
    if (m_bConnectedToDobot)
    {
        qCritical();
        m_pQueue->m_queuedArmCmdsOnCore.clear();
        isArmReceivedCorrectCmd(SetQueuedCmdClear(), SHOW_ERRORS);
        isArmReceivedCorrectCmd(SetQueuedCmdForceStopExec(), SHOW_ERRORS);
        m_bConnectedToDobot = false;
    }
}

Point3D Dobot::getHomePos()
{
    Point3D home(m_home.x, m_home.y, m_home.z);
    return home;
}

QString Dobot::dumpAllData()
{
    QString QStrData;

    QStrData = "[dobot.h]\n";
    QStrData += "m_usItemIDInGripper: " + QString::number(m_usItemIDInGripper) + "\n";
    QStrData += "m_bConnectedToDobot: " + QString::number(m_bConnectedToDobot) + "\n";
    QStrData += "m_bFirstMoveIsDone: " + QString::number(m_bFirstMoveIsDone) + "\n";
    QStrData += "m_realTimePoint: " + m_realTimePoint.getAsQStr() + "\n";
    QStrData += "m_lastGivenPoint: " + m_lastGivenPoint.getAsQStr() + "\n";
    QStrData += "\n";
    QStrData += m_pQueue->dumpAllData();

    return QStrData;
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
