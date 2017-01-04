#ifndef DOBOT_H
#define DOBOT_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include "DobotDll.h"
#include "DobotType.h"
#include "workerThread.h"

class Dobot: public QObject
{
    Q_OBJECT

private:
    bool connectStatus;
    WorkerThread thread;

public:
    Dobot();

    void setPeriodicTaskTimer();
    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void PTPvalues(int nPtpCmd_xVal, int nPtpCmd_yVal, int nPtpCmd_zVal, int nPtpCmd_rVal);
    void gripperOpennedState(bool gripperOpenned);

public slots:
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void getPoseTimer();
    void onGetPoseTimer();

signals:
    //sygnały do używania GUI MainWindow
    void addTextToDobotConsole(QString QS_msg); //dodawanie komunikatu do konsoli dobota
    void JointLabelText(QString QSLabelText, short sJoint);
    void AxisLabelText(QString QSAxisLabelText, char chAxis);
    void ConnectButton(bool bConnectButton);
    void deviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void DobotErrorMsgBox();
};

#endif // DOBOT_H
