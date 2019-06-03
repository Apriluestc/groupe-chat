#ifndef CMDMODEL_H
#define CMDMODEL_H
#include "head.h"
#include "userdata.h"
#include "savelist.h"
class CmdModel
{
public:
    CmdModel();
    ~CmdModel();
    void Cmdstart();
    void Start();
    void StartSendFile();
    void StartWriteFile();
    void dealWithMessage(char *pack);
    void recvFileDecide(char *name, int id);
    static void* start(void *arg);
    static void* startSendFile(void *arg);
    static void* startWriteFile(void *arg);
    static void* startDealUdp(void *arg);
    void printCmdIdList();
    void returnYesorNo(const char *ret, int toWhichId);
    void set_userdata(UserData *userdata);
    void setReadListFromIOModel(SaveList *readList);
    pthread_t get_m_tid();
    void udpSendRegister();
    void writeFile(int fileseek);
    void udpSendFile(int nread);
    void tcpSendFile(int nread);
    void startDealUdpPack();
    void sendHeartBeat();
    void m_usleep(int time);

    int stdinfd;
    int ret;
    int sockfd;
    int userId;
    fd_set rset;
    fd_set wset;
    struct timeval timeout;
    UserData *m_userdata;
    SaveList *m_readList;
    SaveList *m_udpList;
private:
    map<int, int> m_udpMap;
    map<int, char*> m_udpSaveMap;
    size_t nwrite;
    pthread_t  m_tid;
    pthread_t  sendfile_tid;
    pthread_t  writefile_tid;
    pthread_t  dealUdp_tid;
    int nread;
    pthread_mutex_t m_mutex;
    int nowPackNum;
    time_t sendtime;
    struct timeval last_resend_time;
    struct timeval now_resend_time;
    struct messageSend* m_MsgSend;
};

#endif // CMDMODEL_H
