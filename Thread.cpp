#include "Thread.h"
Thread::Thread()
{
    pose = &m_pose;
}

void Thread::run()
{
    while (1) {
        GetPose(&m_pose);
        sleep(0.1);
    }
}
