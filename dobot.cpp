#include "dobot.h"


//TODO: nie mam wogle sprawdzane czy jakiekolwiek dane dotarły do dobota. program wypuszcza dane na...
//...usb i zakłada że ruch się wykonał bez sprawdzania tego.

//TODO: wyciagac wartosci do zewnetrznego xmla aby nie commitowac ciagle zmian tylko kalibracyjnych

Dobot::Dobot(Chessboard *pChessboard, ArduinoUsb *pArduinoUsb):
    m_fGripOpened(6.9f),
    m_fGripClosed(7.55f)
{
    _pChessboard = pChessboard;
    _pArduinoUsb = pArduinoUsb;
    
    connectStatus = false;

    m_PtpCmdActualVal.x = 200;
    m_PtpCmdActualVal.y = 0;
    m_PtpCmdActualVal.z = 25;
    m_PtpCmdActualVal.r = 0;

    HomeChess.x = 140;
    HomeChess.y = 0;
    HomeChess.z = 10; //niżej by waliło w szachownicę
    HomeChess.r = 0;

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

    middleAboveBoard.x = 260; //mniej wiecej srodek planszy w osi x
    middleAboveBoard.y = HomeChess.y;
    middleAboveBoard.z = _pChessboard->getMaxBoardZ() + _pChessboard->getMaxPieceHeight();
    middleAboveBoard.r = 0;

    retreatYPlus.x = 180;
    retreatYPlus.y = HomeChess.y + 100;
    retreatYPlus.z = _pChessboard->getMaxBoardZ() + _pChessboard->getMaxPieceHeight();;
    retreatYPlus.r = 0;

    retreatYMinus.x = 180;
    retreatYMinus.y = HomeChess.y - 100;
    retreatYMinus.z = _pChessboard->getMaxBoardZ() + _pChessboard->getMaxPieceHeight();;
    retreatYMinus.r = 0;
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

    if (!arduinoGripperStateList.isEmpty())
    {
        qDebug() << "arduinoGripperStateList size=" << arduinoGripperStateList.size();
        if (arduinoGripperStateList.first().index <= m_ullDobotQueuedCmdIndex)
        {
            QString QStrServoState = arduinoGripperStateList.first().isGripperOpen ? "Open" : "Close";
            qDebug() << "servo" << QStrServoState << ", servoListLastIndex =" << arduinoGripperStateList.first().index
                     << ", dobotActualIndex =" << m_ullDobotQueuedCmdIndex;
            _pArduinoUsb->sendDataToUsb("servo" + QStrServoState);
            arduinoGripperStateList.removeFirst();
        }
        emit showArduinoGripperStateList(arduinoGripperStateList);
    }

    if (!QueuedCmdIndexList.isEmpty())
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
                case DM_TO_POINT:
                    ptpCmd.x = takenPosId.x;
                    ptpCmd.y = takenPosId.y;
                    ptpCmd.z = takenPosId.z;
                    ptpCmd.r = takenPosId.r;
                    SetPTPCmd(&ptpCmd, true, &takenPosId.index);
                    break;
                case DM_WAIT:
                    SetWAITCmd(&gripperMoveDelay, true, &takenPosId.index);
                    break;
                case DM_OPEN:
                {
                    m_gripperServo.dutyCycle = m_fGripOpened;
                    int openGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (openGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", LOG_DOBOT);
                    }
                }
                    break;
                case DM_CLOSE:
                {
                    m_gripperServo.dutyCycle = m_fGripClosed;
                    int closeGripResult = SetIOPWM(&m_gripperServo, true, &takenPosId.index);
                    if (closeGripResult != DobotCommunicate_NoError)
                    {
                        qDebug() << "ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong";
                        this->addTextToConsole("ERROR: Dobot::QueuedIdList(): SetIOPWM gone wrong\n", LOG_DOBOT);
                    }
                }
                    break;
                case DM_HOME:
                {
                    this->addTextToConsole("HomeCmd: recalibrating arm...\n", LOG_DOBOT);
                    HOMECmd HOMEChess;
                    HOMEChess.reserved = 1; //todo: jak się to ma do innych indexów?
                    int result = SetHOMECmd(&HOMEChess, true, &takenPosId.index);
                    if (result != DobotCommunicate_NoError)
                        qDebug() << "ERROR: SetHOMECmd result != DobotCommunicate_NoError";
                }
                    break;
                case DM_UP:
                case DM_DOWN:
                default:
                    qDebug() << "ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:" << takenPosId.sequence;
                    this->addTextToConsole("ERROR: Dobot::QueuedIdList(): wrong takenPosId.type:"
                                           + takenPosId.sequence, LOG_DOBOT);
                    break;
                }
            } 
        emit showActualDobotQueuedCmdIndexList(QueuedCmdIndexList);
        }
    }

    if (m_ullRetreatIndex <= m_ullDobotQueuedCmdIndex)
    {
        qDebug() << "Dobot::QueuedIdList(): retreat";
        PtpCmdActualVal retreatId;
        retreatId = (pose.y >= middleAboveBoard.y) ?  retreatYPlus : retreatYMinus;
        addCmdToList(DM_TO_POINT, ST_NONE, retreatId.x, retreatId.y, retreatId.z, retreatId.r);
        m_ullRetreatIndex = 10000000; //jakas duza liczba
    }
}

