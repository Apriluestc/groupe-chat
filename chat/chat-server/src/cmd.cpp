#include "cmd.h"

CCmd::CCmd()
{
    m_userListToIO = new CUserList;
    /********初始化UDP套接字**********/
    m_udpsock = socket(AF_INET,SOCK_DGRAM,0);//UDP传输套接字创建
    bzero(&m_serveraddrUdp,0);
    m_serveraddrUdp.sin_family = AF_INET;//sin_family，协议族内一员
    m_serveraddrUdp.sin_port = htons(8888);//端口号，网络字节序,要转换成大端模式
    inet_aton("0.0.0.0", &(m_serveraddrUdp.sin_addr));//ip地址号，网络字节序，十进制与32位地址之间的转换
    bind(m_udpsock, (struct sockaddr *)(&m_serveraddrUdp), sizeof(m_serveraddrUdp));//绑定套节字并强转成struct sockaddr *型
    /******************************/
    m_num = 0;
}

CCmd::~CCmd()
{
    delete m_userListToIO;
}

CUserList *CCmd::get_vcdListToIO()
{
    return m_userListToIO;
}

/*获取I/O模块的数据包指针链表*/
void CCmd::setPackListFromIO(CList *packList)
{
    m_packList = packList;
}

void CCmd::setUnloginUserListFromIO(CUserList *unloginUserList)
{
    m_unLoginList = unloginUserList;
}

int CCmd::getUdpSockfd()
{
    return m_udpsock;
}

void CCmd::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void CCmd::sendHeartBeat(map<int, CUser*>::iterator iter)
{
    iter->second->m_recv.getRecvBuff();
    iter->second->m_recv.m_msgRecv = (struct messageRecv *)iter->second->m_recv.m_recvBuff;
    strcpy(iter->second->m_recv.m_msgRecv->msg, "heartbeat");
    iter->second->m_recv.m_msgRecv->whichClientIdToSend = iter->first;
    iter->second->m_recv.m_msgRecv->head.cmdId  = SENDtoONE;
    iter->second->m_recv.m_msgRecv->head.length = 30;
    iter->second->m_recv.m_msgRecv->head.userId = 0;
    m_packList->packPush(iter->second->m_recv.m_recvBuff);
    iter->second->m_heart.m_sendLastTime = time(NULL);
}

