#include "savemutex.h"

SaveMutex::SaveMutex(pthread_mutex_t *mutex)
{
    m_mutex = mutex;
    pthread_mutex_lock(m_mutex);
}

SaveMutex::~SaveMutex()
{
    pthread_mutex_unlock(m_mutex);
}
