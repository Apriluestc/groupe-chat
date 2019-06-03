#ifndef USERDATA_H
#define USERDATA_H
#include "head.h"
#include "cmdsend.h"
#include "cmdrecv.h"
class UserData
{
public:
    UserData();
    ~UserData();

    int getSockFd();
    void setSockFd(int fd);
    int getUserId();
    void setUserId(int userId);
    CmdSend *m_send;
    CmdRecv m_recv;
    time_t lasttime;
    time_t sendLastTime;
private:
    int m_sockfd;
    int m_userId;
};

#endif // USERDATA_H
