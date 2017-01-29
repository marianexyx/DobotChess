#include "dobot.h"

Dobot::Dobot()
{
    connectStatus = false;

    m_gripperServo1.address = 6;
    m_gripperServo1.frequency = 50.f;
    m_gripperServo1.dutyCycle = 7.5f;
    m_gripperServo2.address = 4;
    m_gripperServo2.frequency = 50.f;
    m_gripperServo2.dutyCycle = 7.5f;

    //potrzebny jest poniższy timer dla dobota, bo tylko w nim obracane są jego komendy.
    QTimer *periodicTaskTimer = new QTimer(this); //stworzenie zegarka liczącego czas. zegar liczy
    //czas jak normalny zegar. new QTimer(this) oznacza że obiekt jest dzieckiem tej klasy, tj.
    //dobota i razem z nim zostanie zniszczony. zegarek wymaga jeszcze
    periodicTaskTimer->setObjectName("periodicTaskTimer");

    //kiedy timer zakończy żywot/zatrzyma się to odpal funkcję spod slotu
    connect(periodicTaskTimer, SIGNAL(timeout()), this, SLOT(onPeriodicTaskTimer()));
    periodicTaskTimer->setSingleShot(true); //ustawia zegar na jednorazowy(?). nie odpala się co ...
    //... jakiś ustalony interwał sam z siebie zegar (cokolwiek to znaczy w praktyce).
    periodicTaskTimer->start(5); //za każdym razem zakończ poprzedni timer i po 5ms zresetuj zegar.
    //ustawienie na 0 (brak) drenowało by kompa z zasobów najprawdopodobniej.
    //ciągle dostaję błąd:
    //QObject::startTimer: Timers can only be used with threads started with QThread

    //getPose Timer
    QTimer *getPoseTimer = new QTimer(this);
    getPoseTimer->setObjectName("getPoseTimer");
    connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
    getPoseTimer->setSingleShot(true);
    //getPoseTimer->start(200);
}

void Dobot::onPeriodicTaskTimer() //wykonywane w kółko
{
    PeriodicTask(); //??? kolejna z funkcji dobota która nei wiadomo jak działa
    QTimer *periodicTaskTimer = findChild<QTimer *>("periodicTaskTimer");
    periodicTaskTimer->start();
}

void Dobot::onGetPoseTimer()
{
    QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
    Pose pose;
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
}

void Dobot::onConnectDobot()
{
    //connect dobot
    if (!connectStatus)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        connectStatus = true;
        this->refreshBtn();
        this->initDobot();

        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->start(200);

        qDebug() << "Dobot connection success";
        this->addTextToDobotConsole("Dobot connected");
    }
    else
    {
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
        //Command timeout
        SetCmdTimeout(3000);
        //clear old commands and set the queued command running
        SetQueuedCmdClear();
        SetQueuedCmdStartExec();

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

        //TODO: nie ma sensu ustawiać JOG parametrów, jak nie będę nigdy sterował tutaj kątami
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

        JOGCommonParams jogCommonParams;
        jogCommonParams.velocityRatio = 50;
        jogCommonParams.accelerationRatio = 50;
        SetJOGCommonParams(&jogCommonParams, false, NULL);

        PTPJointParams ptpJointParams;
        for (int i = 0; i < 4; i++)
        {
            ptpJointParams.velocity[i] = 100;
            ptpJointParams.acceleration[i] = 100;
        }
        SetPTPJointParams(&ptpJointParams, false, NULL);

        PTPCoordinateParams ptpCoordinateParams;
        ptpCoordinateParams.xyzVelocity = 100;
        ptpCoordinateParams.xyzAcceleration = 100;
        ptpCoordinateParams.rVelocity = 100;
        ptpCoordinateParams.rAcceleration = 100;
        SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);

        PTPJumpParams ptpJumpParams;
        ptpJumpParams.jumpHeight = 20;
        ptpJumpParams.zLimit = 150;
        SetPTPJumpParams(&ptpJumpParams, false, NULL);

        HOMEParams HomeChess;
        HomeChess.x = 140;
        HomeChess.y = 0;
        HomeChess.z = 10; //niżej będzie waliło w szachownicę
        HomeChess.r = 0;
        SetHOMEParams(&HomeChess, false, NULL);
}

//TODO: upchać to ładnie gdzie indziej
static int nPtpCmd_xActualVal = 200;
static int nPtpCmd_yActualVal = 0;
static int nPtpCmd_zActualVal = 25;
static int nPtpCmd_rActualVal = 0;

void Dobot::PTPvalues(int nPtpCmd_xVal, int nPtpCmd_yVal, int nPtpCmd_zVal, int nPtpCmd_rVal)
{
    if (nPtpCmd_xVal != 1000) nPtpCmd_xActualVal = nPtpCmd_xVal; //TODO: dla piękna kodu zrobić coś...
    if (nPtpCmd_yVal != 1000) nPtpCmd_yActualVal = nPtpCmd_yVal; //...w stylu: #define ACTUAL_POS 1000;
    if (nPtpCmd_zVal != 1000) nPtpCmd_zActualVal = nPtpCmd_zVal;
    if (nPtpCmd_rVal != 1000) nPtpCmd_rActualVal = nPtpCmd_rVal;

    PTPCmd ptpCmd;
    ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode


    qDebug() << "Dobot::PTPvalues: x =" << nPtpCmd_xActualVal << ", y =" << nPtpCmd_yActualVal <<
                ", z =" << nPtpCmd_zActualVal << ", r =" << nPtpCmd_rActualVal;
    //TODO: sprawdzić jak się sprawdzi ruch kartezjański skokowy: JUMP_XYZ
    ptpCmd.x = nPtpCmd_xActualVal;
    ptpCmd.y = nPtpCmd_yActualVal;
    ptpCmd.z = nPtpCmd_zActualVal;
    ptpCmd.r = nPtpCmd_rActualVal; //TODO: co się stanie jak nie będę podawał do SetPTPCmd
    //paramentru ptpCmd.r? zachowa aktualną wartość?
    //ptpCmd.velocity = 100; //niech używa wartości które ma aktualnie

    //SetQueuedCmdStartExec(); //TODO: co to robi? było w dobocie po patchu. dokumentacja cienka.
    SetPTPCmd(&ptpCmd, true, NULL); //kolejkowanie zapytań ustawione na true
    //TODO: 3ci parametr zmieniać z indexy po których będzie można sprawdzać czy ruch został wykonany...
    //... i który ruch aktualnie się wykonuje (więcej w intrukcji do dobota)
}

void Dobot::gripperOpennedState(bool gripperClosed)
{
    m_gripperServo1.dutyCycle = gripperClosed ? 3.1f : 3.6f;
    m_gripperServo2.dutyCycle = gripperClosed ? 8.8f : 8.3f;

    //SetIOPWM(IOPWM *ioPWM, bool isQueued, uint64_t *queuedCmdIndex);
    SetIOPWM(&m_gripperServo1, true, NULL);
    SetIOPWM(&m_gripperServo2, true, NULL);
}

void Dobot::gripperAngle(float fDutyCycle1, float fDutyCycle2)
{
    if (fDutyCycle1 != 0) m_gripperServo1.dutyCycle = fDutyCycle1;
    if (fDutyCycle2 != 0) m_gripperServo2.dutyCycle = fDutyCycle2;
    qDebug() << "m_gripperServo1.dutyCycle = " << fDutyCycle1;
    qDebug() << "m_gripperServo2.dutyCycle = " << fDutyCycle2;
    SetIOPWM(&m_gripperServo1, true, NULL);
    SetIOPWM(&m_gripperServo2, true, NULL);
}

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
