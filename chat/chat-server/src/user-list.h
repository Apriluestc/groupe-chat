#ifndef CUSERLIST_H
#define CUSERLIST_H
#include "mutex.h"
#include "user.h"
#include "proto.h"
#include "unloginuser.h"

class CUserList
{
public:
    CUserList();
    /*user类型的公共链表*/
    void vcdPush(CUser *user);
    int  getSize();
    CUser *vcdPop();
    /*未登录用户*/
    void unloginUserPush(CUnloginuser *user);
    int getUnloginSize();
    CUnloginuser *unloginUserPop();

private:
    /*user类型的公共链表的变量*/
    int             m_count;
    list<CUser*>    m_user;
    pthread_mutex_t m_mutex;
    /*未登录用户*/
    int             m_unLoginCount;
    list<CUnloginuser*> m_unloginuser;
    pthread_mutex_t m_unloginmutex;
};

#endif
