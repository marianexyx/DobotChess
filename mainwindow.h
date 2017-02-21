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

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(WebTable *pWebTable, Websockets *pWebSockets, Chessboard *pChessboard,
                        TCPMsgs *pTCPmsg, Dobot *pDobotArm, Chess *pChess, QWidget *parent = 0);

    virtual ~MainWindow(); //tutaj virtual?

/*protected:
    void closeEvent(QCloseEvent *);*/ //TODO: to się może chyba przydać

private:
    Ui::MainWindow *ui;

    Dobot *_pDobotArm;
    WebTable *_pWebTable;
    Websockets *_pWebSockets;
    Chessboard *_pChessboard;
    TCPMsgs *_pTCPmsg;
    Chess *_pChess;

    void refreshBtn();
    void initDobot();
    void initControl();

private slots:
    void onChangedMode();
    void onJOGCtrlBtnPressed(int index);
    void onJOGCtrlBtnReleased();
    void onPTPsendBtnClicked();

    /*---sloty do używania GUI MainWindow---*/
    //dobot slots
    void writeInDobotConsole(QString QStrMsg);
    void setDobotValidators();
    void setJointLabelText(QString QSJointLabelText, short sJoint);
    void setAxisLabelText(QString QSAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void setDeviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void showDobotErrorMsgBox();
    void on_emulatePlayerMsgLineEdit_textChanged(const QString &textChanged);
    void on_sendSimulatedMsgBtn_clicked();
    void setQueueLabels(int nSpace, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);

    //tcp slots
    void writeInTcpConsole(QString QS_msg);

    //websocket slots
    void writeInWebsocketConsole(QString QStrMsg);
    void on_gripperBtn_clicked();
    void on_homeBtn_clicked();
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_resetDobotIndexBtn_clicked();
    void on_executeDobotComandsBtn_clicked();
    void on_startPosBtn_clicked();
    void on_AIBtn_clicked();
};

#endif // MAINWINDOW_H
