#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(Chess* pChess, QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pChess = pChess;
    m_pDobot = m_pChess->getDobotPointer();
    m_pWebsockets = m_pChess->getWebsocketsPointer();
    m_pPieceController = m_pChess->getPieceControllerPointer();
    m_pBoardMain = m_pChess->getBoardMainPointer();
    m_pBoardRemoved = m_pChess->getBoardRemovedPointer();
    m_pBoardChenard = m_pChess->getBoardChenardPointer();
    m_pTCPMsgs = m_pChess->getTCPMsgsPointer();
    m_pClientsList = m_pChess->getClientsPointer();

    m_titleFormTimer = new QTimer();
    m_titleFormTimer->setInterval(200);
    m_titleFormTimer->setSingleShot(false);
    connect(m_titleFormTimer, SIGNAL(timeout()), this, SLOT(changeWindowTitle()));
    m_titleFormTimer->start();

    ///GUI signals
    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onChangedMode())); //endtype change
    connect(ui->connectBtn, SIGNAL(clicked(bool)),
            m_pDobot, SLOT(onConnectDobot())); //connect dobot

    ///classes signals
    connect(m_pDobot, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(m_pTCPMsgs, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(m_pWebsockets, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(m_pChess, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(m_pPieceController, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));
    connect(m_pClientsList, SIGNAL(addTextToLogPTE(QString, LOG)),
            this, SLOT(writeInConsole(QString, LOG)));

    //board data signals
    connect(m_pChess, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(m_pBoardMain, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(m_pBoardRemoved, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(m_pClientsList, SIGNAL(setBoardDataLabel(QString, BOARD_DATA_LABEL)),
             this, SLOT(setBoardDataLabel(QString, BOARD_DATA_LABEL)));
    connect(m_pBoardChenard, SIGNAL(showImaginaryBoardInUI(QString)),
            this, SLOT(showImaginaryBoardInUI(QString)));
    connect(m_pPieceController, SIGNAL(showRealBoardInUI()),
            this, SLOT(showRealBoardInUI()));
    connect(m_pBoardMain, SIGNAL(clearBoardInUI()), //connect for removed board not needed
            this, SLOT(clearBoardInUI()));
    connect(m_pChess, SIGNAL(showLegalMovesInUI(QStringList)),
            this, SLOT(showLegalMovesInUI(QStringList)));
    connect(m_pChess, SIGNAL(showHistoryMovesInUI(QStringList)),
            this, SLOT(showHistoryMovesInUI(QStringList)));

    //dobot slots
    connect(m_pDobot, SIGNAL(JointLabelText(QString, short)),
            this, SLOT(setJointLabelText(QString, short)));
    connect(m_pDobot, SIGNAL(AxisLabelText(QString, char)),
            this, SLOT(setAxisLabelText(QString, char)));
    connect(m_pDobot, SIGNAL(setDobotButtonsStates(bool)),
            this, SLOT(setDobotButtonsStates(bool)));
    connect(m_pDobot, SIGNAL(deviceLabels(QString, QString, QString)),
            this, SLOT(setDeviceLabels(QString, QString, QString)));
    connect(m_pDobot, SIGNAL(DobotErrorMsgBox()),
            this, SLOT(showDobotErrorMsgBox()));
    connect(m_pDobot, SIGNAL(queueLabels(uint, uint64_t, uint64_t, int, uint64_t)),
            this, SLOT(setQueueLabels(uint, uint64_t, uint64_t, int, uint64_t)));
    connect(m_pDobot, SIGNAL(showQueuedArmCmdsOnCore(QString)),
            this, SLOT(showQueuedArmCmdsOnCore(QString)));
    connect(m_pDobot, SIGNAL(showSentArmCmdsToDobot(QString)),
            this, SLOT(showSentArmCmdsToDobot(QString)));

    //clients slots
    connect(m_pClientsList, SIGNAL(showClientsListInUI(QList<Client>)),
            this, SLOT(showClientsListInUI(QList<Client>)));
    connect(m_pClientsList, SIGNAL(showQueuedClientsListInUI(QString)),
            this, SLOT(showQueuedClientsListInUI(QString)));

    ui->connectBtn->setToolTip("Connect with arm.");
    ui->homeBtn->setToolTip("Go to start position.");
    ui->calibrateBtn->setToolTip("Initialize move sequence: calibrate stepper"
                                 " motors. Needed after power up arm.");
    ui->startGmPosBtn->setToolTip("Initialize move sequence: go to game's start "
                                  "position, from dobot's start position.");
    ui->startDtPosBtn->setToolTip("Initialize move sequence: go to dobot's start "
                                  "position, from game's start position.");
    ui->retreatLeftBtn->setToolTip("Go to left side of chessboard (retreat) "
                                   "position.");
    ui->middleAboveBtn->setToolTip("Go to middle point of chessboard, above the "
                                   "highest pieces.");
    ui->retreatRightBtn->setToolTip("Go to right side of chessboard (retreat) "
                                    "position.");
    ui->upBtn->setToolTip("Move arm above piece height (works only with field set).");
    ui->downBtn->setToolTip("Move arm do the ground level of the chessboard (works "
                            "only with field set).");
    ui->openGripperBtn->setToolTip("Open robot arm gripper (release piece).");
    ui->closeGripperBtn->setToolTip("Close robot arm gripper (grab piece).");
    ui->emulatePlayerMsgLineEdit->setToolTip("Write player move command, f.e.: "
                                             "'e2' or 'e2r' or 'e2e4.");
    ui->sendSimulatedMsgBtn->setToolTip("Execute player move command, f.e.: 'e2' "
                                        "or 'e2r' or 'e2e4.");
    ui->sendTcpLineEdit->setToolTip("Write command directly to TCP chess engine.");
    ui->sendTcpBtn->setToolTip("Send command directly to TCP chess engine.");
    ui->sendPointBtn->setToolTip("Send Cartesian point immediately to arm to "
                                 "execute.");
    ui->resetDobotIndexBtn->setToolTip("Reset dobot (only) current ID.");
    ui->teachMode->setToolTip("Change manual arm control with buttons between "
                              "joint and axis buttons.");

    this->initDobotsBasicButtonsControl(); //init dobot JOG control from form
    this->setDobotPTEValidatorsInUI();
    m_pWebsockets->listenOnPort(1234); //future: dont start it in here
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDobotsBasicButtonsControl()
{
    QSignalMapper* signalMapper  = new QSignalMapper(this);
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

void MainWindow::writeInConsole(QString QStrMsg, LOG TypeOfMsg)
{
    if(QStrMsg.isEmpty()) return;

    if(QStrMsg == "/clear")
    {
        ui->logPTE->clear();
        return;
    }

    QStrMsg = QTime::currentTime().toString("hh:mm:ss") + " " +
            logAsQstr(TypeOfMsg) + QStrMsg;

    ui->logPTE->setPlainText(ui->logPTE->toPlainText() + QStrMsg);
    qInfo() << QStrMsg;

    //prevent big string data
    int nMaximum = 30 * 1000;
    ui->logPTE->setPlainText(ui->logPTE->toPlainText().right(nMaximum));

    //auto scroll
    QScrollBar *scroll_logPTE = ui->logPTE->verticalScrollBar();
    scroll_logPTE->setValue(scroll_logPTE->maximum());
}

///board data slots
void MainWindow::changeWindowTitle()
{
    this->setWindowTitle(gameStatusAsQStr(m_pChess->getGameStatus()));
}

void MainWindow::setBoardDataLabel(QString QStrLabel, BOARD_DATA_LABEL LabelType)
{
    switch(LabelType)
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
    default: qWarning() << "unknown labelType:" << QString::number(LabelType);
    }
}

void MainWindow::showImaginaryBoardInUI(QString QStrBoard)
{
    QString QStrBoardArray[8][8];

    QStringList QStrFENBoardRows = QStrBoard.split(QRegExp("/"));
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
                    if (nColumn > D_8)
                        qWarning() << "nColumn > 8 =" << QString::number(nColumn);
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        QStrBoardArray[nColumn][nRow] = ".";
                        if (nColumn > D_8)
                            qWarning() << "nColumn > 8 =" << QString::number(nColumn);
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qWarning() << "boardRows.size() != 8. size =" << QString::number(QStrFENBoardRows.size());
        for (int i=0; i<=QStrFENBoardRows.size()-1; ++i)
            qInfo() << "QStrFENBoardRows at" << QString::number(i)
                    << "=" << QStrFENBoardRows.at(i);
    }

    QString QStrBoardPTE;
    for (int i=0; i<=7; ++i)
    {
        for (int j=0; j<=7; ++j)
            QStrBoardPTE += QStrBoardArray[i][j] + " ";
        QStrBoardPTE += "\n";
    }
    while (QStrBoardPTE.right(1) == "\n")
        QStrBoardPTE.remove(QStrBoardPTE.length()-1,1);

    ui->chenardBoardPTE->clear();
    ui->chenardBoardPTE->setPlainText(QStrBoardPTE);
}

void MainWindow::showRealBoardInUI()
{
    QString QStrRealBoards;
    for (int i=1; i<=64; ++i)
    {
        if (i % 8 == 1)
        {
            Field* pRemField1 = m_pChess->getBoardRemovedPointer()->getField((i/4)+1);
            Piece* pRemPiece1 = pRemField1->getPieceOnField();
            Field* pRemField2 = m_pChess->getBoardRemovedPointer()->getField((i/4)+2);
            Piece* pRemPiece2 = pRemField2->getPieceOnField();
            QStrRealBoards += (pRemPiece1 == nullptr ? "." : pRemPiece1->getAsFENSign())
                    + " " + (pRemPiece2 == nullptr ? "." : pRemPiece2->getAsFENSign()) + "   ";
        }

        Field* pField = m_pChess->getBoardMainPointer()->getField(i);
        Piece* pPiece = pField->getPieceOnField();
        QStrRealBoards += (pPiece == nullptr ? "." : pPiece->getAsFENSign()) + " ";

        if (i % 8 == 0)
        {
            Field* pRemField1 = m_pChess->getBoardRemovedPointer()->getField((i/4)+15);
            Piece* pRemPiece1 = pRemField1->getPieceOnField();
            Field* pRemField2 = m_pChess->getBoardRemovedPointer()->getField((i/4)+16);
            Piece* pRemPiece2 = pRemField2->getPieceOnField();
            QStrRealBoards += "  " + (pRemPiece1 == nullptr ? "." : pRemPiece1->getAsFENSign())
                    + " " + (pRemPiece2 == nullptr ? "." : pRemPiece2->getAsFENSign()) + "\n";
        }
    }
    while (QStrRealBoards.right(1) == "\n")
        QStrRealBoards.remove(QStrRealBoards.length()-1,1);

    ui->realBoardPTE->clear();
    ui->realBoardPTE->setPlainText(QStrRealBoards);
}

void MainWindow::clearBoardInUI()
{
    ui->chenardBoardPTE->clear();
}

void MainWindow::showLegalMovesInUI(QStringList legalMoves)
{
    QString legal = "";
    if (!legalMoves.isEmpty()) legal = legalMoves.join(" ");
    ui->legalPTE->clear();
    ui->legalPTE->setPlainText(legal);
}

void MainWindow::showHistoryMovesInUI(QStringList historyMoves)
{
    QString history;
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


///dobot slots
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

void MainWindow::onJOGCtrlBtnPressed(int nID)
{
    m_pPieceController->clearLastPos();

    JOGCmd jogCmd;
    jogCmd.isJoint = ui->teachMode->currentIndex() == 0;
    jogCmd.cmd = nID + 1;
    SetJOGCmd(&jogCmd, false, NULL);
}

void MainWindow::onJOGCtrlBtnReleased()
{
    JOGCmd jogCmd;
    jogCmd.isJoint = ui->teachMode->currentIndex() == 0;
    jogCmd.cmd = JogIdle;
    SetJOGCmd(&jogCmd, false, NULL);
}

void MainWindow::setDobotPTEValidatorsInUI()
{
    QRegExp regExp("0|[-+]?[1-9][0-9]{0,2}[.][0-9]{1,3}");
    QValidator *validator = new QRegExpValidator(regExp);

    ui->xPTPEdit->setValidator(validator);
    ui->yPTPEdit->setValidator(validator);
    ui->zPTPEdit->setValidator(validator);
}

void MainWindow::setJointLabelText(QString QStrJointLabelText, short sJoint)
{
    if (sJoint == 1)  ui->joint1Label->setText(QStrJointLabelText);
    else if (sJoint == 2)  ui->joint2Label->setText(QStrJointLabelText);
    else if (sJoint == 3)  ui->joint3Label->setText(QStrJointLabelText);
    else if (sJoint == 4)  ui->joint4Label->setText(QStrJointLabelText);
}

void MainWindow::setAxisLabelText(QString QStrAxisLabelText, char chAxis)
{
    if (chAxis == 'x') ui->xLabel->setText(QStrAxisLabelText);
    else if (chAxis == 'y') ui->yLabel->setText(QStrAxisLabelText);
    else if (chAxis == 'z') ui->zLabel->setText(QStrAxisLabelText);
    else if (chAxis == 'r') ui->rLabel->setText(QStrAxisLabelText);
}

void MainWindow::setDobotButtonsStates(bool bDobotButtonsStates)
{
    if (!bDobotButtonsStates)
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
        ui->sendPointBtn->setEnabled(false);
        ui->xPTPEdit->setEnabled(false);
        ui->yPTPEdit->setEnabled(false);
        ui->zPTPEdit->setEnabled(false);
        ui->servoGripperEdit->setEnabled(false);
        ui->calibrateBtn->setEnabled(false);
        ui->upBtn->setEnabled(false);
        ui->downBtn->setEnabled(false);
        ui->sendTcpBtn->setEnabled(false);
        ui->sendTcpLineEdit->setEnabled(false);
        ui->emulatePlayerMsgLineEdit->setEnabled(false);
        ui->sendSimulatedMsgBtn->setEnabled(false);
        ui->openGripperBtn->setEnabled(false);
        ui->closeGripperBtn->setEnabled(false);
        ui->homeBtn->setEnabled(false);
        ui->middleAboveBtn->setEnabled(false);
        ui->retreatLeftBtn->setEnabled(false);
        ui->retreatRightBtn->setEnabled(false);
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
        ui->sendPointBtn->setEnabled(true);
        ui->xPTPEdit->setEnabled(true);
        ui->yPTPEdit->setEnabled(true);
        ui->zPTPEdit->setEnabled(true);
        ui->servoGripperEdit->setEnabled(true);
        ui->calibrateBtn->setEnabled(true);
        ui->upBtn->setEnabled(true);
        ui->downBtn->setEnabled(true);
        ui->sendTcpLineEdit->setEnabled(true);
        ui->emulatePlayerMsgLineEdit->setEnabled(true);
        ui->openGripperBtn->setEnabled(true);
        ui->closeGripperBtn->setEnabled(true);
        ui->homeBtn->setEnabled(true);
        ui->middleAboveBtn->setEnabled(true);
        ui->retreatLeftBtn->setEnabled(true);
        ui->retreatRightBtn->setEnabled(true);
    }
}

void MainWindow::setDeviceLabels(QString QStrDeviceSN, QString QStrDeviceName,
                                 QString QStrDeviceVersion)
{
    ui->deviceSNLabel->setText(QStrDeviceSN);
    ui->DeviceNameLabel->setText(QStrDeviceName);
    ui->DeviceInfoLabel->setText(QStrDeviceVersion);
}

void MainWindow::showDobotErrorMsgBox()
{
    QMessageBox::information(this, tr("error"), tr("Connect dobot error"), QMessageBox::Ok);
    return;
}

void MainWindow::setQueueLabels(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                                int nCoreIdLeft, uint64_t un64CoreNextId)
{
    if (unSpace == 0) unSpace = 1;  //future: it's only for temporary block...
    //...warning about unused parameter, till I will not check this function
    //ui->DobotQueuedCmdLeftSpaceLabel->
    //setText(QString::number(nSpace)); //future: make it by myself, or wait for update
    ui->DobotQueuedIndexLabel->setText(QString::number(un64DobotId));
    ui->CoreMaxQueuedIndexLabel->setText(QString::number(un64CoreMaxId));
    ui->CoreIndexAmountlabel->setText(QString::number(nCoreIdLeft));
    ui->CoreNextIdLabel->setText(QString::number(un64CoreNextId));
}

void MainWindow::showQueuedArmCmdsOnCore(QString QStrList)
{
    ui->queuedOnCore->clear();
    ui->queuedOnCore->setPlainText(QStrList);
}

void MainWindow::showSentArmCmdsToDobot(QString QStrList)
{
    ui->queuedOnDobot->clear();
    ui->queuedOnDobot->setPlainText(QStrList);
}


///clients slots
void MainWindow::showClientsListInUI(QList<Client> list)
{
    QString QStrClientsList;
    Client client;

    for(int i=0; i<list.count(); ++i)
    {
       client = list.at(i);
       QStrClientsList += QString::number(i+1) + ". ";

       QString QStrName = "-";
       if (client.sqlID() > 0)
           QStrName = client.name();
       QStrClientsList += QStrName;

       if (client.type() != PT_NONE)
       {
           QString QStrPlayerType = playerTypeAsQStr(client.type());
           QString QStrStartState = client.isStartClickedByPlayer() ? "1" : "0";
           QStrClientsList += ", plr: " + QStrPlayerType + ", st:" + QStrStartState;
       }
       else if (client.queue() != 0)
           QStrClientsList += ", q:" + QString::number(client.queue());

       QStrClientsList += ", sqlID:" + QString::number(client.sqlID());
       QStrClientsList += ", ID:" + QString::number(client.ID()) + "\n";
    }
    ui->clientsPTE->clear();
    ui->clientsPTE->setPlainText(QStrClientsList);
}

void MainWindow::showQueuedClientsListInUI(QString QStrList)
{
    qInfo();
    QStringList list = QStrList.split(" ");
    uint unNrInQueue = 0;
    QString QStrQueuedPTE;
    foreach (QString QStrQueuedClient, list)
    {
        ++unNrInQueue;
        QStrQueuedPTE += QString::number(unNrInQueue) + ". " + QStrQueuedClient + "\n";
    }
    QStrQueuedPTE = QStrQueuedPTE.trimmed();
    ui->queuedClientsPTE->clear();
    ui->queuedClientsPTE->setPlainText(QStrQueuedPTE);
    qInfo();
}

///GUI slots
void MainWindow::on_openGripperBtn_clicked()
{
    m_pDobot->addArmMoveToQueue(DM_OPEN);
}

void MainWindow::on_closeGripperBtn_clicked()
{
    m_pDobot->addArmMoveToQueue(DM_CLOSE);
    m_pDobot->addArmMoveToQueue(DM_WAIT);
}

void MainWindow::on_startGmPosBtn_clicked()
{
    Point3D labelPoint(ui->xLabel->text().toInt(), ui->yLabel->text().toInt(),
                       ui->zLabel->text().toInt());
    //todo: test condition
    if (Point3D::isPointCloseToAnother(labelPoint, m_pDobot->getHomePos()))
    {
        m_pPieceController->clearLastPos();

        this->writeInConsole("Placing arm above the chessboard.\n", LOG_DOBOT);
        m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getHomePos());
        m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().cournerBelow);
        m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().cournerAbove);
        m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().middleAbove);
    }
    else
        qWarning() << "Dobot not in home positions";
}

void MainWindow::on_startDtPosBtn_clicked()
{
    m_pPieceController->clearLastPos();

    this->writeInConsole("Returning safely to the DM_HOME positions.\n", LOG_DOBOT);
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().middleAbove);
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().cournerAbove);
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().cournerBelow);
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getHomePos());
}

