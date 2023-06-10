#ifndef CHATSERVICE_H
#define CHATSERVICE_H
//定义业务处理类 单例模式
/*
    1.根据msgid 处理不同业务 采用map记录不同msgid对应的不同方法
*/

#include<functional>
#include<muduo/net/TcpServer.h>
#include<muduo/base/Logging.h>
#include<json.hpp>
#include<map>

#include"public.hpp"

using MsgHandler = std::function<void(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp)>;
class Chatservice
{
public:

    //单例模式
    static Chatservice* getinstance();
    Chatservice(const Chatservice& chat) = delete;
    Chatservice& operator=(const Chatservice& chat)=delete;
    // 处理登录业务
    void login(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);
    // 处理注册业务
    void reg(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);

    MsgHandler getHandler(int id);
private:
    Chatservice();
    std::unordered_map<int,MsgHandler> m_handler;

};









#endif