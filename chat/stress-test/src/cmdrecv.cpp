#include "cmdrecv.h"

CmdRecv::CmdRecv()
{
    recvHeadFlag      = false;
    totalLengthOfData = 0;
    lengthHasRecv     = 0;
}

char *CmdRecv::get_recvbuff()
{
    recvbuff = new char[1436];
    return recvbuff;
}