void MainWindow::on_sendTcpBtn_clicked()
{
    if (!ui->sendTcpLineEdit->text().isEmpty())
    {
        m_pTCPMsgs->queueCmd(ui->sendTcpLineEdit->text());
        ui->sendTcpLineEdit->clear();
    }
}

void MainWindow::on_sendTcpLineEdit_textChanged(const QString& QStrTextChanged)
{
    if (QStrTextChanged != NULL) ui->sendTcpBtn->setEnabled(true);
    else ui->sendTcpBtn->setEnabled(false);
}

void MainWindow::on_homeBtn_clicked()
{
    m_pPieceController->clearLastPos();
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getHomePos());
}

void MainWindow::on_middleAboveBtn_clicked()
{
    m_pPieceController->clearLastPos();
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().middleAbove);
}

void MainWindow::on_sendPointBtn_clicked()
{
    m_pChess->clearLastSavedMadeMove();

    bool bConversionXOk, bConversionYOk, bConversionZOk;

    Point3D point;
    float xPTE = ui->xPTPEdit->text().toFloat(&bConversionXOk);
    float yPTE = ui->yPTPEdit->text().toFloat(&bConversionYOk);
    float zPTE = ui->zPTPEdit->text().toFloat(&bConversionZOk);

    point.x = bConversionXOk ? xPTE : m_pDobot->getLastGivenPoint().x;
    point.y = bConversionYOk ? yPTE : m_pDobot->getLastGivenPoint().y;
    point.z = bConversionZOk ? zPTE : m_pDobot->getLastGivenPoint().z;

    m_pPieceController->clearLastPos();
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, point);

    //servo
    bool bConversionServoOk;
    float fServoDutyCycle = ui->servoGripperEdit->text().toFloat(&bConversionServoOk);
    if (bConversionServoOk)
        m_pDobot->getServoPointer()->changeGripperAngle(fServoDutyCycle);
}

