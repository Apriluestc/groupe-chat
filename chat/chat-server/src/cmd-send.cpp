#include "cmd-send.h"

CCmdSend::CCmdSend()
{
    m_position = 0;
    m_sendBuff = new char[10240];
    pthread_mutex_init(&m_mutex, NULL);
    m_udpSendFlag = 0;
}

void CCmdSend::setPosition(int pos)
{
    m_position += pos;
}

int CCmdSend::getPosition()
{
    return m_position;
}

char* CCmdSend::getSendBuff()
{
    return m_sendBuff;
}

char* CCmdSend::getUdpSendBuff()
{
    return m_udpSendBuff;
}

char* CCmdSend::m_copy(char *p, int length)
{
    CMutex mutex_lock(&m_mutex);
    memcpy(m_sendBuff + m_position, p, length);
    return m_sendBuff + m_position;
}

char* CCmdSend::m_udpcopy(char *p, int length)
{
    m_udpSendBuff = new char[1436];
    CMutex mutex_lock(&m_mutex);
    memcpy(m_udpSendBuff, p, length);
    return m_udpSendBuff;
}

int CCmdSend::startSend(int sockfd)
{
    CMutex mutex_lock(&m_mutex);
    int nread = send(sockfd, m_sendBuff, m_position, 0);
    if (nread < 0)
    {
        return -1;
    }
    m_position -= nread;
    memmove(m_sendBuff, m_sendBuff + nread, nread);
    return nread;
}

