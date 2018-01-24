#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(Websockets* pWebSockets, Chessboard* pBoardMain, Chessboard* pBoardRemoved,
                       TCPMsgs* pTCPmsg, ArduinoUsb* pArduinoUsb, Dobot* pDobot,
                       Chess* pChess, Client* pClient, QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //bodajże wskazuje że UI dziedziczy to tej (this) klasie, przez...
    //...co zostanie zniszczona razem z mainwindow
    this->setWindowTitle("Chess");

    _pDobot = pDobot;
    _pWebSockets = pWebSockets;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pTCPmsg = pTCPmsg;
    _pArduinoUsb = pArduinoUsb;
    _pClient = pClient;
    _pChess = pChess;

    //TODO: wrzucić do odpwoiednich klas (chyba się da)
    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangedMode())); //endtype change
    connect(ui->connectBtn, SIGNAL(clicked(bool)),
            _pDobot, SLOT(onConnectDobot())); //connect dobot
    connect(ui->sendBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPTPsendBtnClicked())); //send PTP data
    this->setDobotValidators(); //wartości przyjmowane z klawiatury do wysłania na dobota

    connect(_pArduinoUsb, SIGNAL(AIEnemySend(QString)),
            _pIgorBot, SLOT(checkMsgFromWebsockets(QString)));
    connect(_pArduinoUsb, SIGNAL(TcpQueueMsg(int, QString)),
            _pTCPmsg, SLOT(TcpQueueMsg(int, QString)));
    connect(_pArduinoUsb, SIGNAL(sigRestoreGameToInitialState()),
            _pIgorBot, SLOT(resetPiecePositions()));

    //ui connects
    connect(_pDobot, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pTCPmsg, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pWebSockets, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pChess, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pArduinoUsb, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pClient, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(_pChess, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pBoardMain, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pBoardRemoved, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pClient, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(_pChessboard, SIGNAL(clearFormBoard()),
            this, SLOT(clearFormBoard()));
    //todo: przekazujemy tablicę wskaźników
    connect(_pChessboard, SIGNAL(showBoardInForm(QString)),
            this, SLOT(showBoard(QString)));
    connect(_pChess, SIGNAL(showLegalMoves(QStringList)),
            this, SLOT(showLegalMoves(QStringList)));
    connect(_pChess, SIGNAL(showHistoryMoves(QStringList)),
            this, SLOT(showHistoryMoves(QStringList)));
    connect(_pWebSockets, SIGNAL(showClientsList(QList<Clients>)),
            this, SLOT(showClientsList(QList<Clients>)));
    connect(_pDobot, SIGNAL(JointLabelText(QString, short)),
            this, SLOT(setJointLabelText(QString, short)));
    connect(_pDobot, SIGNAL(AxisLabelText(QString, char)),
            this, SLOT(setAxisLabelText(QString, char)));
    connect(_pDobot, SIGNAL(deviceLabels(QString, QString, QString)),
            this, SLOT(setDeviceLabels(QString, QString, QString)));
    connect(_pDobot, SIGNAL(RefreshDobotButtonsStates(bool)),
            this, SLOT(setDobotButtonsStates(bool)));
    connect(_pDobot, SIGNAL(DobotErrorMsgBox()),
            this, SLOT(showDobotErrorMsgBox()));
    connect(_pDobot, SIGNAL(queueLabels(int, int, int, int, int)),
            this, SLOT(setQueueLabels(int, int, int, int, int)));
    connect(_pArduinoUsb, SIGNAL(updatePortsComboBox(int)),
            this, SLOT(updatePortsComboBox(int)));
    connect(_pDobot->getServoPointer(), SIGNAL(showArduinoGripperStateList(QList<ServoArduino>)),
            this, SLOT(showArduinoGripperStateList(QList<ServoArduino>)));
    connect(_pTCPmsg, SIGNAL(msgFromTcpToWeb(QString, QString)),
            _pChess, SLOT(checkMsgFromChenard(QString, QString)));
    connect(_pTCPmsg, SIGNAL(msgFromTcpToCore(QString, QString)),
            this, SLOT(checkMsgFromChenard(QString, QString)));
    connect(_pWebSockets, SIGNAL(msgFromWebsocketsToChess(QString, Client*)),
            _pChess, SLOT(checkMsgFromWebsockets(QString, Client*)));

    this->initControl(); //init dobot JOG control

    //todo: narobić tych tooltipów
    ui->directTcpMsgCheckBox->setToolTip("//todo");
    ui->serviceCheckBox->setToolTip("enable to write only chessboard destination field");
    ui->emulatePlayerMsgLineEdit->setToolTip("send msg to Websockets::receivedMsg");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDobotValidators()
{
    QRegExp regExp("0|[-+]?[1-9][0-9]{0,2}[.][0-9]{1,3}");
    QValidator *validator = new QRegExpValidator(regExp);

    ui->xPTPEdit->setValidator(validator);
    ui->yPTPEdit->setValidator(validator);
    ui->zPTPEdit->setValidator(validator);
    ui->rPTPEdit->setValidator(validator);
}

void MainWindow::setJointLabelText(QString QSJointLabelText, short sJoint)
{
    if (sJoint == 1)  ui->joint1Label->setText(QSJointLabelText);
    else if (sJoint == 2)  ui->joint2Label->setText(QSJointLabelText);
    else if (sJoint == 3)  ui->joint3Label->setText(QSJointLabelText);
    else if (sJoint == 4)  ui->joint4Label->setText(QSJointLabelText);
}

void MainWindow::setAxisLabelText(QString QSAxisLabelText, char chAxis)
{
    if (chAxis == 'x') ui->xLabel->setText(QSAxisLabelText);
    else if (chAxis == 'y') ui->yLabel->setText(QSAxisLabelText);
    else if (chAxis == 'z') ui->zLabel->setText(QSAxisLabelText);
    else if (chAxis == 'r') ui->rLabel->setText(QSAxisLabelText);
}

void MainWindow::setQueueLabels(int nSpace, int nDobotId, int nCoreMaxId,
                                int nCoreIdLeft, int CoreNextId)
{
    if (nSpace == 0) nSpace = 1; //to tylko dla zblokowania wyskakującego warninga o unused parameter
    //ui->DobotQueuedCmdLeftSpaceLabel->
    //  setText(QString::number(nSpace)); //TODO: dobot.cc nie zrobił tego jeszcze
    ui->DobotQueuedIndexLabel->setText(QString::number(nDobotId));
    ui->CoreMaxQueuedIndexLabel->setText(QString::number(nCoreMaxId));
    ui->CoreIndexAmountlabel->setText(QString::number(nCoreIdLeft));
    ui->CoreNextIdLabel->setText(QString::number(CoreNextId));
}

void MainWindow::setDobotButtonsStates(bool bDobotButtonsStates)
{
    if (bDobotButtonsStates)
    {
        ui->connectBtn->setText(tr("Connect"));
        ui->teachMode->setEnabled(false);
        ui->baseAngleAddBtn->setEnabled(false);
        ui->baseAngleSubBtn->setEnabled(false);
        ui->longArmAddBtn->setEnabled(false);
        ui->longArmSubBtn->setEnabled(false);
        ui->shortArmAddBtn->setEnabled(false);
        ui->shortArmSubBtn->setEnabled(false);
        ui->rHeadAddBtn->setEnabled(false);
        ui->rHeadSubBtn->setEnabled(false);
        ui->startGmPosBtn->setEnabled(false);
        ui->startDtPosBtn->setEnabled(false);
        ui->sendBtn->setEnabled(false);
        ui->xPTPEdit->setEnabled(false);
        ui->yPTPEdit->setEnabled(false);
        ui->zPTPEdit->setEnabled(false);
        ui->rPTPEdit->setEnabled(false);
        ui->servoGripperEdit->setEnabled(false);
        ui->calibrateBtn->setEnabled(false);
        ui->serviceCheckBox->setEnabled(false);
        ui->upBtn->setEnabled(false);
        ui->downBtn->setEnabled(false);
        ui->botOffRadioBtn->setEnabled(false);
        ui->botOnRadioBtn->setEnabled(false);
        ui->AIBtn->setEnabled(false);
        ui->AIEnemySendBtn->setEnabled(false);
        ui->sendTcpBtn->setEnabled(false);
        ui->sendTcpLineEdit->setEnabled(false);
        ui->simulateArduinoPlayer2checkBox->setEnabled(false);
        ui->emulatePlayerMsgLineEdit->setEnabled(false);
        ui->sendSimulatedMsgBtn->setEnabled(false);
        ui->openGripperBtn->setEnabled(false);
        ui->closeGripperBtn->setEnabled(false);
        ui->homeBtn->setEnabled(false);
        ui->directTcpMsgCheckBox->setEnabled(false);
        ui->moveRestrictionsCheckBox->setEnabled(false);
    }
    else
    {
        ui->connectBtn->setText(tr("Disconnect"));
        ui->teachMode->setEnabled(true);
        ui->baseAngleAddBtn->setEnabled(true);
        ui->baseAngleSubBtn->setEnabled(true);
        ui->longArmAddBtn->setEnabled(true);
        ui->longArmSubBtn->setEnabled(true);
        ui->shortArmAddBtn->setEnabled(true);
        ui->shortArmSubBtn->setEnabled(true);
        ui->rHeadAddBtn->setEnabled(true);
        ui->rHeadSubBtn->setEnabled(true);
        ui->startGmPosBtn->setEnabled(true);
        ui->startDtPosBtn->setEnabled(true);
        ui->sendBtn->setEnabled(true);
        ui->xPTPEdit->setEnabled(true);
        ui->yPTPEdit->setEnabled(true);
        ui->zPTPEdit->setEnabled(true);
        ui->rPTPEdit->setEnabled(true);
        ui->servoGripperEdit->setEnabled(true);
        ui->calibrateBtn->setEnabled(true);
        ui->serviceCheckBox->setEnabled(true);
        ui->upBtn->setEnabled(true);
        ui->downBtn->setEnabled(true);
        ui->botOffRadioBtn->setEnabled(true);
        ui->botOnRadioBtn->setEnabled(true);
        ui->AIBtn->setEnabled(true);
        ui->sendTcpLineEdit->setEnabled(true);
        ui->simulateArduinoPlayer2checkBox->setEnabled(true);
        ui->emulatePlayerMsgLineEdit->setEnabled(true);
        ui->openGripperBtn->setEnabled(true);
        ui->closeGripperBtn->setEnabled(true);
        ui->homeBtn->setEnabled(true);
        ui->directTcpMsgCheckBox->setEnabled(true);
        ui->moveRestrictionsCheckBox->setEnabled(true);
    }
}

void MainWindow::setDeviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion)
{
    ui->deviceSNLabel->setText(QSdeviceSN);
    ui->DeviceNameLabel->setText(QSdeviceName);
    ui->DeviceInfoLabel->setText(QSdeviceVersion);
}

void MainWindow::onPTPsendBtnClicked()
{
    qDebug() << "sendBtn clicked";

    //point
    bool bConversionXOk, bConversionYOk, bConversionZOk;

    Point3D point;
    float xPTE = ui->xPTPEdit->text().toFloat(&bConversionXOk);
    float yPTE = ui->yPTPEdit->text().toFloat(&bConversionYOk);
    float zPTE = ui->zPTPEdit->text().toFloat(&bConversionZOk);

    point.x = bConversionXOk ? xPTE : _pDobot->getLastGivenPoint().x;
    point.y = bConversionYOk ? yPTE : _pDobot->getLastGivenPoint().y;
    point.z = bConversionZOk ? zPTE : _pDobot->getLastGivenPoint().z;

    _pDobot->addCmdToList(DM_TO_POINT, point);

    //servo
    bool bConversionServoOk;
    float fServoDutyCycle = ui->servoGripperEdit->text().toFloat(&bConversionServoOk);
    if (bConversionServoOk)
        _pDobot->changeGripperAngle(fServoDutyCycle);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error!!!"), QMessageBox::Ok);
    return;
}

//TODO: niech tutaj qDebug wywołuje się automatycznie kopiując wpadającą wiadomość
void MainWindow::writeInConsole(QString QStrMsg, LOG msgType = LOG_NOTHING)
{
    QString QsLogType = "<" + logAsQstr(msgType) + ">: ";
    if(QStrMsg.isEmpty()) return;
    if(QStrMsg == "/clear")
    {
        ui->debug_log->clear();
        return;
    }
    QStrMsg = QTime::currentTime().toString("hh:mm:ss") + " " + QsLogType + QStrMsg;
    ui->debug_log->setPlainText(ui->debug_log->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_debug_log = ui->debug_log->verticalScrollBar();
    scroll_debug_log->setValue(scroll_debug_log->maximum());

    //todo: jeżeli zbyt wiele znaków, to usuwaj poprzednie
    /*int maximum = 500; //wiecej!
    if (ui.textEdit->toPlainText().length() > maximum)
    {
        ui.textEdit->textCursor().deletePreviousChar();
    }*/
}

void MainWindow::initControl() {
    QSignalMapper *signalMapper  = new QSignalMapper(this);
    connect(ui->baseAngleAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->baseAngleSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->longArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->longArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->shortArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->shortArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->rHeadAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->rHeadSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->baseAngleAddBtn, 0);
    signalMapper->setMapping(ui->baseAngleSubBtn, 1);
    signalMapper->setMapping(ui->longArmAddBtn, 2);
    signalMapper->setMapping(ui->longArmSubBtn, 3);
    signalMapper->setMapping(ui->shortArmAddBtn, 4);
    signalMapper->setMapping(ui->shortArmSubBtn, 5);
    signalMapper->setMapping(ui->rHeadAddBtn, 6);
    signalMapper->setMapping(ui->rHeadSubBtn, 7);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onJOGCtrlBtnPressed(int)));

    connect(ui->baseAngleAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->baseAngleSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->longArmAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->longArmSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->shortArmAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->shortArmSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->rHeadAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->rHeadSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
}

void MainWindow::onChangedMode()
{
    if (ui->teachMode->currentIndex() == 1)
    {
        ui->baseAngleAddBtn->setText(tr("X+"));
        ui->baseAngleSubBtn->setText(tr("X-"));
        ui->longArmAddBtn->setText(tr("Y+"));
        ui->longArmSubBtn->setText(tr("Y-"));
        ui->shortArmAddBtn->setText(tr("Z+"));
        ui->shortArmSubBtn->setText(tr("Z-"));
        ui->rHeadAddBtn->setText(tr("R+"));
        ui->rHeadSubBtn->setText(tr("R-"));
    }
    else
    {
        ui->baseAngleAddBtn->setText(tr("J1+"));
        ui->baseAngleSubBtn->setText(tr("J1-"));
        ui->longArmAddBtn->setText(tr("J2+"));
        ui->longArmSubBtn->setText(tr("J2-"));
        ui->shortArmAddBtn->setText(tr("J3+"));
        ui->shortArmSubBtn->setText(tr("J3-"));
        ui->rHeadAddBtn->setText(tr("J4+"));
        ui->rHeadSubBtn->setText(tr("J4-"));
    }
}

void MainWindow::onJOGCtrlBtnPressed(int index) {
    JOGCmd jogCmd;
    jogCmd.isJoint = ui->teachMode->currentIndex() == 0;
    jogCmd.cmd = index + 1;
    SetJOGCmd(&jogCmd, false, NULL);
}

void MainWindow::onJOGCtrlBtnReleased() {
    JOGCmd jogCmd;
    jogCmd.isJoint = ui->teachMode->currentIndex() == 0;
    jogCmd.cmd = JogIdle;
    SetJOGCmd(&jogCmd, false, NULL);
}

void MainWindow::on_emulatePlayerMsgLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->sendSimulatedMsgBtn->setEnabled(true);
    else ui->sendSimulatedMsgBtn->setEnabled(false);
}

