#include"Chatserver.hpp"
#include<functional>
#include<json.hpp>
ChatServer::ChatServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listnAddr, const std::string &name):m_conn(loop,listnAddr,name),m_loop(loop){
    m_conn.setConnectionCallback(std::bind(&ChatServer::OnConnection,this,std::placeholders::_1));
    m_conn.setMessageCallback(std::bind(&ChatServer::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    m_conn.setThreadNum(2);
}
void ChatServer::start(){
    m_conn.start();
}
void ChatServer::OnConnection(const muduo::net::TcpConnectionPtr &conn){
    LOG_INFO<<"连接成功了！！！！！！！！";
    if(!conn->connected()){
        LOG_INFO<<"！！连接断开了！！";
        Chatservice::getinstance()->clinetCloseException(conn);
        conn->shutdown();
    }
}
void ChatServer::OnMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer *buffer, muduo::Timestamp receiveTime){
    std::string temp = buffer->retrieveAllAsString();
    //数据序列化
    nlohmann::json js = js.parse(temp);

    auto handler = Chatservice::getinstance()->getHandler(js["msgid"]);
    handler(conn,js,receiveTime);
}