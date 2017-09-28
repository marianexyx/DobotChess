#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(Websockets *pWebSockets, Chessboard *pChessboard,
                       TCPMsgs *pTCPmsg, ArduinoUsb *pArduinoUsb, Dobot *pDobotArm,
                       IgorBot *pIgorBot, WebChess *pWebChess, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //bodajże wskazuje że UI dziedziczy to tej (this) klasie, przez co zostanie...
    //... zniszczona razem z mainwindow.
    this->setWindowTitle("Awaiting for new game");

    _pDobotArm = pDobotArm;
    _pWebSockets = pWebSockets;
    _pChessboard = pChessboard;
    _pTCPmsg = pTCPmsg;
    _pArduinoUsb = pArduinoUsb;
    _pIgorBot = pIgorBot;
    _pWebChess = pWebChess;

    //TODO: to pewnie dałoby się wrzucić do odpwoiednich klas
    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangedMode())); //endtype change
    connect(ui->connectBtn, SIGNAL(clicked(bool)),
            _pDobotArm, SLOT(onConnectDobot())); //connect dobot
    connect(ui->sendBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPTPsendBtnClicked())); //send PTP data
    this->setDobotValidators(); //wartości przymowane z klawiatury do wysłania na dobota

    connect(_pArduinoUsb, SIGNAL(AIEnemySend(QString)), _pIgorBot, SLOT(checkMsgForChenard(QString)));
    connect(_pArduinoUsb, SIGNAL(TcpQueueMsg(int, QString)), _pTCPmsg, SLOT(TcpQueueMsg(int, QString)));
    connect(_pArduinoUsb, SIGNAL(reset()), _pIgorBot, SLOT(resetPiecePositions()));

    //dzięki tym connectom można wywołać funkcję typu "ui" z innej klasy
    connect(_pDobotArm, SIGNAL(addTextToConsole(QString,LOG)),
            this, SLOT(writeInConsole(QString,LOG)));
    connect(_pTCPmsg, SIGNAL(addTextToConsole(QString,LOG)),
            this, SLOT(writeInConsole(QString,LOG)));
    connect(_pWebSockets, SIGNAL(addTextToConsole(QString,LOG)),
            this, SLOT(writeInConsole(QString,LOG)));
    connect(_pWebChess, SIGNAL(addTextToConsole(QString,LOG)),
            this, SLOT(writeInConsole(QString,LOG)));
    connect(_pArduinoUsb, SIGNAL(addTextToConsole(QString,LOG)),
            this, SLOT(writeInConsole(QString,LOG)));

    connect(_pWebSockets, SIGNAL(setBoardDataLabels(QString,BOARD_DATA_LABELS)),
             this, SLOT(setBoardDataLabels(QString,BOARD_DATA_LABELS)));
    connect(_pDobotArm, SIGNAL(setBoardDataLabels(QString,BOARD_DATA_LABELS)),
             this, SLOT(setBoardDataLabels(QString,BOARD_DATA_LABELS)));
    connect(_pChessboard, SIGNAL(setBoardDataLabels(QString,BOARD_DATA_LABELS)),
             this, SLOT(setBoardDataLabels(QString,BOARD_DATA_LABELS)));
    connect(_pChessboard, SIGNAL(showBoard(QString)),
            this, SLOT(showBoard(QString)));
    connect(_pChessboard, SIGNAL(showLegalMoves(QStringList)),
            this, SLOT(showLegalMoves(QStringList)));
    connect(_pChessboard, SIGNAL(showHistoryMoves(QStringList)),
            this, SLOT(showHistoryMoves(QStringList)));

    connect(_pDobotArm, SIGNAL(JointLabelText(QString, short)),
            this, SLOT(setJointLabelText(QString, short)));
    connect(_pDobotArm, SIGNAL(AxisLabelText(QString, char)),
            this, SLOT(setAxisLabelText(QString, char)));
    connect(_pDobotArm, SIGNAL(deviceLabels(QString,QString,QString)),
            this, SLOT(setDeviceLabels(QString,QString,QString)));
    connect(_pDobotArm, SIGNAL(RefreshDobotButtonsStates(bool)),
            this, SLOT(setDobotButtonsStates(bool)));
    connect(_pDobotArm, SIGNAL(DobotErrorMsgBox()),
            this, SLOT(showDobotErrorMsgBox()));
    connect(_pDobotArm, SIGNAL(QueueLabels(int,int,int,int,int)),
            this, SLOT(setQueueLabels(int,int,int,int,int)));
    connect(_pArduinoUsb, SIGNAL(updatePortsComboBox(int)),
            this, SLOT(updatePortsComboBox(int)));
    connect(_pChessboard, SIGNAL(changeWindowTitle(QString)),
            this, SLOT(changeWindowtitle(QString)));

    //connecty komunikujące klasy z sobą
    connect(_pTCPmsg, SIGNAL(msgFromTcpToWeb(QString, QString)), //przesyłanie odpowiedzi z chenard...
            _pWebChess, SLOT(checkMsgFromChenard(QString, QString)));  //...na WWW przez silnk gry.
    connect(_pTCPmsg, SIGNAL(msgFromTcpToArd(QString, QString)), //przesyłanie odpowiedzi z chenard...
            _pIgorBot, SLOT(checkMsgFromChenard(QString, QString)));  //...na Arduino przez klasę SI.
    connect(_pTCPmsg, SIGNAL(msgFromTcpToCore(QString, QString)), //przesyłanie odpowiedzi z chenard...
            this, SLOT(checkMsgFromChenard(QString, QString)));  //...na Arduino przez klasę SI.
    connect(_pWebSockets, SIGNAL(MsgFromWebsocketsToChess(QString)), //przesyłanie wiadomości z ...
            _pWebChess, SLOT(checkMsgForChenard(QString))); //...websocketów na silnk gry.
    connect(_pChessboard, SIGNAL(msgFromChessboardToWebsockets(QString)),
            _pWebSockets, SLOT(msgFromChessboardToWebsockets(QString)));

    //init JOG control
    this->initControl(); //dobot

    //todo: narobić tych tooltipów
    ui->directTcpMsgCheckBox->setToolTip("enable to send multiple moves");
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

