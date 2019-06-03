#ifndef CLIENTSTART_H
#define CLIENTSTART_H
#include "head.h"
#include <iostream>
#include "userdata.h"
using namespace std;

class ClientStart
{
public:
    ClientStart();
    ~ClientStart();
    void startLoginModel();
    void start();
    UserData* get_userdata();
private:

    int ret;
    struct sockaddr_in servaddr;
    UserData *userdata;
};

#endif // CLIENTSTART_H
