#include"db.hpp"
//初始化数据库连接
Mysql::Mysql(){
    m_sql = mysql_init(nullptr);
    if(m_sql!=nullptr){
        LOG_INFO<<"数据库初始化成功";
    }
}
//释放数据库连接资源
Mysql::~Mysql(){
    if(m_sql!=nullptr){
        mysql_close(m_sql);
        LOG_INFO<<"数据库已经关闭";
    }
}

// 连接数据库
bool Mysql::connect(std::string ip, unsigned short port, std::string user, std::string passsword, std::string dbname)
{
    MYSQL *p = mysql_real_connect(m_sql,ip.c_str(),user.c_str(),passsword.c_str(),dbname.c_str(),port,nullptr,0);
    if (p != nullptr)
    {
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示？
        mysql_query(m_sql, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql fail!";
    }

    return p;
}
//更新操作
bool Mysql::updata(std::string sql){
    if (mysql_query(m_sql, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }

    return true;
}
//查询操作
MYSQL_RES* Mysql::select(std::string sql){

    if (mysql_query(m_sql, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    
    return mysql_use_result(m_sql);
}