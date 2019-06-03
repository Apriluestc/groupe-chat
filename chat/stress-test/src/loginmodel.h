#ifndef LOGINMODEL_H
#define LOGINMODEL_H
#include "head.h"
#include "savelist.h"
#include "userdata.h"
#include "unloginuserdata.h"
#include "userlist.h"

class LoginModel
{
public:
    LoginModel();
    void funcConnect();
    static void* startLogin(void *arg);
    void  start();
    pthread_t get_tid();
    void m_usleep(int time);
    void setuserListFromCmd(UserList *loginuserList);
private:
    int ret;
    pthread_t  tid;
    struct sockaddr_in servaddr;
    UnloginUserData unloginuser[USERNUM];
    struct loginMessageSend send_msg;
    struct loginMessageRecv recv_msg;

    int loginedNum;
    struct timeval timeout;
    fd_set rset;
    UserList *m_loginuserList;
};

#endif // LOGINMODEL_H
