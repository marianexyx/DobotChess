#include "dobot_servo.h"

DobotServo::DobotServo(Dobot *pDobot):
    _fGripOpened(6.9f), //future: ciągnąć z xml
    _fGripClosed(7.55f)
{
    _pDobot = pDobot;

    _gripperServo.address = 4;
    _gripperServo.frequency = 50;
    _gripperServo.dutyCycle = _fGripOpened;

    _gripperMoveDelay.timeout = 400;

    this->checkPWMForErrors();
}

void DobotServo::checkPWMForErrors()
{
    //info: typedef struct tagIOPWM {uint8_t address; float frequency; float dutyCycle;}IOPWM;
    IOPWM PWMData;
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
        _gripperServo.dutyCycle = fDutyCycle;
    qDebug() << "_gripperServo.dutyCycle = " << fDutyCycle;
    Dobot::isArmReceivedCorrectCmd(SetIOPWM(&_gripperServo, false, NULL), SHOW_ERRORS); //unqueued
}

void DobotServo::moveServoManually()
{
    if (_arduinoGripperStates.first().ID <= _n64RealTimeDobotActualID)
    {
        QString QStrServoState =
                _arduinoGripperStates.first().isGripperOpen ? "Open" : "Close";
        qDebug() << "servo" << QStrServoState << ", servoListLastIndex ="
                 << _arduinoGripperStates.first().ID << ", dobotActualIndex ="
                 << _pDobot->getQueuePointer()->getRealTimeDobotActualID();
        _pDobot->getArduinoPointer()->sendDataToUsb("servo" + QStrServoState);
        _arduinoGripperStates.removeFirst();
    }

    emit showArduinoGripperStateList(_arduinoGripperStates);
}

void DobotServo::openGripper(int64_t ID)
{
    _gripperServo.dutyCycle = _fGripOpened;
    Dobot::isArmReceivedCorrectCmd(SetIOPWM(&_gripperServo, true, &ID), SHOW_ERRORS);
}

void DobotServo::closeGripper(int64_t ID)
{
    _gripperServo.dutyCycle = _fGripClosed;
    Dobot::isArmReceivedCorrectCmd(SetIOPWM(&_gripperServo, true, &ID), SHOW_ERRORS);

    //wait for the servo to be closed //todo: 2 id w 1 ruchu
    Dobot::isArmReceivedCorrectCmd(SetWAITCmd(&_gripperMoveDelay, true, &move.ID), SHOW_ERRORS);
}

void DobotServo::addServoMoveToGripperStatesList(DOBOT_MOVE_TYPE MoveType)
{
    if (MoveType != DM_UP && MoveType != DM_DOWN)
    {
        qDebug() << "ERROR: DobotServo::addServoMoveToGripperStatesList(): wrong "
                    "MoveType =" << MoveType;
        return;
    }

    ServoArduino servoState;
    servoState.ID = _n64CoreQueuedCmdID;
    servoState.isGripperOpen = (State == DM_OPEN) ? true : false;
    qDebug() << "DobotQueue::addArmMoveToQueue():" << dobotMoveAsQstr(State);

    _arduinoGripperStates << servoState;
}
