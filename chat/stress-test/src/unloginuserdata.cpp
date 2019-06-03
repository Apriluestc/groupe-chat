#include "unloginuserdata.h"

UnloginUserData::UnloginUserData()
{

}

int UnloginUserData::getSockFd()
{
    return sockfd;
}

void UnloginUserData::setSockFd(int fd)
{
    sockfd = fd;
}

void UnloginUserData::deleteUser()
{
    close(sockfd);
}
