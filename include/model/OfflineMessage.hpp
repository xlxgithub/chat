#ifndef OFFLINEMESSAGE_H
#define OFFLINEMESSAGE_H
#include"dbConnectPool.hpp"
#include<string>
#include<vector>

class OfflineMessage{
public:
    bool insert(const int& userid, const std::string& msg,const std::string& time,const std::string& msgtype);
    bool remove(const int& id);

    //查询用户离线信息
    std::vector<std::string> query(const int& userid);
private:
    int userid;
    std::string message;
};

#endif