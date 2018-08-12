#ifndef DOBOT_QUEUE_H
#define DOBOT_QUEUE_H

#pragma once
#include <QDebug>
#include "vars/const_flags.h"
#include "vars/log.h"
#include "DobotDll/DobotDll.h"
#include "DobotDll/DobotType.h"
#include "xml_reader.h"
#include "dobot/vars/dobot_move_types.h"
#include "vars/intermediate_points.h"

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
    uint64_t _lowestIDMoveInCoreList;
    uint64_t _un64LastDobotIDShownInUI;
    uint64_t _un64EscapeID;
    Point3D _escapeLeft, _escapeRight;
    bool _bEscape;

    void parseNextMoveToArmIfPossible();
    bool isNextPhysicalMoveToQueueOnArmAvailable();
    DobotMove getNextMoveToSendToArm();
    void showLastExecutedArmMoveInUI();
    void removeOldQueuedMovesFromCore();
    DobotMove getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID);
    bool isArmCoveringView();
    void escape(Point3D lastPoint);
    void addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point);
    void saveIDFromConnectedDobot();
    //bool isDobotCmdsLeftSpaceEmpty();
    QString dumpAllData();

    void setescape(bool bEscape) { _bEscape = bEscape; }

    DobotQueue(IntermediatePoints intermediatePoints);

public:
    QString getQueuedArmCmds();
    QString getSentArmCmds();

signals:
    void showQueuedArmCmdsOnCore(QString);
    void showSentArmCmdsToDobot(QString);
    void sendMoveToArm(DobotMove);
    void showQueueLabelsInUI(uint, uint64_t, uint64_t, int, uint64_t);
    void addTextToLogPTEInUI(QString, LOG);
};

#endif // DOBOT_QUEUE_H
