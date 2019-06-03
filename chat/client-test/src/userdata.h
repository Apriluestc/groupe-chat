#ifndef USERDATA_H
#define USERDATA_H
#include "head.h"
#include "savelist.h"
class UserData
{
public:
    UserData();
    ~UserData();

    int userId;
    int	sockfd;
    int sockUdp;
    struct sockaddr_in servaddrudp;
    SaveList *m_writeList;

    /*心跳*/
    time_t lastHeartPackRecvTime;
    time_t lastSendTime;

    /*登录信息*/
    struct loginMessageRecv m_loginRecvMsg;
    struct loginMessageSend m_loginSendMsg;

    /*接收*/
    bool haveReadFlag;
    bool recvHeadFlag;                      /*接收头还是数据的标志为*/
    char *recvbuff;
    char *get_recv();
    struct messageRecv *m_MsgRecv;
    int  totalLengthOfData;                 /*数据包的总长度*/
    int  remainLengthtoRecv;                /*剩下需要接收的长度*/
    int  lengthHasRecv;                     /*已经接收的长度*/

    /*发送*/
    bool haveSendFlag;
    char *sendbuff;
    char *get_send();
    struct messageSend *m_MsgSend;
    bool userSendDataFlag;                  /*判断是否需要将其套接字设置进入集合的标志为*/
    int  totalLengthOfSend;                 /*数据包的总长度*/
    int  lengthHasSend;                     /*还需要发送的长度*/
    int  remainLengthofSend;

    /*文件*/
    struct fileSend *m_fileSend;
    struct fileRecv *m_fileRecv;
    struct udpfileRecv *m_udpfileRecv;
    struct udpfileSend *m_udpfileSend;

    int getFileSize();
    int startTransfer;
    int sendFileToClientId;
    FILE *pf;
    int recvFileCount;
    int sendFileCount;
    int recvFileSize;
    char filename[100];
    struct stat buf;
    int fileSize;

    /*文件*/
//    FileTransfer fileInfo;
};

#endif // USERDATA_H
