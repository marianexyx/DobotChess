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
#include <QTimer>
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "xml_reader.h"
#include "websockets.h"
#include "dobot.h"
#include "chess.h"
#include "client.h"
#include "piece_controller.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Chess* pChess, QWidget* parent = 0);

    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;

    Dobot* _pDobot;
    PieceController* _pPieceController;
    Websockets* _pWebSockets;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Chessboard* _pBoardChenard;
    TCPMsgs* _pTCPMsg;
    Clients* _pClientsList;
    Chess* _pChess;

    QTimer* _titleFormTimer;

    void initControl();

private slots:
    //slots for GUI MainWindow
    void writeInConsole(QString QStrMsg, LOG TypeOfMsg);
    void setBoardDataLabel(QString QStrLabel, BOARD_DATA_LABEL LabelType);
    void showImaginaryBoardInUI(QString QStrBoard);
    void showRealBoardInUI();
    void clearBoardInUI();
    void showLegalMovesInUI(QStringList legalMoves);
    void showHistoryMovesInUI(QStringList historyMoves);
    void changeWindowTitle();

    //dobot slots
    void onChangedMode();
    void onJOGCtrlBtnPressed(int nID);
    void onJOGCtrlBtnReleased();
    void setDobotPTEValidatorsInUI();
    void setJointLabelText(QString QStrJointLabelText, short sJoint);
    void setAxisLabelText(QString QStrAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void setDeviceLabels(QString QStrDeviceSN, QString QStrDeviceName,
                         QString QStrDeviceVersion);
    void showDobotErrorMsgBox();
    void on_emulatePlayerMsgLineEdit_textChanged(const QString& QStrTextChanged);
    void on_sendSimulatedMsgBtn_clicked();
    void setQueueLabels(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                        int nCoreIdLeft, uint64_t un64CoreNextId);
    void showQueuedArmCmdsOnCore();
    void showSentArmCmdsToDobot();

    //websocket slots
    void on_calibrateBtn_clicked();
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_resetDobotIndexBtn_clicked();
    void showClientsListInUI(QList<Client> list);

    void on_openGripperBtn_clicked();
    void on_closeGripperBtn_clicked();
    void on_startGmPosBtn_clicked();
    void on_startDtPosBtn_clicked();
    void on_sendTcpBtn_clicked();
    void on_sendTcpLineEdit_textChanged(const QString& QStrTextChanged);
    void on_homeBtn_clicked();
    void on_middleAboveBtn_clicked();
    void on_sendPointBtn_clicked();
    void on_xPTPEdit_textChanged(const QString& QStrTextChanged);
    void on_yPTPEdit_textChanged(const QString& QStrTextChanged);
    void on_zPTPEdit_textChanged(const QString& QStrTextChanged);
    void on_retreatLeftBtn_clicked();
    void on_retreatRightBtn_clicked();
};

#endif // MAINWINDOW_H
