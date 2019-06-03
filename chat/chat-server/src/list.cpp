#include "list.h"

CList::CList()
{
    pthread_mutex_init(&m_packmutex, NULL);
}

CList::~CList()
{

}

/*数据包*/
char *CList::packFront()
{
    return m_package.front();
}

char* CList::packPop()
{
    CMutex mutex_lock(&m_packmutex);
    char *p = m_package.front();
    m_package.pop_front();
    m_packcount--;
    return p;
}

int CList::getPackSize()
{
    return m_packcount;
}

void CList::packPush(char *p)
{
    CMutex mutex_lock(&m_packmutex);
    m_package.push_back(p);
    m_packcount++;
}
