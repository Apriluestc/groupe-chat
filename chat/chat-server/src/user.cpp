#include "user.h"
using namespace std;

CUser::CUser()
{
    deleteFlag1       = false;
    deleteFlag2       = false;
    clientlen = sizeof(struct sockaddr_in);
    m_send = new CCmdSend;
    m_udpSendList = new CList;
}

CUser::~CUser()
{
    delete m_send;
    delete m_udpSendList;
}

void CUser::setSocket(int socket)
{
    m_socket = socket;
}

void CUser::setUserId(int userId)
{
    m_userId = userId;
}

int CUser::getUserId()
{
    return m_userId;
}

int CUser::getSocket()
{
    return m_socket;
}
