#include "dobot_servo.h"

DobotServo::DobotServo(/*Dobot* pDobot,*/ float fGripperOpened, float fGripperClosed):
    _fGripOpened(fGripperOpened),
    _fGripClosed(fGripperClosed)
{
    //_pDobot = pDobot; //todo: czy to zbędne? tak
    //_pQueue = _pDobot->getQueuePointer();
    qDebug() << "DobotServo::DobotServo(): _fGripOpened =" << _fGripOpened
             << ", _fGripClosed =" << _fGripClosed;

    _gripper.address = 4;
    _gripper.dutyCycle = _fGripOpened;
    _gripper.frequency = 50.f;

    this->checkPWMForErrors();
}

void DobotServo::checkPWMForErrors()
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
            qDebug() << "ERROR: DobotServo::checkPWM(): PWMData.address !="
                        " 4. val =" << PWMData.address;
        if (PWMData.frequency != 50)
            qDebug() << "ERROR: DobotServo::checkPWM(): PWMData.frequency !="
                        " 50. val =" << PWMData.address;
        if (PWMData.dutyCycle != _fGripOpened && PWMData.dutyCycle != _fGripClosed)
            qDebug() << "ERROR: DobotServo::checkPWM(): PWMData.dutyCycle!= 4"
                        " && !=7.40f. val =" << PWMData.dutyCycle;
    }
}

void DobotServo::changeGripperAngle(float fDutyCycle) //info: powinno działać
{
    if (fDutyCycle != 0)
        _gripper.dutyCycle = fDutyCycle;
    qDebug() << "_gripper.dutyCycle = " << fDutyCycle;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, false, NULL), SHOW_ERRORS); //unqueued
}

/*void DobotServo::moveServoManually()
{
    if (_arduinoGripperStates.first().ID <= _pQueue->getRealTimeDobotActualID())
    {
        QString QStrServoState =
                _arduinoGripperStates.first().isGripperOpen ? "Open" : "Close";
        qDebug() << "servo" << QStrServoState << ", servoListLastIndex ="
                 << _arduinoGripperStates.first().ID << ", dobotActualIndex ="
                 << _pDobot->getQueuePointer()->getRealTimeDobotActualID();
        _pDobot->getArduinoPointer()->sendDataToUsb("servo" + QStrServoState);
        _arduinoGripperStates.removeFirst();
    }

    _pDobot->showArduinoGripperStateListInUI(_arduinoGripperStates);
}*/

void DobotServo::openGripper(uint64_t ID)
{
    qDebug() << "DobotServo::openGripper(): _fGripOpened =" << _fGripOpened
             << ", pin =" << _gripper.address << ", freq =" << _gripper.frequency;
    _gripper.dutyCycle = _fGripOpened;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, true, &ID), SHOW_ERRORS);
}

void DobotServo::closeGripper(uint64_t ID)
{
    qDebug() << "DobotServo::closeGripper(): _fGripClosed =" << _fGripClosed
             << ", pin =" << _gripper.address << ", freq =" << _gripper.frequency;
    _gripper.dutyCycle = _fGripClosed;
    isArmReceivedCorrectCmd(SetIOPWM(&_gripper, true, &ID), SHOW_ERRORS);
}

void DobotServo::wait(uint64_t ID)
{
    //wait for the servo to be closed
    WAITCmd gripperMoveDelay;
    gripperMoveDelay.timeout = 300;
    isArmReceivedCorrectCmd(SetWAITCmd(&gripperMoveDelay, true, &ID), SHOW_ERRORS);
}

/*void DobotServo::addServoMoveToGripperStatesList(DOBOT_MOVE_TYPE MoveType)
{
    if (MoveType != DM_UP && MoveType != DM_DOWN)
    {
        qDebug() << "ERROR: DobotServo::addServoMoveToGripperStatesList(): wrong "
                    "MoveType =" << MoveType;
        return;
    }

    ServoArduino servoState;
    servoState.ID = _pQueue->getCoreQueuedCmdID();
    servoState.isGripperOpen = (MoveType == DM_OPEN) ? true : false;
    qDebug() << "DobotQueue::addArmMoveToQueue():" << dobotMoveAsQstr(MoveType);

    _arduinoGripperStates << servoState;
}*/
