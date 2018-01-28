#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(Websockets* pWebSockets, Chessboard* pBoardMain, Chessboard* pBoardRemoved,
                       TCPMsgs* pTCPmsg, ArduinoUsb* pArduinoUsb, Dobot* pDobot,
                       Chess* pChess, Client* pClient, QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chess");

    //todo: ustandaryzować jakoś te nazwy pointerów pomiędzy wszystkimi plikami
    _pDobot = pDobot;
    _pWebSockets = pWebSockets;
    _pBoardMain = pBoardMain;
    _pBoardRemoved = pBoardRemoved;
    _pTCPmsg = pTCPmsg;
    _pArduinoUsb = pArduinoUsb;
    _pClient = pClient;
    _pChess = pChess;

    //todo: niektórych znaczników SIGNALS i SLOTS nie da się usunąć. sprawdzić po...
    //...kompilacji czy się uda
    //ui signals to classes
    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangedMode())); //endtype change
    connect(ui->connectBtn, SIGNAL(clicked(bool)),
            _pDobot, SLOT(onConnectDobot())); //connect dobot
    this->setDobot_UI_PTE_Validators();

    //classes signals to ui
    connect(_pDobot, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pTCPmsg, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pWebSockets, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pChess, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pArduinoUsb, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pClient, addTextToLogPTE(QString, LOG), this, writeInConsole(QString, LOG));
    connect(_pChess, setBoardDataLabel(QString, BOARD_DATA_LABEL),
             this, setBoardDataLabel(QString, BOARD_DATA_LABEL));
    connect(_pBoardMain, setBoardDataLabel(QString, BOARD_DATA_LABEL),
             this, setBoardDataLabel(QString, BOARD_DATA_LABEL));
    connect(_pBoardRemoved, setBoardDataLabel(QString, BOARD_DATA_LABEL),
             this, setBoardDataLabel(QString, BOARD_DATA_LABEL));
    connect(_pClient, setBoardDataLabel(QString, BOARD_DATA_LABEL),
             this, setBoardDataLabel(QString, BOARD_DATA_LABEL));
    connect(_pBoardMain, clearFormBoard(), this, clearFormBoard()); //no need connect for removed
    connect(_pBoardMain, showBoardInForm(QString), this, showBoard(QString)); //todo: chenard
    connect(_pChess, showLegalMoves(QStringList), this, showLegalMoves(QStringList));
    connect(_pChess, showHistoryMoves(QStringList), this, showHistoryMoves(QStringList));
    connect(_pWebSockets, showClientsList(QList<Clients>), this, showClientsList(QList<Clients>));
    connect(_pDobot, SIGNAL(JointLabelText(QString, short)),
            this, SLOT(setJointLabelText(QString, short)));
    connect(_pDobot, SIGNAL(AxisLabelText(QString, char)),
            this, SLOT(setAxisLabelText(QString, char)));
    connect(_pDobot, deviceLabels(QString, QString, QString),
            this, setDeviceLabels(QString, QString, QString));
    connect(_pDobot, SIGNAL(RefreshDobotButtonsStates(bool)),
            this, SLOT(setDobotButtonsStates(bool)));
    connect(_pDobot, SIGNAL(DobotErrorMsgBox()),
            this, SLOT(showDobotErrorMsgBox()));
    connect(_pDobot, SIGNAL(queueLabels(int, int, int, int, int)),
            this, SLOT(setQueueLabels(int, int, int, int, int)));
    connect(_pArduinoUsb, SIGNAL(updatePortsComboBox(int)),
            this, SLOT(updatePortsComboBox(int)));
    connect(_pDobot->getServoPointer(), showArduinoGripperStateList(QList<ServoArduino>),
            this, showArduinoGripperStateList(QList<ServoArduino>));

    this->initControl(); //init dobot JOG control

    //todo: narobić tych tooltipów
    ui->directTcpMsgCheckBox->setToolTip("//todo");
    ui->emulatePlayerMsgLineEdit->setToolTip("send msg to Websockets::receivedMsg");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDobot_UI_PTE_Validators()
{
    QRegExp regExp("0|[-+]?[1-9][0-9]{0,2}[.][0-9]{1,3}");
    QValidator *validator = new QRegExpValidator(regExp);

    ui->xPTPEdit->setValidator(validator);
    ui->yPTPEdit->setValidator(validator);
    ui->zPTPEdit->setValidator(validator);
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

void MainWindow::setQueueLabels(int nSpaceLeft, int nDobotId, int nCoreMaxId,
                                int nCoreIdLeft, int CoreNextId)
{
    if (nSpaceLeft == 0) nSpaceLeft = 1; //to tylko dla tymczasowego zblokowania wyskakującego...
    //...warninga o unused parameter, póki nie wprowadzę tej funkcji
    //ui->DobotQueuedCmdLeftSpaceLabel->
    //setText(QString::number(nSpace)); //future: dobot.cc nie zrobił tego jeszcze- chyba już jest
    ui->DobotQueuedIndexLabel->setText(QString::number(nDobotId));
    ui->CoreMaxQueuedIndexLabel->setText(QString::number(nCoreMaxId));
    ui->CoreIndexAmountlabel->setText(QString::number(nCoreIdLeft));
    ui->CoreNextIdLabel->setText(QString::number(CoreNextId));
}

//todo: wszystkie przyciski się włączą, nawet jeżeli otrzymamy od dobota info że się nie udało
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
        ui->servoGripperEdit->setEnabled(false);
        ui->calibrateBtn->setEnabled(false);
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
        ui->middleAboveBtn->setEnabled(false);
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
        ui->servoGripperEdit->setEnabled(true);
        ui->calibrateBtn->setEnabled(true);
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
        ui->middleAboveBtn->setEnabled(true);
    }
}

