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

class Dobot: public QObject
{
    Q_OBJECT

private:
    IntermediatePoints* _pIntermediatePoints;
    DobotGripper* _pGripper;
    DobotQueue* _pQueue;

    const int _ARM_MAX_VELOCITY, _ARM_MAX_ACCELERATION;

    short _sItemIDInGripper;

    bool _bConnectedToDobot;
    bool _bFirstMoveIsDone;

    HOMEParams _home;
    Point3D _realTimePoint;
    Point3D _lastGivenPoint;

    void createAndStartPeriodicTimer();
    void createAndStartPoseTimer();

public:
    //future: don't add whole RealVars do dobot
    Dobot(RealVars gameConfigVars, IntermediatePoints* pIntermediatePoints);
    ~Dobot();

    void saveActualDobotPosition();
    void initDobot();
    void onPTPsendBtnClicked();

    void queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove =
            VM_NONE, bool bRetreat = false);
    bool isMoveSafe(Point3D point);
    bool isPointDiffrentOnlyInZAxis(Point3D point);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void armUpDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight);
    bool bIsArmConnected() const { return _bConnectedToDobot; }

    void setItemInGripper(short sGrippersItemID);
    void clearGripper();

    bool isGripperEmpty() const { return _sItemIDInGripper == 0 ? true : false; }
    short getItemInGripper() const { return _sItemIDInGripper; }
    Point3D getHomePos();
    Point3D getLastGivenPoint() const { return _lastGivenPoint; }

    IntermediatePoints* getIntermediatePoints() const { return _pIntermediatePoints; }
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
    void showQueuedArmCmdsOnCorePTE() { emit this->showQueuedArmCmdsOnCore(); }
    void showSentArmCmdsToDobotPTE() { emit this->showSentArmCmdsToDobot(); }

signals: //GUI mainWindow
    void addTextToLogPTE(QString, LOG);
    void JointLabelText(QString QStrLabelText, short sJoint);
    void AxisLabelText(QString QStrAxisLabelText, char chAxis);
    void setDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QStrDeviceSN, QString QStrDeviceName, QString QStrDeviceVersion);
    void DobotErrorMsgBox();
    void queueLabels(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                     int nCoreIdLeft, uint64_t un64CoreNextId);
    void showQueuedArmCmdsOnCore();
    void showSentArmCmdsToDobot();
};

#endif // DOBOT_H
