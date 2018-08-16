#include "dobot_queue.h"

DobotQueue::DobotQueue(IntermediatePoints intermediatePoints)
{
    m_un64CoreQueuedCmdID = 1; //set 1st ID
    m_unQueuedCmdLeftSpace = std::numeric_limits<uint>::max(); //future:
    m_un64RealTimeDobotActualID = 0;
    m_un64EscapeID = 0;
    m_un64LastDobotIDShownInUI = 0;
    m_lowestIDMoveInCoreList = 0;
    m_escapeLeft = intermediatePoints.retreatLeft;
    m_escapeRight = intermediatePoints.retreatRight;
    m_bEscape = false;
}

void DobotQueue::parseNextMoveToArmIfPossible()
{
    //if (this->isDobotCmdsLeftSpaceEmpty()) return; //future:
    isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&m_un64RealTimeDobotActualID), SHOW_ERRORS);

    if (this->isNextPhysicalMoveToQueueOnArmAvailable())
    {
        emit this->sendMoveToArm(this->getNextMoveToSendToArm());
        this->removeOldQueuedMovesFromCore();

        emit this->showQueuedArmCmdsOnCore(this->getQueuedArmCmds());
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }
}

bool DobotQueue::isNextPhysicalMoveToQueueOnArmAvailable()
{
    emit this->showQueueLabelsInUI(m_unQueuedCmdLeftSpace, m_un64RealTimeDobotActualID,
                                   m_un64CoreQueuedCmdID, m_queuedArmCmdsOnCore.size(),
                                   m_lowestIDMoveInCoreList);

    if (!m_queuedArmCmdsOnCore.isEmpty())
    {
        QListIterator<DobotMove> QueuedCmdIDIter(m_queuedArmCmdsOnCore);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if (QueuedCmdIDIter.hasNext())
        {
            m_lowestIDMoveInCoreList = QueuedCmdIDIter.peekNext().ID;

            if (m_lowestIDMoveInCoreList >= m_un64RealTimeDobotActualID)
            {
                if (m_lowestIDMoveInCoreList - m_un64RealTimeDobotActualID < 15)
                    return true;
            }
            else qCritical() << "m_lowestIDMoveInCoreList < m_un64RealTimeDobotActualID ("
                          << QString::number(m_lowestIDMoveInCoreList) << "<"
                          << QString::number(m_un64RealTimeDobotActualID) << ")";
        }
    }

    return false;
}

DobotMove DobotQueue::getNextMoveToSendToArm()
{
    if (!m_queuedArmCmdsOnCore.isEmpty()) //2nd security checks is mandatory
    {
        QListIterator<DobotMove> QueuedCmdIDIter(m_queuedArmCmdsOnCore);
        QueuedCmdIDIter.toFront(); //oldest move in list
        if (QueuedCmdIDIter.hasNext())
        {
            m_lowestIDMoveInCoreList = QueuedCmdIDIter.peekNext().ID;
            //future: why those 2 qdebugs below must be here for funtion to work?
            ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
            qInfo() << "new m_lowestIDMoveInCoreList =" << QString::number(m_lowestIDMoveInCoreList);

            if (m_lowestIDMoveInCoreList >= m_un64RealTimeDobotActualID)
            {
                //32 moves was dobot max memory capacity, so 15 moves sent will be safe
                if(m_lowestIDMoveInCoreList - m_un64RealTimeDobotActualID < 15)
                {
                    ///WARNING!!! WITHOUT THIS QDEBUG ARM WILL NOT WORK PROPERLY (DUNNO WHY)
                    qInfo() << "return next queued move. type ="
                             << dobotMoveAsQstr(m_queuedArmCmdsOnCore.first().type)
                             << ", ID =" << QString::number(m_queuedArmCmdsOnCore.first().ID)
                             << ", point =" << m_queuedArmCmdsOnCore.first().xyz.getAsQStr();
                    m_sentArmCmdsToDobot << m_queuedArmCmdsOnCore.first();

                    return m_queuedArmCmdsOnCore.takeFirst();
                }
            }
            else qCritical() << "m_lowestIDMoveInCoreList < m_un64RealTimeDobotActualID ("
                             << QString::number(m_lowestIDMoveInCoreList) << "<"
                             << QString::number(m_un64RealTimeDobotActualID) << ")";
        }
    }

    qCritical() << "reached artificially created anty-error return (shouldn't be possible)";
    return *new DobotMove;
}

void DobotQueue::removeOldQueuedMovesFromCore()
{
    if (!m_sentArmCmdsToDobot.empty()
            && m_un64RealTimeDobotActualID > m_sentArmCmdsToDobot.first().ID)
    {
        m_sentArmCmdsToDobot.removeFirst();
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }
}

bool DobotQueue::isArmCoveringView()
{
    if (m_bEscape && m_un64RealTimeDobotActualID > m_un64EscapeID &&
            m_un64RealTimeDobotActualID >= m_un64CoreQueuedCmdID && m_queuedArmCmdsOnCore.isEmpty())
        return true;
    else return false;
}

void DobotQueue::escape(Point3D lastPoint)
{
    if (qAbs(lastPoint.y - m_escapeLeft.y) > qAbs(lastPoint.y - m_escapeRight.y))
        this->addArmMoveToQueue(DM_TO_POINT, m_escapeRight);
    else this->addArmMoveToQueue(DM_TO_POINT, m_escapeLeft);

    m_un64EscapeID = m_un64CoreQueuedCmdID;
    m_bEscape = false; //prevent unwanted retreats
}

