#ifndef CMD_H
#define CMD_H
#include "user.h"
#include "unloginuser.h"
#include "list.h"
#include "user-list.h"

class CCmd
{
public:

    CCmd();
    ~CCmd();
    static void *startCmd(void *arg);    //新线程
    int getUdpSockfd();
    CUserList* get_vcdListToIO();
    void startDealCmd();

    void setWriteSocketFdFlag(char *pack);   //解析数据包并设置标记
    void m_usleep(int time);
    void start();
    void setHasLoginMapFromIO(map<int, CUser*> *map);    //I/O模块本地已登录用户链表接口
    void setPackListFromIO(CList *packList);    //I/O模块数据包指针链表接口
    void setUnloginUserListFromIO(CUserList *unloginUserList);
    void sendHeartBeat(map<int, CUser*>::iterator iter);
    void printAllLoginUser();

private:
    CList     *m_packList; //等同于I/O模块的数据包指针链表
    CUserList *m_unLoginList;
    CUserList *m_userListToIO;
    map<int, char*> m_udpLoseMap;
    map<int, CUser*> m_map;  //等同于I/O模块本地已登录用户链表
    map<int, CUnloginuser*> m_loginMap;
    pthread_t  m_tid, m_tid2;
    void       *m_retval;
    struct sockaddr_in m_serveraddrUdp;
    int m_udpsock;
    int m_num;
    time_t  m_lastLoopTime;
};

#endif
