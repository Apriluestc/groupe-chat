#ifndef SAVELIST_H
#define SAVELIST_H
#include "head.h"
#include "savemutex.h"

class SaveList
{
public:
    SaveList();
    void pack_push(char *p);
    int  getPackSize();
    char *pack_pop();
private:
    /*数据包指针的公共链表的变量*/
    int             m_packcount;
    list<char *>    m_package;
    pthread_mutex_t m_packmutex;
};

#endif // SAVELIST_H
