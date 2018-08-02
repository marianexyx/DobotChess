#ifndef DOBOT_SERVO_H
#define DOBOT_SERVO_H

#pragma once
#include <QDebug>
#include "vars/log.h"
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "dobot/vars/dobot_move_types.h"

class DobotGripper
{
    friend class Dobot;

private:
    const float _fGripOpened, _fGripClosed;
    IOPWM _gripper;

    void checkPWMForErrors();
    void openGripper(uint64_t ID);
    void closeGripper(uint64_t ID);
    void wait(uint64_t ID);

    DobotGripper(float fGripperOpened, float fGripperClosed);

public:
    ~DobotGripper() { qDebug() << "destroying DobotGripper"; } 

    void changeGripperAngle(float fDutyCycle); //service usage
};

#endif // DOBOT_SERVO_H
