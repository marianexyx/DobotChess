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
    const float m_fGripOpened, m_fGripClosed;
    IOPWM m_gripper;

    void checkPWMForErrors();
    void openGripper(uint64_t ID);
    void closeGripper(uint64_t ID);
    void wait(uint64_t ID);

    DobotGripper(float fGripperOpened, float fGripperClosed);

public:
    void changeGripperAngle(float fDutyCycle); //service usage
};

#endif // DOBOT_SERVO_H
