#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(WebTable *pWebTable, Websockets *pWebSockets, Chessboard *pChessboard,
                       TCPMsgs *pTCPmsg, Dobot *pDobotArm, Chess *pChess, QWidget *parent) :
    QMainWindow(parent),
    /*_pDobotArm(new Dobot()),*/
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //bodajże wskazuje że UI dziedziczy to tej (this) klasie, przez co zostanie...
    //... zniszczona razem z mainwindow.

    _pDobotArm = pDobotArm;
    _pWebTable = pWebTable;
    _pWebSockets = pWebSockets;
    _pChessboard = pChessboard;
    _pTCPmsg = pTCPmsg;
    _pChess = pChess;

    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangedMode())); //endtype change
    connect(ui->connectBtn, SIGNAL(clicked(bool)),
            _pDobotArm, SLOT(onConnectDobot())); //connect dobot
    connect(ui->sendBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPTPsendBtnClicked())); //send PTP data
    this->setDobotValidators(); //wartości przymowane z klawiatury do wysłania na dobota

    //dzięki tym connectom można wywołać funkcję typu "ui" z innej klasy
    connect(_pDobotArm, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString)));
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
    connect(_pTCPmsg, SIGNAL(addTextToTcpConsole(QString)),
            this, SLOT(writeInTcpConsole(QString)));
    connect(_pChess, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString)));
    connect(_pWebSockets, SIGNAL(addTextToWebsocketConsole(QString)),
            this, SLOT(writeInWebsocketConsole(QString)));
    connect(_pChessboard, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString)));
    connect(_pDobotArm, SIGNAL(QueueLabels(int,int,int,int,int)),
            this, SLOT(setQueueLabels(int,int,int,int,int)));

    //connecty komunikujące klasy z sobą
    connect(_pTCPmsg, SIGNAL(MsgFromChenard(QString)), //przesyłanie odpowiedzi z chenard...
            _pChess, SLOT(checkMsgFromChenard(QString)));  //...na silnk gry.
    connect(_pWebSockets, SIGNAL(MsgFromWebsocketsToChess(QString)), //przesyłanie wiadomości z ...
            _pChess, SLOT(checkMsgFromWebsockets(QString))); //...websocketów na silnk gry.
    connect(_pWebSockets, SIGNAL(MsgFromWebsocketsToWebtable(QString)), //przesyłanie wiadomości z ...
            _pWebTable, SLOT(checkWebsocketMsg(QString))); //...websocketów na stół gry.

    //init JOG control
    this->initControl();
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
    //ui->DobotQueuedCmdLeftSpaceLabel->setText(QString::number(nSpace)); //dobot.cc nie zrobił tego jeszcze
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
        ui->startPosBtn->setEnabled(false);

        ui->sendBtn->setEnabled(false);
        ui->xPTPEdit->setEnabled(false);
        ui->yPTPEdit->setEnabled(false);
        ui->zPTPEdit->setEnabled(false);
        ui->rPTPEdit->setEnabled(false);
        ui->servo1GripperEdit->setEnabled(false);
        ui->servo2GripperEdit->setEnabled(false);
        ui->homeBtn->setEnabled(false);
        ui->serviceCheckBox->setEnabled(false);
        ui->upBtn->setEnabled(false);
        ui->downBtn->setEnabled(false);
        ui->botOffRadioBtn->setEnabled(false);
        ui->botOnRadioBtn->setEnabled(false);
        ui->AIBtn->setEnabled(false);
        ui->AIEnemyStartBtn->setEnabled(false);
        ui->AIEnemySendBtn->setEnabled(false);

        ui->emulatePlayerMsgLineEdit->setEnabled(false);
        ui->sendSimulatedMsgBtn->setEnabled(false);

        ui->gripperBtn->setEnabled(false);
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
        ui->startPosBtn->setEnabled(true);

        ui->sendBtn->setEnabled(true);
        ui->xPTPEdit->setEnabled(true);
        ui->yPTPEdit->setEnabled(true);
        ui->zPTPEdit->setEnabled(true);
        ui->rPTPEdit->setEnabled(true);
        ui->servo1GripperEdit->setEnabled(true);
        ui->servo2GripperEdit->setEnabled(true);
        ui->homeBtn->setEnabled(true);
        ui->serviceCheckBox->setEnabled(true);
        ui->upBtn->setEnabled(true);
        ui->downBtn->setEnabled(true);
        ui->botOffRadioBtn->setEnabled(true);
        ui->botOnRadioBtn->setEnabled(true);
        ui->AIBtn->setEnabled(true);

        ui->emulatePlayerMsgLineEdit->setEnabled(true);

        ui->gripperBtn->setEnabled(true);
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
    int nPtpCmd_x = ui->xPTPEdit->text().toFloat();
    int nPtpCmd_y = ui->yPTPEdit->text().toFloat();
    int nPtpCmd_z = ui->zPTPEdit->text().toFloat();
    int nPtpCmd_r = ui->rPTPEdit->text().toFloat();
    if (nPtpCmd_x != 0) _pDobotArm->addCmdToList(-1, false, nPtpCmd_x, nPtpCmd_y, nPtpCmd_z, nPtpCmd_r);

    float fServoDutyCycle1 = ui->servo1GripperEdit->text().toFloat();
    float fServoDutyCycle2 = ui->servo2GripperEdit->text().toFloat();
    if (fServoDutyCycle1 !=0 && fServoDutyCycle2 !=0)
        _pDobotArm->gripperAngle(fServoDutyCycle1, fServoDutyCycle2);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error!!!"), QMessageBox::Ok);
    return;
}

