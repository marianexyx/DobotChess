#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "DobotDll.h"
#include "DobotType.h"
#include <QMutex>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread();

    Pose *pose; //struktura pozycji ramienia

protected:
    void run(); //nadpisanie tej funkcji
    //run() starts the event loop by calling exec() and runs a Qt event loop inside the thread

private:
    Pose m_pose; //struktura pozycji ramienia
};

#endif // WORKERTHREAD_H