void MainWindow::on_xPTPEdit_textChanged(const QString& QStrTextChanged)
{
    if (QStrTextChanged != NULL || ui->yPTPEdit->text() != NULL || ui->zPTPEdit->text() != NULL)
        ui->sendPointBtn->setEnabled(true);
    else ui->sendPointBtn->setEnabled(false);
}

void MainWindow::on_yPTPEdit_textChanged(const QString& QStrTextChanged)
{
    if (ui->xPTPEdit->text() != NULL || QStrTextChanged != NULL || ui->zPTPEdit->text() != NULL)
        ui->sendPointBtn->setEnabled(true);
    else ui->sendPointBtn->setEnabled(false);
}

void MainWindow::on_zPTPEdit_textChanged(const QString& QStrTextChanged)
{
    if (ui->xPTPEdit->text() != NULL || ui->yPTPEdit->text() != NULL  || QStrTextChanged != NULL)
        ui->sendPointBtn->setEnabled(true);
    else ui->sendPointBtn->setEnabled(false);
}

void MainWindow::on_retreatLeftBtn_clicked()
{
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().retreatLeft);
}

void MainWindow::on_retreatRightBtn_clicked()
{
    m_pDobot->addArmMoveToQueue(DM_TO_POINT, m_pDobot->getIntermediatePoints().retreatRight);
}

