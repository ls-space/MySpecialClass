#ifndef CSHARETHREAD_H
#define CSHARETHREAD_H
#include <QMutex>
#include "CWorkItem.h"


class CShareThread : public QThread
{
public:
    CShareThread();

    ~CShareThread();

    void StartThread(CWaitCallback callBack);

    void StopThread();

    void SetThreadSleepTime(uint sleepUS, bool running);

    bool GetRunning();

protected:
    void run();

private:
    CWorkItem m_Work;

    QMutex m_mutex; /**< 修改线程运行状态 */

    unsigned long m_SleepUS = 0;

    bool m_Running = false;
};

#endif // CSHARETHREAD_H
