#include "savelist.h"

SaveList::SaveList()
{
    pthread_mutex_init(&m_packmutex, NULL);
}

char* SaveList::pack_pop()
{
    SaveMutex mutex_lock(&m_packmutex);
    char *p = m_package.front();
    m_package.pop_front();
    m_packcount--;
    return p;
}

int SaveList::getPackSize()
{
    return m_packcount;
}

void SaveList::pack_push(char *p)
{
    SaveMutex mutex_lock(&m_packmutex);
    m_package.push_back(p);
    m_packcount++;
}
