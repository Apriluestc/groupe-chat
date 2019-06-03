#include "cmdsend.h"

CmdSend::CmdSend()
{
    position = 0;
    sendbuff = new char[10240];
    pthread_mutex_init(&m_mutex, NULL);
}

void CmdSend::setPosition(int pos)
{
    SaveMutex mutex_lock(&m_mutex);
    position += pos;
}

int CmdSend::getPosition()
{
    SaveMutex mutex_lock(&m_mutex);
    return position;
}

char* CmdSend::getsendbuff()
{
    SaveMutex mutex_lock(&m_mutex);
    return sendbuff;
}

char* CmdSend::m_copy(char *p, int length)
{
    SaveMutex mutex_lock(&m_mutex);
    memcpy(sendbuff+position, p, length);
    return sendbuff+position;
}

int CmdSend::startSend(int sockfd)
{
    SaveMutex mutex_lock(&m_mutex);
    int nread = send(sockfd, sendbuff, position, 0);
    if (nread < 0)
    {
        return -1;
    }
    position -= nread;
    memmove(sendbuff, sendbuff + nread, nread);
    return nread;
}
