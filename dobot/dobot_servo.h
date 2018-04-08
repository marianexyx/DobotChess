#ifndef DOBOT_SERVO_H
#define DOBOT_SERVO_H

#pragma once
#include "dobot.h"

class Dobot;
class DobotQueue;

struct ServoArduino
{
    int64_t ID;
    bool isGripperOpen;
};

//future: czy servo (nazwę) traktować jako po prostu gripper?
class DobotServo
{
private:
    Dobot* _pDobot;
    DobotQueue* _pQueue;

    IOPWM _gripperServo;
    QList<ServoArduino> _arduinoGripperStates;
    const float _fGripOpened, _fGripClosed;
    WAITCmd _gripperMoveDelay;

public:
    DobotServo(Dobot* pDobot, float fGripperOpened, float fGripperClosed);

    void checkPWMForErrors();
    void changeGripperAngle(float fDutyCycle); //service usage
    void moveServoManually();
    void openGripper(uint64_t ID);
    void closeGripper(uint64_t ID);
    void addServoMoveToGripperStatesList(DOBOT_MOVE_TYPE MoveType);

    bool isServoListEmpty() const { return 1 /*_arduinoGripperStates.isEmpty() ? true : false*/; }
};

#endif // DOBOT_SERVO_H
