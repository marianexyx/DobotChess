#include "workerThread.h"
WorkerThread::WorkerThread()
{
    pose = &m_pose;
}

void WorkerThread::run()
{
    while (1) {
        GetPose(&m_pose); //??? jakoś te pozycje wyciąga z dobota automatycznie chyba
        sleep(0.1);
    }
}
