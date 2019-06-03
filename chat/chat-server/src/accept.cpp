#include "accept.h"

CAccept::CAccept()
{
    /*公用接口*/
    m_vcdListtoIO  = new CList;
    m_vcdLoginList = new CUserList;
    /*用户ID初始化为1，以后不断垒加*/
    m_userId = 1;

    /*长度初始化*/
    m_clilen = sizeof(m_clientaddr);
}

CAccept::~CAccept()
{
    /*关闭监听套接字*/
    close(m_listenfd);

    /*关闭epoll套接字*/
    close(m_epfd);

    /*删除公用链表*/
    delete m_vcdListtoIO;
    delete m_vcdLoginList;
}

/*登录模块公用链表接口*/
CList *CAccept::IOGetVcdList()
{
    return m_vcdListtoIO;
}

CUserList *CAccept::getVcdLoginList()
{
    return m_vcdLoginList;
}

void CAccept::deleteOneUnloginUser(int sockfd)
{
    m_loginMap[sockfd]->deleteFlag1 = true;
    m_loginMap[sockfd]->deleteFlag2 = true;
    m_ev.data.fd = sockfd;//设置与要处理的事件相关的文件描述符
    epoll_ctl(m_epfd, EPOLL_CTL_DEL, sockfd, &m_ev);//注册epoll事件
    m_loginMap.erase(sockfd);
}

/*获取当前时间和本套接字连接时的时间相比，超过一分钟，表示登录出错，删除它，并返回错误信息*/
void CAccept::timeoutFunc(int sockfd)
{
    time_t tmpTime = time(NULL);
    if (tmpTime - m_loginMap[sockfd]->loginTime > 60)
    {
        /*设置错误返回信息*/
        strcpy(m_loginMap[sockfd]->m_loginMsgSend.result, "N");
        send(m_loginMap[sockfd]->m_socket, &m_loginMap[sockfd]->m_loginMsgSend, sizeof(loginMessageSend), 0);
        deleteOneUnloginUser(m_connfd);
    }
}

