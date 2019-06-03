#include "io.h"
#include "accept.h"

CIo::CIo()
{
    m_packList = new CList;
    total = 0;
}

CIo::~CIo()
{
    delete m_packList;
}

/*数据包指针接口*/
CList *CIo::getPackList()
{
    return m_packList;
}

void CIo::setUdpFd(int fd)
{
    udpsock = fd;
}

/*接口链表的接收*/
void CIo::setVcdListFromCmdMode(CUserList *vcdList)
{
    m_vcdList = vcdList;
}

void CIo::deleteloginUser(int sockfd)
{
    ev.data.fd = sockfd;//设置与要处理的事件相关的文件描述符
    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);//注册epoll事件
    m_map[sockfd]->deleteFlag2 = true;
    m_map[sockfd]->deleteFlag1 = true;
    m_map.erase(sockfd);
}

void CIo::epollRead(int sockfd)
{
    /*判断是否满一个头的长度*/
    ioctl(sockfd, FIONREAD, &nread);
    if (nread == 0)
    {
        cout << "client(socket) " << sockfd << " has quit" << endl;
        deleteloginUser(sockfd);
        return;
    }
    if (nread >= sizeof(baseHandle) || m_map[sockfd]->m_recv.m_recvHeadFlag == 1)
    {
        if (m_map[sockfd]->m_recv.m_recvHeadFlag == 0)
        {
            if (!recv(sockfd, m_map[sockfd]->m_recv.getRecvBuff(), sizeof(baseHandle), 0))
            {
                deleteloginUser(sockfd);
                return;
            }
            struct baseHandle *tmp = (struct baseHandle *)m_map[sockfd]->m_recv.m_recvBuff;
            m_map[sockfd]->m_recv.m_totalLengthOfData = tmp->length;
            m_map[sockfd]->m_recv.m_lengthHasRecv = sizeof(baseHandle);
            m_map[sockfd]->m_recv.m_recvHeadFlag = true;
        }
        if (m_map[sockfd]->m_recv.m_recvHeadFlag == 1)
        {
            int nres = m_map[sockfd]->m_recv.m_totalLengthOfData - m_map[sockfd]->m_recv.m_lengthHasRecv;
            nread = recv(sockfd,
                  m_map[sockfd]->m_recv.m_recvBuff + m_map[sockfd]->m_recv.m_lengthHasRecv, nres, 0);
            if (nread < 0)
            {
                deleteloginUser(sockfd);
            }
            m_map[sockfd]->m_recv.m_lengthHasRecv += nread;
            if (m_map[sockfd]->m_recv.m_lengthHasRecv == m_map[sockfd]->m_recv.m_totalLengthOfData)
            {
                m_packList->packPush(m_map[sockfd]->m_recv.m_recvBuff);
                m_map[sockfd]->m_recv.m_lengthHasRecv = 0;
                m_map[sockfd]->m_recv.m_recvHeadFlag = false;
            }
        }
     }
}

void CIo::startRequestFunc()
{
    int i;
    epfd = epoll_create(11000);//生成用于处理accept的epoll专用的文件描述符

    for (; ; )
    { 
        if (m_vcdList->getSize() > 0)
        {
            CUser *user = m_vcdList->vcdPop();
            m_map.insert(pair<int, CUser*>(user->getSocket(), user));/*用户ID和套接字一一对应插入map中*/

            ev.data.fd = user->getSocket();//设置用于写操作的文件描述符
            ev.events = EPOLLIN;//设置用于注测的读操作事件
            epoll_ctl(epfd, EPOLL_CTL_ADD, user->getSocket(), &ev);//注册ev
        }

        if (m_map.size() > 0)
        {
            map<int, CUser*>::iterator iter = m_map.begin();
            while (iter != m_map.end())
            {

                if (iter->second->m_send->getPosition() > 0)
                {
                    ev.data.fd = iter->first;//设置用于写操作的文件描述符
                    ev.events = EPOLLOUT;//设置用于注测的写操作事件
                    epoll_ctl(epfd, EPOLL_CTL_MOD, iter->first, &ev);//修改sockfd上要处理的事件为EPOLLOUT
                }

                /*判断有没有正常退出的*/
                if (iter->second->deleteFlag1)
                {
                    deleteloginUser(iter->first);
                }
                iter++;
            }
        }

        nfds = epoll_wait(epfd, events, 11000, 100);
        //处理所发生的所有事件
        for (i=0; i<nfds; ++i)
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
              nread = m_map[sockfd]->m_send->startSend(sockfd);
              if (nread == -1)
              {
                  deleteloginUser(sockfd);
              }
              ev.data.fd = sockfd;//设置用于写操作的文件描述符
              ev.events = EPOLLIN;//设置用于注测的读操作事件
              epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);//注册ev
         }
      }
   }
}

void CIo::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void CIo::startUdpFunc()
{
    socklen_t tmp_clientlen = sizeof(struct sockaddr_in);
    for ( ; ;)
    {
        struct sockaddr_in *tmp_clientaddrUdp = new (struct sockaddr_in);
        nread = recvfrom(udpsock, m_recv.getRecvBuff(), 1436, 0,
                (struct sockaddr *)tmp_clientaddrUdp, &tmp_clientlen);
        if (nread < 0)
        {
            cout << "recvfrom error" << endl;
            return;
        }
        m_packList->packPush(m_recv.m_recvBuff);
        m_packList->packPush((char *)tmp_clientaddrUdp);
    }
}

void CIo::startUdpSendFunc()
{
    char *pack = NULL;
    int flag = 0;
    for( ; ;)
    {
        if (m_map.size() > 0)
        {
            map<int, CUser*>::iterator iter = m_map.begin();
            while (iter != m_map.end())
            {
                flag +=  iter->second->m_udpSendList->getPackSize();
                if ( iter->second->m_udpSendList->getPackSize() > 0)
                {
                    pack = iter->second->m_udpSendList->packPop();
                    sendto(udpsock, pack, 1436, 0, (struct sockaddr *) iter->second->clientaddrUdp,
                             iter->second->clientlen);
                    delete pack;
                    pack = NULL;
                }
                iter++;
            }
            if (flag == 0)
            {
                m_usleep(10000);
            }else
            {
                m_usleep(40);
            }
            flag = 0;
        }
        else
        {
            m_usleep(10000);
        }
    }
}


/*开启线程函数*/
void *CIo::startRequest(void *arg)
{
    CIo *request = (CIo *)arg;
    request->startRequestFunc();
}

void *CIo::startUdp(void *arg)
{
    CIo *udp = (CIo *)arg;
    udp->startUdpFunc();
}

void *CIo::startUdpSend(void *arg)
{
    CIo *udp = (CIo *)arg;
    //udp->startUdpSendFunc();
}

/*模块启动函数*/
void CIo::start()
{
    int ret;
    ret = pthread_create(&tid, NULL, startRequest, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&tid2, NULL, startUdp, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&tid3, NULL, startUdpSend, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
