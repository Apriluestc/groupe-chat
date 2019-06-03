#include "mutex.h"

CMutex::CMutex(pthread_mutex_t *mutex)
{
    m_mutex = mutex;
    pthread_mutex_lock(mutex);
}

CMutex::~CMutex()
{
    pthread_mutex_unlock(m_mutex);
}