void MainWindow::setDeviceLabels(QString QSdeviceSN, QString QSdeviceName,
                                 QString QSdeviceVersion)
{
    ui->deviceSNLabel->setText(QSdeviceSN);
    ui->DeviceNameLabel->setText(QSdeviceName);
    ui->DeviceInfoLabel->setText(QSdeviceVersion);
}

void MainWindow::on_sendBtn_clicked()
{
    qDebug() << "sendBtn clicked";

    _pChess->getMovementsPointer()->clearMove();

    bool bConversionXOk, bConversionYOk, bConversionZOk;

    Point3D point;
    float xPTE = ui->xPTPEdit->text().toFloat(&bConversionXOk);
    float yPTE = ui->yPTPEdit->text().toFloat(&bConversionYOk);
    float zPTE = ui->zPTPEdit->text().toFloat(&bConversionZOk);

    point.x = bConversionXOk ? xPTE : _pDobot->getLastGivenPoint().x;
    point.y = bConversionYOk ? yPTE : _pDobot->getLastGivenPoint().y;
    point.z = bConversionZOk ? zPTE : _pDobot->getLastGivenPoint().z;

    _pDobot->addArmMoveToQueue(DM_TO_POINT, point);

    //servo
    bool bConversionServoOk;
    float fServoDutyCycle = ui->servoGripperEdit->text().toFloat(&bConversionServoOk);
    if (bConversionServoOk)
        _pDobot->changeGripperAngle(fServoDutyCycle);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error"), QMessageBox::Ok);
    return;
}

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
    qDebug() << QStrMsg;

    //auto scroll
    QScrollBar *scroll_debug_log = ui->debug_log->verticalScrollBar();
    scroll_debug_log->setValue(scroll_debug_log->maximum());

    //prevent big string data
    int nMaximum = 10 * 1000;
    while (ui.textEdit->toPlainText().length() > nMaximum)
        ui.textEdit->textCursor().deletePreviousChar();

    //future: wszystkie debugi i logi zapisywać do plików txt
}

