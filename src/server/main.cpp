#include<iostream>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include"Chatserver.hpp"
#include<signal.h>
#include"Chatservice.hpp"
using namespace std;

// 处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int)
{
    Chatservice::getinstance()->reset();
    exit(0);
}

int main(){

    signal(SIGINT, resetHandler);
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr("127.0.0.1",9999);
    ChatServer server(&loop,listenAddr,"caht");
    server.start();
    loop.loop();
}