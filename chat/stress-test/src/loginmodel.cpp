#include "loginmodel.h"

LoginModel::LoginModel()
{
    loginedNum = 0;
}

pthread_t LoginModel::get_tid()
{
    return tid;
}

void LoginModel::setuserListFromCmd(UserList *loginuserList)
{
    m_loginuserList = loginuserList;
}


void LoginModel::m_usleep(int time)
{
    FD_ZERO(&rset);
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void LoginModel::funcConnect()
{
    for (int i=0; i<USERNUM; i++)
    {
        unloginuser[i].setSockFd(socket(AF_INET, SOCK_STREAM, 0));
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        inet_aton(IP, &servaddr.sin_addr);

        ret = connect(unloginuser[i].getSockFd(), (struct sockaddr *)&servaddr, sizeof(servaddr));
        if(ret < 0)
        {
            cout << strerror(errno) << endl;
//            if (errno == 正在连接)
//            {
//                continue;
//            }
//            else
//            {
//                unloginuser[i].deleteUser();
//                i--;
//                continue;
//            }
        }
        strcpy(send_msg.name, "aa");
        strcpy(send_msg.passwd, "aa");
        send_msg.loginHead.cmdId = 1;
        send_msg.loginHead.version = 1;
        send_msg.loginHead.length  = sizeof(loginMessageSend);

        ret = send(unloginuser[i].getSockFd(), &send_msg, send_msg.loginHead.length, 0);
        if (ret <= 0)
        {
            i--;
            unloginuser[i].deleteUser();
            continue;
        }
        ret = recv(unloginuser[i].getSockFd(), &recv_msg, sizeof(loginMessageRecv), 0);
        if (ret <= 0)
        {
            unloginuser[i].deleteUser();
            i--;
            continue;
        }
        if(strcmp(recv_msg.result, "Y") == 0)
        {
              cout << "登录成功！" << endl;
              cout << "your userId is: " << recv_msg.loginHead.userId << endl;
              fcntl(unloginuser[i].getSockFd(), F_SETFL, O_NONBLOCK);
              UserData *user = new UserData;
              user->setSockFd(unloginuser[i].getSockFd());
              user->setUserId(recv_msg.loginHead.userId);
              m_loginuserList->user_push(user);
              loginedNum++;
        }else
        {
            unloginuser[i].deleteUser();
            i--;
        }
    }
    cout << "已经登录上的用户的个数" << loginedNum << endl;
}

void *LoginModel::startLogin(void *arg)
{
    LoginModel *login = (LoginModel *)arg;
    login->funcConnect();
}

void LoginModel::start()
{
    ret = pthread_create(&tid, NULL, startLogin, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