//TODO: do zmiany
void MainWindow::on_sendSimulatedMsgBtn_clicked()
{
    if (!ui->emulatePlayerMsgLineEdit->text().isEmpty())
    {
        if (ui->serviceCheckBox->isChecked()) //wprowadzać tylko wartości pola np. "e2" lub ew. usuwanie: "move xxe2r"
        { //todo: przywrócić to wtedy gdy bedę chciał się zająć zastrukturyzowaniem m_adRemovedPiecesPositions
            QString QStrServiceMsg = ui->emulatePlayerMsgLineEdit->text();
            if (QStrServiceMsg.left(4) == "move") //przyjmij tylko docelową pozycję z polecenia np. "move e2e4r", tj. "e4r"
                QStrServiceMsg = QStrServiceMsg.mid(7);

            LETTER serviceLetterPos = pieceLetterPos(QStrServiceMsg.left(1));
            DIGIT serviceDigitPos = static_cast<DIGIT>(QStrServiceMsg.mid(1,1).toInt());

            if (QStrServiceMsg.right(1) != "r")
            {
                float fService_x = _pChessboard->m_adChessboardPositions_x[serviceLetterPos][serviceDigitPos];
                float fService_y = _pChessboard->m_adChessboardPositions_y[serviceLetterPos][serviceDigitPos];
                float fService_z = _pChessboard->m_adChessboardPositions_z[serviceLetterPos][serviceDigitPos];

                _pDobot->addCmdToList(DM_TO_POINT, ST_REMOVING, fService_x, fService_y,
                                         fService_z + Piece::dMaxPieceHeight, ACTUAL_POS);
                _pChessboard->PieceActualPos.Letter = serviceLetterPos;
                _pChessboard->PieceActualPos.Digit = serviceDigitPos;
            }
            if (QStrServiceMsg.right(1) == "r") _pDobot->removePiece(serviceLetterPos, serviceDigitPos);
        }
        else
        {
            _pWebSockets->receivedMsg(ui->emulatePlayerMsgLineEdit->text());
            ui->emulatePlayerMsgLineEdit->clear();
        }
    }
}