/*副线程主函数*/
void CAccept::funcAccept ()
{
    int i;
    m_epfd = epoll_create(11000);//生成用于处理accept的epoll专用的文件描述符
    m_ev.data.fd = m_listenfd;//设置与要处理的事件相关的文件描述符
    m_ev.events = EPOLLIN|EPOLLET;//设置要处理的事件类型
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &m_ev);//注册epoll事件
    for (; ; )
    {
        map<int, CUnloginuser*>::iterator iter = m_loginMap.begin();

        while (iter != m_loginMap.end())
        {
            m_ev.data.fd = iter->first;//设置用于写操作的文件描述符
            if (iter->second->loginState != NOLOGIN)
            {
                m_ev.data.fd = iter->first;//设置与要处理的事件相关的文件描述符
                m_ev.events = EPOLLOUT;//设置用于注测的写操作事件
                epoll_ctl(m_epfd, EPOLL_CTL_MOD, iter->first, &m_ev);//修改sockfd上要处理的事件为EPOLLOUT
            }
            iter++;
        }
          m_nfds = epoll_wait(m_epfd, m_events, 11000, 1);
          //处理所发生的所有事件
          for (i=0; i<m_nfds; ++i)
          {
              if (m_events[i].data.fd == m_listenfd)//事件发生在listenfd上
              {
                  m_connfd = accept(m_listenfd, (struct sockaddr *)&m_clientaddr, &m_clilen);
                  if (m_connfd < 0)
                  {
                      perror("connfd < 0");
                      continue;
                  }
                  fcntl(m_connfd, F_SETFL, O_NONBLOCK);

                  cout << "有套接字连接:" << " socket=" << m_connfd << "  userId=" << m_userId << endl;

                  m_ev.data.fd = m_connfd;//设置用于读操作的文件描述符
                  m_ev.events = EPOLLIN;//设置用于注测的读操作事件
                  epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_connfd, &m_ev);//注册ev
                  CUnloginuser *user;
                  user = new CUnloginuser;
                  user->m_socket = m_connfd;
                  user->m_userId   = m_userId;
                  user->loginTime = time(NULL);           /*记录连接的时间，防止连接超时*/
                  m_vcdLoginList->unloginUserPush(user);
                  m_loginMap.insert(pair<int, CUnloginuser*>(m_connfd, user));

              }
              else if (m_events[i].events&EPOLLIN)
              {
                  if ((m_connfd = m_events[i].data.fd) < 0)
                  {
                      continue;
                  }
                  /*判断recv底层buff是否够一个头的长度*/
                  ioctl(m_connfd, FIONREAD, &m_nread);
                  if (m_nread == 0)
                  {
                      deleteOneUnloginUser(m_connfd);
                  }
                  if (m_nread >= sizeof(baseHandle) || !m_loginMap[m_connfd]->recvHeadFlag)
                  {
                      cout << "socket--" << m_connfd << " --发来登录信息" << endl;
                      /*如果第一次，则只接收一个头的长度*/
                      if (m_loginMap[m_connfd]->recvHeadFlag)
                      {
                          if (!recv(m_connfd, m_loginMap[m_connfd]->recvbuff, sizeof(baseHandle), 0))
                          {
                               deleteOneUnloginUser(m_connfd);
                               continue;
                          }
                          struct loginMessageRecv *tmp = (struct loginMessageRecv *)m_loginMap[m_connfd]->recvbuff;
                          /*获取头以后解析并存储整个包的长度*/
                          if (tmp->loginHead.cmdId == LOGIN)
                          {
                              m_loginMap[m_connfd]->totalLengthOfData = tmp->loginHead.length;
                              m_loginMap[m_connfd]->remainLengthofRecv = tmp->loginHead.length - sizeof(baseHandle);
                              m_loginMap[m_connfd]->recvHeadFlag = false;
                          }else
                          {
                               deleteOneUnloginUser(m_connfd);
                          }
                      }

                      /*如果不是第一次，则接收头以外的所有数据*/
                      if (!m_loginMap[m_connfd]->recvHeadFlag)
                      {
                          m_nread = recv(m_loginMap[m_connfd]->m_socket, m_loginMap[m_connfd]->recvbuff+sizeof(baseHandle),
                                  m_loginMap[m_connfd]->remainLengthofRecv, 0);
                          if (m_nread > 0)
                          {
                              m_loginMap[m_connfd]->remainLengthofRecv -= m_nread;
                              if (m_loginMap[m_connfd]->remainLengthofRecv == 0)
                              {
                                  m_loginMap[m_connfd]->recvHeadFlag = true;
                              }
                          }else
                          {
                              deleteOneUnloginUser(m_connfd);
                          }
                      }
                  }else
                  {
                      /*获取当前时间和本套接字连接时的时间相比，超过一分钟，表示登录出错，删除它，并返回错误信息*/
                      timeoutFunc(m_connfd);
                  }
              }
              else if ((m_events[i].events&EPOLLOUT))
              {
                  int nread;
                  m_connfd = m_events[i].data.fd;
                  /*return*/
                  nread = send(m_connfd, &m_loginMap[m_connfd]->m_loginMsgSend, sizeof(loginMessageSend), 0);
                  if (nread == 0)
                  {
                      deleteOneUnloginUser(m_connfd);
                      continue;
                  }
                  m_loginMap[m_connfd]->remainSend -= nread;
                  if (m_loginMap[m_connfd]->remainSend == 0)
                  {
                      m_ev.data.fd = m_connfd;//设置与要处理的事件相关的文件描述符
                      epoll_ctl(m_epfd, EPOLL_CTL_DEL, m_connfd, &m_ev);//注册epoll事件
                      if (m_loginMap[m_connfd]->loginState == LOGINSUCCESS)
                      {
                          //cout << "userId++" << endl;
                          m_userId++;/*用户ID自加1,分给下个用户*/
                      }
                      m_loginMap[m_connfd]->loginReturnFlag = true;
                      /*将用户从未登录链表中删除*/
                      m_loginMap.erase(m_connfd);
                  }
              }
         }
    }
}

void *CAccept::startAccept(void *arg)
{
    CAccept *acc = (CAccept *)arg;
    acc->funcAccept();
}

/*主线程启动*/
void CAccept::start()
{
    struct sockaddr_in servaddr;
    int ret;
    int flag = 1;
    int socket_len = sizeof(int);

    m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenfd < 0)
    {
        perror("socket init failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(IP, &(servaddr.sin_addr));
    servaddr.sin_port = htons(PORT);

    /*设置socket关闭后能马上继续使用*/
    ret = setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, socket_len);
    if( ret == -1)
    {
        perror("setsockopt");
    }

    /*绑定*/
    ret = bind(m_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret < 0)
    {
        perror("bind fail");
        exit(1);
    }

    /*监听*/
    ret = listen(m_listenfd, LISTENNUM);
    if(ret < 0)
    {
        perror("listen fail");
        exit(1);
    }

    /*创建新线程接收新客户端套接字*/
    ret = pthread_create(&m_tid, NULL, startAccept, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