void MainWindow::initControl() {
    QSignalMapper* signalMapper  = new QSignalMapper(this);
    connect(ui->baseAngleAddBtn, pressed(), signalMapper, map());
    connect(ui->baseAngleSubBtn, pressed(), signalMapper, map());
    connect(ui->longArmAddBtn, pressed(), signalMapper, map());
    connect(ui->longArmSubBtn, pressed(), signalMapper, map());
    connect(ui->shortArmAddBtn, pressed(), signalMapper, map());
    connect(ui->shortArmSubBtn, pressed(), signalMapper, map());
    connect(ui->rHeadAddBtn, pressed(), signalMapper, map());
    connect(ui->rHeadSubBtn, pressed(), signalMapper, map());

    signalMapper->setMapping(ui->baseAngleAddBtn, 0);
    signalMapper->setMapping(ui->baseAngleSubBtn, 1);
    signalMapper->setMapping(ui->longArmAddBtn, 2);
    signalMapper->setMapping(ui->longArmSubBtn, 3);
    signalMapper->setMapping(ui->shortArmAddBtn, 4);
    signalMapper->setMapping(ui->shortArmSubBtn, 5);
    signalMapper->setMapping(ui->rHeadAddBtn, 6);
    signalMapper->setMapping(ui->rHeadSubBtn, 7);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onJOGCtrlBtnPressed(int)));

    connect(ui->baseAngleAddBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->baseAngleSubBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->longArmAddBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->longArmSubBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->shortArmAddBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->shortArmSubBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->rHeadAddBtn, released(), this, onJOGCtrlBtnReleased());
    connect(ui->rHeadSubBtn, released(), this, onJOGCtrlBtnReleased());
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

void MainWindow::on_sendSimulatedMsgBtn_clicked()
{
    if (!ui->emulatePlayerMsgLineEdit->text().isEmpty())
    {
        QString QStrServiceMove = ui->emulatePlayerMsgLineEdit->text();
        if (QStrServiceMove.left(5) == "move ")
            QStrServiceMove = QStrServiceMove.mid(5);

        qDebug() << "MainWindow::on_sendSimulatedMsgBtn_clicked(): "
                    "QStrServiceMove =" << QStrServiceMove;

        if (!PosFromTo::isMoveInProperFormat(QStrServiceMove)) return;

        _pChess->findAndSaveMoveAndSendItToTcp(QStrServiceMove);
    }

    ui->emulatePlayerMsgLineEdit->clear();
}

void MainWindow::on_calibrateBtn_clicked()
{
    if (ui->xLabel->text().toInt() == (int)_pDobot->getHomePos().x &&
            ui->yLabel->text().toInt() == (int)_pDobot->getHomePos().y &&
            ui->zLabel->text().toInt() == (int)_pDobot->getHomePos().z)
    {
        _pDobot->addArmMoveToQueue(DM_CALIBRATE);
    }
    else
        qDebug() << "ERROR: MainWindow::on_calibrateBtn_clicked(): Dobot not in home positions";
}

void MainWindow::on_homeBtn_clicked()
{
    _pDobot->addArmMoveToQueue(DM_TO_POINT, _pDobot->getHomePos());
}

void MainWindow::on_upBtn_clicked()
{
    if (_pChess->getMovementsPointer()->isMoveSet())
        _pDobot->addArmMoveToQueue(DM_UP);
    else
        qDebug() << "ERROR: MainWindow::on_upBtn_clicked(): move isn't set";
}

void MainWindow::on_downBtn_clicked()
{
    if (_pChess->getMovementsPointer()->isMoveSet())
        _pDobot->addArmMoveToQueue(DM_DOWN);
    else
        qDebug() << "ERROR: MainWindow::on_upBtn_clicked(): move isn't set";
}

void MainWindow::on_resetDobotIndexBtn_clicked()
{
    int result = SetQueuedCmdClear(); //wyczyść/wyzeruj zapytania w dobocie
    //todo: jak już zrobiłem to w tym miejscu, to rpobić to wszędzie
    if (result == DobotCommunicate_NoError)
        this->writeInConsole("Cleared Dobot Queued Cmds.\n", LOG_DOBOT);
    else
        qDebug() << "ERROR: MainWindow::on_resetDobotIndexBtn_clicked(): reset failed";
}

