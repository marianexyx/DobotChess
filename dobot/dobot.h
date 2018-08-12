#ifndef DOBOT_H
#define DOBOT_H

#pragma once
#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QValidator>
#include <QList>
#include <limits>
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "vars/const_flags.h"
#include "vars/intermediate_points.h"
#include "vars/log.h"
#include "vars/vertical_move.h"
#include "xml_reader.h"
#include "dobot/vars/dobot_move_types.h"
#include "dobot/dobot_gripper.h"
#include "dobot/dobot_queue.h"

//todo: add force stop arm methods
class Dobot: public QObject
{
    Q_OBJECT

private:
    DobotGripper* _pGripper;
    DobotQueue* _pQueue;

    const int _ARM_MAX_VELOCITY, _ARM_MAX_ACCELERATION;
    short _sItemIDInGripper;
    bool _bConnectedToDobot;
    bool _bFirstMoveIsDone;

    IntermediatePoints _intermediatePoints;
    HOMEParams _home;
    Point3D _realTimePoint;
    Point3D _lastGivenPoint;

    void createAndStartPeriodicTimer();
    void createAndStartPoseTimer();

public:
    //future: don't add whole RealVars to dobot
    Dobot(RealVars gameConfigVars, IntermediatePoints intermediatePoints);
    ~Dobot();

    void saveActualDobotPosition();
    void initDobot();
    void onPTPsendBtnClicked();

    void queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove =
            VM_NONE, bool bEscape = false);
    bool isMoveSafe(Point3D point);
    bool isPointDiffrentOnlyInZAxis(Point3D point);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void moveArmUpOrDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight);

    void setItemInGripper(short sGrippersItemID);
    void clearGripper();

    bool isGripperEmpty() const { return _sItemIDInGripper == 0 ? true : false; }
    short getItemInGripper() const { return _sItemIDInGripper; }
    IntermediatePoints getIntermediatePoints() const { return _intermediatePoints; }
    Point3D getHomePos();
    Point3D getLastGivenPoint() const { return _lastGivenPoint; }
    QString dumpAllData();

    DobotGripper* getServoPointer() const { return _pGripper; }
    DobotQueue* getQueuePointer() const { return _pQueue; }


public slots:
    void sendMoveToArm(DobotMove move);
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void onGetPoseTimer();
    void showQueueLabelsInUI(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                             int nCoreIdLeft, uint64_t un64CoreNextId);
    void addTextToLogPTEInUI(QString QStrTxt, LOG log);
    void showQueuedArmCmdsOnCorePTE(QString QStrList) { emit this->showQueuedArmCmdsOnCore(QStrList); }
    void showSentArmCmdsToDobotPTE(QString QStrList) { emit this->showSentArmCmdsToDobot(QStrList); }

signals: //GUI mainWindow
    void addTextToLogPTE(QString, LOG);
    void JointLabelText(QString QStrLabelText, short sJoint);
    void AxisLabelText(QString QStrAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QStrDeviceSN, QString QStrDeviceName, QString QStrDeviceVersion);
    void DobotErrorMsgBox();
    void queueLabels(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                     int nCoreIdLeft, uint64_t un64CoreNextId);
    void showQueuedArmCmdsOnCore(QString);
    void showSentArmCmdsToDobot(QString);
};

#endif // DOBOT_H
