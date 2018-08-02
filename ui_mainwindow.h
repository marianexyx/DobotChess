/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_9;
    QLabel *gameStatusLbl;
    QLabel *whiteNameLbl;
    QLabel *label_13;
    QLabel *label_25;
    QLabel *labelx;
    QLabel *label_24;
    QLabel *turnLbl;
    QLabel *blackNameLbl;
    QLabel *socketsLbl;
    QLabel *label_23;
    QLabel *label_28;
    QLabel *movesLbl;
    QLabel *label_26;
    QLabel *castlingsLbl;
    QLabel *label_27;
    QLabel *enpassantLbl;
    QLabel *label_30;
    QLabel *whiteTimeLbl;
    QLabel *label_31;
    QLabel *blackTimeLbl;
    QLabel *label_29;
    QLabel *queuedPlayersLbl;
    QLabel *label_33;
    QLabel *queueTimeLbl;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_14;
    QLabel *label_35;
    QLabel *label_34;
    QPlainTextEdit *chenardBoardPTE;
    QPlainTextEdit *realBoardPTE;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_19;
    QLabel *label_37;
    QLabel *label_39;
    QPlainTextEdit *historyPTE;
    QLabel *label_36;
    QPlainTextEdit *queuedOnCore;
    QPlainTextEdit *queuedOnDobot;
    QPlainTextEdit *legalPTE;
    QLabel *label_38;
    QPushButton *resetAllDataBtn;
    QGroupBox *log;
    QGridLayout *gridLayout_18;
    QPlainTextEdit *logPTE;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_3;
    QGroupBox *queue;
    QGridLayout *gridLayout_5;
    QLabel *CoreIndexAmountlabel;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_21;
    QLabel *DobotQueuedIndexLabel;
    QLabel *DobotQueuedCmdLeftSpaceLabel;
    QLabel *label_16;
    QLabel *label_22;
    QLabel *CoreMaxQueuedIndexLabel;
    QLabel *CoreNextIdLabel;
    QPushButton *resetDobotIndexBtn;
    QGroupBox *dobotServiceMove;
    QGridLayout *gridLayout_7;
    QLineEdit *xPTPEdit;
    QLabel *label_17;
    QLineEdit *zPTPEdit;
    QLabel *label_19;
    QLabel *label_18;
    QLineEdit *yPTPEdit;
    QLineEdit *servoGripperEdit;
    QLabel *label_12;
    QPushButton *sendPointBtn;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QLineEdit *sendTcpLineEdit;
    QPushButton *sendSimulatedMsgBtn;
    QLineEdit *emulatePlayerMsgLineEdit;
    QPushButton *sendTcpBtn;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_22;
    QPlainTextEdit *plainTextEdit_2;
    QPlainTextEdit *clientsPTE;
    QLabel *label_20;
    QLabel *label_32;
    QGroupBox *JOGgroupBox;
    QGridLayout *gridLayout_10;
    QPushButton *baseAngleSubBtn;
    QPushButton *longArmAddBtn;
    QPushButton *rHeadSubBtn;
    QPushButton *shortArmAddBtn;
    QPushButton *shortArmSubBtn;
    QPushButton *rHeadAddBtn;
    QPushButton *baseAngleAddBtn;
    QComboBox *teachMode;
    QPushButton *longArmSubBtn;
    QGroupBox *dobotMovements;
    QGridLayout *gridLayout_12;
    QPushButton *startGmPosBtn;
    QPushButton *upBtn;
    QPushButton *homeBtn;
    QPushButton *calibrateBtn;
    QPushButton *downBtn;
    QPushButton *startDtPosBtn;
    QPushButton *openGripperBtn;
    QPushButton *closeGripperBtn;
    QPushButton *retreatLeftBtn;
    QPushButton *middleAboveBtn;
    QPushButton *retreatRightBtn;
    QGroupBox *dobotInfo;
    QGridLayout *gridLayout_11;
    QLabel *label_9;
    QLabel *deviceSNLabel;
    QLabel *label_10;
    QLabel *DeviceNameLabel;
    QLabel *label_11;
    QLabel *DeviceInfoLabel;
    QPushButton *connectBtn;
    QGroupBox *dobotPositions;
    QGridLayout *gridLayout;
    QLabel *joint1Label;
    QLabel *label_6;
    QLabel *xLabel;
    QLabel *yLabel;
    QLabel *joint3Label;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *zLabel;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *rLabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *joint4Label;
    QLabel *label_8;
    QLabel *joint2Label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1330, 840);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMaximumSize(QSize(400, 16777215));
        gridLayout_8 = new QGridLayout(groupBox);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_9 = new QGridLayout(groupBox_2);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gameStatusLbl = new QLabel(groupBox_2);
        gameStatusLbl->setObjectName(QStringLiteral("gameStatusLbl"));

        gridLayout_9->addWidget(gameStatusLbl, 4, 1, 1, 1);

        whiteNameLbl = new QLabel(groupBox_2);
        whiteNameLbl->setObjectName(QStringLiteral("whiteNameLbl"));

        gridLayout_9->addWidget(whiteNameLbl, 1, 1, 1, 1);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_9->addWidget(label_13, 1, 0, 1, 1);

        label_25 = new QLabel(groupBox_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout_9->addWidget(label_25, 4, 0, 1, 1);

        labelx = new QLabel(groupBox_2);
        labelx->setObjectName(QStringLiteral("labelx"));

        gridLayout_9->addWidget(labelx, 0, 0, 1, 1);

        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_9->addWidget(label_24, 3, 0, 1, 1);

        turnLbl = new QLabel(groupBox_2);
        turnLbl->setObjectName(QStringLiteral("turnLbl"));

        gridLayout_9->addWidget(turnLbl, 3, 1, 1, 1);

        blackNameLbl = new QLabel(groupBox_2);
        blackNameLbl->setObjectName(QStringLiteral("blackNameLbl"));

        gridLayout_9->addWidget(blackNameLbl, 2, 1, 1, 1);

        socketsLbl = new QLabel(groupBox_2);
        socketsLbl->setObjectName(QStringLiteral("socketsLbl"));

        gridLayout_9->addWidget(socketsLbl, 0, 1, 1, 1);

        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_9->addWidget(label_23, 2, 0, 1, 1);

        label_28 = new QLabel(groupBox_2);
        label_28->setObjectName(QStringLiteral("label_28"));

        gridLayout_9->addWidget(label_28, 5, 0, 1, 1);

        movesLbl = new QLabel(groupBox_2);
        movesLbl->setObjectName(QStringLiteral("movesLbl"));

        gridLayout_9->addWidget(movesLbl, 5, 1, 1, 1);

        label_26 = new QLabel(groupBox_2);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout_9->addWidget(label_26, 0, 2, 1, 1);

        castlingsLbl = new QLabel(groupBox_2);
        castlingsLbl->setObjectName(QStringLiteral("castlingsLbl"));

        gridLayout_9->addWidget(castlingsLbl, 0, 3, 1, 1);

        label_27 = new QLabel(groupBox_2);
        label_27->setObjectName(QStringLiteral("label_27"));

        gridLayout_9->addWidget(label_27, 1, 2, 1, 1);

        enpassantLbl = new QLabel(groupBox_2);
        enpassantLbl->setObjectName(QStringLiteral("enpassantLbl"));

        gridLayout_9->addWidget(enpassantLbl, 1, 3, 1, 1);

        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName(QStringLiteral("label_30"));

        gridLayout_9->addWidget(label_30, 2, 2, 1, 1);

        whiteTimeLbl = new QLabel(groupBox_2);
        whiteTimeLbl->setObjectName(QStringLiteral("whiteTimeLbl"));

        gridLayout_9->addWidget(whiteTimeLbl, 2, 3, 1, 1);

        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout_9->addWidget(label_31, 3, 2, 1, 1);

        blackTimeLbl = new QLabel(groupBox_2);
        blackTimeLbl->setObjectName(QStringLiteral("blackTimeLbl"));

        gridLayout_9->addWidget(blackTimeLbl, 3, 3, 1, 1);

        label_29 = new QLabel(groupBox_2);
        label_29->setObjectName(QStringLiteral("label_29"));

        gridLayout_9->addWidget(label_29, 4, 2, 1, 1);

        queuedPlayersLbl = new QLabel(groupBox_2);
        queuedPlayersLbl->setObjectName(QStringLiteral("queuedPlayersLbl"));

        gridLayout_9->addWidget(queuedPlayersLbl, 4, 3, 1, 1);

        label_33 = new QLabel(groupBox_2);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout_9->addWidget(label_33, 5, 2, 1, 1);

        queueTimeLbl = new QLabel(groupBox_2);
        queueTimeLbl->setObjectName(QStringLiteral("queueTimeLbl"));

        gridLayout_9->addWidget(queueTimeLbl, 5, 3, 1, 1);

        labelx->raise();
        socketsLbl->raise();
        label_13->raise();
        whiteNameLbl->raise();
        label_23->raise();
        blackNameLbl->raise();
        label_24->raise();
        turnLbl->raise();
        label_25->raise();
        gameStatusLbl->raise();
        label_28->raise();
        movesLbl->raise();
        label_26->raise();
        castlingsLbl->raise();
        label_27->raise();
        enpassantLbl->raise();
        label_30->raise();
        whiteTimeLbl->raise();
        label_31->raise();
        blackTimeLbl->raise();
        label_29->raise();
        queuedPlayersLbl->raise();
        label_33->raise();
        queueTimeLbl->raise();

        gridLayout_8->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_14 = new QGridLayout(groupBox_3);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        label_35 = new QLabel(groupBox_3);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout_14->addWidget(label_35, 0, 1, 1, 1);

        label_34 = new QLabel(groupBox_3);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout_14->addWidget(label_34, 0, 0, 1, 1);

        chenardBoardPTE = new QPlainTextEdit(groupBox_3);
        chenardBoardPTE->setObjectName(QStringLiteral("chenardBoardPTE"));
        chenardBoardPTE->setEnabled(true);
        chenardBoardPTE->setMinimumSize(QSize(125, 125));
        chenardBoardPTE->setMaximumSize(QSize(125, 125));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        chenardBoardPTE->setFont(font);

        gridLayout_14->addWidget(chenardBoardPTE, 1, 1, 1, 1);

        realBoardPTE = new QPlainTextEdit(groupBox_3);
        realBoardPTE->setObjectName(QStringLiteral("realBoardPTE"));
        realBoardPTE->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(realBoardPTE->sizePolicy().hasHeightForWidth());
        realBoardPTE->setSizePolicy(sizePolicy1);
        realBoardPTE->setMinimumSize(QSize(0, 125));
        realBoardPTE->setMaximumSize(QSize(16777215, 125));
        QFont font1;
        font1.setFamily(QStringLiteral("Courier New"));
        font1.setPointSize(8);
        realBoardPTE->setFont(font1);

        gridLayout_14->addWidget(realBoardPTE, 1, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_3, 1, 0, 1, 1);

        groupBox_9 = new QGroupBox(groupBox);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        gridLayout_19 = new QGridLayout(groupBox_9);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        label_37 = new QLabel(groupBox_9);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout_19->addWidget(label_37, 0, 2, 1, 1);

        label_39 = new QLabel(groupBox_9);
        label_39->setObjectName(QStringLiteral("label_39"));

        gridLayout_19->addWidget(label_39, 2, 2, 1, 1);

        historyPTE = new QPlainTextEdit(groupBox_9);
        historyPTE->setObjectName(QStringLiteral("historyPTE"));
        historyPTE->setEnabled(true);
        historyPTE->setMaximumSize(QSize(16777215, 120));
        historyPTE->setFont(font);

        gridLayout_19->addWidget(historyPTE, 3, 1, 1, 1);

        label_36 = new QLabel(groupBox_9);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout_19->addWidget(label_36, 0, 1, 1, 1);

        queuedOnCore = new QPlainTextEdit(groupBox_9);
        queuedOnCore->setObjectName(QStringLiteral("queuedOnCore"));
        queuedOnCore->setEnabled(true);
        QFont font2;
        font2.setPointSize(7);
        queuedOnCore->setFont(font2);

        gridLayout_19->addWidget(queuedOnCore, 1, 2, 1, 1);

        queuedOnDobot = new QPlainTextEdit(groupBox_9);
        queuedOnDobot->setObjectName(QStringLiteral("queuedOnDobot"));
        queuedOnDobot->setEnabled(true);
        queuedOnDobot->setFont(font2);

        gridLayout_19->addWidget(queuedOnDobot, 1, 1, 1, 1);

        legalPTE = new QPlainTextEdit(groupBox_9);
        legalPTE->setObjectName(QStringLiteral("legalPTE"));
        legalPTE->setEnabled(true);
        legalPTE->setMinimumSize(QSize(150, 120));
        legalPTE->setMaximumSize(QSize(16777215, 120));
        legalPTE->setFont(font);

        gridLayout_19->addWidget(legalPTE, 3, 2, 1, 1);

        label_38 = new QLabel(groupBox_9);
        label_38->setObjectName(QStringLiteral("label_38"));

        gridLayout_19->addWidget(label_38, 2, 1, 1, 1);


        gridLayout_8->addWidget(groupBox_9, 2, 0, 1, 1);

        resetAllDataBtn = new QPushButton(groupBox);
        resetAllDataBtn->setObjectName(QStringLiteral("resetAllDataBtn"));
        resetAllDataBtn->setEnabled(false);

        gridLayout_8->addWidget(resetAllDataBtn, 3, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 1, 1, 2);

        log = new QGroupBox(centralWidget);
        log->setObjectName(QStringLiteral("log"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(log->sizePolicy().hasHeightForWidth());
        log->setSizePolicy(sizePolicy2);
        log->setMinimumSize(QSize(450, 0));
        log->setBaseSize(QSize(500, 0));
        log->setLayoutDirection(Qt::LeftToRight);
        gridLayout_18 = new QGridLayout(log);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        logPTE = new QPlainTextEdit(log);
        logPTE->setObjectName(QStringLiteral("logPTE"));
        logPTE->setEnabled(true);
        logPTE->setFont(font2);
        logPTE->setReadOnly(true);
        logPTE->setOverwriteMode(false);

        gridLayout_18->addWidget(logPTE, 0, 0, 1, 1);


        gridLayout_4->addWidget(log, 0, 4, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        sizePolicy.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy);
        groupBox_5->setMinimumSize(QSize(0, 0));
        groupBox_5->setMaximumSize(QSize(450, 16777215));
        gridLayout_3 = new QGridLayout(groupBox_5);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        queue = new QGroupBox(groupBox_5);
        queue->setObjectName(QStringLiteral("queue"));
        gridLayout_5 = new QGridLayout(queue);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        CoreIndexAmountlabel = new QLabel(queue);
        CoreIndexAmountlabel->setObjectName(QStringLiteral("CoreIndexAmountlabel"));

        gridLayout_5->addWidget(CoreIndexAmountlabel, 3, 1, 1, 1);

        label_14 = new QLabel(queue);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_5->addWidget(label_14, 0, 0, 1, 1);

        label_15 = new QLabel(queue);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_5->addWidget(label_15, 1, 0, 1, 1);

        label_21 = new QLabel(queue);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_5->addWidget(label_21, 2, 0, 1, 1);

        DobotQueuedIndexLabel = new QLabel(queue);
        DobotQueuedIndexLabel->setObjectName(QStringLiteral("DobotQueuedIndexLabel"));

        gridLayout_5->addWidget(DobotQueuedIndexLabel, 0, 1, 1, 1);

        DobotQueuedCmdLeftSpaceLabel = new QLabel(queue);
        DobotQueuedCmdLeftSpaceLabel->setObjectName(QStringLiteral("DobotQueuedCmdLeftSpaceLabel"));

        gridLayout_5->addWidget(DobotQueuedCmdLeftSpaceLabel, 2, 1, 1, 1);

        label_16 = new QLabel(queue);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_5->addWidget(label_16, 3, 0, 1, 1);

        label_22 = new QLabel(queue);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_5->addWidget(label_22, 4, 0, 1, 1);

        CoreMaxQueuedIndexLabel = new QLabel(queue);
        CoreMaxQueuedIndexLabel->setObjectName(QStringLiteral("CoreMaxQueuedIndexLabel"));

        gridLayout_5->addWidget(CoreMaxQueuedIndexLabel, 1, 1, 1, 1);

        CoreNextIdLabel = new QLabel(queue);
        CoreNextIdLabel->setObjectName(QStringLiteral("CoreNextIdLabel"));

        gridLayout_5->addWidget(CoreNextIdLabel, 4, 1, 1, 1);

        resetDobotIndexBtn = new QPushButton(queue);
        resetDobotIndexBtn->setObjectName(QStringLiteral("resetDobotIndexBtn"));
        resetDobotIndexBtn->setEnabled(false);

        gridLayout_5->addWidget(resetDobotIndexBtn, 5, 0, 1, 1);


        gridLayout_3->addWidget(queue, 3, 0, 1, 1);

        dobotServiceMove = new QGroupBox(groupBox_5);
        dobotServiceMove->setObjectName(QStringLiteral("dobotServiceMove"));
        gridLayout_7 = new QGridLayout(dobotServiceMove);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        xPTPEdit = new QLineEdit(dobotServiceMove);
        xPTPEdit->setObjectName(QStringLiteral("xPTPEdit"));
        xPTPEdit->setEnabled(false);

        gridLayout_7->addWidget(xPTPEdit, 0, 1, 1, 1);

        label_17 = new QLabel(dobotServiceMove);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_7->addWidget(label_17, 0, 0, 1, 1);

        zPTPEdit = new QLineEdit(dobotServiceMove);
        zPTPEdit->setObjectName(QStringLiteral("zPTPEdit"));
        zPTPEdit->setEnabled(false);

        gridLayout_7->addWidget(zPTPEdit, 2, 1, 1, 1);

        label_19 = new QLabel(dobotServiceMove);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_7->addWidget(label_19, 2, 0, 1, 1);

        label_18 = new QLabel(dobotServiceMove);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_7->addWidget(label_18, 1, 0, 1, 1);

        yPTPEdit = new QLineEdit(dobotServiceMove);
        yPTPEdit->setObjectName(QStringLiteral("yPTPEdit"));
        yPTPEdit->setEnabled(false);

        gridLayout_7->addWidget(yPTPEdit, 1, 1, 1, 1);

        servoGripperEdit = new QLineEdit(dobotServiceMove);
        servoGripperEdit->setObjectName(QStringLiteral("servoGripperEdit"));
        servoGripperEdit->setEnabled(false);

        gridLayout_7->addWidget(servoGripperEdit, 3, 1, 1, 1);

        label_12 = new QLabel(dobotServiceMove);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_7->addWidget(label_12, 3, 0, 1, 1);

        sendPointBtn = new QPushButton(dobotServiceMove);
        sendPointBtn->setObjectName(QStringLiteral("sendPointBtn"));
        sendPointBtn->setEnabled(false);

        gridLayout_7->addWidget(sendPointBtn, 4, 1, 1, 1);


        gridLayout_3->addWidget(dobotServiceMove, 3, 1, 1, 1);

        groupBox_6 = new QGroupBox(groupBox_5);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        sendTcpLineEdit = new QLineEdit(groupBox_6);
        sendTcpLineEdit->setObjectName(QStringLiteral("sendTcpLineEdit"));
        sendTcpLineEdit->setEnabled(false);

        gridLayout_6->addWidget(sendTcpLineEdit, 3, 0, 1, 3);

        sendSimulatedMsgBtn = new QPushButton(groupBox_6);
        sendSimulatedMsgBtn->setObjectName(QStringLiteral("sendSimulatedMsgBtn"));
        sendSimulatedMsgBtn->setEnabled(false);
        sendSimulatedMsgBtn->setAutoDefault(true);

        gridLayout_6->addWidget(sendSimulatedMsgBtn, 0, 3, 1, 1);

        emulatePlayerMsgLineEdit = new QLineEdit(groupBox_6);
        emulatePlayerMsgLineEdit->setObjectName(QStringLiteral("emulatePlayerMsgLineEdit"));
        emulatePlayerMsgLineEdit->setEnabled(false);

        gridLayout_6->addWidget(emulatePlayerMsgLineEdit, 0, 0, 1, 3);

        sendTcpBtn = new QPushButton(groupBox_6);
        sendTcpBtn->setObjectName(QStringLiteral("sendTcpBtn"));
        sendTcpBtn->setEnabled(false);

        gridLayout_6->addWidget(sendTcpBtn, 3, 3, 1, 1);


        gridLayout_3->addWidget(groupBox_6, 2, 3, 1, 1);

        groupBox_12 = new QGroupBox(groupBox_5);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setMinimumSize(QSize(0, 300));
        groupBox_12->setMaximumSize(QSize(16777215, 300));
        gridLayout_22 = new QGridLayout(groupBox_12);
        gridLayout_22->setSpacing(6);
        gridLayout_22->setContentsMargins(11, 11, 11, 11);
        gridLayout_22->setObjectName(QStringLiteral("gridLayout_22"));
        plainTextEdit_2 = new QPlainTextEdit(groupBox_12);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));
        plainTextEdit_2->setEnabled(true);
        plainTextEdit_2->setFont(font2);

        gridLayout_22->addWidget(plainTextEdit_2, 1, 1, 1, 1);

        clientsPTE = new QPlainTextEdit(groupBox_12);
        clientsPTE->setObjectName(QStringLiteral("clientsPTE"));
        clientsPTE->setEnabled(true);
        clientsPTE->setMinimumSize(QSize(270, 0));
        clientsPTE->setMaximumSize(QSize(16777215, 16777215));
        clientsPTE->setFont(font2);

        gridLayout_22->addWidget(clientsPTE, 1, 0, 1, 1);

        label_20 = new QLabel(groupBox_12);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_22->addWidget(label_20, 0, 0, 1, 1);

        label_32 = new QLabel(groupBox_12);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_22->addWidget(label_32, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_12, 0, 0, 1, 4);

        JOGgroupBox = new QGroupBox(groupBox_5);
        JOGgroupBox->setObjectName(QStringLiteral("JOGgroupBox"));
        gridLayout_10 = new QGridLayout(JOGgroupBox);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        baseAngleSubBtn = new QPushButton(JOGgroupBox);
        baseAngleSubBtn->setObjectName(QStringLiteral("baseAngleSubBtn"));
        baseAngleSubBtn->setEnabled(false);

        gridLayout_10->addWidget(baseAngleSubBtn, 3, 1, 1, 1);

        longArmAddBtn = new QPushButton(JOGgroupBox);
        longArmAddBtn->setObjectName(QStringLiteral("longArmAddBtn"));
        longArmAddBtn->setEnabled(false);

        gridLayout_10->addWidget(longArmAddBtn, 4, 0, 1, 1);

        rHeadSubBtn = new QPushButton(JOGgroupBox);
        rHeadSubBtn->setObjectName(QStringLiteral("rHeadSubBtn"));
        rHeadSubBtn->setEnabled(false);

        gridLayout_10->addWidget(rHeadSubBtn, 6, 1, 1, 1);

        shortArmAddBtn = new QPushButton(JOGgroupBox);
        shortArmAddBtn->setObjectName(QStringLiteral("shortArmAddBtn"));
        shortArmAddBtn->setEnabled(false);

        gridLayout_10->addWidget(shortArmAddBtn, 5, 0, 1, 1);

        shortArmSubBtn = new QPushButton(JOGgroupBox);
        shortArmSubBtn->setObjectName(QStringLiteral("shortArmSubBtn"));
        shortArmSubBtn->setEnabled(false);

        gridLayout_10->addWidget(shortArmSubBtn, 5, 1, 1, 1);

        rHeadAddBtn = new QPushButton(JOGgroupBox);
        rHeadAddBtn->setObjectName(QStringLiteral("rHeadAddBtn"));
        rHeadAddBtn->setEnabled(false);

        gridLayout_10->addWidget(rHeadAddBtn, 6, 0, 1, 1);

        baseAngleAddBtn = new QPushButton(JOGgroupBox);
        baseAngleAddBtn->setObjectName(QStringLiteral("baseAngleAddBtn"));
        baseAngleAddBtn->setEnabled(false);

        gridLayout_10->addWidget(baseAngleAddBtn, 3, 0, 1, 1);

        teachMode = new QComboBox(JOGgroupBox);
        teachMode->addItem(QString());
        teachMode->addItem(QString());
        teachMode->setObjectName(QStringLiteral("teachMode"));
        teachMode->setEnabled(false);

        gridLayout_10->addWidget(teachMode, 0, 0, 1, 2);

        longArmSubBtn = new QPushButton(JOGgroupBox);
        longArmSubBtn->setObjectName(QStringLiteral("longArmSubBtn"));
        longArmSubBtn->setEnabled(false);

        gridLayout_10->addWidget(longArmSubBtn, 4, 1, 1, 1);


        gridLayout_3->addWidget(JOGgroupBox, 3, 3, 1, 1);

        dobotMovements = new QGroupBox(groupBox_5);
        dobotMovements->setObjectName(QStringLiteral("dobotMovements"));
        gridLayout_12 = new QGridLayout(dobotMovements);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        startGmPosBtn = new QPushButton(dobotMovements);
        startGmPosBtn->setObjectName(QStringLiteral("startGmPosBtn"));
        startGmPosBtn->setEnabled(false);

        gridLayout_12->addWidget(startGmPosBtn, 0, 3, 1, 1);

        upBtn = new QPushButton(dobotMovements);
        upBtn->setObjectName(QStringLiteral("upBtn"));
        upBtn->setEnabled(false);

        gridLayout_12->addWidget(upBtn, 6, 1, 1, 1);

        homeBtn = new QPushButton(dobotMovements);
        homeBtn->setObjectName(QStringLiteral("homeBtn"));
        homeBtn->setEnabled(false);

        gridLayout_12->addWidget(homeBtn, 0, 1, 1, 1);

        calibrateBtn = new QPushButton(dobotMovements);
        calibrateBtn->setObjectName(QStringLiteral("calibrateBtn"));
        calibrateBtn->setEnabled(false);

        gridLayout_12->addWidget(calibrateBtn, 0, 2, 1, 1);

        downBtn = new QPushButton(dobotMovements);
        downBtn->setObjectName(QStringLiteral("downBtn"));
        downBtn->setEnabled(false);

        gridLayout_12->addWidget(downBtn, 6, 2, 1, 1);

        startDtPosBtn = new QPushButton(dobotMovements);
        startDtPosBtn->setObjectName(QStringLiteral("startDtPosBtn"));
        startDtPosBtn->setEnabled(false);

        gridLayout_12->addWidget(startDtPosBtn, 0, 4, 1, 1);

        openGripperBtn = new QPushButton(dobotMovements);
        openGripperBtn->setObjectName(QStringLiteral("openGripperBtn"));
        openGripperBtn->setEnabled(false);

        gridLayout_12->addWidget(openGripperBtn, 6, 3, 1, 1);

        closeGripperBtn = new QPushButton(dobotMovements);
        closeGripperBtn->setObjectName(QStringLiteral("closeGripperBtn"));
        closeGripperBtn->setEnabled(false);

        gridLayout_12->addWidget(closeGripperBtn, 6, 4, 1, 1);

        retreatLeftBtn = new QPushButton(dobotMovements);
        retreatLeftBtn->setObjectName(QStringLiteral("retreatLeftBtn"));
        retreatLeftBtn->setEnabled(false);

        gridLayout_12->addWidget(retreatLeftBtn, 1, 1, 1, 1);

        middleAboveBtn = new QPushButton(dobotMovements);
        middleAboveBtn->setObjectName(QStringLiteral("middleAboveBtn"));
        middleAboveBtn->setEnabled(false);

        gridLayout_12->addWidget(middleAboveBtn, 1, 2, 1, 1);

        retreatRightBtn = new QPushButton(dobotMovements);
        retreatRightBtn->setObjectName(QStringLiteral("retreatRightBtn"));
        retreatRightBtn->setEnabled(false);

        gridLayout_12->addWidget(retreatRightBtn, 1, 3, 1, 1);


        gridLayout_3->addWidget(dobotMovements, 2, 0, 1, 2);

        dobotInfo = new QGroupBox(groupBox_5);
        dobotInfo->setObjectName(QStringLiteral("dobotInfo"));
        gridLayout_11 = new QGridLayout(dobotInfo);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        label_9 = new QLabel(dobotInfo);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_11->addWidget(label_9, 0, 0, 1, 1);

        deviceSNLabel = new QLabel(dobotInfo);
        deviceSNLabel->setObjectName(QStringLiteral("deviceSNLabel"));
        deviceSNLabel->setFrameShape(QFrame::Panel);

        gridLayout_11->addWidget(deviceSNLabel, 0, 1, 1, 1);

        label_10 = new QLabel(dobotInfo);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_11->addWidget(label_10, 1, 0, 1, 1);

        DeviceNameLabel = new QLabel(dobotInfo);
        DeviceNameLabel->setObjectName(QStringLiteral("DeviceNameLabel"));
        DeviceNameLabel->setFrameShape(QFrame::Panel);
        DeviceNameLabel->setTextFormat(Qt::AutoText);

        gridLayout_11->addWidget(DeviceNameLabel, 1, 1, 1, 1);

        label_11 = new QLabel(dobotInfo);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_11->addWidget(label_11, 2, 0, 1, 1);

        DeviceInfoLabel = new QLabel(dobotInfo);
        DeviceInfoLabel->setObjectName(QStringLiteral("DeviceInfoLabel"));
        DeviceInfoLabel->setFrameShape(QFrame::Panel);
        DeviceInfoLabel->setFrameShadow(QFrame::Plain);

        gridLayout_11->addWidget(DeviceInfoLabel, 2, 1, 1, 1);

        connectBtn = new QPushButton(dobotInfo);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        gridLayout_11->addWidget(connectBtn, 3, 1, 1, 1);


        gridLayout_3->addWidget(dobotInfo, 1, 0, 1, 1);

        dobotPositions = new QGroupBox(groupBox_5);
        dobotPositions->setObjectName(QStringLiteral("dobotPositions"));
        gridLayout = new QGridLayout(dobotPositions);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        joint1Label = new QLabel(dobotPositions);
        joint1Label->setObjectName(QStringLiteral("joint1Label"));
        joint1Label->setEnabled(true);

        gridLayout->addWidget(joint1Label, 0, 1, 1, 1);

        label_6 = new QLabel(dobotPositions);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 3, 1, 1);

        xLabel = new QLabel(dobotPositions);
        xLabel->setObjectName(QStringLiteral("xLabel"));
        xLabel->setEnabled(true);

        gridLayout->addWidget(xLabel, 2, 1, 1, 1);

        yLabel = new QLabel(dobotPositions);
        yLabel->setObjectName(QStringLiteral("yLabel"));

        gridLayout->addWidget(yLabel, 2, 4, 1, 1);

        joint3Label = new QLabel(dobotPositions);
        joint3Label->setObjectName(QStringLiteral("joint3Label"));

        gridLayout->addWidget(joint3Label, 1, 1, 1, 1);

        label_7 = new QLabel(dobotPositions);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_3 = new QLabel(dobotPositions);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        zLabel = new QLabel(dobotPositions);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        gridLayout->addWidget(zLabel, 3, 1, 1, 1);

        label_4 = new QLabel(dobotPositions);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(dobotPositions);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        rLabel = new QLabel(dobotPositions);
        rLabel->setObjectName(QStringLiteral("rLabel"));

        gridLayout->addWidget(rLabel, 3, 4, 1, 1);

        label = new QLabel(dobotPositions);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(dobotPositions);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        joint4Label = new QLabel(dobotPositions);
        joint4Label->setObjectName(QStringLiteral("joint4Label"));

        gridLayout->addWidget(joint4Label, 1, 4, 1, 1);

        label_8 = new QLabel(dobotPositions);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 3, 3, 1, 1);

        joint2Label = new QLabel(dobotPositions);
        joint2Label->setObjectName(QStringLiteral("joint2Label"));

        gridLayout->addWidget(joint2Label, 0, 4, 1, 1);


        gridLayout_3->addWidget(dobotPositions, 1, 1, 1, 3);


        gridLayout_4->addWidget(groupBox_5, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_5->raise();
        log->raise();
        groupBox->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1330, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(xPTPEdit, zPTPEdit);
        QWidget::setTabOrder(zPTPEdit, sendPointBtn);

        retranslateUi(MainWindow);

        sendSimulatedMsgBtn->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Board data", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Game", nullptr));
        gameStatusLbl->setText(QApplication::translate("MainWindow", "-1", nullptr));
        whiteNameLbl->setText(QApplication::translate("MainWindow", "WHITE", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "White name", nullptr));
        label_25->setText(QApplication::translate("MainWindow", "Game status", nullptr));
        labelx->setText(QApplication::translate("MainWindow", "Sockets online", nullptr));
        label_24->setText(QApplication::translate("MainWindow", "Turn", nullptr));
        turnLbl->setText(QApplication::translate("MainWindow", "nt", nullptr));
        blackNameLbl->setText(QApplication::translate("MainWindow", "BLACK", nullptr));
        socketsLbl->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_23->setText(QApplication::translate("MainWindow", "Black name", nullptr));
        label_28->setText(QApplication::translate("MainWindow", "Halfmove/fullmove", nullptr));
        movesLbl->setText(QApplication::translate("MainWindow", "-1/-1", nullptr));
        label_26->setText(QApplication::translate("MainWindow", "Castlings", nullptr));
        castlingsLbl->setText(QApplication::translate("MainWindow", "-1", nullptr));
        label_27->setText(QApplication::translate("MainWindow", "Enpassant ", nullptr));
        enpassantLbl->setText(QApplication::translate("MainWindow", "-1", nullptr));
        label_30->setText(QApplication::translate("MainWindow", "White time", nullptr));
        whiteTimeLbl->setText(QApplication::translate("MainWindow", "30:00", nullptr));
        label_31->setText(QApplication::translate("MainWindow", "Black  time", nullptr));
        blackTimeLbl->setText(QApplication::translate("MainWindow", "30:00", nullptr));
        label_29->setText(QApplication::translate("MainWindow", "Queued plrs", nullptr));
        queuedPlayersLbl->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_33->setText(QApplication::translate("MainWindow", "Queue timer", nullptr));
        queueTimeLbl->setText(QApplication::translate("MainWindow", "2:00", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Board", nullptr));
        label_35->setText(QApplication::translate("MainWindow", "Chenard", nullptr));
        label_34->setText(QApplication::translate("MainWindow", "Main + removed", nullptr));
        realBoardPTE->setPlainText(QString());
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Moves", nullptr));
        label_37->setText(QApplication::translate("MainWindow", "Queued on core", nullptr));
        label_39->setText(QApplication::translate("MainWindow", "Legal", nullptr));
        label_36->setText(QApplication::translate("MainWindow", "Queued on Dobot", nullptr));
        label_38->setText(QApplication::translate("MainWindow", "History", nullptr));
        resetAllDataBtn->setText(QApplication::translate("MainWindow", "Reset all data", nullptr));
        log->setTitle(QApplication::translate("MainWindow", "Log", nullptr));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Dobot Arm", nullptr));
        queue->setTitle(QApplication::translate("MainWindow", "Queue", nullptr));
        CoreIndexAmountlabel->setText(QApplication::translate("MainWindow", "-1", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "DobotID", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "CoreMaxID", nullptr));
        label_21->setText(QApplication::translate("MainWindow", "DobotLeftIDsSpc", nullptr));
        DobotQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", nullptr));
        DobotQueuedCmdLeftSpaceLabel->setText(QApplication::translate("MainWindow", "-1", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "IDsLeftToSend", nullptr));
        label_22->setText(QApplication::translate("MainWindow", "CoreNextID", nullptr));
        CoreMaxQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", nullptr));
        CoreNextIdLabel->setText(QApplication::translate("MainWindow", "-1", nullptr));
        resetDobotIndexBtn->setText(QApplication::translate("MainWindow", "Reset arm ID", nullptr));
        dobotServiceMove->setTitle(QApplication::translate("MainWindow", "DobotService PTP", nullptr));
        label_17->setText(QApplication::translate("MainWindow", "X", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "Y", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "s", nullptr));
        sendPointBtn->setText(QApplication::translate("MainWindow", "Send", nullptr));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Simulate player msg ", nullptr));
        sendSimulatedMsgBtn->setText(QApplication::translate("MainWindow", "SimMove", nullptr));
        sendTcpBtn->setText(QApplication::translate("MainWindow", "SendTCP", nullptr));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Clients", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "Data", nullptr));
        label_32->setText(QApplication::translate("MainWindow", "Queued", nullptr));
        JOGgroupBox->setTitle(QApplication::translate("MainWindow", "JOG", nullptr));
        baseAngleSubBtn->setText(QApplication::translate("MainWindow", "J1-", nullptr));
        longArmAddBtn->setText(QApplication::translate("MainWindow", "J2+", nullptr));
        rHeadSubBtn->setText(QApplication::translate("MainWindow", "J4-", nullptr));
        shortArmAddBtn->setText(QApplication::translate("MainWindow", "J3+", nullptr));
        shortArmSubBtn->setText(QApplication::translate("MainWindow", "J3-", nullptr));
        rHeadAddBtn->setText(QApplication::translate("MainWindow", "J4+", nullptr));
        baseAngleAddBtn->setText(QApplication::translate("MainWindow", "J1+", nullptr));
        teachMode->setItemText(0, QApplication::translate("MainWindow", "Joint", nullptr));
        teachMode->setItemText(1, QApplication::translate("MainWindow", "Axis", nullptr));

        longArmSubBtn->setText(QApplication::translate("MainWindow", "J2-", nullptr));
        dobotMovements->setTitle(QApplication::translate("MainWindow", "Dobot Movements", nullptr));
        startGmPosBtn->setText(QApplication::translate("MainWindow", "StGmPos", nullptr));
        upBtn->setText(QApplication::translate("MainWindow", "Up", nullptr));
        homeBtn->setText(QApplication::translate("MainWindow", "Home", nullptr));
        calibrateBtn->setText(QApplication::translate("MainWindow", "Calibrate", nullptr));
        downBtn->setText(QApplication::translate("MainWindow", "Down", nullptr));
        startDtPosBtn->setText(QApplication::translate("MainWindow", "StDtPos", nullptr));
        openGripperBtn->setText(QApplication::translate("MainWindow", "OpenGrip", nullptr));
        closeGripperBtn->setText(QApplication::translate("MainWindow", "CloseGrip", nullptr));
        retreatLeftBtn->setText(QApplication::translate("MainWindow", "retLeft", nullptr));
        middleAboveBtn->setText(QApplication::translate("MainWindow", "MidAbove", nullptr));
        retreatRightBtn->setText(QApplication::translate("MainWindow", "retRight", nullptr));
        dobotInfo->setTitle(QApplication::translate("MainWindow", "Dobot Device Info", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "SN", nullptr));
        deviceSNLabel->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Name", nullptr));
        DeviceNameLabel->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Info", nullptr));
        DeviceInfoLabel->setText(QString());
        connectBtn->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        dobotPositions->setTitle(QApplication::translate("MainWindow", "Pose", nullptr));
        joint1Label->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Y", nullptr));
        xLabel->setText(QApplication::translate("MainWindow", "0", nullptr));
        yLabel->setText(QApplication::translate("MainWindow", "0", nullptr));
        joint3Label->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "J2", nullptr));
        zLabel->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "J3", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "J4", nullptr));
        rLabel->setText(QApplication::translate("MainWindow", "0", nullptr));
        label->setText(QApplication::translate("MainWindow", "J1", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "X", nullptr));
        joint4Label->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "R", nullptr));
        joint2Label->setText(QApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
