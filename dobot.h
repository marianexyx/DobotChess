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
#include "DobotDll.h"
#include "DobotType.h"
#include "arduinousb.h"
#include "vars/log.h"
#include "vars/basic_vars.h"
#include "vars/board_axis.h"
#include "vars/dobot_moves.h"
#include "vars/board_data_labels.h"
#include "dobot/dobot_queue.h"
#include "dobot/dobot_servo.h"

class Dobot: public QObject
{
    Q_OBJECT

private:
    ArduinoUsb *_pArduinoUsb;

    DobotServo* _pServo;
    DobotQueue* _pQueue;

    short _sItemIDInGripper;

    bool _bConnectedToDobot;

    HOMEParams _Home;
    Point3D _lastGivenPoint, _realTimePoint;

public:
    Dobot(ArduinoUsb* pArduinoUsb);
    ~Dobot();

    void saveActualDobotPosition();
    void initDobot();
    void onPTPsendBtnClicked();

    void doMoveSequence(Point3D dest3D, VERTICAL_MOVE VertMove = VM_NONE, double dJump);
    bool isPointTotallyDiffrent(Point3D point);
    bool isPointDiffrentOnlyInZAxis(Point3D point);
    void addCmdToList(DOBOT_MOVE_TYPE Type, Point3D point = _lastGivenPoint); //todo: addMove...
    void armUpDown(DOBOT_MOVE_TYPE ArmDestination, double dHeight);
    void writeMoveTypeInConsole(DOBOT_MOVE_TYPE MoveType);
    bool bIsMoveInAxisRange(Point3D point);

    void setItemInGripper(short sGrippersItemID);
    void clearGripper();

    bool isGripperEmpty() const { return _sItemIDInGripper == 0 ? true : false; }
    short getItemInGripper() const { return _sItemIDInGripper; }
    Point3D getHomePos();
    Point3D getLastGivenPoint() const { return _lastGivenPoint; }
    //todo: poniższe: friends
    DobotServo* getServoPointer() const { return _pServo; }
    DobotQueue* getQueuePointer() const { return _pQueue; }
    ArduinoUsb* getArduinoPointer() const { return _pArduinoUsb; }

public slots:
    void onConnectDobot();
    void onPeriodicTaskTimer();
    void onGetPoseTimer();

signals: //GUI mainWindow
    void addTextToLogPTE(QString, LOG);
    void JointLabelText(QString QSLabelText, short sJoint);
    void AxisLabelText(QString QSAxisLabelText, char chAxis);
    void RefreshDobotButtonsStates(bool bDobotButtonsStates);
    void deviceLabels(QString QSdeviceSN, QString QSdeviceName, QString QSdeviceVersion);
    void DobotErrorMsgBox();
    void queueLabels(int nSpace, int nDobotId, int nCoreMaxId, int nCoreIdLeft, int CoreNextId);
};

#endif // DOBOT_H
