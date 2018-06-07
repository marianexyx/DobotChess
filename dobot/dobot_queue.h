#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include <QDebug>
#include "vars/basic_vars.h"
#include "vars/log.h"
#include "DobotDll.h"
#include "DobotType.h"
#include "dobot/vars/dobot_moves.h"

class DobotQueue: public QObject
{
    Q_OBJECT

private:
    uint64_t _un64CoreQueuedCmdID;
    uint64_t _un64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove> _queuedArmCmds;
    QList<DobotMove> _executedArmCmds; //todo: zła nazwa
    uint64_t _lowestIDMoveInList;
    uint64_t _un64LastDobotIDShownInUI;
    uint64_t _un64RetreatID;
    Point3D _retreatLeft, _retreatRight;
    bool _bRetreat;

public:
    DobotQueue(Point3D retreatLeft, Point3D retreatRight);

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextMoveToSendToArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID);
    bool isArmCoveringGame();
    void retreat(Point3D lastPoint);
    //void sendMoveToArm(DobotMove move); //todo: this should be in dobot...
    //...class? executed with emit?
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void saveIDFromConnectedDobot();
    //bool isDobotCmdsLeftSpaceEmpty();

    void setCoreQueuedCmdID(uint64_t ID) { _un64CoreQueuedCmdID = ID; }
    void setDobotQueuedCmdID(uint64_t ID) { _un64RealTimeDobotActualID = ID; }
    void setQueuedCmdLeftSpace(uint unLeftSpace) { _unQueuedCmdLeftSpace =
                unLeftSpace; }
    void setRetreat(bool bRetreat) { _bRetreat = bRetreat; }

    int64_t getCoreQueuedCmdID() const { return _un64CoreQueuedCmdID; }
    int64_t getDobotQueuedCmdID() const { return _un64RealTimeDobotActualID; }
    uint getQueuedCmdLeftSpace() const { return _unQueuedCmdLeftSpace; }
    int64_t getRealTimeDobotActualID() const { return _un64RealTimeDobotActualID; }

signals:
    void showActualDobotQueuedCmdIDList(QList<DobotMove>); //todo: change name like below
    void showOnDobotQueuedCmdsList(QList<DobotMove>);
    void sendMoveToArm(DobotMove);
    void showQueueLabelsInUI(int, int, int, int, int);
    void addTextToLogPTEInUI(QString, LOG);
};

#endif // DOBOT_QUEUE_H
