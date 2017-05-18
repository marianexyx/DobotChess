#include "dobot.h"

#define ROW 1
#define COLUMN 0

//TODO: nie mam wogle sprawdzane czy jakiekolwiek dane dotarły do dobota. program wypuszcza dane na...
//...usb i zakłada że ruch się wykonał bez sprawdzania tego.

//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian tylko kalibracyjnych

Dobot::Dobot(Chessboard *pChessboard):
    m_nMaxPieceHeight(52), // Dla pola h8 max wysokość bierki to 46. //TODO: zmienna chessboardu
    m_nMaxRemPieceH(44.5), //TODO: zmienna chessboardu
    m_nActualPos(1000),
    m_fPtpCmd_xActualVal(200), //TODO: zmienne chessboardu?
    m_fPtpCmd_yActualVal(0),
    m_fPtpCmd_zActualVal(25),
    m_fPtpCmd_rActualVal(0),
    m_fGripOpened(6.9f),
    m_fGripClosed(7.55f)
{
    _pChessboard = pChessboard;
    
    connectStatus = false;
    
    m_gripperServo.address = 4;
    m_gripperServo.frequency = 50;
    m_gripperServo.dutyCycle = m_fGripOpened;
    
    ptpCmd.ptpMode = PTPMOVLXYZMode; //typ ruchu to kartezjański liniowy.
    //TODO: w dobocie była taka opcja po patchu: CPAbsoluteMode
    
    m_ullCoreQueuedCmdIndex = 1;
    m_uiQueuedCmdLeftSpace = 10000; //jakaś duża liczba, bo mam dalej warunek krytycznego błędu dla zera
    
    firstPosId.index = 0;
    lastPosId.index = 0;
    takenPosId.index = 0;
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
    
    this->QueuedIdList(); //sprawdzaj w kółko czy można wypchać do zakolejkowania dobotowi kolejny ruch
    this->checkPWM();
    //TODO: to tu  powoduje to deziorientację będąc w tym miejscu.
}

void Dobot::QueuedIdList()
{
    //m_uiQueuedCmdLeftSpace= GetQueuedCmdLeftSpace; //ile zostało pamięci w dobocie na ruchy. ...
    //... elseTODO: Póki co nie ma w dobocie tej funkcji. Może z czasem dodadzą.
    /*if (m_uiQueuedCmdLeftSpace <= 0) //jeżeli pamięć dobota spadła do zera- została przepełniona
    { //elseTODO: póki dobot nie wklei tego do swojego dll'a to nie mogę tego używać
        qDebug() << "FATAL ERROR: Dobot queue memory  full. data overflown/lost. Stop arm.";
        this->addTextToConsole("FATAL ERROR: Dobot queue memory full."
            "Data leak. Stop arm.\n", 'd');
        SetQueuedCmdForceStopExec(); //zatrzymaj ramię
    }*/

    GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny index dobota


    emit this->QueueLabels(m_uiQueuedCmdLeftSpace, m_ullDobotQueuedCmdIndex,
                           m_ullCoreQueuedCmdIndex, QueuedCmdIndexList.size(),
                           firstPosId.index); //todo: zastrukturyzować

    if (!QueuedCmdIndexList.isEmpty()) //jeżeli kontener nie jest pusty
    {
        QListIterator<ArmPosForCurrentCmdQueuedIndex> QueuedCmdIndexIter(QueuedCmdIndexList);

        QueuedCmdIndexIter.toFront(); //najstarszy wpis w kontenerze
        if(QueuedCmdIndexIter.hasNext())
        {
            firstPosId = QueuedCmdIndexIter.next(); //najstarszy wpis w kontenerze (najmniejszy ID)

            if(firstPosId.index - m_ullDobotQueuedCmdIndex < 15)
            {
                takenPosId = QueuedCmdIndexList.takeFirst(); //wypchnij na dobota kolejny id...
                //...ruchu z kontenera. najstarszy wpis w kontenerze znajduje się na jego początku.
                switch(takenPosId.move)
                {
                case TO_POINT:
                    ptpCmd.x = takenPosId.x;
                    ptpCmd.y = takenPosId.y;
                    ptpCmd.z = takenPosId.z;
                    ptpCmd.r = takenPosId.r;
                    SetPTPCmd(&ptpCmd, true, &takenPosId.index);
                    break;
                case WAIT:
                    SetWAITCmd(&gripperMoveDelay, true, &takenPosId.index);
                    break;
                case OPEN_GRIP:
                {
                    m_gripperServo.dutyCycle = m_fGripOpened;
                    int openGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (openGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", DOBOT);
                    }
                }
                    break;
                case CLOSE_GRIP:
                {
                    m_gripperServo.dutyCycle = m_fGripClosed;
                    int closeGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (closeGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", DOBOT);
                    }
                }
                    break;
                case HOME:
                {
                    this->addTextToConsole("HomeCmd: recalibrating arm...\n", DOBOT);
                    HOMECmd HOMEChess;
                    HOMEChess.reserved = 1; //todo: jak się to ma do innych indexów?
                    int result = SetHOMECmd(&HOMEChess, true, &takenPosId.index);
                    if (result != DobotCommunicate_NoError)
                        qDebug() << "ERROR: SetHOMECmd result != DobotCommunicate_NoError";
                }
                    break;
                case UP:
                case DOWN:
                default:
                    qDebug() << "ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:" << takenPosId.sequence;
                    this->addTextToConsole("ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:"
                                           + takenPosId.sequence, DOBOT);
                    break;
                }
            }
        }
    }
}

