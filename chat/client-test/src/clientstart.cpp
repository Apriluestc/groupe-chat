#include "clientstart.h"

ClientStart::ClientStart()
{
    userdata = new UserData;

    userdata->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_aton(IP, &servaddr.sin_addr);

    userdata->sockUdp = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&userdata->servaddrudp, sizeof(userdata->servaddrudp));
    userdata->servaddrudp.sin_family = AF_INET;
    userdata->servaddrudp.sin_port = htons(8888);
    inet_aton(IP, &userdata->servaddrudp.sin_addr);
}

ClientStart::~ClientStart()
{

}

UserData* ClientStart::get_userdata()
{
    return userdata;
}

void ClientStart::startLoginModel()
{
    int ret;
    userdata->m_loginSendMsg.loginHead.cmdId = LOGIN;
    userdata->m_loginSendMsg.loginHead.version = 1;
    userdata->m_loginSendMsg.loginHead.length  = sizeof(loginMessageSend);
    ret = send(userdata->sockfd, &userdata->m_loginSendMsg, userdata->m_loginSendMsg.loginHead.length, 0);
    if (ret <= 0)
    {
        exit(0);
    }
    ret = recv(userdata->sockfd, &userdata->m_loginRecvMsg, sizeof(loginMessageRecv), 0);
    if (ret <= 0)
    {
        exit(0);
    }
    if(strcmp(userdata->m_loginRecvMsg.result, "Y") == 0)
    {
          cout << "登录成功！" << endl;
          cout << "your userId is: " << userdata->m_loginRecvMsg.loginHead.userId << endl;
          userdata->userId = userdata->m_loginRecvMsg.loginHead.userId;
          fcntl(userdata->sockfd, F_SETFL, O_NONBLOCK);
    }else
    {
        cout << "input error" << endl;
        exit(0);
    }
}

void ClientStart::start()
{
    cout << "input name:";
    memset(userdata->m_loginSendMsg.name, 0, 50);
    cin.getline(userdata->m_loginSendMsg.name, 50);
    cout << "input passwd:";
    memset(userdata->m_loginSendMsg.passwd, 0, 20);
    cin.getline(userdata->m_loginSendMsg.passwd, 20);
    ret = connect(userdata->sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret != 0)
    {
        cout << errno << endl;
        exit(0);
    }
    startLoginModel();
}
