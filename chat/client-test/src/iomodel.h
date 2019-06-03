#ifndef IOMODEL_H
#define IOMODEL_H
#include "head.h"
#include "userdata.h"
#include "savelist.h"

class IOModel
{
public:
    IOModel();
    ~IOModel();
    void Start();
    void Startudprecv();
    void Startudpsend();
    static void* startudprecv(void *arg);
    static void* start(void *arg);
    static void* startudpsend(void *arg);
    void IOStart();
    pthread_t get_m_tid();
    pthread_t get_m_udptid();
    void set_userdata(UserData *userdata);
    SaveList *get_readList();
    void m_usleep(int time);
private:
    int sendNum;
    int recvNum;
    int udpSendNum;
    fd_set rset;
    fd_set wset;
    int ret;
    int nread;
    struct timeval timeout;
    pthread_t  m_tid;
    pthread_t  m_udptid;
    pthread_t  m_udpsendtid;
    SaveList *m_readList;
    UserData *m_userdata;
};

#endif // IOMODEL_H
