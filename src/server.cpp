/*
 * ========================================================================
 * 
 *      fileName:           server.cpp
 *      description:        多人聊天室的服务端实现
 *
 *      其中:
 *          void recvMessage();     用于接收消息
 *          void sendMessage();     用于发送消息
 *
 *      qq:                 1003625407
 *      email@:             13669186256@163.com
 *
 * ========================================================================
 * */

#include "../include/server.h"

void* recvMessage(void* arg) {
    Server* us = (Server*)arg;
    std::string message;
    while (1) {
        us->recvData(message);
        std::cout << "test" << message << std::endl;
    }
}

void* sendMessage(void* arg) {
    Server* us = (Server*)arg;
    while (1) {
        us->broadCast();
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "usage:" << argv[0] << " " << "port" << std::endl;
        return 1;
    }
    Server ss(atoi(argv[1]));
    ss.initServer();
    pthread_t r, s;
    pthread_create(&r, NULL, recvMessage, (void*)&ss);
    pthread_create(&s, NULL, sendMessage, (void*)&ss);
    pthread_join(r, NULL);
    pthread_join(s, NULL);
    return 0;
}
