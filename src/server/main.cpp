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

int main(int argc, char **argv){

    signal(SIGINT, resetHandler);
    muduo::net::EventLoop loop;
    if (argc < 2)
    {
        cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << endl;
        exit(-1);
    }
    // 解析通过命令行参数传递的ip和port
    uint16_t port = atoi(argv[1]);

    muduo::net::InetAddress listenAddr("127.0.0.1",port);
    ChatServer server(&loop,listenAddr,"caht");
    server.start();
    loop.loop();
}