/*判断哪个用户的套接字需要设置进写集合，通过flag设置*/
void CCmd::setWriteSocketFdFlag(char *pack)
{
    char *resend2    = NULL;
    char *resend     = NULL;
    char *sendpack   = NULL;
    char *clientaddr = NULL;
    char *temaddr    = NULL;
    struct messageRecv *tmp;
    struct fileRecv    *fileTmp;
    struct fileSend    *sfileTmp;
    struct udpfileRecv *udpfileTmp;
    struct udpfileSend *udpsfileTmp;
    struct messageSend *stmp;
    struct udpLoseNumRecv *loseTmpRecv;
    struct udpLoseNumSend *loseTmpSend;
    struct baseHandle  *head = (struct baseHandle *)pack;
    map<int, CUser*>::iterator iter = m_map.begin();
    map<int, char*>::iterator udpiter;
    int num = 0;
    switch(head->cmdId)
    {
        case UDPRESEND:
            if (m_packList->getPackSize() > 0)
            {
                temaddr = m_packList->packPop();
                delete temaddr;
            }
            loseTmpRecv = (struct udpLoseNumRecv *)pack;
            for (int i=0; i<loseTmpRecv->num; i++)
            {
                udpiter = m_udpLoseMap.find(loseTmpRecv->loseNum[i]);
                if(udpiter != m_udpLoseMap.end())
                {
                    resend = new char[1436];
                    memcpy(resend, m_udpLoseMap[loseTmpRecv->loseNum[i]], 1436);
                    m_map[loseTmpRecv->head.userId]->m_udpSendList->packPush(resend);
                }else
                {
                    if (num == 0)
                    {
                        resend2 = new char[220];
                        loseTmpSend = (struct udpLoseNumSend *)resend2;
                        loseTmpSend->recvFromWhichClient = loseTmpRecv->head.userId;
                    }
                    loseTmpSend->loseNum[num] = loseTmpRecv->loseNum[i];
                    num++;
                }
            }
            if (num != 0)
            {
                loseTmpSend->num = num;
                loseTmpSend->head.cmdId = UDPRESEND;
                m_map[loseTmpRecv->whichClientIdToSend]->m_udpSendList->packPush(resend2);
                num = 0;
            }
            delete pack;
            pack = NULL;
        break;

        /*私聊*/
        case SENDtoONE:
            cout << "私聊" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            if (tmp->head.userId == 0)
            {
                stmp->head.cmdId = HEARTBEAT;
            }else
            {
                stmp->recvFromWhichClient = tmp->head.userId;
            }
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*群聊*/
        case SENDtoOTHERS:
            cout << "群聊" << endl;
            tmp = (struct messageRecv *)pack;
            while (iter != m_map.end())
            {
                stmp = (struct messageSend *)iter->second->m_send->m_copy(pack, tmp->head.length);
                stmp->recvFromWhichClient = tmp->head.userId;
                iter->second->m_send->setPosition(tmp->head.length);
                iter++;
            }
            delete pack;
            pack = NULL;
        break;

        /*文件*/
        case SENDFILENAME:
            cout << "发文件名" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            stmp->recvFromWhichClient = tmp->head.userId;
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*接收文件返回*/
        case FILERET:
            cout << "文件返回" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            stmp->recvFromWhichClient = tmp->head.userId;
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*发文件*/
        case SENDFILE:
            fileTmp = (struct fileRecv *)pack;
            sfileTmp = (struct fileSend *)m_map[fileTmp->sendToClientId]->m_send->m_copy(pack, fileTmp->head.length);
            sfileTmp->recvFromWhichId = fileTmp->head.userId;
            m_map[fileTmp->sendToClientId]->m_send->setPosition(fileTmp->head.length);
            delete pack;
            pack = NULL;
        break;

        case UDPSENDFILE:
            if (m_packList->getPackSize() > 0)
            {
                temaddr = m_packList->packPop();
                delete temaddr;
            }
            udpfileTmp = (struct udpfileRecv *)pack;
            sendpack = m_map[udpfileTmp->sendToClientId]->m_send->m_udpcopy(pack, udpfileTmp->head.length);
            udpsfileTmp = (struct udpfileSend *)sendpack;
            udpsfileTmp->recvFromWhichId = udpfileTmp->head.userId;
            m_map[udpfileTmp->sendToClientId]->m_udpSendList->packPush(sendpack);
            udpfileTmp->time = time(NULL);
            m_udpLoseMap.insert(pair<int, char*>(udpfileTmp->count, pack));
        break;

        case UDPREGISTER:
            fileTmp = (struct fileRecv *)pack;
            //cout << head->userId << "注册成功" << endl;
            clientaddr = m_packList->packPop();
            m_map[head->userId]->clientaddrUdp = (struct sockaddr_in *)(clientaddr);
            delete pack;
            pack = NULL;
        break;

        /*心跳*/
        case HEARTBEAT:
            cout << "心跳包" << endl;
            tmp = (struct messageRecv *)pack;
            m_map[tmp->head.userId]->m_heart.m_lastTime = time(NULL);
            delete pack;
            pack = NULL;
            break;

        case QUIT:
            m_map[tmp->head.userId]->deleteFlag1 = true;
            delete pack;
            pack = NULL;
        break;
    }
}

void CCmd::printAllLoginUser()
{
    map<int, CUser*>::iterator iter;
    iter = m_map.begin();
    cout << "login user list now:" << endl;
    cout << "+++++++++++++++++++++++++++" << endl;
    while(iter != m_map.end()) {

        cout << "userid:" << iter->second->getUserId() << "  socket:" << iter->second->getSocket() << endl;
        iter++;
    }
    cout << "+++++++++++++++++++++++++++" << endl;
}