void MainWindow::on_calibrateBtn_clicked()
{
    if (ui->xLabel->text().toInt() == _pDobot->getHomePos().x &&
            ui->yLabel->text().toInt() == _pDobot->getHomePos().y &&
            ui->zLabel->text().toInt() == _pDobot->getHomePos().z)
    {
        _pDobot->addCmdToList(DM_HOME);
    }
    else
    {
        qDebug() << "ERROR: Dobot not in home positions";
        this->writeInConsole("ERROR: Arm not in home positions\n", LOG_DOBOT);
    }
}

void MainWindow::on_homeBtn_clicked()
{
    _pDobot->addCmdToList(DM_TO_POINT, ST_SERVICE,
                             _pDobot->getHomePos('x'),
                             _pDobot->getHomePos('y'),
                             _pDobot->getHomePos('z'));
}

void MainWindow::on_upBtn_clicked()
{
    _pDobot->armUpDown(DM_UP, DM_FROM, ST_SERVICE);
}

void MainWindow::on_downBtn_clicked()
{
    _pDobot->armUpDown(DM_UP, DM_FROM, ST_SERVICE);
}

void MainWindow::on_resetDobotIndexBtn_clicked()
{
    int result = SetQueuedCmdClear(); //wyczyść/wyzeruj zapytania w dobocie

    if (result == DobotCommunicate_NoError)
        this->writeInConsole("Cleared Dobot Queued Cmds.\n", LOG_DOBOT);
    else if (result == DobotCommunicate_BufferFull)
        this->writeInConsole("ERROR: Dobot buffer is full.\n", LOG_DOBOT);
    else if (result == DobotCommunicate_Timeout)
        this->writeInConsole("ERROR: Dobot communicate timeout.", LOG_DOBOT);
    else
        this->writeInConsole("ERROR: wrong result in "
                             "MainWindow::on_resetDobotIndexBtn_clicked().\n", LOG_DOBOT);
}

