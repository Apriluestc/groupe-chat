#ifndef CMDMODEL_H
#define CMDMODEL_H
#include "head.h"
#include "savelist.h"
#include "userlist.h"

class Cmdmodel
{
public:
    Cmdmodel();
    ~Cmdmodel();
    void  funcCmd();
    void setListHasLogined();
    static void* startCmd(void *arg);
    static void* startSetList(void *arg);
    void  start();
    pthread_t get_tid();
    void m_usleep(int time);
    UserList *getloginuserList();
    UserList *getuserList();
    void setuserListFromIo(SaveList *packList);
    void dealWithMessage(char *pack);
    void sendHeartBeat(map<int, UserData*>::iterator iter);


    void cmd333();
    void cmd1000();
    void cmd3000();
    void cmd100qun();
private:
    int ret;
    pthread_t  tid, tid2;
    struct timeval timeout;
    fd_set rset;
    UserList *m_loginuserList;
    map<int, UserData*> m_userMap;
    UserList *m_userList;
    SaveList *m_packList;
};

#endif // CMDMODEL_H
