#include "dobot.h"

#define ACTUAL_POS  1000;

#define ROW 1
#define COLUMN 0

Dobot::Dobot(Chessboard *pChessboard)
{
    _pChessboard = pChessboard;

    connectStatus = false;

    m_gripperServo1.address = 6;
    m_gripperServo1.frequency = 50.f;
    m_gripperServo1.dutyCycle = 7.5f;
    m_gripperServo2.address = 4;
    m_gripperServo2.frequency = 50.f;
    m_gripperServo2.dutyCycle = 7.5f;

    m_nMaxPieceHeight = 45;
    m_nMaxRemovedPieceHeight = 55;

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
        this->addTextToDobotConsole("Dobot connected \n");
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
static int fPtpCmd_xActualVal = 200;
static int fPtpCmd_yActualVal = 0;
static int fPtpCmd_zActualVal = 25;
static int fPtpCmd_rActualVal = 0;

void Dobot::PTPvalues(float fPtpCmd_xVal, float fPtpCmd_yVal, float fPtpCmd_zVal, float fPtpCmd_rVal)
{
    if (fPtpCmd_xVal != 1000) fPtpCmd_xActualVal = fPtpCmd_xVal; //TODO: dla piękna kodu zrobić coś...
    if (fPtpCmd_yVal != 1000) fPtpCmd_yActualVal = fPtpCmd_yVal; //...w stylu: #define ACTUAL_POS 1000;
    if (fPtpCmd_zVal != 1000) fPtpCmd_zActualVal = fPtpCmd_zVal;
    if (fPtpCmd_rVal != 1000) fPtpCmd_rActualVal = fPtpCmd_rVal;

    PTPCmd ptpCmd;
    ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode


    qDebug() << "Dobot::PTPvalues: x =" << fPtpCmd_xActualVal << ", y =" << fPtpCmd_yActualVal <<
                ", z =" << fPtpCmd_zActualVal << ", r =" << fPtpCmd_rActualVal;
    ptpCmd.x = fPtpCmd_xActualVal;
    ptpCmd.y = fPtpCmd_yActualVal;
    ptpCmd.z = fPtpCmd_zActualVal;
    ptpCmd.r = fPtpCmd_rActualVal; //TODO: co się stanie jak nie będę podawał do SetPTPCmd
    //paramentru ptpCmd.r? zachowa aktualną wartość?
    //ptpCmd.velocity = 100; //niech używa wartości które ma aktualnie

    //SetQueuedCmdStartExec(); //TODO: co to robi? było w dobocie po patchu. dokumentacja cienka.

    SetPTPCmd(&ptpCmd, true, NULL); //kolejkowanie zapytań ustawione na true
    //TODO: 3ci parametr zmieniać z indexy po których będzie można sprawdzać czy ruch został ...
    //... wykonany i który ruch aktualnie się wykonuje (więcej w intrukcji do dobota)
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

///TYPY RUCHÓW PO PLANSZY
void Dobot::pieceFromTo(bool bIsPieceMovingFrom, int nLetter, int nDigit, char chMoveType)
{
    float f_xFromTo, f_yFromTo, f_zFromTo, f_rFromTo;
    //jeżeli bierka jest usuwania na pola zbite(pieceTo) albo przywracania z pól zbitych(pieceFrom)
    if ((chMoveType == 'r' && !bIsPieceMovingFrom) || (chMoveType == 's' && bIsPieceMovingFrom))
    {
        int nRemPiece = _pChessboard->nTransferredPiece; //bierka z planszy w chwytaku do usunięcia
        int nRemPieceDestLetter = _pChessboard->fieldNrToFieldPos(nRemPiece, ROW);
        int nRemPieceDestDigit = _pChessboard->fieldNrToFieldPos(nRemPiece, COLUMN);
        f_xFromTo = _pChessboard->afRemovedPiecesPositions_x[nRemPieceDestLetter][nRemPieceDestDigit];
        f_yFromTo = _pChessboard->afRemovedPiecesPositions_y[nRemPieceDestLetter][nRemPieceDestDigit];
        f_zFromTo = _pChessboard->afRemovedPiecesPositions_z[nRemPieceDestLetter][nRemPieceDestDigit];
        f_rFromTo = ACTUAL_POS;
    }
    else //reszta ruchów: normalne ruchy from/to, łapanie bierki do zbijania, i puszczanie bierki przywróconej.
    {
        f_xFromTo = _pChessboard->afChessboardPositions_x[nLetter][nDigit];
        f_yFromTo = _pChessboard->afChessboardPositions_y[nLetter][nDigit];
        f_zFromTo = _pChessboard->afChessboardPositions_z[nLetter][nDigit];
        f_rFromTo = ACTUAL_POS;
    }

    //TODO: reszta warunków. i zrobić z tego switch()
    if (chMoveType == 'n') emit this->addTextToDobotConsole("normalPieceMoving: ");
    else if (chMoveType == 's') emit this->addTextToDobotConsole("servicePieceMoving: ");
    else if (chMoveType == 'r') emit this->addTextToDobotConsole("removePieceMoving: ");
    else emit this->addTextToDobotConsole("ERROR. Wrong movement type in "
                                          "Dobot::pieceFromTo(): " +
                                          static_cast<QString>(chMoveType) + "\n");

    QString QsMoveType = "";
    bIsPieceMovingFrom ? QsMoveType = "PieceFrom: " : QsMoveType = "Pieceto: ";
    qDebug() << "Dobot::pieceFromTo:" << QsMoveType << "nLetter =" << nLetter << ", nDigit =" << nDigit;
    emit this->addTextToDobotConsole(QsMoveType + _pChessboard->findPieceLetterPos(nLetter)
                                     + QString::number(nDigit) + "\n");

    this->PTPvalues(f_xFromTo, f_yFromTo, f_zFromTo + m_nMaxPieceHeight, f_rFromTo);

    _pChessboard->PieceActualPos.Letter = nLetter;
    _pChessboard->PieceActualPos.Digit = nDigit;
}

void Dobot::gripperOpennedState(bool isGripperOpened, char chMovementType) //open/close
{
    m_gripperServo1.dutyCycle = isGripperOpened ? 3.6f : 3.1f;
    m_gripperServo2.dutyCycle = isGripperOpened ? 8.3f : 8.8f;

    //SetIOPWM(IOPWM *ioPWM, bool isQueued, uint64_t *queuedCmdIndex);
    SetIOPWM(&m_gripperServo1, true, NULL);
    SetIOPWM(&m_gripperServo2, true, NULL);

    WAITCmd gripperMoveDelay;
    gripperMoveDelay.timeout = 300; //czekaj 0,3 sekundy po dojściu na pozycję by chwytaka
    //silniki zdążyły się ruszyć, zanimramię zacznie wykonywać kolejny ruch
    SetWAITCmd(&gripperMoveDelay, true, NULL);

    //TODO: reszta warunków. i zrobić z tego switch()
    if (chMovementType == 'n') emit this->addTextToDobotConsole("normalPieceMoving: ");
    else if (chMovementType == 's') emit this->addTextToDobotConsole("servicePieceMoving: ");
    else if (chMovementType == 'r') emit this->addTextToDobotConsole("removePieceMoving: ");
    else emit this->addTextToDobotConsole("ERROR. Wrong movement type in "
                                          "Dobot::gripperOpennedState(): " +
                                          static_cast<QString>(chMovementType) + "\n");

    if (isGripperOpened)
    {
        qDebug() << "Dobot::OpennedSt: GripperOpened";
        emit this->addTextToDobotConsole("GripperOpened\n");
    }
    else
    {
        qDebug() << "Dobot::OpennedSt: GripperClosed";
        emit this->addTextToDobotConsole("GripperClosed\n");
    }
}

void Dobot::armUpDown(bool isArmGoingUp, bool bIsArmAboveFromSquare, char chMoveType)
{
    float f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown;
    //pozycje obszaru bierek usuniętych
    if ((chMoveType == 'r' && !bIsArmAboveFromSquare) //jeżeli odstawiamy bierkę na obszarze bierek zbitych...
            || (chMoveType == 's' && bIsArmAboveFromSquare)) //...lub pochwycamy bierkę z obszaru bierek zbitych.
    {
        qDebug() << "Dobot::armUpDown: nRemovingRWPos =" << _pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, ROW)
                 << _pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, COLUMN) << ", isArmGoingUp?:" << isArmGoingUp;
        f_xUpDown = _pChessboard->afRemovedPiecesPositions_x    //TODO: ten zapisa działa, ale zmienić jakoś...
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, ROW)] //...nazewnictwo funkcji, bo jest ...
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, COLUMN)]; //...nieadekwatna w tym przypadku.
        f_yUpDown = _pChessboard->afRemovedPiecesPositions_y
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, COLUMN)];
        if (isArmGoingUp) f_zUpDown = m_nMaxRemovedPieceHeight + _pChessboard->afRemovedPiecesPositions_z
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, COLUMN)];
        else f_zUpDown = _pChessboard->afRemovedPiecesPositions_z
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nTransferredPiece, COLUMN)];
        f_rUpDown = ACTUAL_POS;
    }
    else //pozycje na szachownicy
    {
        qDebug() << "Dobot::armUpDown: PieceActualPos =" << _pChessboard->PieceActualPos.Letter
                 << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?:" << isArmGoingUp;
        f_xUpDown = _pChessboard->afChessboardPositions_x
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_yUpDown = _pChessboard->afChessboardPositions_y
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        if (isArmGoingUp) f_zUpDown = m_nMaxPieceHeight + _pChessboard->afChessboardPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        else f_zUpDown = _pChessboard->afChessboardPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_rUpDown = ACTUAL_POS;
    }
    this->PTPvalues(f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown);

    //TODO: reszta warunków. i zrobić z tego switch()
    if (chMoveType == 'n') emit this->addTextToDobotConsole("normalPieceMoving: ");
    else if (chMoveType == 's') emit this->addTextToDobotConsole("restorePieceMoving: ");
    else if (chMoveType == 'r') emit this->addTextToDobotConsole("removePieceMoving: ");
    else if (chMoveType == 'v') emit this->addTextToDobotConsole("servicePieceMoving: ");
    else emit this->addTextToDobotConsole("ERROR. Wrong movement type in "
                                          "Dobot::armUpDown(): " +
                                          static_cast<QString>(chMoveType) + "\n");

    if (isArmGoingUp) emit this->addTextToDobotConsole("ArmUp\n");
    else emit this->addTextToDobotConsole("ArmDown\n");
}

//TODO: chyba to się niczym nie różni od Dobot::pieceFromTo oprócz szachownicy/usuniętych
void Dobot::removePiece(int nPieceRowPos, int nPieceColumnPos)
{
    float f_xRemove = _pChessboard->afRemovedPiecesPositions_x[nPieceRowPos][nPieceColumnPos];
    float f_yRemove = _pChessboard->afRemovedPiecesPositions_y[nPieceRowPos][nPieceColumnPos];
    float f_zRemove = _pChessboard->afRemovedPiecesPositions_z[nPieceRowPos][nPieceColumnPos];
    float f_rRemove = ACTUAL_POS;
    qDebug() << "Dobot::removePiece values: x =" << f_xRemove << ", y =" << f_yRemove << ", z =" <<
                f_zRemove + m_nMaxRemovedPieceHeight << ", r =" << f_rRemove;
    this->PTPvalues(f_xRemove, f_yRemove, f_zRemove + m_nMaxRemovedPieceHeight, f_rRemove); //z_max = 40 dla x = 275, a najwyższe z na styku z podłogą to z = -16.5
}
/// END OF: TYPY RUCHÓW PO PLANSZY

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
