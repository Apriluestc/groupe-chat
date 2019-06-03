#ifndef CMDSEND_H
#define CMDSEND_H
#include "head.h"
#include "savemutex.h"

class CmdSend
{
public:
    CmdSend();
    int getPosition();
    void setPosition(int pos);
    char *m_copy(char *p, int length);
    int startSend(int sockfd);
    char *getsendbuff();
private:
    int position;
    char *sendbuff;
    pthread_mutex_t m_mutex;
};

#endif // CMDSEND_H
