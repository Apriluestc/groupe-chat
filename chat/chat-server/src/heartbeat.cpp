#include "heartbeat.h"

CHeartBeat::CHeartBeat()
{
    m_lastTime     = time(NULL);
    m_sendLastTime = time(NULL);
}
