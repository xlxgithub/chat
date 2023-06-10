#ifndef OFFLINEMESSAGE_H
#define OFFLINEMESSAGE_H
#include"dbConnectPool.hpp"
#include<string>
#include<vector>

class OfflineMessage{
public:
    bool insert(int userid, std::string msg);
    bool remove(int id);

    //查询用户离线信息
    std::vector<std::string> query(int userid);
private:
    int userid;
    std::string message;
};

#endif