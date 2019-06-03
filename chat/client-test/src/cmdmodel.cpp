#include "cmdmodel.h"

CmdModel::CmdModel()
{
    stdinfd = 0;
    nwrite = 0;
    pthread_mutex_init(&m_mutex, NULL);
    nowPackNum = 0;
    m_MsgSend = NULL;
    m_udpList = new SaveList;
}

CmdModel::~CmdModel()
{
    delete m_udpList;
}

void CmdModel::set_userdata(UserData *userdata)
{
    m_userdata = userdata;
}

void CmdModel::setReadListFromIOModel(SaveList *readList)
{
    m_readList = readList;
}

pthread_t CmdModel::get_m_tid()
{
    return m_tid;
}

void CmdModel::printCmdIdList()
{
    char c;
    cout << "###### 1. 私聊   ######" << endl;
    cout << "###### 2. 群聊   ######" << endl;
    cout << "###### 3. 文件   ######" << endl;
    cout << "###### 4. 退出   ######" << endl;
    cout << "请选择发送方式：" << endl;
    cin >> c;
    switch(c)
    {
        case '1':
            m_userdata->get_send();
            m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
            cout << "请输入聊天内容:";
            cin >> m_userdata->m_MsgSend->msg;
            m_userdata->m_MsgSend->head.version = 1;
            m_userdata->m_MsgSend->head.userId  = m_userdata->userId;
            m_userdata->m_MsgSend->head.length  = 16 + 4 + strlen(m_userdata->m_MsgSend->msg) + 1;
            m_userdata->remainLengthofSend = m_userdata->m_MsgSend->head.length;
            cout << "请输入对方ID:";
            cin >> m_userdata->m_MsgSend->sayToClientId;
            m_userdata->m_MsgSend->head.cmdId = SENDtoONE;
            break;

        case '2':
            m_userdata->get_send();
            m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
            cout << "请输入聊天内容:";
            cin >> m_userdata->m_MsgSend->msg;
            m_userdata->m_MsgSend->head.version = 1;
            m_userdata->m_MsgSend->head.userId  = m_userdata->userId;
            m_userdata->m_MsgSend->head.length  = 16 + 4 + strlen(m_userdata->m_MsgSend->msg) + 1;
            m_userdata->remainLengthofSend = m_userdata->m_MsgSend->head.length;
            m_userdata->m_MsgSend->head.cmdId = SENDtoOTHERS;
            break;

        case '3':
            m_userdata->get_send();
            m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
            cout << "请输入文件名:";
            cin >> m_userdata->m_MsgSend->msg;
            m_userdata->m_MsgSend->head.version = 1;
            m_userdata->m_MsgSend->head.userId  = m_userdata->userId;
            m_userdata->m_MsgSend->head.length  = 16 + 4 + strlen(m_userdata->m_MsgSend->msg) + 1;
            m_userdata->remainLengthofSend = m_userdata->m_MsgSend->head.length;
            cout << "请输入对方ID:";
            cin >> m_userdata->m_MsgSend->sayToClientId;
            m_userdata->m_MsgSend->head.cmdId = SENDFILENAME;

            m_userdata->sendFileToClientId = m_userdata->m_MsgSend->sayToClientId;//记录文件给谁
            strcpy(m_userdata->filename, m_userdata->m_MsgSend->msg);//记录文件名
            break;

        case '4':
            m_userdata->m_MsgSend->head.cmdId = QUIT;
            break;
    }
}

void CmdModel::returnYesorNo(const char *ret, int toWhichId)
{
    m_userdata->get_send();
    m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
    strcpy(m_userdata->m_MsgSend->msg, ret);
    m_userdata->m_MsgSend->head.version  = 1;
    m_userdata->m_MsgSend->sayToClientId = toWhichId;
    m_userdata->m_MsgSend->head.cmdId    = FILERET;
    m_userdata->m_MsgSend->head.userId   = m_userdata->userId;
    m_userdata->m_MsgSend->head.length   = 16 + 4 + 1 + 1;
    m_userdata->remainLengthofSend       = m_userdata->m_MsgSend->head.length;
    m_userdata->haveSendFlag = 1;
}

