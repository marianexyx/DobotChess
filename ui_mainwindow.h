/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
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
#include <QtWidgets/QSpacerItem>
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
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *dobot_debug;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_9;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLineEdit *xPTPEdit;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *yPTPEdit;
    QLabel *label_19;
    QLineEdit *zPTPEdit;
    QLabel *label_20;
    QLineEdit *rPTPEdit;
    QLabel *label_12;
    QLineEdit *servo1GripperEdit;
    QSpacerItem *verticalSpacer;
    QPushButton *sendBtn;
    QLineEdit *servo2GripperEdit;
    QLabel *label_13;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout_2;
    QLabel *label_14;
    QLabel *DobotQueuedIndexLabel;
    QLabel *label_15;
    QLabel *CoreMaxQueuedIndexLabel;
    QLabel *label_21;
    QLabel *DobotQueuedCmdLeftSpaceLabel;
    QLabel *label_16;
    QLabel *CoreIndexAmountlabel;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *DeviceNameLabel;
    QLabel *DeviceInfoLabel;
    QPushButton *connectBtn;
    QLabel *label_9;
    QLabel *deviceSNLabel;
    QLabel *label_10;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *joint2Label;
    QLabel *joint3Label;
    QLabel *label_5;
    QLabel *label_8;
    QLabel *zLabel;
    QLabel *label_2;
    QLabel *joint4Label;
    QLabel *yLabel;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *joint1Label;
    QLabel *label;
    QLabel *rLabel;
    QLabel *label_6;
    QLabel *xLabel;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QLineEdit *emulatePlayerMsgLineEdit;
    QCheckBox *serviceCheckBox;
    QSpacerItem *verticalSpacer_4;
    QPushButton *sendSimulatedMsgBtn;
    QGroupBox *JOGgroupBox;
    QFormLayout *formLayout;
    QComboBox *teachMode;
    QPushButton *gripperBtn;
    QSpacerItem *verticalSpacer_3;
    QPushButton *homeBtn;
    QPushButton *upBtn;
    QPushButton *downBtn;
    QPushButton *baseAngleAddBtn;
    QPushButton *baseAngleSubBtn;
    QPushButton *longArmAddBtn;
    QPushButton *longArmSubBtn;
    QPushButton *shortArmAddBtn;
    QPushButton *shortArmSubBtn;
    QPushButton *rHeadAddBtn;
    QPushButton *rHeadSubBtn;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_7;
    QPlainTextEdit *websocket_debug;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_8;
    QPlainTextEdit *tcp_debug;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1009, 581);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        verticalLayout = new QVBoxLayout(groupBox_7);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        dobot_debug = new QPlainTextEdit(groupBox_7);
        dobot_debug->setObjectName(QStringLiteral("dobot_debug"));
        dobot_debug->setReadOnly(true);
        dobot_debug->setOverwriteMode(false);

        verticalLayout->addWidget(dobot_debug);


        gridLayout_4->addWidget(groupBox_7, 0, 2, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        gridLayout_9 = new QGridLayout(groupBox_5);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        groupBox_3 = new QGroupBox(groupBox_5);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        xPTPEdit = new QLineEdit(groupBox_3);
        xPTPEdit->setObjectName(QStringLiteral("xPTPEdit"));
        xPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(xPTPEdit, 0, 1, 1, 1);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_3->addWidget(label_17, 0, 0, 1, 1);

        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_3->addWidget(label_18, 1, 0, 1, 1);

        yPTPEdit = new QLineEdit(groupBox_3);
        yPTPEdit->setObjectName(QStringLiteral("yPTPEdit"));
        yPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(yPTPEdit, 1, 1, 1, 1);

        label_19 = new QLabel(groupBox_3);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_3->addWidget(label_19, 2, 0, 1, 1);

        zPTPEdit = new QLineEdit(groupBox_3);
        zPTPEdit->setObjectName(QStringLiteral("zPTPEdit"));
        zPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(zPTPEdit, 2, 1, 1, 1);

        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_3->addWidget(label_20, 3, 0, 1, 1);

        rPTPEdit = new QLineEdit(groupBox_3);
        rPTPEdit->setObjectName(QStringLiteral("rPTPEdit"));
        rPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(rPTPEdit, 3, 1, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_3->addWidget(label_12, 4, 0, 1, 1);

        servo1GripperEdit = new QLineEdit(groupBox_3);
        servo1GripperEdit->setObjectName(QStringLiteral("servo1GripperEdit"));
        servo1GripperEdit->setEnabled(false);

        gridLayout_3->addWidget(servo1GripperEdit, 4, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 6, 1, 1, 1);

        sendBtn = new QPushButton(groupBox_3);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setEnabled(false);

        gridLayout_3->addWidget(sendBtn, 7, 1, 1, 1);

        servo2GripperEdit = new QLineEdit(groupBox_3);
        servo2GripperEdit->setObjectName(QStringLiteral("servo2GripperEdit"));
        servo2GripperEdit->setEnabled(false);

        gridLayout_3->addWidget(servo2GripperEdit, 5, 1, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_3->addWidget(label_13, 5, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_3, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_5);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        formLayout_2 = new QFormLayout(groupBox_4);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_14);

        DobotQueuedIndexLabel = new QLabel(groupBox_4);
        DobotQueuedIndexLabel->setObjectName(QStringLiteral("DobotQueuedIndexLabel"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, DobotQueuedIndexLabel);

        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_15);

        CoreMaxQueuedIndexLabel = new QLabel(groupBox_4);
        CoreMaxQueuedIndexLabel->setObjectName(QStringLiteral("CoreMaxQueuedIndexLabel"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, CoreMaxQueuedIndexLabel);

        label_21 = new QLabel(groupBox_4);
        label_21->setObjectName(QStringLiteral("label_21"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_21);

        DobotQueuedCmdLeftSpaceLabel = new QLabel(groupBox_4);
        DobotQueuedCmdLeftSpaceLabel->setObjectName(QStringLiteral("DobotQueuedCmdLeftSpaceLabel"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, DobotQueuedCmdLeftSpaceLabel);

        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_16);

        CoreIndexAmountlabel = new QLabel(groupBox_4);
        CoreIndexAmountlabel->setObjectName(QStringLiteral("CoreIndexAmountlabel"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, CoreIndexAmountlabel);


        gridLayout_9->addWidget(groupBox_4, 2, 2, 1, 1);

        groupBox = new QGroupBox(groupBox_5);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        DeviceNameLabel = new QLabel(groupBox);
        DeviceNameLabel->setObjectName(QStringLiteral("DeviceNameLabel"));
        DeviceNameLabel->setFrameShape(QFrame::Panel);
        DeviceNameLabel->setTextFormat(Qt::AutoText);

        gridLayout_2->addWidget(DeviceNameLabel, 3, 0, 1, 1);

        DeviceInfoLabel = new QLabel(groupBox);
        DeviceInfoLabel->setObjectName(QStringLiteral("DeviceInfoLabel"));
        DeviceInfoLabel->setFrameShape(QFrame::Panel);
        DeviceInfoLabel->setFrameShadow(QFrame::Plain);

        gridLayout_2->addWidget(DeviceInfoLabel, 5, 0, 1, 1);

        connectBtn = new QPushButton(groupBox);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        gridLayout_2->addWidget(connectBtn, 7, 0, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        deviceSNLabel = new QLabel(groupBox);
        deviceSNLabel->setObjectName(QStringLiteral("deviceSNLabel"));
        deviceSNLabel->setFrameShape(QFrame::Panel);

        gridLayout_2->addWidget(deviceSNLabel, 1, 0, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 2, 0, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 6, 0, 1, 1);


        gridLayout_9->addWidget(groupBox, 1, 0, 2, 1);

        groupBox_2 = new QGroupBox(groupBox_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        joint2Label = new QLabel(groupBox_2);
        joint2Label->setObjectName(QStringLiteral("joint2Label"));

        gridLayout->addWidget(joint2Label, 0, 3, 1, 1);

        joint3Label = new QLabel(groupBox_2);
        joint3Label->setObjectName(QStringLiteral("joint3Label"));

        gridLayout->addWidget(joint3Label, 1, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 2, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 3, 2, 1, 1);

        zLabel = new QLabel(groupBox_2);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        gridLayout->addWidget(zLabel, 3, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        joint4Label = new QLabel(groupBox_2);
        joint4Label->setObjectName(QStringLiteral("joint4Label"));

        gridLayout->addWidget(joint4Label, 1, 3, 1, 1);

        yLabel = new QLabel(groupBox_2);
        yLabel->setObjectName(QStringLiteral("yLabel"));

        gridLayout->addWidget(yLabel, 2, 3, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        joint1Label = new QLabel(groupBox_2);
        joint1Label->setObjectName(QStringLiteral("joint1Label"));
        joint1Label->setEnabled(true);

        gridLayout->addWidget(joint1Label, 0, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        rLabel = new QLabel(groupBox_2);
        rLabel->setObjectName(QStringLiteral("rLabel"));

        gridLayout->addWidget(rLabel, 3, 3, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 2, 1, 1);

        xLabel = new QLabel(groupBox_2);
        xLabel->setObjectName(QStringLiteral("xLabel"));
        xLabel->setEnabled(true);

        gridLayout->addWidget(xLabel, 2, 1, 1, 1);


        gridLayout_9->addWidget(groupBox_2, 2, 1, 1, 1);

        groupBox_6 = new QGroupBox(groupBox_5);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        emulatePlayerMsgLineEdit = new QLineEdit(groupBox_6);
        emulatePlayerMsgLineEdit->setObjectName(QStringLiteral("emulatePlayerMsgLineEdit"));
        emulatePlayerMsgLineEdit->setEnabled(false);

        gridLayout_6->addWidget(emulatePlayerMsgLineEdit, 0, 0, 1, 1);

        serviceCheckBox = new QCheckBox(groupBox_6);
        serviceCheckBox->setObjectName(QStringLiteral("serviceCheckBox"));
        serviceCheckBox->setEnabled(false);

        gridLayout_6->addWidget(serviceCheckBox, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_4, 2, 0, 1, 1);

        sendSimulatedMsgBtn = new QPushButton(groupBox_6);
        sendSimulatedMsgBtn->setObjectName(QStringLiteral("sendSimulatedMsgBtn"));
        sendSimulatedMsgBtn->setEnabled(false);
        sendSimulatedMsgBtn->setAutoDefault(true);

        gridLayout_6->addWidget(sendSimulatedMsgBtn, 4, 0, 1, 1);


        gridLayout_9->addWidget(groupBox_6, 1, 1, 1, 2);

        JOGgroupBox = new QGroupBox(groupBox_5);
        JOGgroupBox->setObjectName(QStringLiteral("JOGgroupBox"));
        formLayout = new QFormLayout(JOGgroupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        teachMode = new QComboBox(JOGgroupBox);
        teachMode->setObjectName(QStringLiteral("teachMode"));
        teachMode->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::LabelRole, teachMode);

        gripperBtn = new QPushButton(JOGgroupBox);
        gripperBtn->setObjectName(QStringLiteral("gripperBtn"));
        gripperBtn->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, gripperBtn);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(1, QFormLayout::LabelRole, verticalSpacer_3);

        homeBtn = new QPushButton(JOGgroupBox);
        homeBtn->setObjectName(QStringLiteral("homeBtn"));
        homeBtn->setEnabled(false);

        formLayout->setWidget(1, QFormLayout::FieldRole, homeBtn);

        upBtn = new QPushButton(JOGgroupBox);
        upBtn->setObjectName(QStringLiteral("upBtn"));
        upBtn->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::LabelRole, upBtn);

        downBtn = new QPushButton(JOGgroupBox);
        downBtn->setObjectName(QStringLiteral("downBtn"));
        downBtn->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, downBtn);

        baseAngleAddBtn = new QPushButton(JOGgroupBox);
        baseAngleAddBtn->setObjectName(QStringLiteral("baseAngleAddBtn"));
        baseAngleAddBtn->setEnabled(false);

        formLayout->setWidget(3, QFormLayout::LabelRole, baseAngleAddBtn);

        baseAngleSubBtn = new QPushButton(JOGgroupBox);
        baseAngleSubBtn->setObjectName(QStringLiteral("baseAngleSubBtn"));
        baseAngleSubBtn->setEnabled(false);

        formLayout->setWidget(3, QFormLayout::FieldRole, baseAngleSubBtn);

        longArmAddBtn = new QPushButton(JOGgroupBox);
        longArmAddBtn->setObjectName(QStringLiteral("longArmAddBtn"));
        longArmAddBtn->setEnabled(false);

        formLayout->setWidget(5, QFormLayout::LabelRole, longArmAddBtn);

        longArmSubBtn = new QPushButton(JOGgroupBox);
        longArmSubBtn->setObjectName(QStringLiteral("longArmSubBtn"));
        longArmSubBtn->setEnabled(false);

        formLayout->setWidget(5, QFormLayout::FieldRole, longArmSubBtn);

        shortArmAddBtn = new QPushButton(JOGgroupBox);
        shortArmAddBtn->setObjectName(QStringLiteral("shortArmAddBtn"));
        shortArmAddBtn->setEnabled(false);

        formLayout->setWidget(6, QFormLayout::LabelRole, shortArmAddBtn);

        shortArmSubBtn = new QPushButton(JOGgroupBox);
        shortArmSubBtn->setObjectName(QStringLiteral("shortArmSubBtn"));
        shortArmSubBtn->setEnabled(false);

        formLayout->setWidget(6, QFormLayout::FieldRole, shortArmSubBtn);

        rHeadAddBtn = new QPushButton(JOGgroupBox);
        rHeadAddBtn->setObjectName(QStringLiteral("rHeadAddBtn"));
        rHeadAddBtn->setEnabled(false);

        formLayout->setWidget(7, QFormLayout::LabelRole, rHeadAddBtn);

        rHeadSubBtn = new QPushButton(JOGgroupBox);
        rHeadSubBtn->setObjectName(QStringLiteral("rHeadSubBtn"));
        rHeadSubBtn->setEnabled(false);

        formLayout->setWidget(7, QFormLayout::FieldRole, rHeadSubBtn);


        gridLayout_9->addWidget(JOGgroupBox, 0, 1, 1, 2);

        groupBox->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        JOGgroupBox->raise();
        groupBox_6->raise();
        groupBox_4->raise();

        gridLayout_4->addWidget(groupBox_5, 0, 0, 1, 2);

        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        gridLayout_7 = new QGridLayout(groupBox_8);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        websocket_debug = new QPlainTextEdit(groupBox_8);
        websocket_debug->setObjectName(QStringLiteral("websocket_debug"));

        gridLayout_7->addWidget(websocket_debug, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_8, 0, 3, 1, 1);

        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        gridLayout_8 = new QGridLayout(groupBox_9);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        tcp_debug = new QPlainTextEdit(groupBox_9);
        tcp_debug->setObjectName(QStringLiteral("tcp_debug"));

        gridLayout_8->addWidget(tcp_debug, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_9, 0, 4, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_5->raise();
        groupBox_7->raise();
        groupBox_8->raise();
        groupBox_9->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1009, 18));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(xPTPEdit, yPTPEdit);
        QWidget::setTabOrder(yPTPEdit, zPTPEdit);
        QWidget::setTabOrder(zPTPEdit, rPTPEdit);
        QWidget::setTabOrder(rPTPEdit, sendBtn);
        QWidget::setTabOrder(sendBtn, connectBtn);
        QWidget::setTabOrder(connectBtn, websocket_debug);

        retranslateUi(MainWindow);

        sendSimulatedMsgBtn->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Arm log", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Dobot Arm", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Service PTP", 0));
        label_17->setText(QApplication::translate("MainWindow", "X", 0));
        label_18->setText(QApplication::translate("MainWindow", "Y", 0));
        label_19->setText(QApplication::translate("MainWindow", "Z", 0));
        label_20->setText(QApplication::translate("MainWindow", "R", 0));
        label_12->setText(QApplication::translate("MainWindow", "s1", 0));
        sendBtn->setText(QApplication::translate("MainWindow", "Send", 0));
        label_13->setText(QApplication::translate("MainWindow", "s2", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Queue", 0));
        label_14->setText(QApplication::translate("MainWindow", "DIndex", 0));
        DobotQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_15->setText(QApplication::translate("MainWindow", "CMaxIdx", 0));
        CoreMaxQueuedIndexLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_21->setText(QApplication::translate("MainWindow", "DLeftSpc", 0));
        DobotQueuedCmdLeftSpaceLabel->setText(QApplication::translate("MainWindow", "-1", 0));
        label_16->setText(QApplication::translate("MainWindow", "CIds", 0));
        CoreIndexAmountlabel->setText(QApplication::translate("MainWindow", "-1", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Device", 0));
        DeviceNameLabel->setText(QString());
        DeviceInfoLabel->setText(QString());
        connectBtn->setText(QApplication::translate("MainWindow", "Connect", 0));
        label_9->setText(QApplication::translate("MainWindow", "Device SN:", 0));
        deviceSNLabel->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Device Name:", 0));
        label_11->setText(QApplication::translate("MainWindow", "Device information:", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Pose", 0));
        label_4->setText(QApplication::translate("MainWindow", "J3", 0));
        joint2Label->setText(QApplication::translate("MainWindow", "0", 0));
        joint3Label->setText(QApplication::translate("MainWindow", "0", 0));
        label_5->setText(QApplication::translate("MainWindow", "J4", 0));
        label_8->setText(QApplication::translate("MainWindow", "R", 0));
        zLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_2->setText(QApplication::translate("MainWindow", "X", 0));
        joint4Label->setText(QApplication::translate("MainWindow", "0", 0));
        yLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_7->setText(QApplication::translate("MainWindow", "Z", 0));
        label_3->setText(QApplication::translate("MainWindow", "J2", 0));
        joint1Label->setText(QApplication::translate("MainWindow", "0", 0));
        label->setText(QApplication::translate("MainWindow", "J1", 0));
        rLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_6->setText(QApplication::translate("MainWindow", "Y", 0));
        xLabel->setText(QApplication::translate("MainWindow", "0", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Simulate player msg ", 0));
        serviceCheckBox->setText(QApplication::translate("MainWindow", "service message", 0));
        sendSimulatedMsgBtn->setText(QApplication::translate("MainWindow", "Send", 0));
        JOGgroupBox->setTitle(QApplication::translate("MainWindow", "JOG", 0));
        teachMode->clear();
        teachMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Joint", 0)
         << QApplication::translate("MainWindow", "Axis", 0)
        );
        gripperBtn->setText(QApplication::translate("MainWindow", "Open gripper", 0));
        homeBtn->setText(QApplication::translate("MainWindow", "HOME", 0));
        upBtn->setText(QApplication::translate("MainWindow", "Up", 0));
        downBtn->setText(QApplication::translate("MainWindow", "Down", 0));
        baseAngleAddBtn->setText(QApplication::translate("MainWindow", "J1+", 0));
        baseAngleSubBtn->setText(QApplication::translate("MainWindow", "J1-", 0));
        longArmAddBtn->setText(QApplication::translate("MainWindow", "J2+", 0));
        longArmSubBtn->setText(QApplication::translate("MainWindow", "J2-", 0));
        shortArmAddBtn->setText(QApplication::translate("MainWindow", "J3+", 0));
        shortArmSubBtn->setText(QApplication::translate("MainWindow", "J3-", 0));
        rHeadAddBtn->setText(QApplication::translate("MainWindow", "J4+", 0));
        rHeadSubBtn->setText(QApplication::translate("MainWindow", "J4-", 0));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "WebSockets", 0));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "TCP socket", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
