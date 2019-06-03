#include <iostream>
#include "accept.h"
#include "io.h"
#include "cmd.h"
#include "sqlite.h"
using namespace std;

int main()
{
    CAccept accept;
    CIo     io;
    CCmd    cmd;

    /*接口传递*/
    io.setVcdListFromCmdMode(cmd.get_vcdListToIO());
    io.setUdpFd(cmd.getUdpSockfd());
    cmd.setUnloginUserListFromIO(accept.getVcdLoginList());
    cmd.setPackListFromIO(io.getPackList()); //指令模块获取I/O模块数据包指针链表

    /*各模块启动*/
    accept.start();  //监听
    io.start(); //I/O
    cmd.start();     //指令

    /*退出*/
    cout << "按任意键退出" << endl;
    char quit;
    cin >> quit;

    return 0;
}
