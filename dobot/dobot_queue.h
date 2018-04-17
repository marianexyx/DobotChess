#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
//#include "dobot.h"
#include <QDebug>
#include "vars/log.h"
#include "DobotDll.h"
#include "DobotType.h"
#include "dobot/vars/dobot_moves.h"

//class Dobot;
//class DobotServo;

class DobotQueue: public QObject
{
    Q_OBJECT

private:
    //Dobot* _pDobot;
    //DobotServo* _pServo;

    uint64_t _un64CoreQueuedCmdID;
    uint64_t _un64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove> _queuedArmCmds;
    QList<DobotMove> _executedArmCmds;
    uint64_t _lowestIDMoveInList;
    uint64_t _un64RetreatID;
    uint64_t _un64LastDobotIDShownInUI;

public:
    DobotQueue(/*Dobot* pDobot*/);

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextMoveToSendToArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID);
    //void retreat();
    //void sendMoveToArm(DobotMove move); //todo: to powinno być w dobocie,...
    //...realizowane przez emit?
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
    void showActualDobotQueuedCmdIDList(QList<DobotMove>);
    void sendMoveToArm(DobotMove);
    void showQueueLabelsInUI(int, int, int, int, int);
    void addTextToLogPTEInUI(QString, LOG);
};

#endif // DOBOT_QUEUE_H
