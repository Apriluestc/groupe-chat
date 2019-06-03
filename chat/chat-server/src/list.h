#ifndef LIST_H
#define LIST_H
#include <iostream>

#include "proto.h"
#include "mutex.h"
#include "cmd-send.h"
#include "constant-macro.h"
using namespace std;
class CList
{
public:
    CList();
    ~CList();

    /*数据包指针的公共链表*/
    void packPush(char *p);
    int  getPackSize();
    char *packPop();
    char *packFront();

    /*发送*/
    void sendPush(char *p);
    int  getSendSize();
    char *sendPop();

    /*数据包指针的公共链表的变量*/
    int             m_packcount;
    list<char *>    m_package;
    pthread_mutex_t m_packmutex;

    /*发送*/
    int             m_sendCount;
    list<char*>     m_sendList;
    pthread_mutex_t m_sendmutex;
};

#endif // VCD_LIST_H
