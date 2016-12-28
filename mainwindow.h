#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
/*#include <QSerialPort>
#include <QSerialPortInfo>*/
/*#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>*/
/*#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>*/
#include <QScrollBar>
#include <QTcpSocket>
#include <QAbstractSocket>
/*#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"*/
#include "DobotDll.h"
#include "DobotType.h"
#include "Thread.h"
//#include "chessboard.h"
//#include "tcpmsgs.h"
#include "websockets.h"
//#include "webtable.h"
#include "dobot.h"
#include "chess.h"

/*QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)*/

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(WebTable *pWebTable, Websockets *pWebSockets, QWidget *parent = 0); //konstruktor. pobiera port tcp.
    //dobrze by było nadać temu portowi jakiś przydomek typu "tcp_port" by się wyróżniał od portu usb...
    //...nie udało mi się to ostatnim razem

    virtual ~MainWindow(); //tutaj virtual?

/*protected:
    void closeEvent(QCloseEvent *);*/

private:
    Ui::MainWindow *ui; //??? o co tu chodzi

    Dobot *_pDobotArm;
    WebTable *_pWebTable;
    Websockets *_pWebSockets;
    //Dobot *DobotArm; //TODO: ?? nie mogę tworzyć tego obiektu tam gdzie inne, bo mam błędy, przez...
    //... co muszę tworzyć obiektyw tutaj. tak wogle można???

    void refreshBtn();
    void initDobot();
    //void initControl();

private slots:
    //void onChangedMode();
    //void onConnectDobot();
    //void onJOGCtrlBtnPressed(int index);
    //void onJOGCtrlBtnReleased();
    //void onPeriodicTaskTimer();
    void onPTPsendBtnClicked();

    /*---sloty do używania GUI MainWindow---*/
    //dobot slots
    void writeInDobotConsole(QString QStrMsg);
    void setDobotValidators();
    void setJointLabelText(QString QSJointLabelText, short sJoint);
    void setAxisLabelText(QString QSAxisLabelText, char chAxis);
    void setConnectButtonText(bool bConnectButton);
    void setDeviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void showDobotErrorMsgBox();

    //tcp slots
    void writeInTcpConsole(QString QS_msg);

    //websocket slots
    void writeInWebsocketConsole(QString QStrMsg);
};

#endif // MAINWINDOW_H
