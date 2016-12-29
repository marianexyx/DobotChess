#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

//mainwindow odpowiada za UI (przyciski itd. inne klasy odwołują się do niego przez:
//http://www.qtcentre.org/threads/25075-How-to-access-the-UI-from-another-class?s=d4d369d4d7c12212dd9ba8eacc9f0dce&p=287198#post287198
MainWindow::MainWindow(WebTable *pWebTable, Websockets *pWebSockets, Chessboard *pChessboard,
                        TCPMsgs *pTCPmsg, Dobot *pDobotArm, Chess *pChess, QWidget *parent) :
    QMainWindow(parent),
    /*_pDobotArm(new Dobot()),*/
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _pDobotArm = pDobotArm;
    _pWebTable = pWebTable;
    _pWebSockets = pWebSockets;
    _pChessboard = pChessboard;
    _pTCPmsg = pTCPmsg;
    _pChess = pChess;

    connect(ui->connectBtn, SIGNAL(clicked(bool)), _pDobotArm, SLOT(onConnectDobot())); //connect dobot
    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(onPTPsendBtnClicked())); //send PTP data
    setDobotValidators(); //wartości przymowane z klawiatury do wysłania na dobota

    _pDobotArm->setPeriodicTaskTimer();
    _pDobotArm->getPoseTimer();

    //Chessboard DobotChessboard001;
    //WebTable WebTable001;
    //Websockets Websockety(&WebTable001, websocketPort); //tworzenie obiektu odpowiadającego za websockety
    //TCPMsgs TCPCommunication;
    //Chess ChessCore(_pDobotArm, _pChessboard, _pTCPmsg, _pWebSockets, _pWebTable);

    //dzięki tym connectom (jeśli działają) można wywołać funkcję typu "ui" z innej klasy
    connect(_pDobotArm, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString)));
    connect(_pDobotArm, SIGNAL(JointLabelText(QString, short)),
            this, SLOT(setJointLabelText(QString, short)));
    connect(_pDobotArm, SIGNAL(AxisLabelText(QString, char)),
            this, SLOT(setAxisLabelText(QString, char)));
    connect(_pDobotArm, SIGNAL(ConnectButton(bool)),
            this, SLOT(setConnectButtonText(bool)));
    connect(_pDobotArm, SIGNAL(DobotErrorMsgBox()),
            this, SLOT(showDobotErrorMsgBox()));
    connect(_pTCPmsg, SIGNAL(addTextToTcpConsole(QString)),
            this, SLOT(writeInTcpConsole(QString))); //ERROR: segmentation fault
    connect(_pChess, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString)));
    connect(_pWebSockets, SIGNAL(addTextToWebsocketConsole(QString)),
            this, SLOT(writeInWebsocketConsole(QString)));
    connect(_pChessboard, SIGNAL(addTextToDobotConsole(QString)),
            this, SLOT(writeInDobotConsole(QString))); //ERROR: segmentation fault

    //connecty komunikujące klasy z sobą
    connect(_pTCPmsg, SIGNAL(MsgFromChenard(QString)), //przesyłanie odpowiedzi z chenard...
            _pChess, SLOT(checkMsgFromChenard(QString)));  //...na silnk gry.
    connect(_pWebSockets, SIGNAL(MsgFromWebsocketsToChess(QString)), //przesyłanie wiadomości z websocketów...
            _pChess, SLOT(checkMsgFromWebsockets(QString))); //...na silnk gry.
    connect(_pWebSockets, SIGNAL(MsgFromWebsocketsToWebtable(QString)), //przesyłanie wiadomości z websocketów...
            _pWebTable, SLOT(checkWebsocketMsg(QString))); //...na stół gry.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeInDobotConsole(QString QStrMsg)
{
    if(QStrMsg.isEmpty()) return; //blokada możliwości wysyłania pustej wiadomości

    if(QStrMsg == "/clear") //czy czyścimy okno
    {
        ui->dobot_debug->clear();
        return;
    }

    ui->dobot_debug->setPlainText(ui->dobot_debug->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_dobot = ui->dobot_debug->verticalScrollBar();
    scroll_dobot->setValue(scroll_dobot->maximum());
}

void MainWindow::setDobotValidators()
{
    //not more than 1000
    //QRegExp regExp("500|[-]|[1-9][0-9]{0,2}[.][0-9]{1,3}"); //oryginał
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

void MainWindow::setConnectButtonText(bool bConnectButton)
{
    if (bConnectButton)
    {
        ui->connectBtn->setText(tr("Connect"));
        ui->sendBtn->setEnabled(false);
        ui->xPTPEdit->setEnabled(false);
        ui->yPTPEdit->setEnabled(false);
        ui->zPTPEdit->setEnabled(false);
        ui->rPTPEdit->setEnabled(false);
    }
    else
    {
        ui->connectBtn->setText(tr("Disconnect"));
        ui->sendBtn->setEnabled(true);
        ui->xPTPEdit->setEnabled(true);
        ui->yPTPEdit->setEnabled(true);
        ui->zPTPEdit->setEnabled(true);
        ui->rPTPEdit->setEnabled(true);
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

    _pDobotArm->PTPvalues(nPtpCmd_x, nPtpCmd_y, nPtpCmd_z, nPtpCmd_r);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error!!!"), QMessageBox::Ok);
    return;
}

void MainWindow::writeInTcpConsole(QString QS_msg)
{
    if(QS_msg.isEmpty()) return; //blokada możliwości wysyłania pustej wiadomości

    if(QS_msg == "/clear") //czy czyścimy okno
    {
        ui->tcp_debug->clear();
        return;
    }

    ui->tcp_debug->setPlainText(ui->tcp_debug->toPlainText() + QS_msg);

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

    ui->websocket_debug->setPlainText(ui->websocket_debug->toPlainText() + QStrMsg);

    //auto scroll
    QScrollBar *scroll_websct = ui->websocket_debug->verticalScrollBar();
    scroll_websct->setValue(scroll_websct->maximum());
}
