#ifndef USERLIST_H
#define USERLIST_H
#include "savemutex.h"
#include "head.h"
#include "userdata.h"

class UserList
{
public:
    UserList();
    void user_push(UserData *userlist);
    int  getUserSize();
    UserData* user_pop();
private:
    int              m_usercount;
    list<UserData *> m_userlist;
    pthread_mutex_t  m_usermutex;
};

#endif // USERLIST_H
