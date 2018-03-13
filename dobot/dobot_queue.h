#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include "dobot.h"
#include "DobotDll.h"
#include "DobotType.h"

class Dobot;
class DobotServo;

struct DobotMove
{
    uint64_t ID;
    DOBOT_MOVE_TYPE type;
    Point3D xyz;

    DobotMove() :ID(0), type(DM_NONE), xyz(0,0,0) {}
    DobotMove(uint64_t id, DOBOT_MOVE_TYPE MT, Point3D p): ID(id), type(MT), xyz(p) {}
};

class DobotQueue: public QObject
{
    Q_OBJECT

private:
    Dobot* _pDobot;
    DobotServo* _pServo;

    uint64_t _un64CoreQueuedCmdID;
    uint64_t _un64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove*> _queuedCmdIDList;
    uint64_t _lowestIDMoveInList;
    uint64_t _un64RetreatID;
    uint64_t _un64LastDobotIDShownInUI;

public:
    DobotQueue(Dobot* pDobot);

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextPhysicalMoveToQueueOnArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMoveInCore(uint64_t un64ID);
    //void retreat();
    void queuePhysicalMoveOnArm(DobotMove move);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void saveIDFromConnectedDobot();
    //bool isDobotCmdsLeftSpaceEmpty();

    void setCoreQueuedCmdID(uint64_t ID) { _un64CoreQueuedCmdID = ID; }
    void setDobotQueuedCmdID(uint64_t ID) { _un64RealTimeDobotActualID = ID; }
    void setRetreatID(uint64_t ID) { _un64RetreatID = ID; }
    void setQueuedCmdLeftSpace(uint unLeftSpace) { _unQueuedCmdLeftSpace = unLeftSpace; }

    int64_t getCoreQueuedCmdID() const { return _un64CoreQueuedCmdID; }
    int64_t getDobotQueuedCmdID() const { return _un64RealTimeDobotActualID; }
    int64_t getRetreatID() const { return _un64RetreatID; }
    uint getQueuedCmdLeftSpace() const { return _unQueuedCmdLeftSpace; }
    int64_t getRealTimeDobotActualID() const { return _un64RealTimeDobotActualID; }

signals:
    void showActualDobotQueuedCmdIDList(QList<DobotMove*>);
};

#endif // DOBOT_QUEUE_H
