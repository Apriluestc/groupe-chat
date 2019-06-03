#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

class CMutex
{
public:
    CMutex(pthread_mutex_t *mutex);
    ~CMutex();

private:
    pthread_mutex_t *m_mutex;
};

#endif // VCD_MUTEX_H
