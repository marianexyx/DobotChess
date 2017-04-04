/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QGroupBox *log;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *debug_log;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_3;
    QGroupBox *chenardAI;
    QGridLayout *gridLayout_2;
    QPushButton *AIBtn;
    QRadioButton *botOnRadioBtn;
    QPushButton *AIEnemySendBtn;
    QLineEdit *AIEnemyLineEdit;
    QRadioButton *botOffRadioBtn;
    QPushButton *AIEnemyStartBtn;
    QGroupBox *JOGgroupBox;
    QGridLayout *gridLayout_10;
    QPushButton *rHeadAddBtn;
    QPushButton *baseAngleSubBtn;
    QPushButton *longArmAddBtn;
    QPushButton *shortArmAddBtn;
    QPushButton *baseAngleAddBtn;
    QPushButton *shortArmSubBtn;
    QPushButton *rHeadSubBtn;
    QPushButton *longArmSubBtn;
    QComboBox *teachMode;
    QGroupBox *dobotMovements;
    QGridLayout *gridLayout_12;
    QPushButton *closeGripperBtn;
    QPushButton *downBtn;
    QPushButton *openGripperBtn;
    QPushButton *upBtn;
    QPushButton *startGmPosBtn;
    QPushButton *homeBtn;
    QPushButton *startDtPosBtn;
    QGroupBox *dobotInfo;
    QGridLayout *gridLayout_11;
    QLabel *label_9;
    QLabel *deviceSNLabel;
    QLabel *label_10;
    QLabel *DeviceNameLabel;
    QLabel *label_11;
    QLabel *DeviceInfoLabel;
    QPushButton *connectBtn;
    QGroupBox *ArduinoUsbPanel;
    QGridLayout *gridLayout_13;
    QComboBox *portsComboBox;
    QPushButton *reloadPortsBtn;
    QLineEdit *usbCmdLine;
    QPushButton *sendUsbBtn;
    QPushButton *SimulateFromUsbBtn;
    QLineEdit *SimulateFromUsbLineEdit;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QPushButton *sendSimulatedMsgBtn;
    QLineEdit *emulatePlayerMsgLineEdit;
    QCheckBox *serviceCheckBox;
    QLineEdit *sendTcpLineEdit;
    QPushButton *sendTcpBtn;
    QGroupBox *dobotServiceMove;
    QFormLayout *formLayout_2;
    QLabel *label_17;
    QLineEdit *xPTPEdit;
    QLabel *label_18;
    QLineEdit *yPTPEdit;
    QLabel *label_19;
    QLineEdit *zPTPEdit;
    QLabel *label_20;
    QLineEdit *rPTPEdit;
    QLabel *label_12;
    QLineEdit *servo1GripperEdit;
    QLabel *label_13;
    QLineEdit *servo2GripperEdit;
    QPushButton *sendBtn;
    QGroupBox *dobotPositions;
    QGridLayout *gridLayout;
    QLabel *joint1Label;
    QLabel *yLabel;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *xLabel;
    QLabel *joint3Label;
    QLabel *label_4;
    QLabel *zLabel;
    QLabel *rLabel;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_8;
    QLabel *joint4Label;
    QLabel *joint2Label;
    QGroupBox *queue;
    QGridLayout *gridLayout_5;
    QLabel *CoreIndexAmountlabel;
    QLabel *label_14;
    QLabel *label_21;
    QLabel *label_15;
    QLabel *CoreMaxQueuedIndexLabel;
    QLabel *DobotQueuedIndexLabel;
    QLabel *label_16;
    QLabel *DobotQueuedCmdLeftSpaceLabel;
    QLabel *label_22;
    QLabel *CoreNextIdLabel;
    QPushButton *executeDobotComandsBtn;
    QPushButton *resetDobotIndexBtn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(861, 737);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        log = new QGroupBox(centralWidget);
        log->setObjectName(QStringLiteral("log"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(log->sizePolicy().hasHeightForWidth());
        log->setSizePolicy(sizePolicy);
        log->setMinimumSize(QSize(100, 0));
        log->setBaseSize(QSize(500, 0));
        verticalLayout = new QVBoxLayout(log);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        debug_log = new QPlainTextEdit(log);
        debug_log->setObjectName(QStringLiteral("debug_log"));
        debug_log->setReadOnly(true);
        debug_log->setOverwriteMode(false);

        verticalLayout->addWidget(debug_log);


        gridLayout_4->addWidget(log, 0, 3, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy1);
        groupBox_5->setMinimumSize(QSize(0, 0));
        groupBox_5->setMaximumSize(QSize(450, 16777215));
        gridLayout_3 = new QGridLayout(groupBox_5);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        chenardAI = new QGroupBox(groupBox_5);
        chenardAI->setObjectName(QStringLiteral("chenardAI"));
        gridLayout_2 = new QGridLayout(chenardAI);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        AIBtn = new QPushButton(chenardAI);
        AIBtn->setObjectName(QStringLiteral("AIBtn"));
        AIBtn->setEnabled(false);

        gridLayout_2->addWidget(AIBtn, 0, 2, 1, 1);

        botOnRadioBtn = new QRadioButton(chenardAI);
        botOnRadioBtn->setObjectName(QStringLiteral("botOnRadioBtn"));
        botOnRadioBtn->setEnabled(false);

        gridLayout_2->addWidget(botOnRadioBtn, 0, 1, 1, 1);

        AIEnemySendBtn = new QPushButton(chenardAI);
        AIEnemySendBtn->setObjectName(QStringLiteral("AIEnemySendBtn"));
        AIEnemySendBtn->setEnabled(false);

        gridLayout_2->addWidget(AIEnemySendBtn, 2, 2, 1, 1);

        AIEnemyLineEdit = new QLineEdit(chenardAI);
        AIEnemyLineEdit->setObjectName(QStringLiteral("AIEnemyLineEdit"));
        AIEnemyLineEdit->setEnabled(false);

        gridLayout_2->addWidget(AIEnemyLineEdit, 2, 0, 1, 2);

        botOffRadioBtn = new QRadioButton(chenardAI);
        botOffRadioBtn->setObjectName(QStringLiteral("botOffRadioBtn"));
        botOffRadioBtn->setEnabled(false);
        botOffRadioBtn->setChecked(true);

        gridLayout_2->addWidget(botOffRadioBtn, 0, 0, 1, 1);

        AIEnemyStartBtn = new QPushButton(chenardAI);
        AIEnemyStartBtn->setObjectName(QStringLiteral("AIEnemyStartBtn"));
        AIEnemyStartBtn->setEnabled(false);

        gridLayout_2->addWidget(AIEnemyStartBtn, 1, 2, 1, 1);


        gridLayout_3->addWidget(chenardAI, 1, 0, 1, 2);

        JOGgroupBox = new QGroupBox(groupBox_5);
        JOGgroupBox->setObjectName(QStringLiteral("JOGgroupBox"));
        gridLayout_10 = new QGridLayout(JOGgroupBox);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        rHeadAddBtn = new QPushButton(JOGgroupBox);
        rHeadAddBtn->setObjectName(QStringLiteral("rHeadAddBtn"));
        rHeadAddBtn->setEnabled(false);

        gridLayout_10->addWidget(rHeadAddBtn, 6, 0, 1, 1);

        baseAngleSubBtn = new QPushButton(JOGgroupBox);
        baseAngleSubBtn->setObjectName(QStringLiteral("baseAngleSubBtn"));
        baseAngleSubBtn->setEnabled(false);

        gridLayout_10->addWidget(baseAngleSubBtn, 3, 1, 1, 1);

        longArmAddBtn = new QPushButton(JOGgroupBox);
        longArmAddBtn->setObjectName(QStringLiteral("longArmAddBtn"));
        longArmAddBtn->setEnabled(false);

        gridLayout_10->addWidget(longArmAddBtn, 4, 0, 1, 1);

        shortArmAddBtn = new QPushButton(JOGgroupBox);
        shortArmAddBtn->setObjectName(QStringLiteral("shortArmAddBtn"));
        shortArmAddBtn->setEnabled(false);

        gridLayout_10->addWidget(shortArmAddBtn, 5, 0, 1, 1);

        baseAngleAddBtn = new QPushButton(JOGgroupBox);
        baseAngleAddBtn->setObjectName(QStringLiteral("baseAngleAddBtn"));
        baseAngleAddBtn->setEnabled(false);

        gridLayout_10->addWidget(baseAngleAddBtn, 3, 0, 1, 1);

        shortArmSubBtn = new QPushButton(JOGgroupBox);
        shortArmSubBtn->setObjectName(QStringLiteral("shortArmSubBtn"));
        shortArmSubBtn->setEnabled(false);

        gridLayout_10->addWidget(shortArmSubBtn, 5, 1, 1, 1);

        rHeadSubBtn = new QPushButton(JOGgroupBox);
        rHeadSubBtn->setObjectName(QStringLiteral("rHeadSubBtn"));
        rHeadSubBtn->setEnabled(false);

        gridLayout_10->addWidget(rHeadSubBtn, 6, 1, 1, 1);

        longArmSubBtn = new QPushButton(JOGgroupBox);
        longArmSubBtn->setObjectName(QStringLiteral("longArmSubBtn"));
        longArmSubBtn->setEnabled(false);

        gridLayout_10->addWidget(longArmSubBtn, 4, 1, 1, 1);

        teachMode = new QComboBox(JOGgroupBox);
        teachMode->setObjectName(QStringLiteral("teachMode"));
        teachMode->setEnabled(false);

        gridLayout_10->addWidget(teachMode, 0, 0, 1, 2);


        gridLayout_3->addWidget(JOGgroupBox, 2, 2, 1, 1);

        dobotMovements = new QGroupBox(groupBox_5);
        dobotMovements->setObjectName(QStringLiteral("dobotMovements"));
        gridLayout_12 = new QGridLayout(dobotMovements);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        closeGripperBtn = new QPushButton(dobotMovements);
        closeGripperBtn->setObjectName(QStringLiteral("closeGripperBtn"));
        closeGripperBtn->setEnabled(false);

        gridLayout_12->addWidget(closeGripperBtn, 2, 0, 1, 1);

        downBtn = new QPushButton(dobotMovements);
        downBtn->setObjectName(QStringLiteral("downBtn"));
        downBtn->setEnabled(false);

        gridLayout_12->addWidget(downBtn, 6, 0, 1, 1);

        openGripperBtn = new QPushButton(dobotMovements);
        openGripperBtn->setObjectName(QStringLiteral("openGripperBtn"));
        openGripperBtn->setEnabled(false);

        gridLayout_12->addWidget(openGripperBtn, 1, 0, 1, 1);

        upBtn = new QPushButton(dobotMovements);
        upBtn->setObjectName(QStringLiteral("upBtn"));
        upBtn->setEnabled(false);

        gridLayout_12->addWidget(upBtn, 5, 0, 1, 1);

        startGmPosBtn = new QPushButton(dobotMovements);
        startGmPosBtn->setObjectName(QStringLiteral("startGmPosBtn"));
        startGmPosBtn->setEnabled(false);

        gridLayout_12->addWidget(startGmPosBtn, 3, 0, 1, 1);

        homeBtn = new QPushButton(dobotMovements);
        homeBtn->setObjectName(QStringLiteral("homeBtn"));
        homeBtn->setEnabled(false);

        gridLayout_12->addWidget(homeBtn, 0, 0, 1, 1);

        startDtPosBtn = new QPushButton(dobotMovements);
        startDtPosBtn->setObjectName(QStringLiteral("startDtPosBtn"));
        startDtPosBtn->setEnabled(false);

        gridLayout_12->addWidget(startDtPosBtn, 4, 0, 1, 1);


        gridLayout_3->addWidget(dobotMovements, 2, 1, 1, 1);

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


        gridLayout_3->addWidget(dobotInfo, 1, 2, 1, 1);

        ArduinoUsbPanel = new QGroupBox(groupBox_5);
        ArduinoUsbPanel->setObjectName(QStringLiteral("ArduinoUsbPanel"));
        gridLayout_13 = new QGridLayout(ArduinoUsbPanel);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        portsComboBox = new QComboBox(ArduinoUsbPanel);
        portsComboBox->setObjectName(QStringLiteral("portsComboBox"));
        portsComboBox->setEnabled(false);

        gridLayout_13->addWidget(portsComboBox, 2, 0, 1, 1);

        reloadPortsBtn = new QPushButton(ArduinoUsbPanel);
        reloadPortsBtn->setObjectName(QStringLiteral("reloadPortsBtn"));

        gridLayout_13->addWidget(reloadPortsBtn, 2, 1, 1, 1);

        usbCmdLine = new QLineEdit(ArduinoUsbPanel);
        usbCmdLine->setObjectName(QStringLiteral("usbCmdLine"));
        usbCmdLine->setEnabled(false);

        gridLayout_13->addWidget(usbCmdLine, 1, 0, 1, 1);

        sendUsbBtn = new QPushButton(ArduinoUsbPanel);
        sendUsbBtn->setObjectName(QStringLiteral("sendUsbBtn"));
        sendUsbBtn->setEnabled(false);

        gridLayout_13->addWidget(sendUsbBtn, 1, 1, 1, 1);

        SimulateFromUsbBtn = new QPushButton(ArduinoUsbPanel);
        SimulateFromUsbBtn->setObjectName(QStringLiteral("SimulateFromUsbBtn"));
        SimulateFromUsbBtn->setEnabled(false);

        gridLayout_13->addWidget(SimulateFromUsbBtn, 0, 1, 1, 1);

        SimulateFromUsbLineEdit = new QLineEdit(ArduinoUsbPanel);
        SimulateFromUsbLineEdit->setObjectName(QStringLiteral("SimulateFromUsbLineEdit"));
        SimulateFromUsbLineEdit->setEnabled(false);

        gridLayout_13->addWidget(SimulateFromUsbLineEdit, 0, 0, 1, 1);


        gridLayout_3->addWidget(ArduinoUsbPanel, 0, 0, 1, 2);

        groupBox_6 = new QGroupBox(groupBox_5);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        sendSimulatedMsgBtn = new QPushButton(groupBox_6);
        sendSimulatedMsgBtn->setObjectName(QStringLiteral("sendSimulatedMsgBtn"));
        sendSimulatedMsgBtn->setEnabled(false);
        sendSimulatedMsgBtn->setAutoDefault(true);

        gridLayout_6->addWidget(sendSimulatedMsgBtn, 1, 1, 1, 1);

        emulatePlayerMsgLineEdit = new QLineEdit(groupBox_6);
        emulatePlayerMsgLineEdit->setObjectName(QStringLiteral("emulatePlayerMsgLineEdit"));
        emulatePlayerMsgLineEdit->setEnabled(false);

        gridLayout_6->addWidget(emulatePlayerMsgLineEdit, 0, 0, 1, 2);

        serviceCheckBox = new QCheckBox(groupBox_6);
        serviceCheckBox->setObjectName(QStringLiteral("serviceCheckBox"));
        serviceCheckBox->setEnabled(false);

        gridLayout_6->addWidget(serviceCheckBox, 1, 0, 1, 1);

        sendTcpLineEdit = new QLineEdit(groupBox_6);
        sendTcpLineEdit->setObjectName(QStringLiteral("sendTcpLineEdit"));
        sendTcpLineEdit->setEnabled(false);

        gridLayout_6->addWidget(sendTcpLineEdit, 2, 0, 1, 1);

        sendTcpBtn = new QPushButton(groupBox_6);
        sendTcpBtn->setObjectName(QStringLiteral("sendTcpBtn"));
        sendTcpBtn->setEnabled(false);

        gridLayout_6->addWidget(sendTcpBtn, 2, 1, 1, 1);


        gridLayout_3->addWidget(groupBox_6, 0, 2, 1, 1);

        dobotServiceMove = new QGroupBox(groupBox_5);
        dobotServiceMove->setObjectName(QStringLiteral("dobotServiceMove"));
        formLayout_2 = new QFormLayout(dobotServiceMove);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_17 = new QLabel(dobotServiceMove);
        label_17->setObjectName(QStringLiteral("label_17"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_17);

        xPTPEdit = new QLineEdit(dobotServiceMove);
        xPTPEdit->setObjectName(QStringLiteral("xPTPEdit"));
        xPTPEdit->setEnabled(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, xPTPEdit);

        label_18 = new QLabel(dobotServiceMove);
        label_18->setObjectName(QStringLiteral("label_18"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_18);

        yPTPEdit = new QLineEdit(dobotServiceMove);
        yPTPEdit->setObjectName(QStringLiteral("yPTPEdit"));
        yPTPEdit->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, yPTPEdit);

        label_19 = new QLabel(dobotServiceMove);
        label_19->setObjectName(QStringLiteral("label_19"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_19);

        zPTPEdit = new QLineEdit(dobotServiceMove);
        zPTPEdit->setObjectName(QStringLiteral("zPTPEdit"));
        zPTPEdit->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, zPTPEdit);

        label_20 = new QLabel(dobotServiceMove);
        label_20->setObjectName(QStringLiteral("label_20"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_20);

        rPTPEdit = new QLineEdit(dobotServiceMove);
        rPTPEdit->setObjectName(QStringLiteral("rPTPEdit"));
        rPTPEdit->setEnabled(false);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, rPTPEdit);

        label_12 = new QLabel(dobotServiceMove);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_12);

        servo1GripperEdit = new QLineEdit(dobotServiceMove);
        servo1GripperEdit->setObjectName(QStringLiteral("servo1GripperEdit"));
        servo1GripperEdit->setEnabled(false);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, servo1GripperEdit);

        label_13 = new QLabel(dobotServiceMove);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout_2->setWidget(8, QFormLayout::LabelRole, label_13);

        servo2GripperEdit = new QLineEdit(dobotServiceMove);
        servo2GripperEdit->setObjectName(QStringLiteral("servo2GripperEdit"));
        servo2GripperEdit->setEnabled(false);

        formLayout_2->setWidget(8, QFormLayout::FieldRole, servo2GripperEdit);

        sendBtn = new QPushButton(dobotServiceMove);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setEnabled(false);

        formLayout_2->setWidget(9, QFormLayout::FieldRole, sendBtn);


        gridLayout_3->addWidget(dobotServiceMove, 2, 0, 1, 1);

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

        yLabel = new QLabel(dobotPositions);
        yLabel->setObjectName(QStringLiteral("yLabel"));

        gridLayout->addWidget(yLabel, 2, 4, 1, 1);

        label_7 = new QLabel(dobotPositions);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_3 = new QLabel(dobotPositions);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        label_6 = new QLabel(dobotPositions);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 3, 1, 1);

        xLabel = new QLabel(dobotPositions);
        xLabel->setObjectName(QStringLiteral("xLabel"));
        xLabel->setEnabled(true);

        gridLayout->addWidget(xLabel, 2, 1, 1, 1);

        joint3Label = new QLabel(dobotPositions);
        joint3Label->setObjectName(QStringLiteral("joint3Label"));

        gridLayout->addWidget(joint3Label, 1, 1, 1, 1);

        label_4 = new QLabel(dobotPositions);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        zLabel = new QLabel(dobotPositions);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        gridLayout->addWidget(zLabel, 3, 1, 1, 1);

        rLabel = new QLabel(dobotPositions);
        rLabel->setObjectName(QStringLiteral("rLabel"));

        gridLayout->addWidget(rLabel, 3, 4, 1, 1);

        label = new QLabel(dobotPositions);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(dobotPositions);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        label_2 = new QLabel(dobotPositions);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_8 = new QLabel(dobotPositions);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 3, 3, 1, 1);

        joint4Label = new QLabel(dobotPositions);
        joint4Label->setObjectName(QStringLiteral("joint4Label"));

        gridLayout->addWidget(joint4Label, 1, 4, 1, 1);

        joint2Label = new QLabel(dobotPositions);
        joint2Label->setObjectName(QStringLiteral("joint2Label"));

        gridLayout->addWidget(joint2Label, 0, 4, 1, 1);


        gridLayout_3->addWidget(dobotPositions, 4, 0, 2, 2);

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

        label_21 = new QLabel(queue);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_5->addWidget(label_21, 2, 0, 1, 1);

        label_15 = new QLabel(queue);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_5->addWidget(label_15, 1, 0, 1, 1);

        CoreMaxQueuedIndexLabel = new QLabel(queue);
        CoreMaxQueuedIndexLabel->setObjectName(QStringLiteral("CoreMaxQueuedIndexLabel"));

        gridLayout_5->addWidget(CoreMaxQueuedIndexLabel, 1, 1, 1, 1);

        DobotQueuedIndexLabel = new QLabel(queue);
        DobotQueuedIndexLabel->setObjectName(QStringLiteral("DobotQueuedIndexLabel"));

        gridLayout_5->addWidget(DobotQueuedIndexLabel, 0, 1, 1, 1);

        label_16 = new QLabel(queue);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_5->addWidget(label_16, 3, 0, 1, 1);

        DobotQueuedCmdLeftSpaceLabel = new QLabel(queue);
        DobotQueuedCmdLeftSpaceLabel->setObjectName(QStringLiteral("DobotQueuedCmdLeftSpaceLabel"));

        gridLayout_5->addWidget(DobotQueuedCmdLeftSpaceLabel, 2, 1, 1, 1);

        label_22 = new QLabel(queue);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_5->addWidget(label_22, 4, 0, 1, 1);

        CoreNextIdLabel = new QLabel(queue);
        CoreNextIdLabel->setObjectName(QStringLiteral("CoreNextIdLabel"));

        gridLayout_5->addWidget(CoreNextIdLabel, 4, 1, 1, 1);

        executeDobotComandsBtn = new QPushButton(queue);
        executeDobotComandsBtn->setObjectName(QStringLiteral("executeDobotComandsBtn"));
        executeDobotComandsBtn->setEnabled(false);

        gridLayout_5->addWidget(executeDobotComandsBtn, 5, 1, 1, 1);

        resetDobotIndexBtn = new QPushButton(queue);
        resetDobotIndexBtn->setObjectName(QStringLiteral("resetDobotIndexBtn"));
        resetDobotIndexBtn->setEnabled(false);

        gridLayout_5->addWidget(resetDobotIndexBtn, 5, 0, 1, 1);


        gridLayout_3->addWidget(queue, 4, 2, 2, 1);


        gridLayout_4->addWidget(groupBox_5, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_5->raise();
        log->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 861, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(xPTPEdit, zPTPEdit);
        QWidget::setTabOrder(zPTPEdit, sendBtn);

        retranslateUi(MainWindow);

        sendSimulatedMsgBtn->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        log->setTitle(QApplication::translate("MainWindow", "Log", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Dobot Arm", 0));
        chenardAI->setTitle(QApplication::translate("MainWindow", "AI", 0));
        AIBtn->setText(QApplication::translate("MainWindow", "Set", 0));
        botOnRadioBtn->setText(QApplication::translate("MainWindow", "On", 0));
        AIEnemySendBtn->setText(QApplication::translate("MainWindow", "PlayerSend", 0));
        botOffRadioBtn->setText(QApplication::translate("MainWindow", "Off", 0));
        AIEnemyStartBtn->setText(QApplication::translate("MainWindow", "PlayerStart", 0));
        JOGgroupBox->setTitle(QApplication::translate("MainWindow", "JOG", 0));
        rHeadAddBtn->setText(QApplication::translate("MainWindow", "J4+", 0));
        baseAngleSubBtn->setText(QApplication::translate("MainWindow", "J1-", 0));
        longArmAddBtn->setText(QApplication::translate("MainWindow", "J2+", 0));
        shortArmAddBtn->setText(QApplication::translate("MainWindow", "J3+", 0));
        baseAngleAddBtn->setText(QApplication::translate("MainWindow", "J1+", 0));
        shortArmSubBtn->setText(QApplication::translate("MainWindow", "J3-", 0));
        rHeadSubBtn->setText(QApplication::translate("MainWindow", "J4-", 0));
        longArmSubBtn->setText(QApplication::translate("MainWindow", "J2-", 0));
        teachMode->clear();
        teachMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Joint", 0)
         << QApplication::translate("MainWindow", "Axis", 0)
        );
        dobotMovements->setTitle(QApplication::translate("MainWindow", "Dobot Movements", 0));
        closeGripperBtn->setText(QApplication::translate("MainWindow", "CloseGrip", 0));
        downBtn->setText(QApplication::translate("MainWindow", "Down", 0));
        openGripperBtn->setText(QApplication::translate("MainWindow", "OpenGrip", 0));
        upBtn->setText(QApplication::translate("MainWindow", "Up", 0));
        startGmPosBtn->setText(QApplication::translate("MainWindow", "StGmPos", 0));
        homeBtn->setText(QApplication::translate("MainWindow", "HOME", 0));
        startDtPosBtn->setText(QApplication::translate("MainWindow", "StDtPos", 0));
        dobotInfo->setTitle(QApplication::translate("MainWindow", "Device", 0));
        label_9->setText(QApplication::translate("MainWindow", "SN", 0));
        deviceSNLabel->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Name", 0));
        DeviceNameLabel->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Info", 0));
        DeviceInfoLabel->setText(QString());
        connectBtn->setText(QApplication::translate("MainWindow", "Connect", 0));
        ArduinoUsbPanel->setTitle(QApplication::translate("MainWindow", "Arduino Usb", 0));
        reloadPortsBtn->setText(QApplication::translate("MainWindow", "Reload", 0));
        sendUsbBtn->setText(QApplication::translate("MainWindow", "SendToUSB", 0));
        SimulateFromUsbBtn->setText(QApplication::translate("MainWindow", "SimFromUSB", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Simulate player msg ", 0));
        sendSimulatedMsgBtn->setText(QApplication::translate("MainWindow", "SendWS", 0));
        serviceCheckBox->setText(QApplication::translate("MainWindow", "servMsg", 0));
        sendTcpBtn->setText(QApplication::translate("MainWindow", "SendTCP", 0));
        dobotServiceMove->setTitle(QApplication::translate("MainWindow", "Service PTP", 0));
        label_17->setText(QApplication::translate("MainWindow", "X", 0));
        label_18->setText(QApplication::translate("MainWindow", "Y", 0));
        label_19->setText(QApplication::translate("MainWindow", "Z", 0));
        label_20->setText(QApplication::translate("MainWindow", "R", 0));
        label_12->setText(QApplication::translate("MainWindow", "s1", 0));
        label_13->setText(QApplication::translate("MainWindow", "s2", 0));
        sendBtn->setText(QApplication::translate("MainWindow", "Send", 0));
        dobotPositions->setTitle(QApplication::translate("MainWindow", "Pose", 0));
        joint1Label->setText(QApplication::translate("MainWindow", "0", 0));
        yLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_7->setText(QApplication::translate("MainWindow", "Z", 0));
        label_3->setText(QApplication::translate("MainWindow", "J2", 0));
        label_6->setText(QApplication::translate("MainWindow", "Y", 0));
        xLabel->setText(QApplication::translate("MainWindow", "0", 0));
        joint3Label->setText(QApplication::translate("MainWindow", "0", 0));
        label_4->setText(QApplication::translate("MainWindow", "J3", 0));
        zLabel->setText(QApplication::translate("MainWindow", "0", 0));
        rLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label->setText(QApplication::translate("MainWindow", "J1", 0));
        label_5->setText(QApplication::translate("MainWindow", "J4", 0));
        label_2->setText(QApplication::translate("MainWindow", "X", 0));
        label_8->setText(QApplication::translate("MainWindow", "R", 0));
        joint4Label->setText(QApplication::translate("MainWindow", "0", 0));
        joint2Label->setText(QApplication::translate("MainWindow", "0", 0));
        queue->setTitle(QApplication::translate("MainWindow", "Queue", 0));
        CoreIndexAmountlabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_14->setText(QApplication::translate("MainWindow", "DIndex", 0));
        label_21->setText(QApplication::translate("MainWindow", "DLeftSpc", 0));
        label_15->setText(QApplication::translate("MainWindow", "CMaxIdx", 0));
        CoreMaxQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        DobotQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_16->setText(QApplication::translate("MainWindow", "CIds", 0));
        DobotQueuedCmdLeftSpaceLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_22->setText(QApplication::translate("MainWindow", "CNextId", 0));
        CoreNextIdLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        executeDobotComandsBtn->setText(QApplication::translate("MainWindow", "ExecDtCmds", 0));
        resetDobotIndexBtn->setText(QApplication::translate("MainWindow", "ResDtId", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
