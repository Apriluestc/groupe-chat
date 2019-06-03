#include "userlist.h"

UserList::UserList()
{
    pthread_mutex_init(&m_usermutex, NULL);
}

UserData* UserList::user_pop()
{
    SaveMutex mutex_lock(&m_usermutex);
    UserData* user = m_userlist.front();
    m_userlist.pop_front();
    m_usercount--;
    return user;
}

int UserList::getUserSize()
{
    return m_usercount;
}

void UserList::user_push(UserData *userlist)
{
    SaveMutex mutex_lock(&m_usermutex);
    m_userlist.push_back(userlist);
    m_usercount++;
}
