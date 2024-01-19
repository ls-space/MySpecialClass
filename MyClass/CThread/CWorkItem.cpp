#include "CWorkItem.h"

CWorkItem::CWorkItem()
{

}

CWorkItem::CWorkItem(CWaitCallback callback)
{
    m_Callback = callback;
}

CWorkItem::~CWorkItem()
{

}

void CWorkItem::SetCallback(CWaitCallback callback)
{
    m_Callback = callback;
}

void CWorkItem::Invoke()
{
    if(m_Callback)
    {
        m_Callback();
    }
}
