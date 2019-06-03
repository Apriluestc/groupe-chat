#ifndef CMDRECV_H
#define CMDRECV_H
#include "head.h"

class CmdRecv
{
public:
    CmdRecv();
    char *get_recvbuff();
    char *recvbuff;
    bool recvHeadFlag;                      /*接收头还是数据的标志为*/
    int  totalLengthOfData;                 /*数据包的总长度*/
    int  remainLengthtoRecv;                /*剩下需要接收的长度*/
    int  lengthHasRecv;                     /*已经接收的长度*/
    struct messageRecv *m_msgRecv;          /*指向开辟的内存解析数据*/
};

#endif // CMDRECV_H
