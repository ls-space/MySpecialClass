#include "CShareThread.h"

CShareThread::CShareThread()
{

}

CShareThread::~CShareThread()
{
    quit();
    wait();
}

void CShareThread::StartThread(CWaitCallback callBack)
{
    QMutexLocker locker(&m_mutex);
    m_Work.SetCallback(callBack);
    start();
}

void CShareThread::StopThread()
{
    QMutexLocker locker(&m_mutex);
    m_Running = false;
    quit();
    wait();
}


void CShareThread::SetThreadSleepTime(uint sleepUS, bool running)
{
    m_SleepUS = sleepUS;
    m_Running = running;
}

bool CShareThread::GetRunning()
{
    return isRunning();
}

void CShareThread::run()
{
    if(m_Running)
    {
        while (m_Running)
        {
            m_Work.Invoke();
            usleep(m_SleepUS);
        }
    }
    else
    {
        m_Work.Invoke();
    }
}
