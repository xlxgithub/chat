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
#include"UserModel.hpp"
#include<unordered_map>
#include<mutex>
#include"OfflineMessage.hpp"
#include"Friend.hpp"
#include"Groupmodel.hpp"

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

    //处理客户端异常断开的函数
    void clinetCloseException(const muduo::net::TcpConnectionPtr &conn);
    //一对一聊天业务实现
    void OneChat(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);

    //加好友业务的实现
    void addfriend(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);
    //创建群组业务的实现
    void creategroup(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);
    //加入群组业务的实现
    void addgroup(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);
    //群组聊天业务的实现
    void groupchat(const muduo::net::TcpConnectionPtr &conn,nlohmann::json &js, muduo::Timestamp);


    //服务端异常退出处理代码
    void reset();
    MsgHandler getHandler(int id);
private:
    Chatservice();
    std::unordered_map<int,MsgHandler> m_handler;


    // 存储在线用户的通信连接
    std::unordered_map<int, muduo::net::TcpConnectionPtr> m_ConMap;
    // 定义互斥锁，保证_userConnMap的线程安全
    std::mutex m_mutex;


    UserModel m_usermodel;
    OfflineMessage m_offlineMessage;
    Friend m_friend;
    Groupmodel m_groupmodel;

};









#endif