# 整个项目流程
## 1.构建项目简单CMake文档
    1.主CMake 各个文件夹Cmake 
    2.根据 gcc -g -o *.cpp -lpthread 编译命令配置
        2.1 配置编译选项
        2.2 配置生成的程序所在路径
        2.3 配置头文件包含路径
    3.最终的子CMake文件 配置所有.cpp 文件 编译链接的动态库
## 2.编写网络模块 服务端代码
    1.组合TcpServer对象
    2.创建EventLoop 事件循环对象指针
    3.明确TcpServer构造函数需要什么参数,输出ChatServer对象构造函数
    4.在当前服务器类的构造函数中,注册处理连接的回调函数和处理读写事件的回调函数
    5.设置合适的服务端线程数量，muduo库会自己划分I/O线程和worker线程

## 3.业务模块代码
    1.给网络模块代码提供一个接口 根据msgid不同 处理不同情景
    2.定义msgid的意义 与处理函数一一对应
    3.根据业务需求 编写 处理函数其中包括
        1.注册函数reg
        2.登陆函数login
        3.一对一私聊函数OneChat
        4.添加好友函数addfriend
        5.创建群组函数 creategroup
        6.加入群组函数 addgroup
        7.群聊天函数   chatgroup