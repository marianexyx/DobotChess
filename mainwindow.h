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
#include "igorbot.h"
#include "webchess.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Websockets *pWebSockets, Chessboard *pChessboard,
                        TCPMsgs *pTCPmsg, ArduinoUsb *pArduinoUsb, Dobot *pDobotArm,
                        IgorBot *pIgorBot, WebChess *pWebChess, QWidget *parent = 0);

    virtual ~MainWindow(); //tutaj virtual?

/*protected:
    void closeEvent(QCloseEvent *);*/ //TODO: to się może chyba przydać

public slots:

    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);

private:
    Ui::MainWindow *ui;

    Dobot *_pDobotArm;
    Websockets *_pWebSockets;
    Chessboard *_pChessboard;
    TCPMsgs *_pTCPmsg;
    ArduinoUsb *_pArduinoUsb;
    IgorBot *_pIgorBot;
    WebChess *_pWebChess;

    void refreshBtn();
    void initDobot();
    void initControl();
    QString checkMoveForTcp(QString QsFT); //sprawdź czy w line edicie jest wpisany ruch w poprawnym formacie

private slots:
    void onChangedMode();
    void onJOGCtrlBtnPressed(int index);
    void onJOGCtrlBtnReleased();
    void onPTPsendBtnClicked();

    /*---sloty do używania GUI MainWindow---*/
    void writeInConsole(QString QStrMsg, LOG msgType);
    void changeWindowtitle(QString title);
    void setBoardDataLabels(QString QStrLabel, BOARD_DATA_LABELS labelType);
    void showBoard(QString QStrBoard);
    void showLegalMoves(QStringList legalMoves);
    void showHistoryMoves(QStringList historyMoves);

    //dobot slots
    void setDobotValidators();
    void setJointLabelText(QString QSJointLabelText, short sJoint);
    void setAxisLabelText(QString QSAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void setDeviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void showDobotErrorMsgBox();
    void on_emulatePlayerMsgLineEdit_textChanged(const QString &textChanged);
    void on_sendSimulatedMsgBtn_clicked();
    void setQueueLabels(int nSpace, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);

    //websocket slots
    void on_calibrateBtn_clicked();
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_resetDobotIndexBtn_clicked();
    void on_executeDobotComandsBtn_clicked();
    void on_AIBtn_clicked();
    void on_AIEnemySendBtn_clicked();

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
};

#endif // MAINWINDOW_H
