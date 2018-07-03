#ifndef DOBOT_SERVO_H
#define DOBOT_SERVO_H

#pragma once
#include <QDebug>
#include "vars/log.h"
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "dobot/vars/dobot_move_types.h"

struct ServoArduino
{
    int64_t ID;
    bool isGripperOpen;
};

//todo: change class name for "gripper"? it's not just servo anymore

class DobotServo
{
private:
    QList<ServoArduino> _arduinoGripperStates;
    const float _fGripOpened, _fGripClosed;
    IOPWM _gripper;

public:
    DobotServo(float fGripperOpened, float fGripperClosed);
    ~DobotServo() { qDebug() << "destroying DobotServo"; }

    void checkPWMForErrors();
    void changeGripperAngle(float fDutyCycle); //service usage
    void openGripper(uint64_t ID);
    void closeGripper(uint64_t ID);
    void wait(uint64_t ID);
};

#endif // DOBOT_SERVO_H