void MainWindow::on_AIBtn_clicked()
{
    if (ui->botOffRadioBtn->isChecked())
    {
        _pChess->getBotPointer()->setAI(false);
        this->writeInConsole("Turned off Igors AI\n", LOG_MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(false);
        ui->AIEnemyLineEdit->setEnabled(false);
    }
    else if (ui->botOnRadioBtn->isChecked())
    {
        _pChess->getBotPointer()->setAI(true);
        this->writeInConsole("Turned on Igors AI\n", LOG_MAINWINDOW);
        ui->AIEnemySendBtn->setEnabled(true);
        ui->AIEnemyLineEdit->setEnabled(true);
    }

    _pChess->getBotPointer()->enemyStart();
}

void MainWindow::on_AIEnemySendBtn_clicked()
{
    _pChess->getBotPointer()->setAIAsPlayer2(ui->simulateArduinoPlayer2checkBox->isChecked());

    if (PosFromTo::isMoveInProperFormat(ui->AIEnemyLineEdit->text()))
        _pChess->findAndSaveMoveAndSendItToTcp(ui->AIEnemyLineEdit->text());
}

void MainWindow::updatePortsComboBox(int nUsbPorst)
{
    QString QStrUsbPortsAmount = QString::number(nUsbPorst)
            + (QStrUsbPortsAmount == 1 ? " port is ready to use\n" : " ports are ready to use\n");
    this->writeInConsole(QStrUsbPortsAmount, LOG_USB);

    //refresh ports list
    ui->portsComboBox->clear();
    ui->portsComboBox->addItem("NULL");
    for(int i=0; i<nUsbPorst; i++)
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
    _pDobot->addArmMoveToQueue(DM_OPEN);
}


void MainWindow::on_closeGripperBtn_clicked()
{
    _pDobot->addArmMoveToQueue(DM_CLOSE);
}

void MainWindow::on_middleAboveBtn_clicked()
{
    Point3D mid; //todo: ciągnąc z xml
    _pDobot->addArmMoveToQueue(mid);
}

//todo: ogarnąć punkty przejściowe
void MainWindow::on_startGmPosBtn_clicked()
{ //todo: te punkty wstawić pierwej jako stałe z xmla
    //todo: bezsensowny zawijaniec przez klasy:
    if (ui->xLabel->text().toInt() == (int)_pDobot->getHomePos().x &&
            ui->yLabel->text().toInt() == (int)_pDobot->getHomePos().y &&
            ui->zLabel->text().toInt() == (int)_pDobot->getHomePos().z)
    {
        this->writeInConsole("Placing arm above the chessboard.\n", LOG_DOBOT);
        _pDobot->addArmMoveToQueue(DM_TO_POINT, _pDobot->getHomePos());
        //todo: te liczby nazwać tym gdzie i czym są
        Point3D rightBottomLowerMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                    _pDobot->getHomePos().z);
        _pDobot->addArmMoveToQueue(DM_TO_POINT, rightBottomLowerMainBoardSafeCorner);
        Point3D rightBottomHigherMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                     _pBoardMain->getBoardPoint3D(BP_MIDDLE).z);
        _pDobot->addArmMoveToQueue(DM_TO_POINT, rightBottomHigherMainBoardSafeCorner);
        _pDobot->addArmMoveToQueue(DM_TO_POINT, _pBoardMain->getBoardPoint3D(BP_MIDDLE));
    }
    else
        qDebug() << "ERROR: MainWindow::on_startGmPosBtn_clicked(): Dobot not in home positions";
}

//todo: ogarnąć punkty przejściowe
void MainWindow::on_startDtPosBtn_clicked()
{
    this->writeInConsole("Returning safely to the DM_HOME positions.\n", LOG_DOBOT);

    _pDobot->addArmMoveToQueue(DM_TO_POINT, _pBoardMain->getBoardPoint3D(BP_MIDDLE));
    Point3D rightBottomHigherMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pBoardMain->getBoardPoint3D(BP_MIDDLE).z);
    _pDobot->addArmMoveToQueue(DM_TO_POINT, rightBottomHigherMainBoardSafeCorner);
    Point3D rightBottomLowerMainBoardSafeCorner(_pDobot->getHomePos().x, -103,
                                                _pDobot->getHomePos().z);
    _pDobot->addArmMoveToQueue(DM_TO_POINT, rightBottomLowerMainBoardSafeCorner);
    _pDobot->addArmMoveToQueue(DM_TO_POINT, _pDobot->getHomePos());
}

void MainWindow::on_SimulateFromUsbBtn_clicked()
{
    if (!ui->SimulateFromUsbLineEdit->text().isEmpty())
    {
        _pArduinoUsb->msgFromUsbToChess(ui->SimulateFromUsbLineEdit->text());
        ui->SimulateFromUsbLineEdit->clear();
    }
}

