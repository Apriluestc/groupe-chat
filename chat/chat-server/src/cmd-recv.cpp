#include "cmd-recv.h"

CCmdRecv::CCmdRecv()
{
    m_recvHeadFlag      = false;
    m_totalLengthOfData = 0;
    m_lengthHasRecv     = 0;
}

char *CCmdRecv::getRecvBuff()
{
    m_recvBuff = new char[1436];
    return m_recvBuff;
}