void CmdModel::recvFileDecide(char *info, int id)
{
    char c;
    cout << "y: 接收      n: 拒绝" << endl;
    cin >> c;
    switch (c)
    {
        case 'y':
            strcat(info, "++");
            m_userdata->pf = fopen(info, "a+");
            returnYesorNo("Y", id);
            break;

        case 'n':
            cout << "拒绝" << endl;
            returnYesorNo("N", id);
            break;
    }
}

void CmdModel::writeFile(int fileseek)
{
    fseek(m_userdata->pf, 1390*(fileseek-1), SEEK_SET);
    nwrite = fwrite(m_userdata->m_udpfileRecv->fileContext, 1,
                    m_userdata->m_udpfileRecv->head.length-46, m_userdata->pf);
    if (nwrite < 0)
    {
        perror("fwrite error:");
        exit(0);
    }
    if (nwrite != 1390)
    {
        cout << "nwrite = " << nwrite;
        cout << "长度:" << m_userdata->m_udpfileRecv->head.length-46 << endl;
    }
    m_userdata->recvFileSize += nwrite;
    m_userdata->recvFileCount ++;
    if (m_userdata->recvFileSize == m_userdata->m_udpfileRecv->filesize)
    {
        cout << "文件收完，最后总共收了:" << m_userdata->recvFileSize << endl;
        m_userdata->recvFileSize  = 0;
        m_userdata->recvFileCount = 0;
        m_userdata->sendFileCount = 0;
        fclose(m_userdata->pf);
        cout << "收完的时间:" << time(NULL) << endl;
    }
}

void CmdModel::dealWithMessage(char *pack)
{
    struct baseHandle *tmp = (struct baseHandle *)pack;
    map<int, int>::iterator iter;
    switch(tmp->cmdId)
    {
        case UDPRESEND:
            m_udpList->pack_push(pack);
        break;
        case SENDtoONE:
            m_userdata->m_MsgRecv = (struct messageRecv *)pack;
            cout << "ID " << m_userdata->m_MsgRecv->recvFromWhichId << " 对你私聊说: ";
            cout << m_userdata->m_MsgRecv->msg << endl;
            delete pack;
            pack = NULL;
        break;

        case SENDtoOTHERS:
            m_userdata->m_MsgRecv = (struct messageRecv *)pack;
            cout << "ID " << m_userdata->m_MsgRecv->recvFromWhichId << " 群聊说: ";
            cout << m_userdata->m_MsgRecv->msg << endl;
            delete pack;
            pack = NULL;
        break;

        case SENDFILENAME:
            m_userdata->m_MsgRecv = (struct messageRecv *)pack;
            cout << "ID " << m_userdata->m_MsgRecv->recvFromWhichId << " 想发送文件: ";
            cout << m_userdata->m_MsgRecv->msg << endl;
            cout << "是否接收?" << endl;
            recvFileDecide(m_userdata->m_MsgRecv->msg, m_userdata->m_MsgRecv->recvFromWhichId);
            delete pack;
            pack = NULL;
        break;

        case FILERET:
            m_userdata->m_MsgRecv = (struct messageRecv *)pack;
            if (strcmp(m_userdata->m_MsgRecv->msg, "Y") == 0)
            {
                cout << "开始传输" << endl;
                m_userdata->fileSize = m_userdata->getFileSize();
                cout << m_userdata->fileSize << endl;
                m_userdata->pf = fopen(m_userdata->filename, "r");
                if (m_userdata->pf == NULL)
                {
                    perror("open error:");
                    exit(0);
                }
                m_userdata->startTransfer = true;
                sendtime = time(NULL);
            }else
            {
                cout << "传输失败，对方拒绝" << endl;
            }
            delete pack;
            pack = NULL;
        break;

        case SENDFILE:
            m_userdata->m_fileRecv = (struct fileRecv *)pack;
            nwrite = fwrite(m_userdata->m_fileRecv->fileContext, 1, tmp->length-124, m_userdata->pf);
            if (nwrite < 0)
            {
                perror("fwrite error:");
                exit(0);
            }
            if (nwrite != 1300)
            {
                cout << "nwrite=" << nwrite;
                cout << "长度:" << tmp->length-124 << endl;
            }
            m_userdata->recvFileSize += nwrite;
            if (m_userdata->recvFileSize == m_userdata->m_fileRecv->filesize)
            {
                cout << "文件收完，最后总共收了:" << m_userdata->recvFileSize << endl;
                m_userdata->recvFileSize = 0;
                m_userdata->recvFileCount = 0;
                m_userdata->sendFileCount = 0;
                fclose(m_userdata->pf);
            }
            delete pack;
            pack = NULL;
        break;

        case UDPSENDFILE:
            m_userdata->m_udpfileRecv = (struct udpfileRecv *)pack;
                if((m_userdata->m_udpfileRecv->count - nowPackNum) < 1)
                {
                    iter = m_udpMap.find(m_userdata->m_udpfileRecv->count);
                    if(iter != m_udpMap.end())
                    {
                        m_udpMap.erase(iter);
                        writeFile(m_userdata->m_udpfileRecv->count);
                    }
                }
                else if ((m_userdata->m_udpfileRecv->count - nowPackNum) == 1)
                {
                    nowPackNum = m_userdata->m_udpfileRecv->count;
                    writeFile(m_userdata->m_udpfileRecv->count);
                }
                else
                {
                    for (int i= nowPackNum+1; i<m_userdata->m_udpfileRecv->count; i++)
                    {
                        m_udpMap[i] = m_userdata->m_udpfileRecv->head.userId;
                    }
                    nowPackNum = m_userdata->m_udpfileRecv->count;
                    writeFile(m_userdata->m_udpfileRecv->count);
                }

            delete pack;
            pack = NULL;
        break;

        case HEARTBEAT:
            cout << "heartbeat" << endl;
            if (time(NULL) - m_userdata->lastHeartPackRecvTime > 40)
            {
                //cout << "server is close recv" << endl;
            }
            m_userdata->lastHeartPackRecvTime = time(NULL);
            delete pack;
            pack = NULL;
        break;
    }
}

