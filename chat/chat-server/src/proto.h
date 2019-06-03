#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <list>
#include <map>
#include <iostream>
#pragma pack(1)

enum cmdIdEnum{ LOGIN = 1, SENDtoONE, SENDtoOTHERS, SENDFILENAME, FILERET, SENDFILE,
                UDPSENDFILE, UDPREGISTER, UDPRESEND, HEARTBEAT, QUIT};

enum loginFlag{ LOGINSUCCESS = 1, LOGINFAILED, NOLOGIN};

struct baseHandle
{
    int cmdId;
    int userId;
    int length;
    int version;
};

struct messageSend
{
    struct baseHandle head;
    int recvFromWhichClient;
    char msg[1024];
};

struct messageRecv
{
    struct baseHandle head;
    int whichClientIdToSend;
    char msg[1024];
};

struct loginMessageSend
{
    struct baseHandle loginHead;
    char result[2];
};

struct loginMessageRecv
{
    struct baseHandle loginHead;
    char name[50];
    char passwd[20];
};

struct fileRecv
{
    struct baseHandle head;
    int sendToClientId;
    int filesize;
    char filename[100];
    char fileContext[1424];
};

struct fileSend
{
    struct baseHandle head;
    int recvFromWhichId;
    int filesize;
    char filename[100];
    char fileContext[1424];
};
struct udpfileRecv
{
    struct baseHandle head;
    int count;
    time_t time;
    int sendToClientId;
    int filesize;
    char filename[10];
    char fileContext[1390];
};

struct udpfileSend
{
    struct baseHandle head;
    int count;
    time_t time;
    int recvFromWhichId;
    int filesize;
    char filename[10];
    char fileContext[1390];
};
struct udpLoseNumSend
{
    struct baseHandle head;
    int recvFromWhichClient;
    int num;
    int loseNum[50];
};
struct udpLoseNumRecv
{
    struct baseHandle head;
    int whichClientIdToSend;
    int num;
    int loseNum[50];
};
#endif // HEAD_H
