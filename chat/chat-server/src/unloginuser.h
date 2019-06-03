#ifndef UNLOGINUSER_H
#define UNLOGINUSER_H
#include "proto.h"
#include "constant-macro.h"
using namespace std;


class CUnloginuser
{
public:
    CUnloginuser();
    ~CUnloginuser();
    int    m_socket;    /*未登录用户套接字*/
    int    m_userId;
    time_t loginTime;   /*用户连接时间*/
    int    loginState;/*认证用户名密码标志为*/
    bool   deleteFlag1;
    bool   deleteFlag2;
    bool   loginReturnFlag;
    bool   authFlag;

    /*发送相关*/
    int remainSend;
    struct loginMessageSend m_loginMsgSend; /*登录信息发送结构体*/

    /*接收相关*/
    char recvbuff[1500];                    /*分配内存用来接收信息*/
    int  remainLengthofRecv;                 /*还需要发送的长度*/
    int  totalLengthOfData;                 /*数据包的总长度*/
    bool recvHeadFlag;                      /*接收头还是数据的标志为*/
    struct loginMessageRecv *m_loginMsgRecv;/*指向开辟的内存解析数据*/
};

#endif // UNLOGINUSER_H
