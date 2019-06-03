#include "iomodel.h"

IOModel::IOModel()
{
    m_readList = new SaveList;
    sendNum = 0;
    recvNum = 0;
    udpSendNum = 1;
}

IOModel::~IOModel()
{
    delete m_readList;
}

SaveList *IOModel::get_readList()
{
    return m_readList;
}

void IOModel::set_userdata(UserData *userdata)
{
    m_userdata = userdata;
}

pthread_t IOModel::get_m_udptid()
{
    return m_udptid;
}

pthread_t IOModel::get_m_tid()
{
    return m_tid;
}

void IOModel::Start()
{
    while(1)
    {
        FD_ZERO(&wset);
        FD_ZERO(&rset);
        FD_SET(m_userdata->sockfd,  &rset);

        if (m_userdata->haveSendFlag)
        {
            FD_SET(m_userdata->sockfd, &wset);
        }

        timeout.tv_sec  = 0;
        timeout.tv_usec = 1000;
        int max = m_userdata->sockfd > m_userdata->sockUdp ? m_userdata->sockfd : m_userdata->sockUdp;
        ret = select(max + 1, &rset, &wset, NULL, &timeout);
        if(ret <= 0)
        {
            continue;
        }

        if (FD_ISSET(m_userdata->sockfd, &rset))
        {
            ioctl(m_userdata->sockfd, FIONREAD, &nread);
            if (nread == 0)
            {
                cout << "ioctl = 0" << endl;
                exit(0);
            }
            if (nread >= sizeof(baseHandle) || m_userdata->recvHeadFlag)
            {
                /*如果是第一次,则只接收一个头*/
                if (!m_userdata->recvHeadFlag)
                {
                    if (!recv(m_userdata->sockfd, m_userdata->get_recv(), sizeof(baseHandle), 0))
                    {
                        perror("recv:");
                        exit(0);
                    }
                    struct baseHandle *tmp = (struct baseHandle *)m_userdata->recvbuff;
                    m_userdata->totalLengthOfData = tmp->length;
                    m_userdata->remainLengthtoRecv = tmp->length - sizeof(baseHandle);
                    m_userdata->recvHeadFlag = true;
                }

                /*如果不是第一次，则接收完其他的数据*/
                if (m_userdata->recvHeadFlag)
                {
                    nread = recv(m_userdata->sockfd,
                    m_userdata->recvbuff+16+m_userdata->lengthHasRecv, m_userdata->remainLengthtoRecv, 0);
                    if (nread > 0)
                    {
                        m_userdata->remainLengthtoRecv -= nread;
                        m_userdata->lengthHasRecv += nread;
                        if (m_userdata->remainLengthtoRecv == 0)
                        {
                            m_userdata->recvHeadFlag = false;
                            m_userdata->remainLengthtoRecv = m_userdata->totalLengthOfData;
                            m_userdata->lengthHasRecv = 0;
                            m_userdata->haveReadFlag = true;
                            m_readList->pack_push(m_userdata->recvbuff);
                        }
                    }else
                    {
                        perror("recv:");
                        exit(0);
                    }
                }
            }
        }

        if (FD_ISSET(m_userdata->sockfd, &wset))
        {
            while (m_userdata->remainLengthofSend > 0)
            {
                nread = send(m_userdata->sockfd,
                    m_userdata->sendbuff+m_userdata->lengthHasSend, m_userdata->remainLengthofSend, 0);
                if (nread < 0)
                {
                    perror("send error");
                    exit(0);
                }
                m_userdata->lengthHasSend += nread;
                m_userdata->remainLengthofSend -= nread;
            }
            delete m_userdata->sendbuff;
            m_userdata->lengthHasSend = 0;
            m_userdata->haveSendFlag = 0;
        }
    }
}

void IOModel::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void IOModel::Startudpsend()
{
    char *pack = NULL;
    struct udpfileSend *tmp;
    socklen_t tmp_serverlen = sizeof(m_userdata->servaddrudp);
    for( ; ;)
    {
        if (m_userdata->m_writeList->getPackSize() > 0)
        {
            pack = m_userdata->m_writeList->pack_pop();
            tmp = (struct udpfileSend *)pack;
            if (udpSendNum % 3 == 1 || (tmp->count == tmp->filesize/1390+1))
            {
                sendto(m_userdata->sockUdp, pack, 1436, 0,
                      (struct sockaddr *)(&m_userdata->servaddrudp), tmp_serverlen);
            }
            delete pack;
            pack = NULL;
            udpSendNum++;
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }
    }
}

void IOModel::Startudprecv()
{
    while (1)
    {
        char* udprecvbuff = new char[1436];
        nread = recvfrom(m_userdata->sockUdp, udprecvbuff, 1436, 0, NULL, NULL);
        if (nread < 0)
        {
            cout << "recvfrom error" << endl;
            return;
        }
        m_readList->pack_push(udprecvbuff);
    }
}

void *IOModel::start(void *arg)
{
    IOModel *IO = (IOModel *)arg;
    IO->Start();
}

void *IOModel::startudprecv(void *arg)
{
    IOModel *IO = (IOModel *)arg;
    IO->Startudprecv();
}

void *IOModel::startudpsend(void *arg)
{
    IOModel *IO = (IOModel *)arg;
    IO->Startudpsend();
}

void IOModel::IOStart()
{
    /*创建新线程接收新客户端套接字*/
    ret = pthread_create(&m_tid, NULL, start, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&m_udptid, NULL, startudprecv, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&m_udpsendtid, NULL, startudpsend, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
