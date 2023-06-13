#ifndef DB_H
#define DB_H

#include<mysql/mysql.h>
#include<string>
#include<muduo/base/Logging.h>
#include<ctime>
// 定义数据库类 
class Mysql{
public:
    //初始化数据库连接
    Mysql();
    //释放数据库连接资源
    ~Mysql();
    //连接数据库
    bool connect();
    bool connect(const std::string& ip,const unsigned short& port,const std::string& user,const std::string& passsword,const std::string& dbname);
    //更新操作
    bool update(const std::string& sql);
    //查询操作
    MYSQL_RES* query(const std::string& sql);

    //返回数据库连接
    MYSQL* get();

    //刷新连接的起始空闲时间
    void refreshAlivetime(){m_alivetime=clock();}
    //返回存活的时间
    const clock_t getAliceTime(){return clock()-m_alivetime;}


private:
    MYSQL* m_sql;
    clock_t m_alivetime;//记录进入空闲状态之后的存活时间
};
#endif