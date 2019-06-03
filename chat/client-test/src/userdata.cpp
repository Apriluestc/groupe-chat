#include "userdata.h"

UserData::UserData()
{
    haveSendFlag      = false;
    userSendDataFlag  = false;
    lengthHasSend     = 0;

    recvHeadFlag      = false;
    totalLengthOfData = 0;
    lengthHasRecv     = 0;
    haveReadFlag      = false;
    startTransfer     = false;

    sendFileCount     = 0;
    recvFileCount     = 0;
    recvFileSize      = 0;
    m_writeList = new SaveList;
}

UserData::~UserData()
{
    delete m_writeList;
}

char *UserData::get_recv()
{
    recvbuff = new char[1436];
    return recvbuff;
}

char *UserData::get_send()
{
    sendbuff = new char[1436];
    return sendbuff;
}

int UserData::getFileSize()
{
    stat(filename, &buf);
    return buf.st_size;
}
