#include "dobot.h"

#define ROW 1
#define COLUMN 0

#define NORMAL -1
#define WAIT 0
#define GRIPPER1 1
#define GRIPPER2 2

//TODO: nie mam wogle sprawdzane czy jakiekolwiek dane dotarły do dobota. program wypuszcza dane na...
//...usb i zakłada że ruch się wykonał bez sprawdzania tego.

Dobot::Dobot(Chessboard *pChessboard):
    m_nMaxPieceHeight(45),
    m_nMaxRemPieceH(44.5),
    m_nActualPos(1000),
    m_fPtpCmd_xActualVal(200),
    m_fPtpCmd_yActualVal(0),
    m_fPtpCmd_zActualVal(25),
    m_fPtpCmd_rActualVal(0)
{
    _pChessboard = pChessboard;
    
    connectStatus = false;
    
    m_gripperServo1.address = 6;
    m_gripperServo1.frequency = 50.f;
    m_gripperServo1.dutyCycle = 7.5f;
    m_gripperServo2.address = 4;
    m_gripperServo2.frequency = 50.f;
    m_gripperServo2.dutyCycle = 7.5f;
    
    ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
    //TODO: ptpCmd.velocity = 100; //niech używa wartości które ma aktualnie
    
    m_ullCoreQueuedCmdIndex = 1; //TODO: dobot startuje bodajże z jedynką
    m_uiQueuedCmdLeftSpace = 10000; //jakaś duża liczba, bo mam dalej warunek krytycznego błędu dla zera
    
    firstPosId.index = 0;
    lastPosId.index = 0;
    takenPosId.index = 0;
    
    //to poniżej to w skrócie utrzymywanie ciągłej komunikacji z dobotem
    
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
    PeriodicTask(); //zapytuj się dobota co tam u niego co 100ms
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
    
    this->QueuedIdList(); //sprawdzaj w kółko czy można wypchać do zakolejkowania dobotowi kolejny ruch
    //TODO: to tu  powoduje to deziorientację będąc w tym miejscu.
    
    getPoseTimer->start();
}

