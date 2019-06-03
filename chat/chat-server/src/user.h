#ifndef USER_H
#define USER_H
#include "proto.h"
#include "constant-macro.h"
#include "cmd-recv.h"
#include "cmd-send.h"
#include "heartbeat.h"
#include "list.h"
using namespace std;

class CUser
{
public:
    CUser();
    ~CUser();
    void setSocket(int socket);
    void setUserId(int userId);
    int  getSocket();
    int  getUserId();

    /*关于删除*/
    bool deleteFlag1;
    bool deleteFlag2;

    /*每个用户udp发送的list*/
    CList *m_udpSendList;

    /*关于心跳*/
    CHeartBeat m_heart;

    /*发送相关*/
    CCmdSend  *m_send;

    /*接收相关*/
    CCmdRecv   m_recv;

    /*UDP*/
    struct sockaddr_in *clientaddrUdp;
    socklen_t clientlen;

private:

    int  m_socket;      /*本用户套接字*/
    int  m_userId;      /*用户id*/
};

#endif // USER_H