void MainWindow::on_emulatePlayerMsgLineEdit_textChanged(const QString& QStrTextChanged)
{
    if (QStrTextChanged != NULL) ui->sendSimulatedMsgBtn->setEnabled(true);
    else ui->sendSimulatedMsgBtn->setEnabled(false);
}

void MainWindow::on_sendSimulatedMsgBtn_clicked()
{
    if (!ui->emulatePlayerMsgLineEdit->text().isEmpty())
    {
        QString QStrServiceMove = ui->emulatePlayerMsgLineEdit->text();

        if (QStrServiceMove.length() == 2)
        {
            QStrServiceMove += "a1"; //walkaround
            if (PosFromTo::isMoveInProperFormat(QStrServiceMove))
            {
                PosFromTo fromTo = PosFromTo::fromQStr(QStrServiceMove);
                Field* pFieldFrom = m_pBoardMain->getField(fromTo.from);
                m_pPieceController->movePieceWithManipulator(m_pBoardMain, pFieldFrom);
            }
        }
        else if (QStrServiceMove.length() == 3 && QStrServiceMove.right(1) == "r")
        {
            QStrServiceMove = QStrServiceMove.left(2) + "a1"; //walkaround
            if (PosFromTo::isMoveInProperFormat(QStrServiceMove))
            {
                PosFromTo fromTo = PosFromTo::fromQStr(QStrServiceMove);
                Field* pFieldFrom = m_pBoardRemoved->getField(fromTo.from);
                if ((int)fromTo.from.Digit > 4)
                {
                    qWarning() << "fromTo.from.Digit > 4";
                    return;
                }
                m_pPieceController->movePieceWithManipulator(m_pBoardRemoved, pFieldFrom);
            }
        }
        else if (QStrServiceMove.length() == 4)
        {
            if (PosFromTo::isMoveInProperFormat(QStrServiceMove))
            {
                PosFromTo fromTo = PosFromTo::fromQStr(QStrServiceMove);
                Field* pFieldFrom = m_pBoardMain->getField(fromTo.from);
                Field* pFieldTo = m_pBoardMain->getField(fromTo.to);
                m_pPieceController->movePieceWithManipulator(m_pBoardMain, pFieldFrom, VM_GRAB);
                m_pPieceController->movePieceWithManipulator(m_pBoardMain, pFieldTo, VM_PUT);
            }

        }
        else qWarning() << "QStrServiceMove msg =" << QStrServiceMove;
    }

    ui->emulatePlayerMsgLineEdit->clear();
}

