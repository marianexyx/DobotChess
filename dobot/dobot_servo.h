#ifndef DOBOT_SERVO_H
#define DOBOT_SERVO_H

#pragma once
//#include "dobot.h"
#include <QDebug>
#include "vars/log.h"
#include "DobotDll.h"
#include "DobotType.h"
#include "dobot/vars/dobot_moves.h"

//class Dobot;
//class DobotQueue;

struct ServoArduino
{
    int64_t ID;
    bool isGripperOpen;
};

//future: change class name for "gripper"? it's not just servo anymore

class DobotServo
{
private:
    //Dobot* _pDobot;
    //DobotQueue* _pQueue;

    QList<ServoArduino> _arduinoGripperStates;
    const float _fGripOpened, _fGripClosed;
    IOPWM _gripper;

public:
    DobotServo(float fGripperOpened, float fGripperClosed);
    ~DobotServo() { qDebug() << "destroying DobotServo"; }

    void checkPWMForErrors();
    void changeGripperAngle(float fDutyCycle); //service usage
    //void moveServoManually();
    void openGripper(uint64_t ID);
    void closeGripper(uint64_t ID);
    void wait(uint64_t ID);
    //void addServoMoveToGripperStatesList(DOBOT_MOVE_TYPE MoveType); //todo: useless(?)

    bool isServoListEmpty() const { return 1 /*_arduinoGripperStates.isEmpty() ? true : false*/; }
};

#endif // DOBOT_SERVO_H
