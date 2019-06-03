#include "head.h"
#include "loginmodel.h"
#include "iomodel.h"
#include "cmdmodel.h"

int main(int argc, char *argv[])
{
    LoginModel login;
    Cmdmodel   cmd;
    Iomodel    io;

    login.setuserListFromCmd(cmd.getloginuserList());
    io.setuserListFromCmd(cmd.getuserList());
    cmd.setuserListFromIo(io.getpackList());

    login.start();
    pthread_join(login.get_tid(), NULL);

    cmd.start();

    io.start();


    pthread_join(cmd.get_tid(),   NULL);
    pthread_join(io.get_tid(),    NULL);

    /*退出*/
    cout << "按任意键退出" << endl;
    char quit;
    cin >> quit;
}
