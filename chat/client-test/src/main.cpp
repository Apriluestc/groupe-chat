#include <iostream>
#include "clientstart.h"
#include "iomodel.h"
#include "cmdmodel.h"
using namespace std;

int main(int argc, char *argv[])
{
    ClientStart cstart;
    IOModel     io;
    CmdModel    cmd;

    cstart.start();

    io.set_userdata(cstart.get_userdata());
    cmd.setReadListFromIOModel(io.get_readList());
    cmd.set_userdata(cstart.get_userdata());

    io.IOStart();
    cmd.Cmdstart();

    pthread_join(io.get_m_tid(), NULL);
    pthread_join(io.get_m_udptid(), NULL);
    pthread_join(cmd.get_m_tid(), NULL);
    return 0;
}
