#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include <QDebug>
#include "vars/basic_vars.h"
#include "vars/log.h"
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "xml_reader.h"
#include "dobot/vars/dobot_move_types.h"

class DobotQueue: public QObject
{
    Q_OBJECT

    friend class Dobot;

private:
    uint64_t _un64CoreQueuedCmdID;
    uint64_t _un64RealTimeDobotActualID;
    uint _unQueuedCmdLeftSpace;
    QList<DobotMove> _queuedArmCmdsOnCore;
    QList<DobotMove> _sentArmCmdsToDobot;
    uint64_t _lowestIDMoveInList;
    uint64_t _un64LastDobotIDShownInUI;
    uint64_t _un64RetreatID;
    Point3D _escape1, _escape2;
    bool _bRetreat;

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextMoveToSendToArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID);
    bool isArmCoveringGame();
    void retreat(Point3D lastPoint);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void saveIDFromConnectedDobot();
    //bool isDobotCmdsLeftSpaceEmpty();

    void setRetreat(bool bRetreat) { _bRetreat = bRetreat; }

    DobotQueue(Point3D escape1, Point3D escape2);

public:
    QList<DobotMove> getQueuedArmCmds () const { return _queuedArmCmdsOnCore; }
    QList<DobotMove> getSentArmCmds () const { return _sentArmCmdsToDobot; }

signals:
    void showQueuedArmCmdsOnCore();
    void showSentArmCmdsToDobot();
    void sendMoveToArm(DobotMove);
    void showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t);
    void addTextToLogPTEInUI(QString, LOG);
};

#endif // DOBOT_QUEUE_H