void MainWindow::on_calibrateBtn_clicked()
{
    if (ui->xLabel->text().toInt() == (int)m_pDobot->getHomePos().x &&
            ui->yLabel->text().toInt() == (int)m_pDobot->getHomePos().y &&
            ui->zLabel->text().toInt() == (int)m_pDobot->getHomePos().z)
    {
        m_pPieceController->clearLastPos();
        m_pDobot->addArmMoveToQueue(DM_CALIBRATE);
    }
    else
        qWarning() << "Dobot not in home positions";
}

void MainWindow::on_upBtn_clicked()
{
    if (m_pPieceController->isMoveSet())
    {
        double dZAxisVal = m_pBoardMain->getField(m_pPieceController->getLastPos())
                ->getLocation3D().z + m_pBoardMain->dMaxPieceHeight;
        qInfo() << "dZAxisVal =" << QString::number(dZAxisVal);
        m_pDobot->moveArmUpOrDown(DM_DOWN, dZAxisVal);
    }
    else
        qWarning() << "move isn't set";
}

void MainWindow::on_downBtn_clicked()
{
    if (m_pPieceController->isMoveSet())
    {
        double dZAxisVal = m_pBoardMain->getField(m_pPieceController->getLastPos())
                ->getLocation3D().z;
        qInfo() << "dZAxisVal =" << QString::number(dZAxisVal);
        m_pDobot->moveArmUpOrDown(DM_DOWN, dZAxisVal);
    }
    else
        qWarning() << "move isn't set";
}

void MainWindow::on_resetDobotIndexBtn_clicked()
{
    if (isArmReceivedCorrectCmd(SetQueuedCmdClear(), SHOW_ERRORS))
        this->writeInConsole("Cleared Dobot Queued Cmds.\n", LOG_DOBOT);
}