void Dobot::onConnectDobot()
{
    if (!connectStatus)
    {
        if (ConnectDobot(0, 115200) != DobotConnect_NoError) emit DobotErrorMsgBox();
        connectStatus = true;
        qDebug() << "Dobot connection success";
        this->addTextToConsole("Dobot connected \n", LOG_DOBOT);

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
                                   "GetQueuedCmdCurrentIndex gone wrong \n", LOG_DOBOT);
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
void Dobot::pieceFromTo(DOBOT_MOVE partOfSequence, LETTER letter, DIGIT digit, SEQUENCE_TYPE Type)
{
    float f_xFromTo, f_yFromTo, f_zFromTo, f_rFromTo;

    if (letter >=0 && letter <=7 && digit >=0 && digit <=7)
    { //todo: powtórzony kod zabezpieczeń z Dobot::armUpDown
        if ((Type == ST_REMOVING && partOfSequence == DM_TO) || (Type == ST_RESTORE && partOfSequence == DM_FROM))
        {
            int nRemPieceNr;
            FieldLinesPos remPieceDest;

            if (Type == ST_REMOVING)
            {
                if (_pChessboard->nGripperPiece >= 1 && _pChessboard->nGripperPiece <= 32)
                {
                    nRemPieceNr = _pChessboard->nGripperPiece; //bierka z planszy w chwytaku do usunięcia
                    qDebug() << "fieldNrToFieldLinesPos16";
                    remPieceDest.Letter = _pChessboard->fieldNrToFieldLinesPos(nRemPieceNr).Letter;
                    qDebug() << "fieldNrToFieldLinesPos17";
                    remPieceDest.Digit = _pChessboard->fieldNrToFieldLinesPos(nRemPieceNr).Digit;
                }
                else
                {
                    qDebug() << "ERROR: Dobot::pieceFromTo: ST_REMOVING: wrong _pChessboard->nGripperPiece value:" <<
                                _pChessboard->nGripperPiece;
                    return;
                }
            }
            else if (Type == ST_RESTORE) //dla restore() pozycja pola na obszarze usuniętych musi...
                //...być podana wprost do pieceFromTo()
            {
                //dla ST_RESTORE wartość nGripperPiece nie zadziała, bo nie ma jeszcze nic w chwytaku
                remPieceDest.Letter = letter;
                remPieceDest.Digit = digit;
            }

            f_xFromTo = _pChessboard->m_adRemovedPiecesPositions_x[remPieceDest.Letter][remPieceDest.Digit];
            f_yFromTo = _pChessboard->m_adRemovedPiecesPositions_y[remPieceDest.Letter][remPieceDest.Digit];
            f_zFromTo = _pChessboard->m_adRemovedPiecesPositions_z[remPieceDest.Letter][remPieceDest.Digit];
            f_rFromTo = ACTUAL_POS;
        }
        else //ruchy FromTo, łapanie bierki do zbijania, puszczanie bierki przywróconej
        {
            f_xFromTo = _pChessboard->m_adChessboardPositions_x[letter][digit];
            f_yFromTo = _pChessboard->m_adChessboardPositions_y[letter][digit];
            f_zFromTo = _pChessboard->m_adChessboardPositions_z[letter][digit];
            f_rFromTo = ACTUAL_POS;
        }
    }
    else qDebug() << "ERROR: Dobot::pieceFromTo: wrong letter/digit value=" <<
                     letter << "," << digit;
    
    if (f_xFromTo > 130.f && f_xFromTo < 350.f && f_yFromTo > -200 && f_yFromTo < 200 && f_zFromTo > -22.f &&
            f_zFromTo < 100.f) //zabezpieczenia odległościowe. todo: da się je ustawić jako alarmy w odobcie?
    { //todo: warunki wpisać tak by się dostosowywały wartosci do zmian. można w funckję zamknąć
        ArmPosForCurrentCmdQueuedIndex actualPosIdx;
        actualPosIdx = QueuedCmdIndexList.first();
        qDebug() << "Previous move parameteres: x y z=" << actualPosIdx.x << actualPosIdx.y << actualPosIdx.z;
        if (f_zFromTo != actualPosIdx.z && (f_xFromTo != actualPosIdx.x || f_yFromTo != actualPosIdx.y))
        {
            this->addCmdToList(DM_TO_POINT, Type, f_xFromTo, f_yFromTo, f_zFromTo +
                               _pChessboard->getMaxPieceHeight(), f_rFromTo);

            this->writeMoveTypeInConsole(DM_TO_POINT, Type);
            QString QstrMoveType;
            partOfSequence == DM_FROM ? QstrMoveType = ": piece from " : QstrMoveType = ": piece to ";
            qDebug() << "Dobot::pieceFromTo:" << QstrMoveType << "nLetter ="
                     << letter << ", nDigit =" << digit << ", Type =" << Type;
            emit this->addTextToConsole(QstrMoveType + pieceLetterPos(letter)
                                        + QString::number(digit+1) + "\n", LOG_NOTHING);
            //todo: naprzemiennie używam writeMoveTypeInConsole() i addTextToConsole() - ?
        }
        else qDebug() << "ERROR: Dobot::pieceFromTo: trying to move Z axis with X or Y axis";
    }
    else qDebug() << "ERROR: Dobot::pieceFromTo: out of XYZ wornking range: x=" << f_xFromTo <<
                     ", y=" << f_yFromTo << ", z=" << f_zFromTo;
    
    _pChessboard->PieceActualPos.Letter = letter; //todo: wrzucić to do warunków na letter/digit
    _pChessboard->PieceActualPos.Digit = digit;
}

void Dobot::gripperState(DOBOT_MOVE state, SEQUENCE_TYPE Type)
{
    qDebug() << "Dobot::gripperState:" << (state == DM_OPEN ? "open" : "close");
    this->addCmdToList(state, Type);
    this->writeMoveTypeInConsole(state, Type);
}

void Dobot::wait(int nMs, SEQUENCE_TYPE sequence)
{
    gripperMoveDelay.timeout = nMs; //czekaj chwilę po dojściu na pozycję by silniki...
    //...chwytaka zdążyły się ruszyć, zanim ramię zacznie wykonywać kolejny ruch
    this->addCmdToList(DM_WAIT, sequence);
    this->writeMoveTypeInConsole(DM_WAIT, sequence);
}

void Dobot::armUpDown(DOBOT_MOVE armDestination, DOBOT_MOVE partOfSequence, SEQUENCE_TYPE Type)
{ //TODO: cała ta metoda to syf jeżeli chodzi o przejrzystość i nazewnictwo funkcji
    float f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown;
    //pozycje obszaru bierek usuniętych
    if (Type == ST_REMOVING && partOfSequence == DM_TO) //jeżeli odstawiamy bierkę na obszar zbitych...
    {
        //TODO: ten zapis fieldNrToFieldPos działa, ale zmienić jakoś nazewnictwo funkcji, bo jest...
        //...nieadekwatna w tym przypadku.
        //TODO2: zastrukturyzować, spakować w funkcję
        //todo3: uporządkować kod zabezpieczeń
        //todo4: w całej klasie dobot ie powinna się pojawić ani 1 linia kodu z której by wynikało że...
        //...ma onacoś wspólnego z szachami

        if (_pChessboard->nGripperPiece >= 1 && _pChessboard->nGripperPiece <= 32)
        {
            qDebug() << "Dobot::armUpDown: nGripperPiece =" << _pChessboard->nGripperPiece
                     << ", isArmGoingUp?: " << (armDestination == DM_UP ? "up" : "down");

            qDebug() << "fieldNrToFieldLinesPos18";
            f_xUpDown = _pChessboard->m_adRemovedPiecesPositions_x
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Letter]
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Digit];
            f_yUpDown = _pChessboard->m_adRemovedPiecesPositions_y
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Letter]
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Digit];
            if (armDestination == DM_UP) f_zUpDown = _pChessboard->getMaxRemovedPieceHeight();
            else if (armDestination == DM_DOWN) f_zUpDown = _pChessboard->m_adRemovedPiecesPositions_z
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Letter]
                    [_pChessboard->fieldNrToFieldLinesPos(_pChessboard->nGripperPiece).Digit];
            f_rUpDown = ACTUAL_POS;
        }
        else
        {
            qDebug() << "ERROR: Dobot::armUpDown: ST_REMOVING: wrong _pChessboard->nGripperPiece value:" <<
                         _pChessboard->nGripperPiece;
            return;
        }
    }
    else if (Type == ST_RESTORE && partOfSequence == DM_FROM) //...lub pochwycamy bierkę z obszaru zbitych
    {
        if (_pChessboard->PieceActualPos.Letter >=0 && _pChessboard->PieceActualPos.Letter <=7 &&
                _pChessboard->PieceActualPos.Digit >=0 && _pChessboard->PieceActualPos.Digit <=7)
        {
            qDebug() << "Dobot::armUpDown: nRemovingRWPos =" << _pChessboard->PieceActualPos.Letter
                     << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?: " << (armDestination == DM_UP ? "up" : "down");

            f_xUpDown = _pChessboard->m_adRemovedPiecesPositions_x
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            f_yUpDown = _pChessboard->m_adRemovedPiecesPositions_y
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            if (armDestination == DM_UP) f_zUpDown = _pChessboard->getMaxPieceHeight() + _pChessboard->m_adRemovedPiecesPositions_z
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            else if (armDestination == DM_DOWN) f_zUpDown = _pChessboard->m_adRemovedPiecesPositions_z
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            f_rUpDown = ACTUAL_POS;
        }
        else qDebug() << "ERROR: Dobot::armUpDown: ST_RESTORE: wrong _pChessboard->PieceActualPos value:" <<
                         _pChessboard->PieceActualPos.Letter << "," << _pChessboard->PieceActualPos.Digit;
    }
    else //pozycje na szachownicy
    {
        if (_pChessboard->PieceActualPos.Letter >=0 && _pChessboard->PieceActualPos.Letter <=7 &&
                _pChessboard->PieceActualPos.Digit >=0 && _pChessboard->PieceActualPos.Digit <=7)
        {
            qDebug() << "Dobot::armUpDown: PieceActualPos =" << _pChessboard->PieceActualPos.Letter
                     << _pChessboard->PieceActualPos.Digit << ", isArmGoingUp?:" << (armDestination == DM_UP ? "up" : "down");

            f_xUpDown = _pChessboard->m_adChessboardPositions_x
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            f_yUpDown = _pChessboard->m_adChessboardPositions_y
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            if (armDestination == DM_UP) f_zUpDown = _pChessboard->getMaxPieceHeight() + _pChessboard->m_adChessboardPositions_z
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            else if (armDestination == DM_DOWN) f_zUpDown = _pChessboard->m_adChessboardPositions_z
                    [_pChessboard->PieceActualPos.Letter][_pChessboard->PieceActualPos.Digit];
            f_rUpDown = ACTUAL_POS;
        }
        else qDebug() << "ERROR: Dobot::armUpDown: NORMAL: wrong _pChessboard->PieceActualPos value:" <<
                         _pChessboard->PieceActualPos.Letter << "," << _pChessboard->PieceActualPos.Digit;
    }

    if (f_xUpDown > 130.f && f_xUpDown < 350.f && f_yUpDown > -200 && f_yUpDown < 200 && f_zUpDown > -22.f &&
            f_zUpDown < 100.f)
    { //todo: warunki wpisać tak by się dostosowywały wartosci do zmian. można w funckję zamknąć
        ArmPosForCurrentCmdQueuedIndex actualPosIdx;
        actualPosIdx = QueuedCmdIndexList.first();
        qDebug() << "Previous move parameteres: x y z=" << actualPosIdx.x << actualPosIdx.y << actualPosIdx.z;
        if (f_zUpDown != actualPosIdx.z && (f_xUpDown != actualPosIdx.x || f_yUpDown != actualPosIdx.y))
        {
            this->addCmdToList(DM_TO_POINT, Type, f_xUpDown, f_yUpDown, f_zUpDown, f_rUpDown);
            this->writeMoveTypeInConsole(armDestination, Type);
        }
        else qDebug() << "ERROR: Dobot::armUpDown: trying to move Z axis with X or Y axis";
    }
    else qDebug() << "ERROR: Dobot::armUpDown: out of XYZ wornking range: x=" << f_xUpDown <<
                     ", y=" << f_yUpDown << ", z=" << f_zUpDown;
}