void MainWindow::on_executeDobotComandsBtn_clicked()
{
    int result = SetQueuedCmdStartExec(); //rozpocznij wykonywanie zakolejkowanych komend.

    if (result == DobotCommunicate_NoError)
        this->writeInConsole("Start executing dobot queued cmds.\n", LOG_DOBOT);
    else if (result == DobotCommunicate_BufferFull)
        this->writeInConsole("ERROR: Dobot buffer is full.\n", LOG_DOBOT);
    else if (result == DobotCommunicate_Timeout)
        this->writeInConsole("ERROR: Dobot communicate timeout.\n", LOG_DOBOT);
    else
        this->writeInConsole("ERROR: wrong result in "
                             "MainWindow::on_executeDobotComandsBtn_clicked().\n", LOG_DOBOT);
}

void MainWindow::on_AIBtn_clicked()
{
    if (ui->botOffRadioBtn->isChecked()) //po prostu odstawi bierki i włączy nową grę
    {
        _pIgorBot->setAI(false);
        this->writeInConsole("Turned off Igors AI\n", LOG_MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(false);
        ui->AIEnemyLineEdit->setEnabled(false);
    }
    else if (ui->botOnRadioBtn->isChecked())
    {
        _pIgorBot->setAI(true); //spowoduje, że bot wymyśli ruch, poczeka aż gracz kliknie start i...
        //...wtedy ruszy z ruchem swoim
        this->writeInConsole("Turned on Igors AI\n", LOG_MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(true);
        ui->AIEnemyLineEdit->setEnabled(true);
    }

    _pIgorBot->enemyStart();
}

void MainWindow::on_AIEnemySendBtn_clicked()
{
    _pIgorBot->setAIAsPlayer2(ui->simulateArduinoPlayer2checkBox->isChecked());

    if (PosFromTo::isMoveInProperFormat(ui->AIEnemyLineEdit->text()))
        _pChess->findAndSaveMoveAndSendItToTcp(ui->AIEnemyLineEdit->text());
}

void MainWindow::updatePortsComboBox(int nUsbPorst)
{
    QString QSUsbPorst = QString::number(nUsbPorst)
            + (nUsbPorst == 1 ? " port is ready to use\n" : " ports are ready to use\n");
    this->writeInConsole(QSUsbPorst, LOG_USB); //pokaż ile znaleziono portów

    //aktualizacja listy portów
    ui->portsComboBox->clear();
    ui->portsComboBox->addItem("NULL");
    for(int i=0; i<nUsbPorst; i++) //wypełnianie combo boxa portami
        ui->portsComboBox->addItem(_pArduinoUsb->availablePort.at(i).portName());
}

void MainWindow::on_portsComboBox_currentIndexChanged(int index)
{
    _pArduinoUsb->portIndexChanged(index);
}

void MainWindow::on_reloadPortsBtn_clicked()
{
    ui->usbCmdLine->setEnabled(true);
    ui->portsComboBox->setEnabled(true);
    ui->SimulateFromUsbLineEdit->setEnabled(true);
    _pArduinoUsb->searchDevices();
}

void MainWindow::on_sendUsbBtn_clicked()
{
    if(_pArduinoUsb->usbInfo == NULL)
        this->writeInConsole("None port selected\n", LOG_USB);
    else
    {
        _pArduinoUsb->sendDataToUsb(ui->usbCmdLine->text());
        ui->usbCmdLine->clear();
    }
}

void MainWindow::on_openGripperBtn_clicked()
{
    _pDobot->addCmdToList(DM_OPEN);
}


void MainWindow::on_closeGripperBtn_clicked()
{
    _pDobot->addCmdToList(DM_CLOSE);
}

//todo: ogarnąć punkty przejściowe
void MainWindow::on_startGmPosBtn_clicked()
{ //todo: te punkty wstawićî jako stałe z xmla
    //todo: bezsensowny zawijaniec przez klasy:
    this->writeInConsole("Placing arm above the chessboard.\n", LOG_DOBOT);
    _pDobot->addCmdToList(DM_TO_POINT, _pDobot->getHomePos());
    //todo: te liczby nazwać tym gdzie i czy są
    Point3D rightBottomLowerMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pDobot->getHomePos().z);
    _pDobot->addCmdToList(DM_TO_POINT, rightBottomLowerMainBoardSafeCorner);
    Point3D rightBottomHigherMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pBoardMain->getBoardPoint3D(BP_MIDDLE).z);
    _pDobot->addCmdToList(DM_TO_POINT, rightBottomHigherMainBoardSafeCorner);
    _pDobot->addCmdToList(DM_TO_POINT, _pBoardMain->getBoardPoint3D(BP_MIDDLE));
}

