#include "dobot_gripper.h"

DobotGripper::DobotGripper(float fGripperOpened, float fGripperClosed):
    _fGripOpened(fGripperOpened),
    _fGripClosed(fGripperClosed)
{
    qDebug() << "DobotGripper::DobotGripper(): _fGripOpened =" << _fGripOpened
             << ", _fGripClosed =" << _fGripClosed;

    _gripper.address = 4;
    _gripper.dutyCycle = _fGripOpened;
    _gripper.frequency = 50.f;

    this->checkPWMForErrors();
}

void DobotGripper::checkPWMForErrors()
{
    //info: typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM PWMData;
    PWMData.address = 0.f;
    PWMData.frequency = 0.f;
    PWMData.dutyCycle = 0.f;
    if (PWMData.address != 0 && PWMData.frequency != 0 &&
            PWMData.dutyCycle != 0)
    {
        GetIOPWM(&PWMData);

        if (PWMData.address!= 4 )
            qDebug() << "ERROR: DobotGripper::checkPWM(): PWMData.address !="
                        " 4. val =" << PWMData.address;
        if (PWMData.frequency != 50)
            qDebug() << "ERROR: DobotGripper::checkPWM(): PWMData.frequency !="
                        " 50. val =" << PWMData.address;
        if (PWMData.dutyCycle != _fGripOpened && PWMData.dutyCycle != _fGripClosed)
            qDebug() << "ERROR: DobotGripper::checkPWM(): PWMData.dutyCycle!= 4"
                        " && !=7.40f. val =" << PWMData.dutyCycle;
    }
}

void DobotGripper::changeGripperAngle(float fDutyCycle) //info: that should work
{
    if (fDutyCycle != 0)
        _gripper.dutyCycle = fDutyCycle;
    qDebug() << "_gripper.dutyCycle = " << fDutyCycle;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, false, NULL), SHOW_ERRORS); //unqueued
}

void DobotGripper::openGripper(uint64_t ID)
{
    qDebug() << "DobotGripper::openGripper(): _fGripOpened =" << _fGripOpened
             << ", pin =" << _gripper.address << ", freq =" << _gripper.frequency;
    _gripper.dutyCycle = _fGripOpened;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, true, &ID), SHOW_ERRORS);
}

void DobotGripper::closeGripper(uint64_t ID)
{
    qDebug() << "DobotGripper::closeGripper(): _fGripClosed =" << _fGripClosed
             << ", pin =" << _gripper.address << ", freq =" << _gripper.frequency;
    _gripper.dutyCycle = _fGripClosed;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, true, &ID), SHOW_ERRORS);
}

void DobotGripper::wait(uint64_t ID)
{
    //wait for the servo to be closed
    WAITCmd gripperMoveDelay;
    gripperMoveDelay.timeout = 300;
    isArmReceivedCorrectCmd(SetWAITCmd(&gripperMoveDelay, true, &ID), SHOW_ERRORS);
}
