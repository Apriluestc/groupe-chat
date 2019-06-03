#ifndef CMDRECV_H
#define CMDRECV_H


class CCmdRecv
{
public:
    CCmdRecv();

    char *getRecvBuff();
    char *m_recvBuff;
    bool m_recvHeadFlag;                      /*接收头还是数据的标志为*/
    int  m_totalLengthOfData;                 /*数据包的总长度*/
    int  m_remainLengthtoRecv;                /*剩下需要接收的长度*/
    int  m_lengthHasRecv;                     /*已经接收的长度*/
    struct messageRecv *m_msgRecv;          /*指向开辟的内存解析数据*/
    struct fileRecv *m_fileRecv;
};

#endif // CMDRECV_H