/*线程启动函数*/
void CCmd::startDealCmd()
{
    char *pack = NULL;
    struct timeval currenttime;
    struct timeval lasttime;
    gettimeofday(&lasttime,NULL);
    m_lastLoopTime = time(NULL);
    for (; ; )
    {
        gettimeofday(&currenttime,NULL);
        if(currenttime.tv_sec-lasttime.tv_sec > 1)
        {
            gettimeofday(&lasttime,NULL);
            if (m_udpLoseMap.size() > 0)
            {
                map<int, char*>::iterator iter = m_udpLoseMap.begin();
                struct udpfileRecv *tmp = NULL;
                while (iter != m_udpLoseMap.end())
                {
                    tmp = (struct udpfileRecv *)iter->second;
                    if ((time(NULL) - tmp->time) > 2)
                    {
                        delete iter->second;
                        iter->second = NULL;
                        m_udpLoseMap.erase(iter);
                    }
                    iter++;
                }
            }
        }

        if (m_packList->getPackSize() > 0)
        {
            pack = m_packList->packPop();
            setWriteSocketFdFlag(pack);
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }

        if (m_unLoginList->getUnloginSize() > 0)
        {
            CUnloginuser *user = m_unLoginList->unloginUserPop();
            m_loginMap.insert(pair<int, CUnloginuser*>(user->m_socket, user));
        }

        if (time(NULL) - m_lastLoopTime > 1)
        {
            if (m_map.size() > 0)
            {
                map<int, CUser*>::iterator iter = m_map.begin();
                while (iter != m_map.end())
                {
                    /*判断客户端发国来的心跳包有没有超时,超时则删除*/
                    if (time(NULL) - iter->second->m_heart.m_lastTime > 40)
                    {
                        //iter->second->deleteFlag1 = true;
                    }

                    /*定时发送心跳包*/
                    if (time(NULL)-iter->second->m_heart.m_sendLastTime > 20)
                    {
                        //sendHeartBeat(iter);
                    }

                    /*删除*/
                    if (iter->second->deleteFlag1 && iter->second->deleteFlag2)
                    {
                        close(iter->second->getSocket());
                        delete(iter->second);
                        iter->second = NULL;
                        m_map.erase(iter);
                        printAllLoginUser();
                    }
                    iter++;
                }
            }
            m_lastLoopTime = time(NULL);
        }
        if (m_loginMap.size() > 0)
        {
            map<int, CUnloginuser*>::iterator loginiter = m_loginMap.begin();
            while (loginiter != m_loginMap.end())
            {
                loginiter->second->m_loginMsgRecv = (struct loginMessageRecv *)loginiter->second->recvbuff;
                if (loginiter->second->authFlag == false)
                {
                    if (strcmp(loginiter->second->m_loginMsgRecv->name, "admin") == 0
                            && strcmp(loginiter->second->m_loginMsgRecv->passwd, "admin") == 0)
                    {
                        /*认证成功*/
                        //cout << "用户名密码认证成功" << endl;
                        loginiter->second->loginState = LOGINSUCCESS;
                        strcpy(loginiter->second->m_loginMsgSend.result, "Y");
                        loginiter->second->m_loginMsgSend.loginHead.userId = loginiter->second->m_userId;
                    } else {
                        cout << "用户名密码认证failed" << endl;
                        loginiter->second->loginState = LOGINFAILED;
                        strcpy(loginiter->second->m_loginMsgSend.result, "N");
                        loginiter->second->m_loginMsgSend.loginHead.userId = loginiter->second->m_userId;
                    }
                    loginiter->second->authFlag = true;
                }
                if (loginiter->second->deleteFlag1 && loginiter->second->deleteFlag2)
                {
                    close(loginiter->first);
                    delete(loginiter->second);
                    loginiter->second = NULL;
                    m_loginMap.erase(loginiter);
                }
                if (loginiter->second->loginReturnFlag)
                {
                    if (loginiter->second->loginState == LOGINSUCCESS)
                    {
                        CUser *user;
                        user = new CUser;
                        user->setSocket(loginiter->first);
                        user->setUserId(loginiter->second->m_userId);

                        m_map.insert(pair<int, CUser*>(loginiter->second->m_userId, user));
                        //cout << "有一个正式用户" << endl;
                        m_userListToIO->vcdPush(user);
                        printAllLoginUser();
                    }
                    delete loginiter->second;
                    loginiter->second = NULL;
                    m_loginMap.erase(loginiter);
                }
                loginiter++;
            }
        }
    }
}

void *CCmd::startCmd(void *arg)
{
    CCmd *cmd = (CCmd *)arg;
    cmd->startDealCmd();
}

/*模块启动*/
void CCmd::start()
{
    int ret;
    ret = pthread_create(&m_tid, NULL, startCmd, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
