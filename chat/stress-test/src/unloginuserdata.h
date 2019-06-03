#ifndef UNLOGINUSERDATA_H
#define UNLOGINUSERDATA_H
#include "unloginuserdata.h"
#include "head.h"

class UnloginUserData
{
public:
    UnloginUserData();
    int getSockFd();
    void setSockFd(int fd);
    void deleteUser();

private:
    int sockfd;
};

#endif // UNLOGINUSERDATA_H
