#include "cmdmodel.h"

Cmdmodel::Cmdmodel()
{
    m_loginuserList = new UserList;
    m_userList      = new UserList;
}

Cmdmodel::~Cmdmodel()
{
    delete m_loginuserList;
    delete m_userList;
}

pthread_t Cmdmodel::get_tid()
{
    return tid;
}

UserList *Cmdmodel::getloginuserList()
{
    return m_loginuserList;
}

UserList *Cmdmodel::getuserList()
{
    return m_userList;
}

void Cmdmodel::m_usleep(int time)
{
    FD_ZERO(&rset);
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void Cmdmodel::setuserListFromIo(SaveList *packList)
{
    m_packList = packList;
}

void Cmdmodel::dealWithMessage(char *pack)
{
    struct baseHandle *tmp = (struct baseHandle *)pack;
    struct messageRecv *recv = NULL;
    switch(tmp->cmdId)
    {
        case SENDtoONE:
            recv = (struct messageRecv *)pack;
            cout << "ID " << recv->recvFromWhichId << "私聊说: ";
            cout << recv->msg << endl;
            delete pack;
            pack = NULL;
        break;

        case SENDtoOTHERS:
            recv = (struct messageRecv *)pack;
            cout << "ID " << recv->recvFromWhichId << " 群聊说: ";
            cout << recv->msg << endl;
            delete pack;
            pack = NULL;
        break;
        case HEARTBEAT:
            cout << "heartbeat" << endl;
            delete pack;
            pack = NULL;
        break;
    }
}

void Cmdmodel::setListHasLogined()
{
    cout << "指令模块用户的数量:" << m_loginuserList->getUserSize() << endl;
    UserData *user = new UserData;
    m_userMap.insert(pair<int, UserData*>(0, user));

    for ( ; ;)
    {
        if (m_loginuserList->getUserSize())
        {
            UserData *user = m_loginuserList->user_pop();
            m_userMap.insert(pair<int, UserData*>(user->getUserId(), user));
            m_userList->user_push(user);
        }
        if (m_packList->getPackSize() > 0)
        {
            char *pack = m_packList->pack_pop();
            dealWithMessage(pack);
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }
//        if (m_userMap.size() > 0)
//        {
//            map<int, UserData*>::iterator iter = m_userMap.begin();
//            while (iter != m_userMap.end())
//            {
//                /*定时发送心跳包*/
//                if (time(NULL)-iter->second->sendLastTime > 20)
//                {
//                    sendHeartBeat(iter);
//                }

//                iter++;
//            }
//        }
    }
}

void Cmdmodel::sendHeartBeat(map<int, UserData*>::iterator iter)
{
    char *sendbuff = new char[26];
    struct messageSend *struct_send = (struct messageSend *)sendbuff;

    strcpy(struct_send->msg, "heart");
    struct_send->head.version  = 1;
    struct_send->head.userId   = iter->second->getUserId();
    struct_send->head.length   = 26;
    struct_send->sayToClientId = 0;
    struct_send->head.cmdId = HEARTBEAT;

    m_userMap[0]->m_send->m_copy(sendbuff, 26);

    m_userMap[0]->m_send->setPosition(26);
    iter->second->sendLastTime = time(NULL);

    delete sendbuff;
    sendbuff = NULL;
}

void Cmdmodel::cmd333()
{
    for (int i=1; i<333; i++)
    {
        char *sendbuff = new char[26];
        struct messageSend *struct_send = (struct messageSend *)sendbuff;

        strcpy(struct_send->msg, "hello");
        struct_send->head.version = 1;
        struct_send->head.userId  = i;
        struct_send->head.length  = 26;
        struct_send->sayToClientId = i+1;
        struct_send->head.cmdId = SENDtoONE;

        m_userMap[i+1]->m_send->m_copy(sendbuff, 26);

        m_userMap[i+1]->m_send->setPosition(26);

        delete sendbuff;
        sendbuff = NULL;
    }
}

void Cmdmodel::cmd1000()
{
    for (int i=1; i<1000; i++)
    {
        char *sendbuff = new char[26];
        struct messageSend *struct_send = (struct messageSend *)sendbuff;

        strcpy(struct_send->msg, "hello");
        struct_send->head.version = 1;
        struct_send->head.userId  = i;
        struct_send->head.length  = 26;
        struct_send->sayToClientId = i+1;
        struct_send->head.cmdId = SENDtoONE;

        m_userMap[i+1]->m_send->m_copy(sendbuff, 26);

        m_userMap[i+1]->m_send->setPosition(26);

        delete sendbuff;
        sendbuff = NULL;
    }
}

void Cmdmodel::cmd3000()
{
    for (int i=1; i<3000; i++)
    {
        char *sendbuff = new char[26];
        struct messageSend *struct_send = (struct messageSend *)sendbuff;

        strcpy(struct_send->msg, "hello");
        struct_send->head.version = 1;
        struct_send->head.userId  = i;
        struct_send->head.length  = 26;
        struct_send->sayToClientId = i+1;
        struct_send->head.cmdId = SENDtoONE;

        m_userMap[i+1]->m_send->m_copy(sendbuff, 26);

        m_userMap[i+1]->m_send->setPosition(26);

        delete sendbuff;
        sendbuff = NULL;
    }
}

void Cmdmodel::cmd100qun()
{
    for (int i=1; i<100; i++)
    {
        char *sendbuff = new char[26];
        struct messageSend *struct_send = (struct messageSend *)sendbuff;

        strcpy(struct_send->msg, "hello");
        struct_send->head.version = 1;
        struct_send->head.userId  = i;
        struct_send->head.length  = 26;
        struct_send->sayToClientId = i+1;
        struct_send->head.cmdId = SENDtoOTHERS;

        m_userMap[i+1]->m_send->m_copy(sendbuff, 26);

        m_userMap[i+1]->m_send->setPosition(26);

        delete sendbuff;
        sendbuff = NULL;
    }
}

void Cmdmodel::funcCmd()
{
    while (1)
    {
        char c;
        cout << "###### 1. 每三秒1000人瞬时说话  ######" << endl;
        cout << "###### 2. 每秒1000人说话       ######" << endl;
        cout << "###### 3. 每秒3000人私聊      ######" << endl;
        cout << "###### 4. 每秒100人群聊        ######" << endl;
        cout << "请选择发送方式：" << endl;
        cin >> c;
        switch(c)
        {
            case '1':
                cmd333();
                break;

            case '2':
                cmd1000();
                break;

            case '3':
                cmd3000();
                break;

            case '4':
                cmd100qun();
                break;
        }
    }
}

void *Cmdmodel::startCmd(void *arg)
{
    Cmdmodel *cmd = (Cmdmodel *)arg;
    cmd->funcCmd();
}
void *Cmdmodel::startSetList(void *arg)
{
    Cmdmodel *cmd = (Cmdmodel *)arg;
    cmd->setListHasLogined();
}

void Cmdmodel::start()
{
    ret = pthread_create(&tid, NULL, startCmd, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
    ret = pthread_create(&tid2, NULL, startSetList, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
