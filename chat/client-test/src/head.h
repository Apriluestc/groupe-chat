#ifndef HEAD_H
#define HEAD_H

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
using namespace std;
#include <iostream>

#pragma pack(1)

enum cmdIdEnum{ LOGIN = 1, SENDtoONE, SENDtoOTHERS, SENDFILENAME,
                FILERET, SENDFILE, UDPSENDFILE, UDPREGISTER, UDPRESEND, HEARTBEAT, QUIT};

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

struct fileRecv
{
    struct baseHandle head;
    int recvFromWhichId;
    int filesize;
    char filename[100];
    char fileContext[1424];
};

struct fileSend
{
    struct baseHandle head;
    int sendToClientId;
    int filesize;
    char filename[100];
    char fileContext[1424];
};

struct udpfileRecv
{
    struct baseHandle head;
    int count;
    time_t time;
    int recvFromWhichId;
    int filesize;
    char filename[10];
    char fileContext[1399];
};

struct udpfileSend
{
    struct baseHandle head;
    int count;
    time_t time;
    int sendToClientId;
    int filesize;
    char filename[10];
    char fileContext[1390];
};

struct udpLoseNumSend
{
    struct baseHandle head;
    int sayToClientId;
    int num;
    int loseNum[10];
};
struct udpLoseNumRecv
{
    struct baseHandle head;
    int recvFromWhichId;
    int num;
    int loseNum[10];
};
#endif // HEAD_H
