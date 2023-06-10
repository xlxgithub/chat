#include<iostream>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include"Chatserver.hpp"
using namespace std;
int main(){
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr("127.0.0.1",9999);
    ChatServer server(&loop,listenAddr,"caht");
    server.start();
    loop.loop();
}