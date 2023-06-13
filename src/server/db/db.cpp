#include"db.hpp"
// 数据库配置信息
static std::string server = "127.0.0.1";
static std::string user = "debian-sys-maint";
static std::string password = "LZcsEeYtC2itengH";
static std::string dbname = "chat";
//初始化数据库连接
Mysql::Mysql(){
    m_sql = mysql_init(nullptr);
    if(m_sql==nullptr){
        LOG_INFO<<"数据库初始化失败";
    }
}
//释放数据库连接资源
Mysql::~Mysql(){
    if(m_sql!=nullptr){
        mysql_close(m_sql);
        LOG_INFO<<"数据库已经关闭";
    }
}
bool Mysql::connect()
{
    // LOG_INFO<<"IP:"<<server.c_str()<<"User:"<<user.c_str()<<"Passwd:"<<password.c_str()<<"DB:"<<dbname.c_str();
    MYSQL *p = mysql_real_connect(m_sql, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示？
        mysql_query(m_sql, "set names gbk");
        //LOG_INFO << "connect mysql success!";

    }
    else
    {
        LOG_INFO << "!!connect mysql fail!";
        LOG_INFO << "!!connect mysql fail: " << mysql_error(m_sql);
    }

    return p;
}
// 连接数据库
bool Mysql::connect(const std::string& ip,const unsigned short& port,const std::string& user,const std::string& passsword,const std::string& dbname)
{
    
    MYSQL *p = mysql_real_connect(m_sql,ip.c_str(),user.c_str(),passsword.c_str(),dbname.c_str(),3306,nullptr,0);
    if (p != nullptr)
    {
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文显示？
        mysql_query(m_sql, "set names gbk");
        //LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql fail!";
    }

    return p;
}
//更新操作
bool Mysql::update(const std::string& sql){
    if (mysql_query(m_sql, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }

    return true;
}
//查询操作
MYSQL_RES* Mysql::query(const std::string& sql){

    if (mysql_query(m_sql, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    
    return mysql_use_result(m_sql);
}

MYSQL *Mysql::get()
{
    return m_sql;
}