void Dobot::QueuedIdList()
{
    //m_uiQueuedCmdLeftSpace= GetQueuedCmdLeftSpace; //ile zostało pamięci w dobocie na ruchy. ...
    //... elseTODO: Póki co nie ma w dobocie tej funkcji. Może z czasem dodadzą.
    /*if (m_uiQueuedCmdLeftSpace <= 0) //jeżeli pamięć dobota spadła do zera- została przepełniona
    { //elseTODO: póki dobot nie wklei tego do swojego dll'a to nie mogę tego używać
        qDebug() << "FATAL ERROR: Dobot queue memory  full. data overflown/lost. Stop arm.";
        this->addTextToDobotConsole("FATAL ERROR: Dobot queue memory full. Data overflown. Stop arm.\n");
        SetQueuedCmdForceStopExec(); //zatrzymaj ramię
    }*/

    GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny index dobota

    emit this->QueueLabels(m_uiQueuedCmdLeftSpace, m_ullDobotQueuedCmdIndex,
                           m_ullCoreQueuedCmdIndex, QueuedCmdIndexList.size(),
                           firstPosId.index);

    if (!QueuedCmdIndexList.isEmpty()) //jeżeli kontener nie jest pusty
    {
        /*qDebug() << "firstPosId.index =" << firstPosId.index;
        qDebug() << "m_ullDobotQueuedCmdIndex =" << m_ullDobotQueuedCmdIndex;
        qDebug() << "firstPosId.index - m_ullDobotQueuedCmdIndex ="
                 << firstPosId.index - m_ullDobotQueuedCmdIndex;*/

        QListIterator<ArmPosCrntCmdQIdx> QueuedCmdIndexIter(QueuedCmdIndexList);

        QueuedCmdIndexIter.toFront(); //najstarszy wpis w kontenerze
        if(QueuedCmdIndexIter.hasNext())  //zabezpieczenie przed segmentation fault
        {
           firstPosId = QueuedCmdIndexIter.next(); //najstarszy wpis w kontenerze (najmniejszy ID)

            if(firstPosId.index  - m_ullDobotQueuedCmdIndex < 15) //ten warunek powinien działać w...
                //...100%, jeżeli nie działa to błąd jest gdzie indziej
            {
                takenPosId = QueuedCmdIndexList.takeFirst(); //wypchnij na dobota kolejny id...
                //...ruchu z kontenera. najstarszy wpis w kontenerze znajduje się na jego początku.
                switch(takenPosId.type)
                {
                //TODO: brakuje tu HOMEcmd
                case NORMAL:
                    ptpCmd.x = takenPosId.x;
                    ptpCmd.y = takenPosId.y;
                    ptpCmd.z = takenPosId.z;
                    ptpCmd.r = takenPosId.r;
                    SetPTPCmd(&ptpCmd, true, &takenPosId.index);
                    break;
                case WAIT:
                    SetWAITCmd(&gripperMoveDelay, true, &takenPosId.index);
                    break;
                case GRIPPER1:
                    m_gripperServo1.dutyCycle = takenPosId.state ? 3.6f : 3.1f;
                    SetIOPWM(&m_gripperServo1, true, &takenPosId.index);
                    break;
                case GRIPPER2:
                    m_gripperServo2.dutyCycle = takenPosId.state ? 8.3f : 8.8f;
                    SetIOPWM(&m_gripperServo2, true, &takenPosId.index);
                    break;
                default:
                    qDebug() << "ERROR: Dobot::QueuedIdList(): unknown takenPosId.type:" << takenPosId.type;
                    this->addTextToDobotConsole("ERROR: Dobot::QueuedIdList(): unknown takenPosId.type:"
                                                + takenPosId.type);
                }
            }
        }
    }
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
        
        SetQueuedCmdClear(); //wyczyść/wyzeruj zapytania w dobocie
        GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny index dobota.
        SetQueuedCmdStartExec(); //rozpocznij wykonywanie zakolejkowanych komend.
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
    SetHOMEParams(&HomeChess, false, NULL); //todo: NULL- pewnie dlatego mi się wykrzacza ID
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
        int nRemPiece, nRemPieceDestLetter, nRemPieceDestDigit;
        if (chMoveType == 'r') //jako że usuwanie na obszar zbity jest drugim ruchem, to...
            //...pozycję docelową bierki możemy wywnioskować z bierki trzymanej przez chwytak
        {
            nRemPiece = _pChessboard->nGripperPiece; //bierka z planszy w chwytaku do usunięcia
            nRemPieceDestLetter = _pChessboard->fieldNrToFieldPos(nRemPiece, ROW);
            nRemPieceDestDigit = _pChessboard->fieldNrToFieldPos(nRemPiece, COLUMN);
        }
        else if (chMoveType == 's') //dla restore() pozycja pola na obszarze usuniętych musi...
            //...być podana wprost do pieceFromTo()
        {
            //dla 's' wartość nGripperPiece nie zadziała, bo nie ma jeszcze nic w chwytaku
            nRemPieceDestLetter = nLetter;
            nRemPieceDestDigit = nDigit;
        }
        f_xFromTo = _pChessboard->afRemovedPiecesPositions_x[nRemPieceDestLetter][nRemPieceDestDigit];
        f_yFromTo = _pChessboard->afRemovedPiecesPositions_y[nRemPieceDestLetter][nRemPieceDestDigit];
        f_zFromTo = _pChessboard->afRemovedPiecesPositions_z[nRemPieceDestLetter][nRemPieceDestDigit];
        f_rFromTo = m_nActualPos;
    }
    //reszta ruchów: normalne ruchy FromTo, łapanie bierki do zbijania, puszczanie bierki przywróconej
    else
    {
        f_xFromTo = _pChessboard->afChessboardPositions_x[nLetter][nDigit];
        f_yFromTo = _pChessboard->afChessboardPositions_y[nLetter][nDigit];
        f_zFromTo = _pChessboard->afChessboardPositions_z[nLetter][nDigit];
        f_rFromTo = m_nActualPos;
    }
    
    this->writeMoveTypeInConsole(chMoveType);
    
    QString QsMoveType = "";
    bIsPieceMovingFrom ? QsMoveType = "PieceFrom: " : QsMoveType = "Pieceto: ";
    qDebug() << "Dobot::pieceFromTo:" << QsMoveType << "nLetter ="
             << nLetter << ", nDigit =" << nDigit;
    emit this->addTextToDobotConsole(QsMoveType + _pChessboard->findPieceLetterPos(nLetter)
                                     + QString::number(nDigit) + "\n");
    
    this->addCmdToList(NORMAL, false, f_xFromTo, f_yFromTo, f_zFromTo + m_nMaxPieceHeight, f_rFromTo);
    
    _pChessboard->PieceActualPos.Letter = nLetter;
    _pChessboard->PieceActualPos.Digit = nDigit;
}