//TODO: chyba to się niczym nie różni od Dobot::pieceFromTo oprócz szachownicy/usuniętych...
//... - używam tego tylko w funkcji serwisowej- pozbyć się tego
void Dobot::removePiece(int nPieceRowPos, int nPieceColumnPos)
{
    float f_xRemove = _pChessboard->m_adRemovedPiecesPositions_x[nPieceRowPos][nPieceColumnPos];
    float f_yRemove = _pChessboard->m_adRemovedPiecesPositions_y[nPieceRowPos][nPieceColumnPos];
    //float f_zRemove = _pChessboard->m_adRemovedPiecesPositions_z[nPieceRowPos][nPieceColumnPos]; //unused
    float f_rRemove = ACTUAL_POS;
    qDebug() << "Dobot::removePiece values: x =" << f_xRemove << ", y =" << f_yRemove << ", z =" <<
                _pChessboard->getMaxRemovedPieceHeight() << ", r =" << f_rRemove;
    
    //z_max = 40 dla x = 275, a najwyższe z na styku z podłogą to z = -16.5 - stare info?

    this->addCmdToList(DM_TO_POINT, ST_REMOVING, f_xRemove, f_yRemove, _pChessboard->getMaxRemovedPieceHeight(), f_rRemove);
}
/// END OF: TYPY RUCHÓW PO PLANSZY

