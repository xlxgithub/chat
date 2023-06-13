#ifndef FRIEND_H
#define FRIEND_H
#include"dbConnectPool.hpp"
#include<string>
#include<vector>
#include"User.hpp"
class Friend{
public:
    bool insert(const int& userid,const int& friendid);
    bool remove(const int& userid,const int& friendid);
    std::vector<User> query(const int& userid);

private:
    int userid;
    int friendid;
};


#endif