//todo: ogarnąć punkty przejściowe
void MainWindow::on_startDtPosBtn_clicked()
{
    this->writeInConsole("Returning safely to the DM_HOME positions.\n", LOG_DOBOT);

    _pDobot->addCmdToList(DM_TO_POINT, _pBoardMain->getBoardPoint3D(BP_MIDDLE));
    Point3D rightBottomHigherMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pBoardMain->getBoardPoint3D(BP_MIDDLE).z);
    _pDobot->addCmdToList(DM_TO_POINT, rightBottomHigherMainBoardSafeCorner);
    Point3D rightBottomLowerMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pDobot->getHomePos().z);
    _pDobot->addCmdToList(DM_TO_POINT, rightBottomLowerMainBoardSafeCorner);
    _pDobot->addCmdToList(DM_TO_POINT, _pDobot->getHomePos());
}

void MainWindow::on_SimulateFromUsbBtn_clicked()
{
    if (!ui->SimulateFromUsbLineEdit->text().isEmpty())
    {
        _pArduinoUsb->ManageMsgFromUsb(ui->SimulateFromUsbLineEdit->text());
        ui->SimulateFromUsbLineEdit->clear();
    }
}

void MainWindow::on_SimulateFromUsbLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->SimulateFromUsbBtn->setEnabled(true);
    else ui->SimulateFromUsbBtn->setEnabled(false);
}

