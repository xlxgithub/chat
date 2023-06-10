#ifndef DBCONNECTPOOL_H
#define DBCONNECTPOOL_H
#include"db.hpp"
#include<memory>
#include<mutex>
#include<queue>
#include<atomic>
#include<muduo/base/Logging.h>
#include<thread>
#include<functional>
#include<condition_variable>
//定义数据库连接池 单例模式 超时关闭连接
class DbPool{
public:
    static DbPool* getinstance();
    //提供给外部的接口获取数据库连接
    std::shared_ptr<Mysql> get();//对外接口

    bool ReleaseConnection(Mysql *conn); //释放连接
    void DestroyPool();					 //销毁所有连接
private:
    DbPool();
    ~DbPool();

    void produceConnectionTask(); //生产者
    
    bool loadConfigfile(); //加载配置文件
    void ScanConnectionTask();//管理超时连接

    std::string m_ip;//mysql ip地址
    unsigned int m_port;//mysql 端口号
    std::string m_username;  //mysql 用户名
    std::string m_password;  //mysql 密码
    std::string m_dbname;   //mysql 数据库


    int m_initSize;// 连接池初始连接数量
    int m_MaxSize;//  连接池最大连接数量
    int m_MaxidleTime;// 连接池最大空闲时间
    int m_connectionTimeout;//连接池获取连接的超时时间

    //存放连接池的阻塞队列
    std::queue<Mysql*> m_queue;
    std::mutex m_queue_mutex;//阻塞队列的线程安全锁
    std::atomic_int m_cur_queue_count; 
    std::condition_variable cv;
    bool flag = true;

};





#endif