void DobotQueue::addArmMoveToQueue(DOBOT_MOVE_TYPE Type, Point3D point)
{    
    if (Type != DM_OPEN && Type != DM_CLOSE && !XmlReader::isPointInLimits(point))
    {
        qCritical() << "isPointInLimits == false. move type =" << dobotMoveAsQstr(Type);
        return;
    }

    m_un64CoreQueuedCmdID++;
    DobotMove cmdToQueue(m_un64CoreQueuedCmdID, Type, point);
    m_queuedArmCmdsOnCore << cmdToQueue;

    emit this->showQueuedArmCmdsOnCore(this->getQueuedArmCmds());
}

void DobotQueue::saveIDFromConnectedDobot()
{
    if (isArmReceivedCorrectCmd(GetQueuedCmdCurrentIndex(&m_un64RealTimeDobotActualID),
                                SHOW_ERRORS))
   {
       emit this->showQueueLabelsInUI(m_unQueuedCmdLeftSpace, m_un64RealTimeDobotActualID,
                                      m_un64CoreQueuedCmdID, m_queuedArmCmdsOnCore.size(),
                                      m_lowestIDMoveInCoreList);
   }

    m_un64EscapeID = m_un64CoreQueuedCmdID = m_un64RealTimeDobotActualID;
}

//future: dobot wanted to introduce this function in dll, but till now it isn't
/*bool DobotQueue::isDobotCmdsLeftSpaceEmpty()
{
    GetQueuedCmdLeftSpace(&m_unQueuedCmdLeftSpace);
    if (m_unQueuedCmdLeftSpace <= 0)
    {
        emit showQueueLabelsInUI->addTextToLogPTE("ERROR: Dobot queue memory full. "
            "Stop arm.\n", LOG_DOBOT);
        SetQueuedCmdForceStopExec(); //stop arm
    }
}*/

QString DobotQueue::dumpAllData()
{
    QString QStrData;

    QStrData = "[dobot_queue.h]\n";
    QStrData += "m_un64CoreQueuedCmdID: " + QString::number(m_un64CoreQueuedCmdID) + "\n";
    QStrData += "m_un64RealTimeDobotActualID: "
            + QString::number(m_un64RealTimeDobotActualID) + "\n";
    QStrData += "m_unQueuedCmdLeftSpace: " + QString::number(m_unQueuedCmdLeftSpace) + "\n";
    QStrData += "m_queuedArmCmdsOnCore: " + this->getQueuedArmCmds() + "\n";
    QStrData += "m_sentArmCmdsToDobot: " + this->getSentArmCmds() + "\n";
    QStrData += "m_lowestIDMoveInCoreList: " + QString::number(m_lowestIDMoveInCoreList) + "\n";
    QStrData += "m_un64LastDobotIDShownInUI: "
            + QString::number(m_un64LastDobotIDShownInUI) + "\n";
    QStrData += "m_un64EscapeID: " + QString::number(m_un64EscapeID) + "\n";
    QStrData += "m_bEscape: " + QString::number(m_bEscape) + "\n";

    return QStrData;
}

QString DobotQueue::getQueuedArmCmds()
{
    QString QStrQueuedList;
    DobotMove item;

    for(int i=0; i<m_queuedArmCmdsOnCore.count(); ++i)
    {
       item = m_queuedArmCmdsOnCore.at(i);
       QStrQueuedList += QString::number(item.ID) + ". " + dobotMoveAsQstr(item.type)
               + ": " + item.xyz.getAsQStr() + "\n";
    }

    return QStrQueuedList;
}

QString DobotQueue::getSentArmCmds()
{
    QString QStrQueuedList;
    DobotMove item;

    for(int i=0; i<m_sentArmCmdsToDobot.count(); ++i)
    {
       item = m_sentArmCmdsToDobot.at(i);
       if (m_un64RealTimeDobotActualID == item.ID)
           QStrQueuedList += ">";
       QStrQueuedList += QString::number(item.ID) + ". " +  dobotMoveAsQstr(item.type)
               + ": " + item.xyz.getAsQStr() + "\n";
    }

    return QStrQueuedList;
}

DobotMove DobotQueue::getQueuedMove(QList<DobotMove>& cmdsList, uint64_t un64ID)
{
    foreach(DobotMove move, cmdsList)
    {
        if (move.ID == un64ID)
            return move;
    }

    qCritical() << "move with ID nr" << QString::number(un64ID) << "doesn't exists";
    return *new DobotMove;
}

void DobotQueue::showLastExecutedArmMoveInUI()
{
    if (m_un64RealTimeDobotActualID > m_un64LastDobotIDShownInUI && m_un64LastDobotIDShownInUI > 0)
    {
        QString QStrMoveID = "";
        QString QStrDobotMove = "";

        DobotMove Move = this->getQueuedMove(m_sentArmCmdsToDobot, m_un64RealTimeDobotActualID);
        QStrMoveID = QString::number(Move.ID);
        QStrDobotMove = dobotMoveAsQstr(Move.type);
        emit this->addTextToLogPTEInUI("Executed move ID = " + QStrMoveID +
                                       ", type = " + QStrDobotMove + "\n", LOG_DOBOT);
        emit this->showSentArmCmdsToDobot(this->getSentArmCmds());
    }

    m_un64LastDobotIDShownInUI = m_un64RealTimeDobotActualID;
}