void MainWindow::on_sendTcpBtn_clicked()
{
    if (!ui->sendTcpLineEdit->text().isEmpty())
    {
        if (ui->directTcpMsgCheckBox->isChecked())
            _pTCPmsg->TcpQueueMsg(TEST, ui->sendTcpLineEdit->text());
        else
            _pTCPmsg->TcpQueueMsg(_pIgorBot->getAI() ? ARDUINO : WEBSITE, ui->sendTcpLineEdit->text());
        ui->sendTcpLineEdit->clear();
    }
}

void MainWindow::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    if (tcpRespond.left(2) == "OK")
    {
        QString lastMove = tcpMsgType.right(4);
        _pChessboard->findBoardPos(lastMove);
        _pChess->continueGameplay();
    }
    else
    {
        qDebug() << "direct TCP unknown respond:" << tcpRespond
                 << ", msg for tcp:" << tcpMsgType;
    }
}

void MainWindow::on_sendTcpLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->sendTcpBtn->setEnabled(true);
    else ui->sendTcpBtn->setEnabled(false);
}

void MainWindow::showActualDobotQueuedCmdIDList(QList<DobotMove> list)
{
    QString QStrQueuedList;
    DobotMove item;

    for(int i=0; i<list.count(); ++i)
    {
       item = list.at(i);
       QStrQueuedList += QString::number(item.index) + ". " +  dobotMoveAsQstr(item.type) +
               " " + sequenceTypeAsQstr(item.sequence) + QString::number(item.x) + " " +
               QString::number(item.y) + " " + QString::number(item.z) + "\n";
    }
    ui->queuedPTE->clear();
    ui->queuedPTE->setPlainText(QStrQueuedList);
}

