#ifndef CMDSEND_H
#define CMDSEND_H
#include "mutex.h"
#include "proto.h"

class CCmdSend
{
public:
    CCmdSend();

    /*发送相关*/
    int getPosition();
    void setPosition(int pos);
    void setUdpSendFlag(int flag);
    char *m_udpcopy(char *p, int length);
    char *m_copy(char *p, int length);
    int startSend(int sockfd);
    char *getSendBuff();
    char *getUdpSendBuff();
private:
    int m_position;
    char *m_sendBuff;
    int  m_udpSendFlag;
    char *m_udpSendBuff;

    pthread_mutex_t m_mutex;
};

#endif // CMDSEND_H
