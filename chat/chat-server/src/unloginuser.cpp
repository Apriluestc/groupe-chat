#include "unloginuser.h"

CUnloginuser::CUnloginuser()
{
    recvHeadFlag      = true;
    loginTime         = 0;
    totalLengthOfData = 0;
    loginState        = NOLOGIN;
    authFlag          = false;
    loginReturnFlag   = false;
    deleteFlag1       = false;
    deleteFlag2       = false;
    remainSend        = sizeof(loginMessageSend);
}

CUnloginuser::~CUnloginuser()
{

}