void MainWindow::showArduinoGripperStateList(QList<ServoArduino> list)
{
    QString QStrQueuedList;
    ServoArduino item;

    for(int i=0; i<list.count(); ++i)
    {
       item = list.at(i);
       QString QStrState = item.isGripperOpen ? "open" : "close";
       QStrQueuedList += QString::number(item.index) + ". " + QStrState + "\n";
    }
    ui->servoQueuePTE->clear();
    ui->servoQueuePTE->setPlainText(QStrQueuedList);
}

void MainWindow::showClientsList(QList<Clients> list)
{
    QString QStrClientsList;
    Clients item;

    for(int i=0; i<list.count(); ++i)
    {
       item = list.at(i);
       QStrClientsList += QString::number(i+1) + ". ";

       QString QStrName = "-";
       if (!item.name.isNull())
           QStrName = item.name;
       QStrClientsList += QStrName;

       if (item.type != PT_NONE)
       {
           QString QStrPlayerType = playerTypeAsQStr(item.type);
           QString QStrStartState = item.isStartClickedByPlayer ? "1" : "0";
           QStrClientsList += ", plr: " + QStrPlayerType + ", st:" + QStrStartState;
       }
       else if (item.queue != -1)
           QStrClientsList += ", q:" + QString::number(item.queue);

       QStrClientsList += "\n";
    }
    ui->clientsPTE->clear();
    ui->clientsPTE->setPlainText(QStrClientsList);
}