void Dobot::gripperOpennedState(bool isGripperOpened, char chMovetType) //open/close
{
    this->addCmdToList(GRIPPER1, isGripperOpened);
    this->addCmdToList(GRIPPER2, isGripperOpened);
    
    this->writeMoveTypeInConsole(chMovetType, isGripperOpened ? 'o' : 'c'); //o-open, c-close
}

void Dobot::wait(int nMs)
{
    gripperMoveDelay.timeout = nMs; //czekaj 0,3 sekundy po dojściu na pozycję by chwytaka
    //silniki zdążyły się ruszyć, zanimramię zacznie wykonywać kolejny ruch
    this->addCmdToList(WAIT);
}

void Dobot::armUpDown(bool isArmGoingUp, bool bIsArmAboveFromSquare, char chMoveType)
{ //TODO: cała ta metoda to syf jeżeli chodzi o przejrzystość i nazewnictwo funkcji
    float f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown;
    //pozycje obszaru bierek usuniętych
    if (chMoveType == 'r' && !bIsArmAboveFromSquare) //jeżeli odstawiamy bierkę na obszar zbitych...
    {
        qDebug() << "Dobot::armUpDown: nRemovingRWPos ="
                 << _pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)
                 << _pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)
                 << ", isArmGoingUp?:" << isArmGoingUp;
        //TODO: ten zapis fieldNrToFieldPos działa, ale zmienić jakoś nazewnictwo funkcji, bo jest...
        //...nieadekwatna w tym przypadku.
        f_xUpDown = _pChessboard->afRemovedPiecesPositions_x
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        f_yUpDown = _pChessboard->afRemovedPiecesPositions_y
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        if (isArmGoingUp) f_zUpDown = m_nMaxRemPieceH;
        else f_zUpDown = _pChessboard->afRemovedPiecesPositions_z
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        f_rUpDown = m_nActualPos;
    }
    else if (chMoveType == 's' && bIsArmAboveFromSquare) //...lub pochwycamy bierkę z obszaru zbitych
    {
        qDebug() << "Dobot::armUpDown: nRemovingRWPos =" << _pChessboard->PieceActualPos.Letter
                 << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?:" << isArmGoingUp;
        f_xUpDown = _pChessboard->afRemovedPiecesPositions_x
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_yUpDown = _pChessboard->afRemovedPiecesPositions_y
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        if (isArmGoingUp) f_zUpDown = m_nMaxPieceHeight + _pChessboard->afRemovedPiecesPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        else f_zUpDown = _pChessboard->afRemovedPiecesPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_rUpDown = m_nActualPos;
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
        f_rUpDown = m_nActualPos;
    }
    this->addCmdToList(NORMAL, false, f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown);
    
    this->writeMoveTypeInConsole(chMoveType, isArmGoingUp ? 'u' : 'd'); //u-up, d-down
}

