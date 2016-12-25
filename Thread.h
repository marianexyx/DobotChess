#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "DobotDll.h"
#include "DobotType.h"
#include <QMutex>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();

    Pose *pose;

protected:
    void run();

private:
    Pose m_pose;
};

#endif // THREAD_H