void MainWindow::setBoardDataLabel(QString QStrLabel, BOARD_DATA_LABEL labelType)
{
    switch(labelType)
    {
    case BDL_SOCKETS_ONLINE: ui->socketsLbl->setText(QStrLabel); break;
    case BDL_WHITE_NAME: ui->whiteNameLbl->setText(QStrLabel); break;
    case BDL_BLACK_NAME: ui->blackNameLbl->setText(QStrLabel); break;
    case BDL_TURN: ui->turnLbl->setText(QStrLabel); break;
    case BDL_GAME_STATUS: ui->gameStatusLbl->setText(QStrLabel); break;
    case BDL_MOVES: ui->movesLbl->setText(QStrLabel); break;
    case BDL_CASTLINGS: ui->castlingsLbl->setText(QStrLabel); break;
    case BDL_ENPASSANT: ui->enpassantLbl->setText(QStrLabel); break;
    case BDL_WHITE_TIME: ui->whiteTimeLbl->setText(QStrLabel); break;
    case BDL_BLACK_TIME: ui->blackTimeLbl->setText(QStrLabel); break;
    case BDL_QUEUE_PLAYERS: ui->queuedPlayersLbl->setText(QStrLabel); break;
    case BDL_QUEUE_TIME: ui->queueTimeLbl->setText(QStrLabel); break;
    default: qDebug() << "ERROR: MainWindow::setBoardDataLabel(): unknown labelType"
                      << labelType;
    }
}

void MainWindow::clearFormBoard()
{
    ui->boardPTE->clear();
}

void MainWindow::showBoard(QString QStrBoard)
{
    ui->boardPTE->clear();
    ui->boardPTE->setPlainText(QStrBoard);
}

void MainWindow::showLegalMoves(QStringList legalMoves)
{
    QString legal = "";
    if (!legalMoves.isEmpty()) legal = legalMoves.join(" ");
    ui->legalPTE->clear();
    ui->legalPTE->setPlainText(legal);
}

void MainWindow::showHistoryMoves(QStringList historyMoves)
{
    QString history = "";
    ui->historyPTE->clear();
    if (!historyMoves.isEmpty())
    {
        int turn = 1;
        do
        {
            if (!historyMoves.isEmpty())
            {
                history += QString::number(turn) + ". " + historyMoves.takeFirst();
                if (!historyMoves.isEmpty()) history += "  " + historyMoves.takeFirst() + "\n";
                ++turn;
            }
        } while (!historyMoves.isEmpty());
    }

    ui->historyPTE->setPlainText(history);
}
