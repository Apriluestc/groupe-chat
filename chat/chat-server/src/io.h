#ifndef IO_H
#define iO_H
#include "proto.h"
#include "list.h"
#include "accept.h"
#include "user.h"
#include "constant-macro.h"
#include "cmd.h"
#include "list.h"

class CIo
{
public:
    CIo();
    ~CIo();

    void startUdpFunc();
    void setUdpFd(int fd);
    static void *startUdp(void *arg);
    static void *startUdpSend(void *arg);
    static void *startRequest(void *arg);               //新线程
    void deleteloginUser(int sockfd);
    void start();
    void startRequestFunc();
    void startUdpSendFunc();
    void epollRead(int sockfd);
    void setVcdListFromCmdMode(CUserList *vcdList);   //从监听模块获取公用user链表
    void m_usleep(int time);
    /*本地数据包指针链表的接口*/
    CList *getPackList();

private:
    /*用来接收监听模块user链表的指针*/
    CUserList       *m_vcdList;
    /*本地数据包指针链表*/
    CList       *m_packList;
    map<int, CUser*>  m_map;
    pthread_t      tid, tid2, tid3;
    int epfd;
    int nfds;
    int nread;
    int sockfd;
    struct epoll_event ev, events[11000];//ev用于注册事件,数组用于回传要处理的事件

    int udpsock;
    CCmdRecv m_recv;
    int total;
};

#endif
