#ifndef HEAD_H
#define HEAD_H
#include <iostream>
using namespace std;
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <list>
#include <map>
#include <sys/ioctl.h>

#define IP "127.0.0.1"
#define PORT 8884
#define USERNUM 10000
#pragma pack(1)

enum cmdIdEnum{ LOGIN = 1, SENDtoONE, SENDtoOTHERS, SENDFILENAME, FILERET, SENDFILE,
                UDPSENDFILE, UDPREGISTER, UDPRESEND, HEARTBEAT, QUIT};

struct baseHandle
{
    int cmdId;
    int userId;
    int length;
    int version;
};
struct loginMessageRecv
{
    struct baseHandle loginHead;
    char result[2];
};

struct loginMessageSend
{
    struct baseHandle loginHead;
    char name[50];
    char passwd[20];
};

struct messageSend
{
    struct baseHandle head;
    int sayToClientId;
    char msg[1024];
};

struct messageRecv
{
    struct baseHandle head;
    int recvFromWhichId;
    char msg[1024];
};
#endif // HEAD_H