void Dobot::addCmdToList(DOBOT_MOVE move, SEQUENCE_TYPE sequence, float x, float y, float z, float r)
{
    //todo: usuwać z tablicy wiadomości na dobota wiadomości te które już poszły (może minus 1)
    m_ullCoreQueuedCmdIndex += 1; //aktualne id ruchu = +1 większe od ostatniego
    m_posIdx.index = m_ullCoreQueuedCmdIndex;
    m_posIdx.sequence = sequence;
    m_posIdx.move = move;
    m_posIdx.x = (x != ACTUAL_POS) ? x : m_PtpCmdActualVal.x;
    m_posIdx.y = (y != ACTUAL_POS) ? y : m_PtpCmdActualVal.y;
    m_posIdx.z = (z != ACTUAL_POS) ? z : m_PtpCmdActualVal.z;
    m_posIdx.r = (r != ACTUAL_POS) ? r : m_PtpCmdActualVal.r;
    QueuedCmdIndexList << m_posIdx;

    if (move == DM_OPEN || move == DM_CLOSE)
    {
        ServoArduino servoState;
        servoState.index = m_ullCoreQueuedCmdIndex;
        servoState.isGripperOpen = (move == DM_OPEN) ? true : false;
        qDebug() << "add command to arduinoGripperStateList:" << move;
        arduinoGripperStateList << servoState;
    }
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
                                   QString::number(gripperControl.address) + "\n", LOG_DOBOT);
        }
        if (gripperControl.frequency != 50)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" << gripperControl.address;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.frequency != 50. val =" +
                                   QString::number(gripperControl.frequency) + "\n", LOG_DOBOT);
        }
        if (gripperControl.dutyCycle != m_fGripOpened && gripperControl.dutyCycle != m_fGripClosed)
        {
            qDebug() << "ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" << gripperControl.dutyCycle;
            this->addTextToConsole("ERROR: Dobot::checkPWM(): gripperControl.dutyCycle!= 4 && !=7.40f. val =" +
                                   QString::number(gripperControl.dutyCycle) + "\n", LOG_DOBOT);
        }
    }
}

