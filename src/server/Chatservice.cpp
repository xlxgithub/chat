#include"Chatservice.hpp"


//单例模式
Chatservice* Chatservice::getinstance(){
    static Chatservice instance;
    return &instance;
}
Chatservice::Chatservice(){
    //初始化消息id对应的处理器
    m_handler.insert({MSGID::LOG_ID,std::bind(&Chatservice::login,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    m_handler.insert({MSGID::REG_ID,std::bind(&Chatservice::reg,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}
void Chatservice::login(const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp time)
{
    LOG_INFO<<"!!!登陆成功";
}

void Chatservice::reg(const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp)
{
    LOG_INFO<<"!!!注册成功";
}

MsgHandler Chatservice::getHandler(int id)
{
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = m_handler.find(id);
    if (it == m_handler.end())
    {
        // 返回一个默认的处理器，空操作
        return [=](const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp) {
            LOG_ERROR << "msgid:" << id << " can not find handler!";
        };
    }
    else
    {
        return m_handler[id];
    }
}
