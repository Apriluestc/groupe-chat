#ifndef SAVEMUTEX_H
#define SAVEMUTEX_H
#include "head.h"

class SaveMutex
{
public:
    SaveMutex(pthread_mutex_t *mutex);
    ~SaveMutex();
private:
    pthread_mutex_t *m_mutex;
};

#endif // SAVEMUTEX_H