//TODO: chyba to się niczym nie różni od Dobot::pieceFromTo oprócz szachownicy/usuniętych
void Dobot::removePiece(int nPieceRowPos, int nPieceColumnPos)
{
    float f_xRemove = _pChessboard->afRemovedPiecesPositions_x[nPieceRowPos][nPieceColumnPos];
    float f_yRemove = _pChessboard->afRemovedPiecesPositions_y[nPieceRowPos][nPieceColumnPos];
    float f_zRemove = _pChessboard->afRemovedPiecesPositions_z[nPieceRowPos][nPieceColumnPos];
    float f_rRemove = m_nActualPos;
    qDebug() << "Dobot::removePiece values: x =" << f_xRemove << ", y =" << f_yRemove << ", z =" <<
                m_nMaxRemPieceH << ", r =" << f_rRemove;
    
    //z_max = 40 dla x = 275, a najwyższe z na styku z podłogą to z = -16.5
    this->addCmdToList(NORMAL, false, f_xRemove, f_yRemove, m_nMaxRemPieceH, f_rRemove);
}
/// END OF: TYPY RUCHÓW PO PLANSZY

void Dobot::addCmdToList(int nType, bool bState, int x, int y, int z, int r)
{
    if (x != m_nActualPos) m_fPtpCmd_xActualVal = x;
    if (y != m_nActualPos) m_fPtpCmd_yActualVal = y;
    if (z != m_nActualPos) m_fPtpCmd_zActualVal = z;
    if (r != m_nActualPos) m_fPtpCmd_rActualVal = r;
    
    /*qDebug() << "x =" << m_fPtpCmd_xActualVal << ", y =" << m_fPtpCmd_yActualVal <<
                ", z =" << m_fPtpCmd_zActualVal << ", r =" << m_fPtpCmd_rActualVal; //jeśli to się...
    //...jeszcze ma przydać, to niech wyląduje gdzieś w QueuedIdList.*/
    
    m_ullCoreQueuedCmdIndex += 1; //aktualne id ruchu = +1 większe od ostatniego
    m_posIdx.index = m_ullCoreQueuedCmdIndex;
    m_posIdx.type = nType; //wait, gripper1, gripper2
    m_posIdx.state = bState; //open/close gripper
    m_posIdx.x = m_fPtpCmd_xActualVal;
    m_posIdx.y = m_fPtpCmd_yActualVal;
    m_posIdx.z = m_fPtpCmd_zActualVal;
    m_posIdx.r = m_fPtpCmd_rActualVal;
    QueuedCmdIndexList << m_posIdx; //wepchnij do kontenera strukturę. << == append == push_back
}

void Dobot::writeMoveTypeInConsole(char chMoveType, char chMoveState)
{
    QString QsConsoleMsg;
    switch(chMoveType)
    {
    case 'n': QsConsoleMsg = "normal"; break; //ruch z szachownicy na szachownicę (zwykły)
    case 'c': QsConsoleMsg = "castling(king)"; break; //ruch po szachownicy (roszada królem)
    case 'o': QsConsoleMsg = "castling(rook)"; break; //ruch po szachownicy (roszada wieżą)
    case 'e': QsConsoleMsg = "enpassant"; break; //ruch z szachownicy na szachownicę (enpassant)
    case 'p': QsConsoleMsg = "promotion"; break; //ruch z szachownicy na szachownicę (promocja)
    case 'r': QsConsoleMsg = "remove"; break; //ruch z szachownicy na obszar zbitych bierek
    case 's': QsConsoleMsg = "restore"; break; //ruch z obszaru zbitych bierek na szachownicę
    case 'v': QsConsoleMsg = "service"; break; //ruch serwisowy
    default: QsConsoleMsg = "ERROR. Wrong movement type: "
                + static_cast<QString>(chMoveType) + "\n"; break;
    }
    emit this->addTextToDobotConsole(QsConsoleMsg + "PieceMove");
    
    QString QsSecondMsg;
    switch(chMoveState)
    {
    case 'o': QsSecondMsg = ": GripperOpened\n"; break;
    case 'c': QsSecondMsg = ": GripperClosed\n"; break;
    case 'u': QsSecondMsg = ": ArmUp\n"; break;
    case 'd': QsSecondMsg = ": ArmDown\n"; break;
    case 'x': break; //tak aby default się na to nie łapał
    default: QsSecondMsg = "ERROR. Wrong movement state: "
                + static_cast<QString>(chMoveState) + "\n"; break;
    }
    if (chMoveState != 'x') emit this->addTextToDobotConsole(QsSecondMsg);
    
    qDebug() << "Start" << QsConsoleMsg << "move sequence" << QsSecondMsg;
}

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