void Dobot::writeMoveTypeInConsole(DOBOT_MOVE moveState, SEQUENCE_TYPE sequence)
{
    QString QsConsoleMsg;
    switch(sequence)
    {
    case ST_REGULAR: QsConsoleMsg = "normal"; break;
    case ST_CASTLING_KING: QsConsoleMsg = "castling(king)"; break;
    case ST_CASTLING_ROOK: QsConsoleMsg = "castling(rook)"; break;
    case ST_ENPASSANT: QsConsoleMsg = "enpassant"; break;
    case ST_PROMOTION: QsConsoleMsg = "promotion"; break;
    case ST_REMOVING: QsConsoleMsg = "remove"; break;
    case ST_RESTORE: QsConsoleMsg = "restore"; break;
    case ST_SERVICE: QsConsoleMsg = "service"; break;
    default: QsConsoleMsg = "ERROR. Dobot::writeMoveTypeInConsole: Wrong movement type: " +
                static_cast<QString>(sequence) + "\n"; break;
    }
    
    QString QsSecondMsg;
    switch(moveState)
    {
    case DM_TO_POINT: QsSecondMsg = ""; break;
    case DM_WAIT: QsSecondMsg = ": wait " + QString::number(gripperMoveDelay.timeout) + " ms\n"; break;
    case DM_OPEN: QsSecondMsg = ": gripper opened\n"; break;
    case DM_CLOSE: QsSecondMsg = ": gripper closed\n"; break;
    case DM_UP: QsSecondMsg = ": arm up\n"; break;
    case DM_DOWN: QsSecondMsg = ": arm down\n"; break;
    case DM_INDIRECT: QsSecondMsg = ": indirect\n"; break;
    default:
        QsSecondMsg = "ERROR. Dobot::writeMoveTypeInConsole: Wrong movement state: " +
                static_cast<QString>(moveState) + "\n"; break;
    }

    emit this->addTextToConsole(QsConsoleMsg + " piece move" + QsSecondMsg, LOG_DOBOT);
}

void Dobot::closeEvent(QCloseEvent *)
{
    //TODO: w oryginale też jest puste od nowszej wersji. usunąć?
}
