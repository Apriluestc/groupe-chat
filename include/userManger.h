#ifndef INCLUDE_USERMANGER_H
#define INCLUDE_USERMANGER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <pthread.h>

#include <boost/noncopyable.hpp>

const int USEROFFLINE = 0;
const int USERLOGINED = 1;
const int USERONLINE = 2;

struct Register {
    char _name;
    char _school[32];
    char _password[32];
};

struct Login {
    std::string _id;
    char _password[32];
};

struct Reply {
    int _status;
    std::string _id;
};

class User {
public:
    std::string _name;
    std::string _school;

private:
};

class UserManger {
private:
    int _id;
    int _login_count;
    std::unordered_map<std::string, User> _registerUser;
    std::vector<User> _onlineUser;
    pthread_mutex_t _lock;
private:
};

#endif