void Dobot::onConnectDobot()
{
    if (!connectStatus)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        connectStatus = true;
        qDebug() << "Dobot connection success";
        this->addTextToConsole("Dobot connected \n", DOBOT);

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
        
        int result = GetQueuedCmdCurrentIndex(&m_ullDobotQueuedCmdIndex); //sprawdź aktualny id i zapisz w zmiennej
        if (result == DobotCommunicate_NoError)
        {
            emit this->QueueLabels(m_uiQueuedCmdLeftSpace, m_ullDobotQueuedCmdIndex,
                                   m_ullCoreQueuedCmdIndex, QueuedCmdIndexList.size(),
                                   firstPosId.index); //TODO: w zasadzie potrzebuje tylko zaaktualizować...
            //...m_ullDobotQueuedCmdIndex, i to możę nie warto tutaj nawet. bardziej...
            //...są to testy czy to się dzieje
        }
        else
        {
            qDebug() << "ERROR: Dobot::onConnectDobot(): GetQueuedCmdCurrentIndex gone wrong";
            this->addTextToConsole("ERROR: Dobot::onConnectDobot(): "
                                   "GetQueuedCmdCurrentIndex gone wrong \n", DOBOT);
        }

        m_ullCoreQueuedCmdIndex = m_ullDobotQueuedCmdIndex; //jeśli dobot przed aktualnym uruchomieniem programu...
        //...wykonywał jakieś ruchy, to startowy index na core byłby normalnie mniejszy od aktualnego
    }
    else
    {
        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
        getPoseTimer->stop();
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
    
    HOMEParams HomeChess;
    HomeChess.x = nHome_x;
    HomeChess.y = nHome_y;
    HomeChess.z = nHome_z;
    HomeChess.r = nHome_r;
    SetHOMEParams(&HomeChess, false, NULL); //todo: NULL- pewnie dlatego mi się wykrzacza ID
}

//TODO: tu nie zadziała to
void Dobot::gripperAngle(float fDutyCycle)
{
    //uwaga: wykonuje się to polecenie bez kolejki
    if (fDutyCycle != 0) m_gripperServo.dutyCycle = fDutyCycle;
    qDebug() << "m_gripperServo.dutyCycle = " << fDutyCycle;
    SetIOPWM(&m_gripperServo, false, NULL);
}

///TYPY RUCHÓW PO PLANSZY
void Dobot::pieceFromTo(bool bIsPieceMovingFrom, int nLetter, int nDigit, SEQUENCE_TYPE Type)
{
    float f_xFromTo, f_yFromTo, f_zFromTo, f_rFromTo;
    //jeżeli bierka jest usuwania na pola zbite(pieceTo) albo przywracania z pól zbitych(pieceFrom)
    if ((Type == REMOVING && !bIsPieceMovingFrom) || (Type == RESTORE && bIsPieceMovingFrom))
    {
        int nRemPiece, nRemPieceDestLetter, nRemPieceDestDigit;
        if (Type == REMOVING) //jako że usuwanie na obszar zbity jest drugim ruchem, to...
            //...pozycję docelową bierki możemy wywnioskować z bierki trzymanej przez chwytak
        {
            nRemPiece = _pChessboard->nGripperPiece; //bierka z planszy w chwytaku do usunięcia
            nRemPieceDestLetter = _pChessboard->fieldNrToFieldPos(nRemPiece, ROW);
            nRemPieceDestDigit = _pChessboard->fieldNrToFieldPos(nRemPiece, COLUMN);
        }
        else if (Type == RESTORE) //dla restore() pozycja pola na obszarze usuniętych musi...
            //...być podana wprost do pieceFromTo()
        {
            //dla RESTORE wartość nGripperPiece nie zadziała, bo nie ma jeszcze nic w chwytaku
            nRemPieceDestLetter = nLetter;
            nRemPieceDestDigit = nDigit;
        }
        f_xFromTo = _pChessboard->adRemovedPiecesPositions_x[nRemPieceDestLetter][nRemPieceDestDigit];
        f_yFromTo = _pChessboard->adRemovedPiecesPositions_y[nRemPieceDestLetter][nRemPieceDestDigit];
        f_zFromTo = _pChessboard->adRemovedPiecesPositions_z[nRemPieceDestLetter][nRemPieceDestDigit];
        f_rFromTo = m_nActualPos;
    }
    else //ruchy FromTo, łapanie bierki do zbijania, puszczanie bierki przywróconej
    {
        f_xFromTo = _pChessboard->adChessboardPositions_x[nLetter][nDigit];
        f_yFromTo = _pChessboard->adChessboardPositions_y[nLetter][nDigit];
        f_zFromTo = _pChessboard->adChessboardPositions_z[nLetter][nDigit];
        f_rFromTo = m_nActualPos;
    }
    
    this->writeMoveTypeInConsole(TO_POINT, Type);
    
    QString QsMoveType;
    bIsPieceMovingFrom ? QsMoveType = ": piece from " : QsMoveType = ": piece to ";
    qDebug() << "Dobot::pieceFromTo:" << QsMoveType << "nLetter ="
             << nLetter << ", nDigit =" << nDigit << ", Type =" << Type;
    emit this->addTextToConsole(QsMoveType + _pChessboard->findPieceLetterPos(nLetter)
                                + QString::number(nDigit+1) + "\n", NOTHING);
    
    this->addCmdToList(TO_POINT, Type, f_xFromTo, f_yFromTo, f_zFromTo + m_nMaxPieceHeight, f_rFromTo);
    
    _pChessboard->PieceActualPos.Letter = nLetter;
    _pChessboard->PieceActualPos.Digit = nDigit;
}

