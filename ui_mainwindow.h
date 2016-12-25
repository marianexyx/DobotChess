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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QLabel *label_14;
    QLabel *label_12;
    QLabel *label_13;
    QPlainTextEdit *dobot_debug;
    QPlainTextEdit *websocket_debug;
    QPlainTextEdit *tcp_debug;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
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
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_19;
    QLineEdit *zPTPEdit;
    QLineEdit *xPTPEdit;
    QLineEdit *rPTPEdit;
    QLabel *label_17;
    QPushButton *sendBtn;
    QLabel *label_18;
    QLabel *label_20;
    QLineEdit *yPTPEdit;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *joint4Label;
    QLabel *joint1Label;
    QLabel *label_5;
    QLabel *xLabel;
    QLabel *label_4;
    QLabel *joint3Label;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *joint2Label;
    QLabel *label_2;
    QLabel *zLabel;
    QLabel *yLabel;
    QLabel *label_7;
    QLabel *rLabel;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(970, 400);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(310, 0, 651, 351));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_4->addWidget(label_14, 0, 0, 1, 1);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_4->addWidget(label_12, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_4->addWidget(label_13, 0, 2, 1, 1);

        dobot_debug = new QPlainTextEdit(groupBox_4);
        dobot_debug->setObjectName(QStringLiteral("dobot_debug"));
        dobot_debug->setReadOnly(true);
        dobot_debug->setOverwriteMode(false);

        gridLayout_4->addWidget(dobot_debug, 1, 0, 1, 1);

        websocket_debug = new QPlainTextEdit(groupBox_4);
        websocket_debug->setObjectName(QStringLiteral("websocket_debug"));

        gridLayout_4->addWidget(websocket_debug, 1, 1, 1, 1);

        tcp_debug = new QPlainTextEdit(groupBox_4);
        tcp_debug->setObjectName(QStringLiteral("tcp_debug"));

        gridLayout_4->addWidget(tcp_debug, 1, 2, 1, 1);

        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 0, 291, 351));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
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


        gridLayout_5->addWidget(groupBox, 1, 1, 1, 1);

        groupBox_3 = new QGroupBox(groupBox_5);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_19 = new QLabel(groupBox_3);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_3->addWidget(label_19, 2, 0, 1, 1);

        zPTPEdit = new QLineEdit(groupBox_3);
        zPTPEdit->setObjectName(QStringLiteral("zPTPEdit"));
        zPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(zPTPEdit, 2, 1, 1, 1);

        xPTPEdit = new QLineEdit(groupBox_3);
        xPTPEdit->setObjectName(QStringLiteral("xPTPEdit"));
        xPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(xPTPEdit, 0, 1, 1, 1);

        rPTPEdit = new QLineEdit(groupBox_3);
        rPTPEdit->setObjectName(QStringLiteral("rPTPEdit"));
        rPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(rPTPEdit, 3, 1, 1, 1);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_3->addWidget(label_17, 0, 0, 1, 1);

        sendBtn = new QPushButton(groupBox_3);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));
        sendBtn->setEnabled(false);

        gridLayout_3->addWidget(sendBtn, 5, 1, 1, 1);

        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_3->addWidget(label_18, 1, 0, 1, 1);

        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_3->addWidget(label_20, 3, 0, 1, 1);

        yPTPEdit = new QLineEdit(groupBox_3);
        yPTPEdit->setObjectName(QStringLiteral("yPTPEdit"));
        yPTPEdit->setEnabled(false);

        gridLayout_3->addWidget(yPTPEdit, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 4, 1, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(groupBox_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        joint4Label = new QLabel(groupBox_2);
        joint4Label->setObjectName(QStringLiteral("joint4Label"));

        gridLayout->addWidget(joint4Label, 1, 4, 1, 1);

        joint1Label = new QLabel(groupBox_2);
        joint1Label->setObjectName(QStringLiteral("joint1Label"));
        joint1Label->setEnabled(true);

        gridLayout->addWidget(joint1Label, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        xLabel = new QLabel(groupBox_2);
        xLabel->setObjectName(QStringLiteral("xLabel"));
        xLabel->setEnabled(true);

        gridLayout->addWidget(xLabel, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        joint3Label = new QLabel(groupBox_2);
        joint3Label->setObjectName(QStringLiteral("joint3Label"));

        gridLayout->addWidget(joint3Label, 1, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 3, 1, 1);

        joint2Label = new QLabel(groupBox_2);
        joint2Label->setObjectName(QStringLiteral("joint2Label"));

        gridLayout->addWidget(joint2Label, 0, 4, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        zLabel = new QLabel(groupBox_2);
        zLabel->setObjectName(QStringLiteral("zLabel"));

        gridLayout->addWidget(zLabel, 3, 1, 1, 1);

        yLabel = new QLabel(groupBox_2);
        yLabel->setObjectName(QStringLiteral("yLabel"));

        gridLayout->addWidget(yLabel, 2, 4, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        rLabel = new QLabel(groupBox_2);
        rLabel->setObjectName(QStringLiteral("rLabel"));

        gridLayout->addWidget(rLabel, 3, 4, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 3, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 0, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 970, 18));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Logs", 0));
        label_14->setText(QApplication::translate("MainWindow", "Arm log", 0));
        label_12->setText(QApplication::translate("MainWindow", "WebSockets", 0));
        label_13->setText(QApplication::translate("MainWindow", "TCP socket", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Dobot Arm", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Device", 0));
        DeviceNameLabel->setText(QString());
        DeviceInfoLabel->setText(QString());
        connectBtn->setText(QApplication::translate("MainWindow", "Connect", 0));
        label_9->setText(QApplication::translate("MainWindow", "Device SN:", 0));
        deviceSNLabel->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Device Name:", 0));
        label_11->setText(QApplication::translate("MainWindow", "Device information:", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "PTP", 0));
        label_19->setText(QApplication::translate("MainWindow", "Z", 0));
        label_17->setText(QApplication::translate("MainWindow", "X", 0));
        sendBtn->setText(QApplication::translate("MainWindow", "Send", 0));
        label_18->setText(QApplication::translate("MainWindow", "Y", 0));
        label_20->setText(QApplication::translate("MainWindow", "R", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Pose", 0));
        joint4Label->setText(QApplication::translate("MainWindow", "0", 0));
        joint1Label->setText(QApplication::translate("MainWindow", "0", 0));
        label_5->setText(QApplication::translate("MainWindow", "J4", 0));
        xLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_4->setText(QApplication::translate("MainWindow", "J3", 0));
        joint3Label->setText(QApplication::translate("MainWindow", "0", 0));
        label->setText(QApplication::translate("MainWindow", "J1", 0));
        label_3->setText(QApplication::translate("MainWindow", "J2", 0));
        label_6->setText(QApplication::translate("MainWindow", "Y", 0));
        joint2Label->setText(QApplication::translate("MainWindow", "0", 0));
        label_2->setText(QApplication::translate("MainWindow", "X", 0));
        zLabel->setText(QApplication::translate("MainWindow", "0", 0));
        yLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_7->setText(QApplication::translate("MainWindow", "Z", 0));
        rLabel->setText(QApplication::translate("MainWindow", "0", 0));
        label_8->setText(QApplication::translate("MainWindow", "R", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
