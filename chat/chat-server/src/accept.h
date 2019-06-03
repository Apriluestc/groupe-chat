#ifndef ACCEPT_H
#define ACCEPT_H

#include "proto.h"
#include "constant-macro.h"
#include "list.h"
#include "mutex.h"
#include "user.h"
#include "unloginuser.h"
#include "user-list.h"

class CAccept
{
public:
    CAccept();
    ~CAccept();

    /*副线程函数*/
    static void *startAccept(void *arg);    //新线程
    void funcAccept();                     //主线程函数
    void timeoutFunc(int sockfd);           //超时判断函数
    void deleteOneUnloginUser(int sockfd);

    /*主线程函数*/
    CList *IOGetVcdList();
    CUserList *getVcdLoginList();
    void start();

private:
    /*副线程变量*/
    map<int, CUnloginuser*> m_loginMap; //未登录用户链表
    CList   *m_vcdListtoIO;
    CUserList   *m_vcdLoginList;
    int        m_connfd;                 //新连接的客户端套接字
    int        m_userId;                 //分配给客户端的用户ID

    /*主副线程公用变量--监听套接字*/
    int        m_listenfd;
    /*主线程变量*/
    void       *m_retval;
    pthread_t  m_tid;
    int m_epfd;
    int m_nfds;
    int m_nread;
    struct sockaddr_in m_clientaddr;
    socklen_t m_clilen;
    struct epoll_event m_ev, m_events[11000];//ev用于注册事件,数组用于回传要处理的事件

    /********/
};

#endif
