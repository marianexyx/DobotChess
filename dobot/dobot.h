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

//future: check this in documentation: SetAutoLevelingCmd, SetPTPPOCmd, SetLostStepParams,...
//...SetAngleSensorStaticError(+few),


class Dobot: public QObject
{
    Q_OBJECT

private:
    DobotGripper* m_pGripper;
    DobotQueue* m_pQueue;

    const int m_ARM_MAX_VELOCITY, m_ARM_MAX_ACCELERATION;
    ushort m_usItemIDInGripper;
    bool m_bConnectedToDobot;
    bool m_bFirstMoveIsDone;
    IntermediatePoints m_intermediatePoints;
    HOMEParams m_home;
    Point3D m_realTimePoint;
    Point3D m_lastGivenPoint;

    void createAndStartPeriodicTimer();
    void createAndStartPoseTimer();
    void initDobot();
    void saveActualDobotPosition();
    bool isMoveSafe(Point3D point);
    bool isPointDiffrentOnlyInZAxis(Point3D point);
    bool isGripperEmpty() const { return m_usItemIDInGripper == 0 ? true : false; }

private slots:
    void onPeriodicTaskTimer();
    void onGetPoseTimer();

public:
    Dobot(DobotRealVars realVars, IntermediatePoints intermediatePoints);
    ~Dobot();

    void queueMoveSequence(Point3D dest3D, double dJump, VERTICAL_MOVE VertMove =
            VM_NONE, bool bEscape = false);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void moveArmUpOrDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight);
    void setItemInGripper(short sGrippersItemID);
    void clearGripper();
    //future: GetAlarmsState, ClearAllAlarmsState- react on them somehow
    void forceStopArm();

    ushort getItemInGripper() const { return m_usItemIDInGripper; }
    IntermediatePoints getIntermediatePoints() const { return m_intermediatePoints; }
    Point3D getHomePos();
    Point3D getLastGivenPoint() const { return m_lastGivenPoint; }
    QString dumpAllData();

    DobotGripper* getServoPointer() const { return m_pGripper; }

public slots:
    void onConnectDobot();
    void sendMoveToArm(DobotMove move);
    void showQueueLabelsInUI(uint unSpace, uint64_t un64DobotId, uint64_t un64CoreMaxId,
                             int nCoreIdLeft, uint64_t un64CoreNextId);
    void addTextToLogPTEInUI(QString QStrTxt, LOG log);
    void showQueuedArmCmdsOnCorePTE(QString QStrList)
    { emit this->showQueuedArmCmdsOnCore(QStrList); }
    void showSentArmCmdsToDobotPTE(QString QStrList)
    { emit this->showSentArmCmdsToDobot(QStrList); }

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