void Dobot::gripperOpennedState(bool isGripperOpened, SEQUENCE_TYPE Type)
{
    this->addCmdToList(isGripperOpened ? OPEN_GRIP : CLOSE_GRIP, Type);
    this->writeMoveTypeInConsole(isGripperOpened ? OPEN_GRIP : CLOSE_GRIP, Type);
}

void Dobot::wait(int nMs, SEQUENCE_TYPE sequence)
{
    gripperMoveDelay.timeout = nMs; //czekaj chwilę po dojściu na pozycję by silniki...
    //...chwytaka zdążyły się ruszyć, zanim ramię zacznie wykonywać kolejny ruch
    this->addCmdToList(WAIT, sequence);
    this->writeMoveTypeInConsole(WAIT, sequence);
}

void Dobot::armUpDown(bool isArmGoingUp, bool bIsArmAboveFromSquare, SEQUENCE_TYPE Type)
{ //TODO: cała ta metoda to syf jeżeli chodzi o przejrzystość i nazewnictwo funkcji
    float f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown;
    //pozycje obszaru bierek usuniętych
    if (Type == REMOVING && !bIsArmAboveFromSquare) //jeżeli odstawiamy bierkę na obszar zbitych...
    {
        /*qDebug() << "Dobot::armUpDown: _pChessboard->nGripperPiece =" << _pChessboard->nGripperPiece;
        qDebug() << "Dobot::armUpDown: nRemovingRWPos ="
                 << _pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)
                 << _pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)
                 << ", isArmGoingUp?:" << isArmGoingUp;*/
        //TODO: ten zapis fieldNrToFieldPos działa, ale zmienić jakoś nazewnictwo funkcji, bo jest...
        //...nieadekwatna w tym przypadku.
        f_xUpDown = _pChessboard->adRemovedPiecesPositions_x
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        f_yUpDown = _pChessboard->adRemovedPiecesPositions_y
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        if (isArmGoingUp) f_zUpDown = m_nMaxRemPieceH;
        else f_zUpDown = _pChessboard->adRemovedPiecesPositions_z
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, ROW)]
                [_pChessboard->fieldNrToFieldPos(_pChessboard->nGripperPiece, COLUMN)];
        f_rUpDown = m_nActualPos;
    }
    else if (Type == RESTORE && bIsArmAboveFromSquare) //...lub pochwycamy bierkę z obszaru zbitych
    {
        qDebug() << "Dobot::armUpDown: nRemovingRWPos =" << _pChessboard->PieceActualPos.Letter
                 << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?:" << isArmGoingUp;
        f_xUpDown = _pChessboard->adRemovedPiecesPositions_x
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_yUpDown = _pChessboard->adRemovedPiecesPositions_y
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        if (isArmGoingUp) f_zUpDown = m_nMaxPieceHeight + _pChessboard->adRemovedPiecesPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        else f_zUpDown = _pChessboard->adRemovedPiecesPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_rUpDown = m_nActualPos;
    }
    else //pozycje na szachownicy
    {
        qDebug() << "Dobot::armUpDown: PieceActualPos =" << _pChessboard->PieceActualPos.Letter
                 << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?:" << isArmGoingUp;
        f_xUpDown = _pChessboard->adChessboardPositions_x
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_yUpDown = _pChessboard->adChessboardPositions_y
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        if (isArmGoingUp) f_zUpDown = m_nMaxPieceHeight + _pChessboard->adChessboardPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        else f_zUpDown = _pChessboard->adChessboardPositions_z
                [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
        f_rUpDown = m_nActualPos;
    }
    this->addCmdToList(TO_POINT, Type, f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown);
    
    this->writeMoveTypeInConsole(isArmGoingUp ? UP : DOWN, Type);
}

