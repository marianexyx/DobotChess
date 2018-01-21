#ifndef DOBOT_H
#define DOBOT_H

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include <QList>
#include <limits>
#include "DobotDll.h"
#include "DobotType.h"
#include "arduinousb.h"
#include "vars/log.h"
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"
#include "dobot/dobot_queue.h"

struct ServoArduino
{
    int64_t ID;
    bool isGripperOpen;
};

class Dobot: public QObject
{
    Q_OBJECT

private:
    DobotQueue* _pQueue;
    ArduinoUsb *_pArduinoUsb;

    short _sGrippersItemID;

    bool _connectStatus;
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM _gripperServo;

    HOMEParams _Home;
    Pose _pose;
    Point3D _lastGivenPoint, _realTimePoint;
    WAITCmd _gripperMoveDelay;

    void checkPWM();

public:
    Dobot(ArduinoUsb* pArduinoUsb, DobotQueue pQueue);

    void refreshBtn();
    void initDobot();
    void onPTPsendBtnClicked();
    void closeEvent(QCloseEvent *);

    void gripperAngle(float fDutyCycle);

    void doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump);
    bool isPointTotallyDiffrent(Point3D point);
    bool isPointDiffrentOnlyInZAxis(Point3D point);
    void gripperState(DOBOT_MOVE state);
    void wait(int nMs);
    void addCmdToList(DOBOT_MOVE Move, Point3D point = _lastGivenPoint);
    void armUpDown(DOBOT_MOVE ArmDestination, double dHeight);
    void writeMoveTypeInConsole(DOBOT_MOVE MoveState);
    bool bIsMoveInAxisRange(Point3D point);

    //metody dostępowe do pól
    void setItemInGripper(short sGrippersItemID);
    void clearGripper();

    bool isGripperEmpty() const { return _sGrippersItemID == 0 ? true : false; }
    short getItemInGripper() const { return _sGrippersItemID; }
    Point3D getHomePos();

public slots:
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void onGetPoseTimer();

signals:
    //sygnały do używania GUI MainWindow
    void addTextToConsole(QString, LOG);
    void JointLabelText(QString QSLabelText, short sJoint);
    void AxisLabelText(QString QSAxisLabelText, char chAxis);
    void RefreshDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void DobotErrorMsgBox();
    void QueueLabels(int nSpace, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);
    void setBoardDataLabels(QString, BOARD_DATA_LABELS);
    void showArduinoGripperStateList(QList<ServoArduino>);
};

#endif // DOBOT_H