void CmdModel::udpSendFile(int nread)
{
    char *udpsend = new char[1436];
    struct udpfileSend *udpfilesend = (struct udpfileSend *)udpsend;
    nread = fread(udpfilesend->fileContext, 1, 1390, m_userdata->pf);
    if (nread <= 0)
    {
        perror("fread error:");
        exit(0);
    }
    m_userdata->sendFileCount += 1;
    udpfilesend->head.version = 1;
    udpfilesend->head.cmdId = UDPSENDFILE;
    udpfilesend->head.userId  = m_userdata->userId;
    udpfilesend->head.length  = 16 + 20 + 10 + nread;
    udpfilesend->count = m_userdata->sendFileCount;
    udpfilesend->time = time(NULL);
    udpfilesend->sendToClientId = m_userdata->sendFileToClientId;
    udpfilesend->filesize = m_userdata->fileSize;
    strcpy(udpfilesend->filename, m_userdata->filename);

    char *udpresend = new char[1436];
    memcpy(udpresend, udpsend, 1436);
    m_udpSaveMap.insert(pair<int, char*>(m_userdata->sendFileCount, udpresend));
    m_userdata->m_writeList->pack_push(udpsend);
}

void CmdModel::tcpSendFile(int nread)
{
    m_userdata->get_send();
    m_userdata->m_fileSend = (struct fileSend *)m_userdata->sendbuff;
    nread = fread(m_userdata->m_fileSend->fileContext, 1, 1300, m_userdata->pf);
    if (nread <= 0)
    {
        perror("fread error:");
        exit(0);
    }
    m_userdata->sendFileCount += 1;
    cout << "已经发的次数:" << m_userdata->sendFileCount << endl;
    m_userdata->m_fileSend->head.version = 1;
    m_userdata->m_fileSend->head.cmdId = SENDFILE;
    m_userdata->m_fileSend->head.userId  = m_userdata->userId;
    m_userdata->m_fileSend->head.length  = 16 + 8 + 100 + nread;
    m_userdata->m_fileSend->sendToClientId = m_userdata->sendFileToClientId;
    m_userdata->m_fileSend->filesize = m_userdata->fileSize;
    strcpy(m_userdata->m_fileSend->filename, m_userdata->filename);
    m_userdata->remainLengthofSend = m_userdata->m_fileSend->head.length;
    m_userdata->lengthHasSend = 0;
    m_userdata->haveSendFlag = 1;
}

