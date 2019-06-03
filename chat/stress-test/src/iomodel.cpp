#include "iomodel.h"
#include "head.h"

Iomodel::Iomodel()
{
    m_packList = new SaveList;
    m_userList = new UserList;
}

Iomodel::~Iomodel()
{
    delete m_packList;
    delete m_userList;
}

SaveList* Iomodel::getpackList()
{
    return m_packList;
}

pthread_t Iomodel::get_tid()
{
    return tid;
}

void Iomodel::m_usleep(int time)
{
    FD_ZERO(&rset);
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void Iomodel::setuserListFromCmd(UserList *userList)
{
    m_userList = userList;
}

void Iomodel::epollRead(int sockfd)
{
    /*判断是否满一个头的长度*/
    ioctl(sockfd, FIONREAD, &nread);
    if (nread == 0)
    {
        cout << "ioctl = 0" << endl;
        return;
    }
    if (nread >= sizeof(baseHandle) || m_map[sockfd]->m_recv.recvHeadFlag == 1)
    {
        if (m_map[sockfd]->m_recv.recvHeadFlag == 0)
        {
            if (!recv(sockfd, m_map[sockfd]->m_recv.get_recvbuff(), sizeof(baseHandle), 0))
            {
                return;
            }
            struct baseHandle *tmp = (struct baseHandle *)m_map[sockfd]->m_recv.recvbuff;
            m_map[sockfd]->m_recv.totalLengthOfData = tmp->length;
            m_map[sockfd]->m_recv.lengthHasRecv = sizeof(baseHandle);
            m_map[sockfd]->m_recv.recvHeadFlag = true;
        }
        if (m_map[sockfd]->m_recv.recvHeadFlag == 1)
        {
            int nres = m_map[sockfd]->m_recv.totalLengthOfData - m_map[sockfd]->m_recv.lengthHasRecv;
            nread = recv(sockfd,
                  m_map[sockfd]->m_recv.recvbuff + m_map[sockfd]->m_recv.lengthHasRecv, nres, 0);
            if (nread < 0)
            {

            }
            m_map[sockfd]->m_recv.lengthHasRecv += nread;
            if (m_map[sockfd]->m_recv.lengthHasRecv == m_map[sockfd]->m_recv.totalLengthOfData)
            {
                m_packList->pack_push(m_map[sockfd]->m_recv.recvbuff);
                m_map[sockfd]->m_recv.lengthHasRecv = 0;
                m_map[sockfd]->m_recv.recvHeadFlag = false;
            }
        }
     }
}

void Iomodel::funcIo()
{
    epfd = epoll_create(11000);//生成用于处理accept的epoll专用的文件描述符

    while (1)
    {
        if (m_userList->getUserSize() > 0)
        {
            UserData *user = m_userList->user_pop();
            m_map.insert(pair<int, UserData*>(user->getSockFd(), user));/*用户ID和套接字一一对应插入map中*/

            ev.data.fd = user->getSockFd();//设置用于写操作的文件描述符
            ev.events = EPOLLIN;//设置用于注测的读操作事件
            epoll_ctl(epfd, EPOLL_CTL_ADD, user->getSockFd(), &ev);//注册ev

        }
        if (m_map.size() > 0)
        {
            map<int, UserData*>::iterator iter = m_map.begin();
            while (iter != m_map.end())
            {
                ev.data.fd = iter->first;//设置用于写操作的文件描述符
                ev.events = EPOLLIN;//设置用于注测的读操作事件
                epoll_ctl(epfd, EPOLL_CTL_MOD, iter->first, &ev);//注册ev

                if (m_map[iter->first]->m_send->getPosition() > 0)
                {
                    ev.events = EPOLLOUT;//设置用于注测的写操作事件
                    epoll_ctl(epfd, EPOLL_CTL_MOD, iter->first, &ev);//修改sockfd上要处理的事件为EPOLLOUT
                }
                iter++;
            }
        }

        nfds = epoll_wait(epfd, events, 11000, 100);
        //处理所发生的所有事件
        for (int i=0; i<nfds; ++i)
        {
            if (events[i].events&EPOLLIN)
            {
                if ((sockfd = events[i].data.fd) < 0)
                {
                    continue;
                }
                epollRead(sockfd);
            }
            else if (events[i].events&EPOLLOUT)
            {
                if ((sockfd = events[i].data.fd) < 0)
                {
                    continue;
                }
                m_map[sockfd]->m_send->startSend(sockfd);
            }
        }
    }
}

void *Iomodel::startIo(void *arg)
{
    Iomodel *io = (Iomodel *)arg;
    io->funcIo();
}

void Iomodel::start()
{
    ret = pthread_create(&tid, NULL, startIo, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
