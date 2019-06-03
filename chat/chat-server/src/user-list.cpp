#include "user-list.h"

CUserList::CUserList()
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_unloginmutex, NULL);
}

/*登录用户user链表*/
CUser *CUserList::vcdPop()
{
    CMutex mutex_lock(&m_mutex);
    CUser *user = m_user.front();
    m_user.pop_front();
    m_count--;
    return user;
}

int CUserList::getSize()
{
    return m_count;
}

void CUserList::vcdPush(CUser *user)
{
    CMutex mutex_lock(&m_mutex);
    m_user.push_back(user);
    m_count++;
}
/*未登录用户*/
CUnloginuser *CUserList::unloginUserPop()
{
    CMutex mutex_lock(&m_unloginmutex);
    CUnloginuser *user = m_unloginuser.front();
    m_unloginuser.pop_front();
    m_unLoginCount--;
    return user;
}

int CUserList::getUnloginSize()
{
    return m_unLoginCount;
}

void CUserList::unloginUserPush(CUnloginuser *user)
{
    CMutex mutex_lock(&m_unloginmutex);
    m_unloginuser.push_back(user);
    m_unLoginCount++;
}
