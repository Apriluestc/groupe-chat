#include "userdata.h"

UserData::UserData()
{
    m_send = new CmdSend;
    lasttime          = time(NULL);
    sendLastTime      = time(NULL);
}

UserData::~UserData()
{
    delete m_send;
}

int UserData::getSockFd()
{
    return m_sockfd;
}

void UserData::setSockFd(int fd)
{
    m_sockfd = fd;
}

int UserData::getUserId()
{
    return m_userId;
}

void UserData::setUserId(int userId)
{
    m_userId = userId;
}
