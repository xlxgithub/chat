#ifndef CHATSERVER_H
#define CHATSERVER_H

// 利用muduo网络库定义服务端的类

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <string>
#include<muduo/base/Logging.h>
#include"Chatservice.hpp"
class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listnAddr, const std::string &name);
    void start();

private:
    /*
      TcpServer 构造函数参数列表
        TcpServer(EventLoop* loop,
            const InetAddress& listenAddr,
         const string& nameArg,
    */
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    void OnMessage(const muduo::net::TcpConnectionPtr &conn,
                   muduo::net::Buffer *buffer,
                   muduo::Timestamp receiveTime);
    muduo::net::TcpServer m_conn;
    muduo::net::EventLoop *m_loop;
};

#endif