//TODO: dodać tu kod actual_state
void MainWindow::onPTPsendBtnClicked()
{
    qDebug() << "sendBtn clicked";

    int nPtpCmd_x = ui->xPTPEdit->text().toFloat();
    int nPtpCmd_y = ui->yPTPEdit->text().toFloat();
    int nPtpCmd_z = ui->zPTPEdit->text().toFloat();
    int nPtpCmd_r = ui->rPTPEdit->text().toFloat();
    if (nPtpCmd_x != 0 && nPtpCmd_y != 0 && nPtpCmd_z != 0) // dla wygody zera są zabronione
        _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, nPtpCmd_x, nPtpCmd_y, nPtpCmd_z, nPtpCmd_r);

    float fServoDutyCycle = ui->servoGripperEdit->text().toFloat();
    if (fServoDutyCycle !=0)
        _pDobotArm->gripperAngle(fServoDutyCycle);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error!!!"), QMessageBox::Ok);
    return;
}

//TODO2: ogarnąć aby w tej metodzie znalazło się wywoływanie wszystkich...
//...qDebug, by nie robić tego wszędzie.
//TODO3: dodać przed każdą komendą czas jej wywołania
void MainWindow::writeInConsole(QString QStrMsg, LOG msgType = NOTHING)
{
    QString QsLogType;
    switch(msgType)
    {
    case NOTHING: QsLogType.clear(); break;
    case CORE: QsLogType = "<Core>: "; break;
    case DOBOT: QsLogType = "<Dobot>: "; break;
    case TCP: QsLogType = "<TCP>: "; break;
    case WEBSOCKET: QsLogType = "<Websockets>: "; break;
    case MAINWINDOW: QsLogType = "<Mainwindow>: "; break;
    case USB: QsLogType = "<USB>: "; break;
    case USB_SENT: QsLogType = "<USB> Sent: "; break;
    case USB_RECEIVED: QsLogType = "<USB> Received: "; break;
    default:
        QString QsChar = static_cast<QString>(msgType);
        QsLogType = "<ERROR: Wrong log type>: " + QsChar + ": ";
        break;
    }

    if(QStrMsg.isEmpty()) return;

    if(QStrMsg == "/clear")
    {
        ui->debug_log->clear();
        return;
    }
    QStrMsg = QsLogType + QStrMsg;
    ui->debug_log->setPlainText(ui->debug_log->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_debug_log = ui->debug_log->verticalScrollBar();
    scroll_debug_log->setValue(scroll_debug_log->maximum());

    //todo: jeżeli zbyt wiele znaków, to usuwaj poprzednie
    /*int maximum = 500;
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

void MainWindow::on_sendSimulatedMsgBtn_clicked()
{
    if (!ui->emulatePlayerMsgLineEdit->text().isEmpty())
    {
        if (ui->serviceCheckBox->isChecked()) //wprowadzać tylko wartości pola np. "e2" lub ew. usuwanie: "move xxe2r"
        {
            QString QStrServiceMsg = ui->emulatePlayerMsgLineEdit->text();
            if (QStrServiceMsg.left(4) == "move") //przyjmij tylko docelową pozycję z polecenia np. "move e2e4r", tj. "e4r"
                QStrServiceMsg = QStrServiceMsg.mid(7);

            LETTER serviceLetterPos = _pChessboard->findPieceLetterPos(QStrServiceMsg.left(1));
            DIGIT serviceDigitPos = static_cast<DIGIT>(QStrServiceMsg.mid(1,1).toInt() - 1);

            if (QStrServiceMsg.right(1) != "r")
            {
                float fService_x = _pChessboard->adChessboardPositions_x[serviceLetterPos][serviceDigitPos];
                float fService_y = _pChessboard->adChessboardPositions_y[serviceLetterPos][serviceDigitPos];
                float fService_z = _pChessboard->adChessboardPositions_z[serviceLetterPos][serviceDigitPos];

                _pDobotArm->addCmdToList(TO_POINT, ST_REMOVING, fService_x, fService_y, fService_z +
                                         _pChessboard->getMaxPieceHeight(), ACTUAL_POS);
                _pChessboard->PieceActualPos.Letter = serviceLetterPos;
                _pChessboard->PieceActualPos.Digit = serviceDigitPos;
            }
            if (QStrServiceMsg.right(1) == "r") _pDobotArm->removePiece(serviceLetterPos, serviceDigitPos);
        }
        else
        {
            _pWebChess->setServiceTests(true);
            _pWebSockets->receivedMsg(ui->emulatePlayerMsgLineEdit->text());
            ui->emulatePlayerMsgLineEdit->clear();
            _pWebChess->setServiceTests(false);
        }
    }
}

void MainWindow::on_calibrateBtn_clicked()
{
    if (ui->xLabel->text().toInt() == _pDobotArm->getHomePos('x') &&
            ui->yLabel->text().toInt() == _pDobotArm->getHomePos('y') &&
            ui->zLabel->text().toInt() == _pDobotArm->getHomePos('z'))
    {
        _pDobotArm->addCmdToList(HOME);
    }
    else
    {
        qDebug() << "ERROR: Dobot not in home positions";
        this->writeInConsole("ERROR: Arm not in home positions", DOBOT);
    }
}

void MainWindow::on_homeBtn_clicked()
{
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE,
                             _pDobotArm->getHomePos('x'),
                             _pDobotArm->getHomePos('y'),
                             _pDobotArm->getHomePos('z'));
}

void MainWindow::on_upBtn_clicked()
{
    _pDobotArm->armUpDown(UP, FROM, ST_SERVICE);
}

void MainWindow::on_downBtn_clicked()
{
    _pDobotArm->armUpDown(UP, FROM, ST_SERVICE);
}

void MainWindow::on_resetDobotIndexBtn_clicked()
{
    int result = SetQueuedCmdClear(); //wyczyść/wyzeruj zapytania w dobocie

    if (result == DobotCommunicate_NoError)
        _pDobotArm->addTextToConsole("Cleared Dobot Queued Cmds.\n", DOBOT);
    else if (result == DobotCommunicate_BufferFull)
        _pDobotArm->addTextToConsole("ERROR: Dobot buffer is full.\n", DOBOT);
    else if (result == DobotCommunicate_Timeout)
        _pDobotArm->addTextToConsole("ERROR: Dobot communicate timeout.", DOBOT);
    else _pDobotArm->addTextToConsole("ERROR: wrong result in "
                                           "MainWindow::on_resetDobotIndexBtn_clicked().\n", DOBOT);
}

void MainWindow::on_executeDobotComandsBtn_clicked()
{
    int result = SetQueuedCmdStartExec(); //rozpocznij wykonywanie zakolejkowanych komend.

    if (result == DobotCommunicate_NoError)
        _pDobotArm->addTextToConsole("Start executing dobot queued cmds.\n", DOBOT);
    else if (result == DobotCommunicate_BufferFull)
        _pDobotArm->addTextToConsole("ERROR: Dobot buffer is full.\n", DOBOT);
    else if (result == DobotCommunicate_Timeout)
        _pDobotArm->addTextToConsole("ERROR: Dobot communicate timeout.\n", DOBOT);
    else _pDobotArm->addTextToConsole("ERROR: wrong result in "
                                           "MainWindow::on_executeDobotComandsBtn_clicked().\n", DOBOT);
}

void MainWindow::on_AIBtn_clicked()
{
    if (ui->botOffRadioBtn->isChecked()) //po prostu odstawi bierki i włączy nową grę
    {
        _pIgorBot->setAI(false);
        this->writeInConsole("Turned off Igors AI\n", MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(false);
        ui->AIEnemyLineEdit->setEnabled(false);
    }
    else if (ui->botOnRadioBtn->isChecked())
    {
        _pIgorBot->setAI(true); //spowoduje, że bot wymyśli ruch, poczeka aż gracz kliknie start i...
        //...wtedy ruszy z ruchem swoim
        this->writeInConsole("Turned on Igors AI\n", MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(true);
        ui->AIEnemyLineEdit->setEnabled(true);
    }

    _pIgorBot->EnemyStart();
}

void MainWindow::on_AIEnemySendBtn_clicked()
{
    _pIgorBot->setAIAsPlayer2(ui->simulateArduinoPlayer2checkBox->isChecked());
    QString QsAIEnemySend = this->checkMoveForTcp(ui->AIEnemyLineEdit->text());
    this->writeInConsole(QsAIEnemySend, MAINWINDOW);
    qDebug() << QsAIEnemySend;
}

QString MainWindow::checkMoveForTcp(QString QsFT)
{
    QString QsRespond;
    if (QsFT.length() == 4 || (QsFT.length() == 5 && (QsFT.right(1) == "q" || QsFT.right(1) == "r" ||
                                                      QsFT.right(1) == "b" || QsFT.right(1) == "k")))
    {
        //TODO: nie ogarniam wyrażeń regularnych:
        if ((QsFT.at(0) == 'a' || QsFT.at(0) == 'b' || QsFT.at(0) == 'c' || QsFT.at(0) == 'd' ||
             QsFT.at(0) == 'e' || QsFT.at(0) == 'f' || QsFT.at(0) == 'g' || QsFT.at(0) == 'h') &&
                (QsFT.at(1) == '1' || QsFT.at(1) == '2' || QsFT.at(1) == '3' || QsFT.at(1) == '4' ||
                 QsFT.at(1) == '5' || QsFT.at(1) == '6' || QsFT.at(1) == '7' || QsFT.at(1) == '8') &&
                (QsFT.at(2) == 'a' || QsFT.at(2) == 'b' || QsFT.at(2) == 'c' || QsFT.at(2) == 'd' ||
                 QsFT.at(2) == 'e' || QsFT.at(2) == 'f' || QsFT.at(2) == 'g' || QsFT.at(2) == 'h') &&
                (QsFT.at(3) == '1' || QsFT.at(3) == '2' || QsFT.at(3) == '3' || QsFT.at(3) == '4' ||
                 QsFT.at(3) == '5' || QsFT.at(3) == '6' || QsFT.at(3) == '7' || QsFT.at(3) == '8'))
        {
            QsRespond = "move " + QsFT;
            _pIgorBot->checkMsgForChenard(QsRespond); //wyślij zapytanie o ruch jak z arduino
        }
        else QsRespond = "ERROR: MainWindow::checkMoveForTcp: Wrong square positions\n";
    }
    else QsRespond = "ERROR: MainWindow::checkMoveForTcp: Wrong lenght of move cmd\n";

    return QsRespond;
}

void MainWindow::updatePortsComboBox(int nUsbPorst)
{
    QString QSUsbPorst = QString::number(nUsbPorst)
            + (nUsbPorst == 1 ? " port is ready to use\n" : " ports are ready to use\n");
    this->writeInConsole(QSUsbPorst, USB); //pokaż ile znaleziono portów

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

void MainWindow::on_sendUsbBtn_clicked() //wyślij wiadomość na usb
{
    if(_pArduinoUsb->usbInfo == NULL) //by coś wysyłać musi istnieć wybrany jakiś port
        this->writeInConsole("None port selected\n", USB);
    else
    {
        _pArduinoUsb->sendDataToUsb(ui->usbCmdLine->text()); //wyślij na usb wiadomość z pola textowego
        ui->usbCmdLine->clear(); // wyczyść pole textowe
    }
}

void MainWindow::on_openGripperBtn_clicked()
{
    _pDobotArm->gripperState(OPEN, ST_SERVICE);
}


void MainWindow::on_closeGripperBtn_clicked()
{
    _pDobotArm->gripperState(CLOSE, ST_SERVICE);
}

void MainWindow::on_startGmPosBtn_clicked()
{ //todo: te punkty wstawićî jako stałe z xmla
    qDebug() << "Placing arm above the chessboard.";
    _pDobotArm->addTextToConsole("Placing arm above the chessboard.\n", DOBOT);
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), _pDobotArm->getHomePos('y'),
                             _pDobotArm->getHomePos('z'));
    //todo: te liczby nazwać tym gdzie i czy są
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), -103, _pDobotArm->getHomePos('z'));
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), -103, _pDobotArm->getmiddleAboveBoardPos('z'));
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getmiddleAboveBoardPos('x'),
                             _pDobotArm->getmiddleAboveBoardPos('y'),
                             _pDobotArm->getmiddleAboveBoardPos('z'));
}
//retreatYPlus, retreatYMinus, middleAboveBoard;

void MainWindow::on_startDtPosBtn_clicked()
{
    qDebug() << "Returning safely to the HOME positions.";
    _pDobotArm->addTextToConsole("Returning safely to the HOME positions.\n", DOBOT);
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getmiddleAboveBoardPos('x'),
                             _pDobotArm->getmiddleAboveBoardPos('y'),
                             _pDobotArm->getmiddleAboveBoardPos('z'));
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), -103, _pDobotArm->getmiddleAboveBoardPos('z'));
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), -103, _pDobotArm->getHomePos('z'));
    _pDobotArm->addCmdToList(TO_POINT, ST_SERVICE, _pDobotArm->getHomePos('x'), _pDobotArm->getHomePos('y'),
                             _pDobotArm->getHomePos('z'));
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
        {
            _pTCPmsg->TcpQueueMsg(TEST, ui->sendTcpLineEdit->text());
        }
        else
        {
            _pTCPmsg->TcpQueueMsg(_pIgorBot->getAI() ? ARDUINO : WEBSITE, ui->sendTcpLineEdit->text());
        }
        ui->sendTcpLineEdit->clear();
    }
}

void MainWindow::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    if (tcpRespond.left(2) == "OK")
    {
        qDebug() << "direct TCP moves done:" << tcpRespond;

         //wytnij tylko ostatni ruch. sprawdzi status, legal, turę i odeśle na stronę
        //_pWebChess->checkMsgForChenard("move " + tcpMsgType.right(4));
        QString lastMove = tcpMsgType.right(4);
        _pChessboard->findBoardPos(lastMove);
        _pWebChess->TcpMoveOk();
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

void MainWindow::changeWindowtitle(QString title)
{
    this->setWindowTitle(title);
}


void MainWindow::setBoardDataLabels(QString QStrLabel, BOARD_DATA_LABELS labelType)
{
    switch(labelType)
    {
    case BDL_SOCKETS_ONLINE:
        ui->socketsLbl->setText(QStrLabel);
        break;
    case BDL_WHITE_NAME:
        ui->whiteNameLbl->setText(QStrLabel);
        break;
    case BDL_BLACK_NAME:
        ui->blackNameLbl->setText(QStrLabel);
        break;
    case BDL_TURN:
        ui->turnLbl->setText(QStrLabel);
        break;
    case BDL_GAME_STATUS:
        ui->gameStatusLbl->setText(QStrLabel);
        break;
    case BDL_MOVES:
        ui->movesLbl->setText(QStrLabel);
        break;
    case BDL_CASTLINGS:
        ui->castlingsLbl->setText(QStrLabel);
        break;
    case BDL_ENPASSANT:
        ui->enpassantLbl->setText(QStrLabel);
        break;
    case BDL_WHITE_TIME:
        ui->whiteTimeLbl->setText(QStrLabel);
        break;
    case BDL_BLACK_TIME:
        ui->blackTimeLbl->setText(QStrLabel);
        break;
    case BDL_QUEUE_PLAYERS:
        ui->queuedPlayersLbl->setText(QStrLabel);
        break;
    case BDL_QUEUE_TIME:
        ui->queueTimeLbl->setText(QStrLabel);
        break;
    default:
        qDebug() << "ERROR: unknown MainWindow::setBoardDataLabels labelType";
        break;
    }
}

void MainWindow::showBoard(QString QStrBoard)
{
    ui->boardPTE->clear();
    ui->boardPTE->setPlainText(QStrBoard);
}

void MainWindow::showLegalMoves(QStringList legalMoves)
{
    QString legal = legalMoves.join(" ");
    ui->legalPTE->clear();
    ui->legalPTE->setPlainText(legal);
}

void MainWindow::showHistoryMoves(QStringList historyMoves)
{
    ui->historyPTE->clear();
    QString history;
    int turn = 1;
    do
    {
        if (!historyMoves.isEmpty())
        {
            history += QString::number(turn) + ". " + historyMoves.takeFirst();
            if (!historyMoves.isEmpty()) history += "\t" + historyMoves.takeFirst();
            turn++;
        }
    } while (historyMoves.isEmpty());

    ui->historyPTE->setPlainText(history);
}