void CmdModel::udpSendRegister()
{
    m_userdata->get_send();
    m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
    strcpy(m_userdata->m_MsgSend->msg, "Y");
    m_userdata->m_MsgSend->head.version  = 1;
    m_userdata->m_MsgSend->head.cmdId    = UDPREGISTER;
    m_userdata->m_MsgSend->head.userId   = m_userdata->userId;
    m_userdata->m_MsgSend->head.length   = 1436;
    m_userdata->m_writeList->pack_push(m_userdata->sendbuff);
}

void CmdModel::sendHeartBeat()
{
    m_userdata->get_send();
    m_userdata->m_MsgSend = (struct messageSend *)m_userdata->sendbuff;
    strcpy(m_userdata->m_MsgSend->msg, "heartbeat");
    m_userdata->m_MsgSend->head.version  = 1;
    m_userdata->m_MsgSend->sayToClientId = 0;
    m_userdata->m_MsgSend->head.cmdId    = HEARTBEAT;
    m_userdata->m_MsgSend->head.userId   = m_userdata->userId;
    m_userdata->m_MsgSend->head.length   = 16 + 4 + 9 + 1;
    m_userdata->remainLengthofSend       = m_userdata->m_MsgSend->head.length;
    m_userdata->haveSendFlag = 1;
    m_userdata->lastSendTime  = time(NULL);
}

void CmdModel::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void CmdModel::startDealUdpPack()
{

}

void CmdModel::StartSendFile()
{
    struct timeval currenttime;
    struct timeval lasttime;
    struct udpLoseNumRecv *loseTmpRecv;
    map<int, char*>::iterator udpiter;
    gettimeofday(&lasttime,NULL);
    for( ; ;)
    {
        gettimeofday(&currenttime,NULL);
        if(currenttime.tv_sec-lasttime.tv_sec > 1)
        {
            gettimeofday(&lasttime,NULL);
            if (m_udpSaveMap.size() > 0)
            {
                map<int, char*>::iterator iter = m_udpSaveMap.begin();
                struct udpfileSend *tmp = NULL;
                while (iter != m_udpSaveMap.end())
                {
                    tmp = (struct udpfileSend *)iter->second;
                    if ((time(NULL) - tmp->time) > 200)
                    {
                        delete iter->second;
                        iter->second = NULL;
                        m_udpSaveMap.erase(iter);
                    }
                    iter++;
                }
            }
        }

        if (m_udpList->getPackSize() > 0)
        {
            char *pack = m_udpList->pack_pop();
            loseTmpRecv = (struct udpLoseNumRecv *)pack;
            for (int i=0; i<loseTmpRecv->num; i++)
            {
                udpiter = m_udpSaveMap.find(loseTmpRecv->loseNum[i]);
                if(udpiter != m_udpSaveMap.end())
                {
                    char *resend = new char[1436];
                    memcpy(resend, m_udpSaveMap[loseTmpRecv->loseNum[i]], 1436);
                    m_userdata->m_writeList->pack_push(resend);
                }
            }
            delete pack;
            pack = NULL;
        }

        if (m_userdata->m_writeList->getPackSize() > 100)
        {
            m_usleep(400);
        }

        if (m_userdata->startTransfer && m_userdata->pf)
        {
            if (!feof(m_userdata->pf))
            {
//                if (m_userdata->haveSendFlag == 0)
//                {
//                    tcpSendFile(nread);
//                }
                udpSendFile(nread);
            }else
            {\
                cout << "开始发送的时间:" << sendtime << endl;
                cout << "发出去使用的时间:" << time(NULL) - sendtime << endl;

                cout << "时间:";
                gettimeofday(&currenttime,NULL);
                cout << currenttime.tv_sec << endl;
                cout << currenttime.tv_usec << endl;

                cout << "总共发了:" << m_userdata->sendFileCount << endl;
                fclose(m_userdata->pf);
                m_userdata->startTransfer = false;
            }
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }
    }
}

