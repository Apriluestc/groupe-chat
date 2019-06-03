#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include "proto.h"

class CHeartBeat
{
public:
    CHeartBeat();
    time_t m_lastTime;
    time_t m_sendLastTime;
};

#endif // HEARTBEAT_H
