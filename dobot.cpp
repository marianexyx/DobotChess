#include "dobot.h"

//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian tylko kalibracyjnych

Dobot::Dobot(ArduinoUsb *pUsb)
{
    _pUsb = pUsb;

    _pQueue = new DobotQueue(*this);
    _pServo = new DobotServo(*this);

    _sItemIDInGripper = 0;
    
    _bConnectedToDobot = false;

    _lastGivenPoint(200,0,25); //todo: pierwszy punkt jako middle above

    _Home.x = 140; //todo: home ciągnąć z xml
    _Home.y = 0;
    _Home.z = 10;
    _Home.r = 0;
}

Dobot::~Dobot()
{
    delete _pQueue;
    delete _pServo;
}

void Dobot::onPeriodicTaskTimer()
{
    PeriodicTask(); //check dobot actual data
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

void Dobot::saveActualDobotPosition()
{
    Pose pose;
    GetPose(&pose); //from dobot

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

Point3D Dobot::getHomePos()
{
    Point3D home(_Home);
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

        //create dobot periodic timer
        QTimer *periodicTaskTimer = new QTimer(this);
        periodicTaskTimer->setObjectName("periodicTaskTimer");
        connect(periodicTaskTimer, timeout(), this, onPeriodicTaskTimer());
        periodicTaskTimer->setSingleShot(true);
        periodicTaskTimer->start(5);
        //future: ciągle dostaję błąd: "QObject::startTimer: Timers can only...
        //...be used with threads started with QThread", jednak ciągle to działa.

        //create dobot pose timer
        QTimer *getPoseTimer = new QTimer(this);
        getPoseTimer->setObjectName("getPoseTimer");
        connect(getPoseTimer, timeout(), this, onGetPoseTimer());
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
    
    //set the end effector parameters
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 71.6f; //59.7f; TODO: wcześniej była ta wartość. co...
    //...ona oznacza w praktyce?
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

void Dobot::doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump)
{
    if (this->isPointTotallyDiffrent(dest3D)) return;

    if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_OPEN);

    dest3D.z += dJump;
    this->addArmMoveToQueue(DM_TO_POINT, dest3D);

    if (VertMove == VM_NONE) return;

    if (!this->isPointDiffrentOnlyInZAxis(dest3D)) return;
    this->armUpDown(DM_DOWN, dest3D - dJump);

    if (VertMove == VM_PUT)
        this->addArmMoveToQueue(DM_OPEN);
    else if (VertMove == VM_GRAB)
        this->addArmMoveToQueue(DM_CLOSE);

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

void Dobot::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point = _lastGivenPoint)
{
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

void Dobot::writeMoveTypeInConsole(DOBOT_MOVE_TYPE MoveType)
{
    QString QStrMsg;

    switch(MoveType)
    {
    case DM_TO_POINT: QStrMsg = ""; break;
    case DM_OPEN: QStrMsg = "gripper opened\n"; break;
    case DM_CLOSE: QStrMsg = "gripper closed\n"; break;
    case DM_UP: QStrMsg = "arm up\n"; break;
    case DM_DOWN: QStrMsg = "arm down\n"; break;
    default: QStrMsg = "ERROR: Dobot::writeMoveTypeInConsole(): wrong movement state: "
                + dobotMoveAsQstr(MoveType) + "\n"; break;
    }

    emit this->addTextToLogPTE(QStrMsg, LOG_DOBOT);
}
