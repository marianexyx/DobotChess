#include "dobot_gripper.h"

DobotGripper::DobotGripper(float fGripperOpened, float fGripperClosed):
    m_fGripOpened(fGripperOpened),
    m_fGripClosed(fGripperClosed)
{
    m_gripper.address = 4;
    m_gripper.dutyCycle = m_fGripOpened;
    m_gripper.frequency = 50.f;

    this->checkPWMForErrors();
}

void DobotGripper::checkPWMForErrors()
{
    //typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM PWMData;
    PWMData.address = 0.f;
    PWMData.frequency = 0.f;
    PWMData.dutyCycle = 0.f;
    if (PWMData.address != 0 && PWMData.frequency != 0 && PWMData.dutyCycle != 0)
    {
        GetIOPWM(&PWMData);

        if (PWMData.address!= 4 )
            qCritical() << "PWMData.address != 4. val =" << QString::number(PWMData.address);
        if (PWMData.frequency != 50)
            qCritical() << "PWMData.frequency != 50. val =" << QString::number(PWMData.address);
        if (PWMData.dutyCycle != m_fGripOpened && PWMData.dutyCycle != m_fGripClosed)
            qCritical() << "wrong PWMData.dutyCycle val. it's ="
                        << QString::number(PWMData.dutyCycle);
    }
}

void DobotGripper::changeGripperAngle(float fDutyCycle)
{
    if (fDutyCycle != 0)
        m_gripper.dutyCycle = fDutyCycle;
    qInfo() << "m_gripper.dutyCycle = " << QString::number(fDutyCycle);
    isArmReceivedCorrectCmd(SetIOPWM(&m_gripper, false, NULL), SHOW_ERRORS); //unqueued
}

void DobotGripper::openGripper(uint64_t ID)
{
    m_gripper.dutyCycle = m_fGripOpened;
    isArmReceivedCorrectCmd(SetIOPWM(&m_gripper, true, &ID), SHOW_ERRORS);
}

void DobotGripper::closeGripper(uint64_t ID)
{
    m_gripper.dutyCycle = m_fGripClosed;
    isArmReceivedCorrectCmd(SetIOPWM(&m_gripper, true, &ID), SHOW_ERRORS);
}

void DobotGripper::wait(uint64_t ID)
{
    //wait for the servo to be closed
    WAITCmd gripperMoveDelay;
    gripperMoveDelay.timeout = 300;
    isArmReceivedCorrectCmd(SetWAITCmd(&gripperMoveDelay, true, &ID), SHOW_ERRORS);
}
