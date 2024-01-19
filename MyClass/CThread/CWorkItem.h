#ifndef CWORKITEM_H
#define CWORKITEM_H
#include <QThread>

using CWaitCallback = std::function<void()>;

class CWorkItem
{
public:
    CWorkItem();

    CWorkItem(CWaitCallback callback);

    ~CWorkItem();

    void SetCallback(CWaitCallback callback);

    void Invoke();

private:
    CWaitCallback m_Callback = nullptr;
};

#endif // CWORKITEM_H
