#ifndef IOMODEL_H
#define IOMODEL_H
#include "head.h"
#include "savelist.h"
#include "userlist.h"

class Iomodel
{
public:
    Iomodel();
    ~Iomodel();
    void  funcIo();
    static void* startIo(void *arg);
    void  start();
    SaveList* getpackList();
    pthread_t get_tid();
    void m_usleep(int time);
    void setuserListFromCmd(UserList *userList);
    void epollRead(int sockfd);

private:
    int ret;
    int epfd;
    int nfds;
    int sockfd;
    int nread;
    struct epoll_event ev, events[11000];//ev用于注册事件,数组用于回传要处理的事件
    map<int, UserData*>  m_map;

    pthread_t  tid;
    struct timeval timeout;
    fd_set rset;
    SaveList *m_packList;
    UserList *m_userList;
};

#endif // IOMODEL_H