void MainWindow::on_sendTcpBtn_clicked()
{
    if (!ui->sendTcpLineEdit->text().isEmpty())
    {
        if (ui->directTcpMsgCheckBox->isChecked())
            _pTCPmsg->queueCmd(TEST, ui->sendTcpLineEdit->text());
        else
            _pTCPmsg->queueCmd(_pChess->getBotPointer()->getAI() ? ARDUINO : WEBSITE, ui->sendTcpLineEdit->text());
        ui->sendTcpLineEdit->clear();
    }
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

//todo: przerobić tak by działało tutaj
void MainWindow::showBoard(QString QStrBoard)
{
    /*
QString **ChessStatus::FENToBoard(QString FENBoard)
{
    QString** QStrBoardArray = create2DArray();

    QStringList QStrFENBoardRows = FENBoard.split(QRegExp("/"));
    std::reverse(QStrFENBoardRows.begin(), QStrFENBoardRows.end());
    if (QStrFENBoardRows.size() == 8)
    {
        QRegExp rxEmpty("\\d");
        for (int nRow=0; nRow<=7; ++nRow)
        {
            int nColumn = 0;
            QString QStrFENBoardRow = QStrFENBoardRows.at(nRow);
            QStringList FENSigns = QStrFENBoardRow.split("");
            FENSigns.removeFirst();
            FENSigns.removeLast();

            for (int nFENSignPos=0; nFENSignPos<FENSigns.size(); ++nFENSignPos)
            {
                QString QStrFENSign = FENSigns.at(nFENSignPos);
                if (!rxEmpty.exactMatch(QStrFENSign)) //not digits
                {
                    QStrBoardArray[nColumn][nRow] = QStrFENSign;
                    if (nColumn > D_8) qDebug() << "ERROR: ChessStatus::FENToBoard(): nColumn"
                                                   " > 8 =" << nColumn;
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        QStrBoardArray[nColumn][nRow] = ".";
                        if (nColumn > D_8) qDebug() << "ERROR: ChessStatus::FENToBoard(): nColumn"
                                                       " > 8 =" << nColumn;
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: ChessStatus::FENToBoard(): boardRows.size() != 8. Size = " <<
                    QStrFENBoardRows.size();
        for (int i=0; i<=QStrFENBoardRows.size()-1; ++i)
            qDebug() << "QStrFENBoardRows at" << i << "=" << QStrFENBoardRows.at(i);
    }

    return QStrBoardArray;
}
    */

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

void MainWindow::on_usbCmdLine_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->sendUsbBtn->setEnabled(true);
    else ui->sendUsbBtn->setEnabled(false);
}

void MainWindow::on_emulatePlayerMsgLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->sendSimulatedMsgBtn->setEnabled(true);
    else ui->sendSimulatedMsgBtn->setEnabled(false);
}

void MainWindow::on_SimulateFromUsbLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->SimulateFromUsbBtn->setEnabled(true);
    else ui->SimulateFromUsbBtn->setEnabled(false);
}

void MainWindow::on_sendTcpLineEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL) ui->sendTcpBtn->setEnabled(true);
    else ui->sendTcpBtn->setEnabled(false);
}

void MainWindow::on_xPTPEdit_textChanged(const QString &textChanged)
{
    if (textChanged != NULL || ui->yPTPEdit->text() != NULL || ui->zPTPEdit->text() != NULL)
        ui->sendBtn->setEnabled(true);
    else ui->sendBtn->setEnabled(false);
}

void MainWindow::on_yPTPEdit_textChanged(const QString &textChanged)
{
    if (ui->xPTPEdit->text() != NULL || textChanged != NULL || ui->zPTPEdit->text() != NULL)
        ui->sendBtn->setEnabled(true);
    else ui->sendBtn->setEnabled(false);
}

void MainWindow::on_zPTPEdit_textChanged(const QString &textChanged)
{
    if (ui->xPTPEdit->text() != NULL || ui->yPTPEdit->text() != NULL  || textChanged != NULL)
        ui->sendBtn->setEnabled(true);
    else ui->sendBtn->setEnabled(false);
}
