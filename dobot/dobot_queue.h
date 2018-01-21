#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include "dobot.h"

struct DobotMove
{
    int64_t ID;
    DOBOT_MOVE move;
    Point3D xyz;
};

class DobotQueue
{
private:
    int64_t _n64CoreQueuedCmdID;
    int64_t _n64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove> _QueuedCmdIDList;
    DobotMove _cmd; //todo: nazwa?
    DobotMove _firstPosId, _lastPosId, _takenPosId; //todo: używam tego chyba tylko lokalnie
    int64_t _n64RetreatID; //todo: czy to potrzebne? gdyby na każde nowe zapytanie sprawdzać w ...
    //...pętli czy maxymalny ID == aktualny ID, i robić retreat zawsze kiedy są one ==, po czym ...
    //... wyłączać pętlę, póki nie pojawi się nic nowego w addCmd
    PTPCmd _ptpCmd;
    QList<ServoArduino> _arduinoGripperStates;
    const float _fGripOpened, _fGripClosed;

public:
    DobotQueue();

    void queuedIDList();
    void retreat();
    void queuePhysicalMoveOnArm(DOBOT_MOVE Move);
    void addCmdToList(DOBOT_MOVE Move, Point3D point);

    void setCoreQueuedCmdID(int64_t n64CmdID) { _n64CoreQueuedCmdID = n64CmdID; }
    void setDobotQueuedCmdID(int64_t n64CmdID) { _n64RealTimeDobotActualID = n64CmdID; }
    void setRetreatID(int64_t n64RetreatID) { _n64RetreatID = n64RetreatID; }
    void setQueuedCmdLeftSpace(uint unLeftSpace) { _unQueuedCmdLeftSpace = unLeftSpace; }

    int64_t getCoreQueuedCmdID() const { return _n64CoreQueuedCmdID; }
    int64_t getDobotQueuedCmdID() const { return _n64RealTimeDobotActualID; }
    int64_t getRetreatID() const { return _n64RetreatID; }
    uint getQueuedCmdLeftSpace() const { return _unQueuedCmdLeftSpace; }

signals:
    void showActualDobotQueuedCmdIDList(QList<DobotMove>);
};

#endif // DOBOT_QUEUE_H