//TODO: chyba to się niczym nie różni od Dobot::pieceFromTo oprócz szachownicy/usuniętych
void Dobot::removePiece(int nPieceRowPos, int nPieceColumnPos)
{
    float f_xRemove = _pChessboard->adRemovedPiecesPositions_x[nPieceRowPos][nPieceColumnPos];
    float f_yRemove = _pChessboard->adRemovedPiecesPositions_y[nPieceRowPos][nPieceColumnPos];
    //float f_zRemove = _pChessboard->adRemovedPiecesPositions_z[nPieceRowPos][nPieceColumnPos];//unused
    float f_rRemove = m_nActualPos;
    qDebug() << "Dobot::removePiece values: x =" << f_xRemove << ", y =" << f_yRemove << ", z =" <<
                m_nMaxRemPieceH << ", r =" << f_rRemove;
    
    //z_max = 40 dla x = 275, a najwyższe z na styku z podłogą to z = -16.5
    this->addCmdToList(TO_POINT, REMOVING, f_xRemove, f_yRemove, m_nMaxRemPieceH, f_rRemove);
}
/// END OF: TYPY RUCHÓW PO PLANSZY

void Dobot::addCmdToList(DOBOT_MOVE move, SEQUENCE_TYPE sequence, float x, float y, float z, float r)
{
    m_ullCoreQueuedCmdIndex += 1; //aktualne id ruchu = +1 większe od ostatniego
    m_posIdx.index = m_ullCoreQueuedCmdIndex;
    m_posIdx.sequence = sequence;
    m_posIdx.move = move;
    m_posIdx.x = (x != m_nActualPos) ? x : m_fPtpCmd_xActualVal;
    m_posIdx.y = (y != m_nActualPos) ? y : m_fPtpCmd_yActualVal;
    m_posIdx.z = (z != m_nActualPos) ? z : m_fPtpCmd_zActualVal;
    m_posIdx.r = (r != m_nActualPos) ? r : m_fPtpCmd_rActualVal;
    QueuedCmdIndexList << m_posIdx; //wepchnij do kontenera strukturę
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
                                   QString::number(gripperControl.address) + "\n", DOBOT);
        }
        if (gripperControl.frequency != 50)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" << gripperControl.address;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" +
                                   QString::number(gripperControl.frequency) + "\n", DOBOT);
        }
        if (gripperControl.dutyCycle != m_fGripOpened && gripperControl.dutyCycle != m_fGripClosed)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" << gripperControl.dutyCycle;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" +
                                   QString::number(gripperControl.dutyCycle) + "\n", DOBOT);
        }
    }
}

void Dobot::writeMoveTypeInConsole(DOBOT_MOVE moveState, SEQUENCE_TYPE sequence)
{
    QString QsConsoleMsg;
    switch(sequence)
    {
    case REGULAR: QsConsoleMsg = "normal"; break;
    case CASTLING_KING: QsConsoleMsg = "castling(king)"; break;
    case CASTLING_ROOK: QsConsoleMsg = "castling(rook)"; break;
    case ENPASSANT: QsConsoleMsg = "enpassant"; break;
    case PROMOTION: QsConsoleMsg = "promotion"; break;
    case REMOVING: QsConsoleMsg = "remove"; break;
    case RESTORE: QsConsoleMsg = "restore"; break;
    case SERVICE: QsConsoleMsg = "service"; break;
    default: QsConsoleMsg = "ERROR. Wrong movement type: "
                + static_cast<QString>(sequence) + "\n"; break;
    }
    
    QString QsSecondMsg;
    switch(moveState)
    {
    case TO_POINT: QsSecondMsg = ""; break;
    case WAIT: QsSecondMsg = ": wait " + QString::number(gripperMoveDelay.timeout) + " ms\n"; break;
    case OPEN_GRIP: QsSecondMsg = ": gripper opened\n"; break;
    case CLOSE_GRIP: QsSecondMsg = ": gripper closed\n"; break;
    case UP: QsSecondMsg = ": arm up\n"; break;
    case DOWN: QsSecondMsg = ": arm down\n"; break;
    default:
        QsSecondMsg = "ERROR. Wrong movement state: " + static_cast<QString>(moveState) + "\n";
        break;
    }

    emit this->addTextToConsole(QsConsoleMsg + " piece move" + QsSecondMsg, DOBOT);
}

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
