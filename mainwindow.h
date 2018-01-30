#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QScrollBar>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "DobotDll.h"
#include "DobotType.h"
#include "websockets.h"
#include "dobot.h"
#include "chess.h"
#include "client.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Websockets* pWebSockets, Chessboard* pBoardMain,
                        Chessboard* pBoardRemoved,  Chessboard *pBoardChenard, TCPMsgs* pTCPMsg,
                        ArduinoUsb* pUsb, Dobot* pDobot, Chess* pChess, Client* pClient,
                        QWidget* parent = 0);

    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;

    Dobot* _pDobot;
    Websockets* _pWebSockets;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Chessboard* _pBoardChenard;
    TCPMsgs* _pTCPmsg;
    ArduinoUsb* _pUsb;
    Client* _pClient;
    Chess* _pChess;

    void initControl();

private slots:
    void onChangedMode();
    void onJOGCtrlBtnPressed(int index);
    void onJOGCtrlBtnReleased();

    //sloty do używania GUI MainWindow
    void writeInConsole(QString QStrMsg, LOG msgType);
    void setBoardDataLabel(QString QStrLabel, BOARD_DATA_LABEL labelType);
    void showBoardInUI(QString QStrBoard, BOARD BoardType);
    void clearBoardInUI();
    void showLegalMoves(QStringList legalMoves);
    void showHistoryMoves(QStringList historyMoves);

    //dobot slots
    void setDobot_UI_PTE_Validators();
    void setJointLabelText(QString QSJointLabelText, short sJoint);
    void setAxisLabelText(QString QSAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void setDeviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void showDobotErrorMsgBox();
    void on_emulatePlayerMsgLineEdit_textChanged(const QString &textChanged);
    void on_sendSimulatedMsgBtn_clicked();
    void setQueueLabels(int nSpaceLeft, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);
    void showActualDobotQueuedCmdIDList(QList<DobotMove> list);
    void showArduinoGripperStateList(QList<ServoArduino> list);

    //websocket slots
    void on_calibrateBtn_clicked();
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_resetDobotIndexBtn_clicked();
    void on_AIBtn_clicked();
    void on_AIEnemySendBtn_clicked();
    void showClientsList(QList<Client> list);

    //arduino usb slots
    void updatePortsComboBox(int nUsbPorst);
    void on_portsComboBox_currentIndexChanged(int index);
    void on_reloadPortsBtn_clicked();
    void on_sendUsbBtn_clicked();
    void on_openGripperBtn_clicked();
    void on_closeGripperBtn_clicked();
    void on_startGmPosBtn_clicked();
    void on_startDtPosBtn_clicked();
    void on_SimulateFromUsbBtn_clicked();
    void on_sendTcpBtn_clicked();
    void on_sendTcpLineEdit_textChanged(const QString &textChanged);
    void on_SimulateFromUsbLineEdit_textChanged(const QString &textChanged);
    void on_homeBtn_clicked();
    void on_usbCmdLine_textChanged(const QString &arg1);
    void on_middleAboveBtn_clicked();
    void on_sendBtn_clicked();
    void on_xPTPEdit_textChanged(const QString &arg1);
    void on_yPTPEdit_textChanged(const QString &arg1);
    void on_zPTPEdit_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