void CmdModel::Start()
{
    m_userdata->lastHeartPackRecvTime = time(NULL);
    m_userdata->lastSendTime = time(NULL);

    /*登录成功以后发送一个udp包*/
    udpSendRegister();
    /***********************/

    for (; ; )
    {
        if (time(NULL) - m_userdata->lastSendTime > 20)
        {
            if (m_userdata->haveSendFlag == 0)
            {
                //sendHeartBeat();
            }
        }

        FD_ZERO(&rset);
        FD_SET(stdinfd, &rset);
        FD_SET(m_userdata->sockfd,  &rset);
        timeout.tv_sec  = 0;
        timeout.tv_usec = 1000;
        ret = select(m_userdata->sockfd + 1, &rset, NULL, NULL, &timeout);
        if(ret <= 0)
        {
            continue;
        }
        if (FD_ISSET(stdinfd, &rset))
        {
            char tmp[10];
            cin >> tmp;
            printCmdIdList();
            m_userdata->haveSendFlag = 1;
        }
    }
}

void CmdModel::StartWriteFile()
{
    char *tmp = NULL;
    struct udpLoseNumSend *loseSend = NULL;
    int i = 0;
    gettimeofday(&last_resend_time, NULL);
    for( ; ;)
    {
        if (m_readList->getPackSize() > 0)
        {
            char *pack = m_readList->pack_pop();
            dealWithMessage(pack);
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }
        if (m_udpMap.size() > 0)
        {
            gettimeofday(&now_resend_time, NULL);
            if ((1000000*(now_resend_time.tv_sec - last_resend_time.tv_sec) +
                         (now_resend_time.tv_usec - last_resend_time.tv_usec)) > 32000)
            {
                map<int, int>::iterator iter = m_udpMap.begin();
                while (iter != m_udpMap.end())
                {
                    if (i == 0)
                    {
                        tmp = new char[220];
                        loseSend = (struct udpLoseNumSend *)tmp;
                    }
                    loseSend->head.version  = 1;
                    loseSend->head.cmdId    = UDPRESEND;
                    loseSend->head.userId   = m_userdata->userId;
                    loseSend->head.length   = 220;
                    loseSend->sayToClientId = iter->second;
                    loseSend->loseNum[i] = iter->first;
                    i++;
                    if (i == 10)
                    {
                        loseSend->num = 10;
                        m_userdata->m_writeList->pack_push(tmp);
                        i = 0;
                    }
                    iter++;
                }
                if (i != 0)
                {
                    loseSend->num = i;
                    m_userdata->m_writeList->pack_push(tmp);
                    i = 0;
                }
                gettimeofday(&last_resend_time, NULL);
            }
        }
    }
}

void *CmdModel::startSendFile(void *arg)
{
    CmdModel *cmd = (CmdModel *)arg;
    cmd->StartSendFile();
}


void *CmdModel::start(void *arg)
{
    CmdModel *cmd = (CmdModel *)arg;
    cmd->Start();
}

void *CmdModel::startWriteFile(void *arg)
{
    CmdModel *cmd = (CmdModel *)arg;
    cmd->StartWriteFile();
}

void *CmdModel::startDealUdp(void *arg)
{
    CmdModel *cmd = (CmdModel *)arg;
   cmd->startDealUdpPack();
}

void CmdModel::Cmdstart()
{
    /*创建新线程接收新客户端套接字*/
    ret = pthread_create(&m_tid, NULL, start, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
    /*创建新线程接收新客户端套接字*/
    ret = pthread_create(&sendfile_tid, NULL, startSendFile, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&writefile_tid, NULL, startWriteFile, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&dealUdp_tid, NULL, startDealUdp, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