void MainWindow::writeInTcpConsole(QString QStrMsg)
{
    if(QStrMsg.isEmpty()) return; //blokada możliwości wysyłania pustej wiadomości

    if(QStrMsg == "/clear") //czy czyścimy okno
    {
        ui->tcp_debug->clear();
        return;
    }
    QStrMsg = ">" + QStrMsg;
    ui->tcp_debug->setPlainText(ui->tcp_debug->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_tcp = ui->tcp_debug->verticalScrollBar();
    scroll_tcp->setValue(scroll_tcp->maximum());
}

void MainWindow::writeInWebsocketConsole(QString QStrMsg)
{
    //qDebug() << "MainWindow::writeInWebsocketConsole received: " << QStrMsg;
    if(QStrMsg.isEmpty()) return; //blokada możliwości wysyłania pustej wiadomości

    if(QStrMsg == "/clear") //czy czyścimy okno
    {
        ui->websocket_debug->clear();
        return;
    }
    QStrMsg = ">" + QStrMsg;
    ui->websocket_debug->setPlainText(ui->websocket_debug->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_websct = ui->websocket_debug->verticalScrollBar();
    scroll_websct->setValue(scroll_websct->maximum());
}

void MainWindow::writeInDobotConsole(QString QStrMsg)
{
    if(QStrMsg.isEmpty()) return; //blokada możliwości wysyłania pustej wiadomości

    if(QStrMsg == "/clear") //czy czyścimy okno
    {
        ui->dobot_debug->clear();
        return;
    }
    QStrMsg = ">" + QStrMsg;
    ui->dobot_debug->setPlainText(ui->dobot_debug->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_dobot = ui->dobot_debug->verticalScrollBar();
    scroll_dobot->setValue(scroll_dobot->maximum());
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
        if (ui->serviceCheckBox->isChecked()) //tylko wartości pola np. "e2" lub ew. usuwanie: "move xxe2"
        {
            QString QsServiceMsg = ui->emulatePlayerMsgLineEdit->text();
            if (QsServiceMsg.left(4) == "move") QsServiceMsg = QsServiceMsg.mid(7);
            qDebug() << "QsServiceMsg.mid(7) = " << QsServiceMsg;
            //qDebug() << "QsServiceMsg = " << QsServiceMsg;
            int nServiceLetterPos = _pChessboard->findPieceLetterPos(QsServiceMsg.left(1));
            int nServiceDigitPos = QsServiceMsg.mid(1,1).toInt() - 1;
            //qDebug() << "nServiceLetterPos = " << nServiceLetterPos
            //<< ", nServiceDigitPos = " << nServiceDigitPos;
            float fService_x = _pChessboard->afChessboardPositions_x[nServiceLetterPos][nServiceDigitPos];
            float fService_y = _pChessboard->afChessboardPositions_y[nServiceLetterPos][nServiceDigitPos];
            qDebug() << "fService_x = " << fService_x << ", fService_y = " << fService_y;
            _pDobotArm->addCmdToList(-1, false, fService_x, fService_y, ACTUAL_POS, ACTUAL_POS);
            _pChessboard->PieceActualPos.Letter = nServiceLetterPos;
            _pChessboard->PieceActualPos.Digit = nServiceDigitPos;
            if (QsServiceMsg.right(1) == "r") _pDobotArm->removePiece(nServiceLetterPos, nServiceDigitPos);
        }
        else
        {
            _pChess->setServiceTests(true);
            _pWebSockets->processWebsocketMsg(ui->emulatePlayerMsgLineEdit->text());
            ui->emulatePlayerMsgLineEdit->clear();
            _pChess->setServiceTests(false);
        }
    }
}

void MainWindow::on_gripperBtn_clicked()
{
    qDebug() << "on_gripperBtn_clicked";
    if (ui->gripperBtn->text() == "Open gripper")
    {
        _pDobotArm->gripperOpennedState(true, 'v');
        ui->gripperBtn->setText(tr("Close gripper"));
    }
    else if (ui->gripperBtn->text() == "Close gripper")
    {
        _pDobotArm->gripperOpennedState(false, 'v');
        ui->gripperBtn->setText(tr("Open gripper"));
    }
}

void MainWindow::on_homeBtn_clicked()
{
    _pDobotArm->addTextToDobotConsole("HomeCmd: recalibrating arm...\n");
    HOMECmd HOMEChess;
    HOMEChess.reserved = 1; //todo: jak się to ma do innych indexów?
    uint64_t homeIndex = _pDobotArm->getDobotQueuedCmdIndex();
    int result = SetHOMECmd(&HOMEChess, true, &homeIndex);
    if (result == DobotCommunicate_NoError)
    {
        uint64_t dobotQueue;
        _pDobotArm->setDobotQueuedCmdIndex(GetQueuedCmdCurrentIndex(&dobotQueue));
        _pDobotArm->setDobotQueuedCmdIndex(dobotQueue);
    }
}

void MainWindow::on_upBtn_clicked()
{
    _pDobotArm->armUpDown(true, true, 'v');
}

void MainWindow::on_downBtn_clicked()
{
    _pDobotArm->armUpDown(false, true, 'v');
}

void MainWindow::on_resetDobotIndexBtn_clicked()
{
    int result = SetQueuedCmdClear(); //wyczyść/wyzeruj zapytania w dobocie

    if (result == DobotCommunicate_NoError)
        _pDobotArm->addTextToDobotConsole("Cleared Dobot Queued Cmds.\n");
    else if (result == DobotCommunicate_BufferFull)
        _pDobotArm->addTextToDobotConsole("ERROR: Dobot buffer is full.\n");
    else if (result == DobotCommunicate_Timeout)
        _pDobotArm->addTextToDobotConsole("ERROR: Dobot communicate timeout.");
    else _pDobotArm->addTextToDobotConsole("ERROR: wrong result in "
                                           "MainWindow::on_resetDobotIndexBtn_clicked().\n");
}

void MainWindow::on_executeDobotComandsBtn_clicked()
{
    int result = SetQueuedCmdStartExec(); //rozpocznij wykonywanie zakolejkowanych komend.

    if (result == DobotCommunicate_NoError)
        _pDobotArm->addTextToDobotConsole("Start executing dobot queued cmds.\n");
    else if (result == DobotCommunicate_BufferFull)
        _pDobotArm->addTextToDobotConsole("ERROR: Dobot buffer is full.\n");
    else if (result == DobotCommunicate_Timeout)
        _pDobotArm->addTextToDobotConsole("ERROR: Dobot communicate timeout.\n");
    else _pDobotArm->addTextToDobotConsole("ERROR: wrong result in "
                                           "MainWindow::on_executeDobotComandsBtn_clicked().\n");
}

void MainWindow::on_startPosBtn_clicked()
{
    if (ui->startPosBtn->text() == "StartGmPos")
    {
        ui->startPosBtn->setText(tr("StartDtPos"));
        qDebug() << "Placing arm above the chessboard.";
        _pDobotArm->addTextToDobotConsole("Placing arm above the chessboard.\n");
        _pDobotArm->addCmdToList(-1, false, 144, 0, 10);
        _pDobotArm->addCmdToList(-1, false, 144, -103, 10);
        _pDobotArm->addCmdToList(-1, false, 145, -103, 45);
        _pDobotArm->addCmdToList(-1, false, 260, -10, 65);
    }
    else if (ui->startPosBtn->text() == "StartDtPos")
    {
        ui->startPosBtn->setText(tr("StartGmPos"));
        qDebug() << "Returning safely to the HOME positions.";
        _pDobotArm->addTextToDobotConsole("Returning safely to the HOME positions.\n");
        _pDobotArm->addCmdToList(-1, false, 260, -10, 65);
        _pDobotArm->addCmdToList(-1, false, 145, -103, 45);
        _pDobotArm->addCmdToList(-1, false, 144, -103, 10);
        _pDobotArm->addCmdToList(-1, false, 144, 0, 10);
    }
}

void MainWindow::on_AIBtn_clicked()
{
    if (ui->botOffRadioBtn->isChecked()) //po prostu odstawi bierki i włączy nową grę
    {
        _pChess->setAI(false);
        ui->AIEnemyStartBtn->setEnabled(false);
        ui->AIEnemySendBtn->setEnabled(false);
    }
    else if (ui->botOnRadioBtn->isChecked())
    {
        _pChess->setAI(true); //spowoduje, że bot wymyśli ruch, poczeka aż gracz kliknie start i...
        //...wtedy ruszy z ruchem swoim
        ui->AIEnemyStartBtn->setEnabled(true);
        ui->AIEnemySendBtn->setEnabled(true);
    }
    _pChess->resetPiecePositions();
    _pChess->NewGame();

}

void MainWindow::on_AIEnemyStartBtn_clicked() //jeżeli ktoś wciska start to niech bot zacznie swój ruch
{
    if (_pChess->getAI()) //dodatkowe zabezpieczenie. przycisk powinien być...
        //...nieaktywny jeżeli AI nie jest włączone
    {
        _pChess->resetPiecePositions(); //przywróć bierki na pierwotne pozycje
        _pChess->NewGame(); //zacznij w pamięci chenardu nową grę
        //pierwszy ruch bota nigdy nie będzie specjalny, zatem można go od razu normalnie wykonać
        _pChessboard->findBoardPos("move " + _pChessboard->QsAIPiecieFromTo);
        _pChess->pieceMovingSequence('n');
    }
    else
    {
        this->writeInDobotConsole("ERROR: somehow initiated MainWindow::on_AIEnemyStartBtn_clicked()"
                                  " function with _pChess->getAI() as false value\n");
        qDebug() << "ERROR: somehow initiated MainWindow::on_AIEnemyStartBtn_clicked()"
                    " function with _pChess->getAI() as false value";
    }
}

void MainWindow::on_AIEnemySendBtn_clicked()
{ //TODO: wyrzucić wykonywanie tych funkcji do klasy 'chess'
    if (_pChess->getAI()) //dodatkowe zabezpieczenie. przycisk powinien być...
        //...nieaktywny jeżeli AI nie jest włączone
    {
        QString QsEnLF = ui->AIEnLtrFromLbl->text(); //enemy's from letter
        int nEnDF = ui->AIEnDgtFromLbl->text().toInt(); //enemy's from digit
        QString QsEnLT = ui->AIEnLtrToLbl->text(); //enemy's to letter
        int nEnDT = ui->AIEnDgtToLbl->text().toInt(); //enemy's to digit
        QString QsEnemyMove;
        //TODO: nie ogarniam wyrażeń regularnych:
        if (nEnDF >= 1 && nEnDF <= 8 &&
                (QsEnLF == "a" || QsEnLF == "b" || QsEnLF == "c" || QsEnLF == "d" ||
                 QsEnLF == "e" || QsEnLF == "f" || QsEnLF == "g" || QsEnLF == "h") &&
                nEnDT >= 1 && nEnDT <= 8 &&
                (QsEnLT == "a" || QsEnLT == "b" || QsEnLT == "c" || QsEnLT == "d" ||
                 QsEnLT == "e" || QsEnLT == "f" || QsEnLT == "g" || QsEnLT == "h"))
        {
            QsEnemyMove = "move " + QsEnLF + QString::number(nEnDF)
                     + QsEnLT + QString::number(nEnDT); //sklej wiadomość w np.: "move e2e4"
            _pWebSockets->sendToChess(QsEnemyMove); //wyślij zapytanie o ruch tak, jakby...
            //...szło ono ze strony
        }
        else
        {
            this->writeInDobotConsole("ERROR: on_AIEnemySendBtn_clicked(): Wrong square positions\n");
            qDebug() << "ERROR: on_AIEnemySendBtn_clicked(): Wrong square positions";
        }
    }
    else
    {
        this->writeInDobotConsole("ERROR: somehow initiated MainWindow::on_AIEnemyStartBtn_clicked()"
                                  " function with _pChess->getAI() as false value\n");
        qDebug() << "ERROR: somehow initiated MainWindow::on_AIEnemyStartBtn_clicked()"
                    " function with _pChess->getAI() as false value";
    }
}
