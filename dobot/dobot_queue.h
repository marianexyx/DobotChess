#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include "dobot.h"

struct DobotMove
{
    int64_t ID;
    DOBOT_MOVE_TYPE type;
    Point3D xyz;
};

class DobotQueue
{
private:
    Dobot* _pDobot;
    DobotServo* _pServo;

    int64_t _n64CoreQueuedCmdID;
    int64_t _n64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove> _queuedCmdIDList;
    DobotMove _lowestIDMoveInList;
    int64_t _n64RetreatID;
    int64_t _n64LastDobotIDShownInUI;

public:
    DobotQueue(Dobot* pDobot);

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextPhysicalMoveToQueueOnArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMoveInCore(int64_t ID);
    //void retreat();
    void queuePhysicalMoveOnArm(DobotMove move);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void saveIDFromConnectedDobot();
    //bool isDobotCmdsLeftSpaceEmpty();

    void setCoreQueuedCmdID(int64_t n64CmdID) { _n64CoreQueuedCmdID = n64CmdID; }
    void setDobotQueuedCmdID(int64_t n64CmdID) { _n64RealTimeDobotActualID = n64CmdID; }
    void setRetreatID(int64_t n64RetreatID) { _n64RetreatID = n64RetreatID; }
    void setQueuedCmdLeftSpace(uint unLeftSpace) { _unQueuedCmdLeftSpace = unLeftSpace; }

    int64_t getCoreQueuedCmdID() const { return _n64CoreQueuedCmdID; }
    int64_t getDobotQueuedCmdID() const { return _n64RealTimeDobotActualID; }
    int64_t getRetreatID() const { return _n64RetreatID; }
    uint getQueuedCmdLeftSpace() const { return _unQueuedCmdLeftSpace; }
    int64_t getRealTimeDobotActualID() const { return _n64RealTimeDobotActualID; }

signals:
    void showActualDobotQueuedCmdIDList(QList<DobotMove>);
};

#endif // DOBOT_QUEUE_H
