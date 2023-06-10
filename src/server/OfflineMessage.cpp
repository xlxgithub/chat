#include"OfflineMessage.hpp"

bool OfflineMessage::insert(int userid, std::string msg)
{
    //1.组装sql语句
    char sql[1024]={0};
    std::sprintf(sql,"insert into offlineMessage(userid,message) values('%d','%s')",userid,msg.c_str());
    if(DbPool::getinstance()->get()->update(sql)){
        return true;
    }
    return false;
}

bool OfflineMessage::remove(int id)
{
    //1.组装sql语句
    char sql[1024]={0};
    std::sprintf(sql,"delete from offlineMessage where userid='%d'",userid);
    if(DbPool::getinstance()->get()->update(sql)){
        return true;
    }
    return false;
}

std::vector<std::string> OfflineMessage::query(int userid)
{
    //1.组装sql语句
    std::vector<std::string> vec;
    char sql[1024]={0};
    std::sprintf(sql,"select message from offlineMessage where userid='%d'",userid);
    MYSQL_RES* mysql_res = DbPool::getinstance()->get()->query(sql);
    if(mysql_res==nullptr){
        LOG_INFO<<"查询离线信息失败";
        return vec;
    }
    MYSQL_ROW row;
    while((row= mysql_fetch_row(mysql_res)) !=nullptr){
        vec.push_back(row[0]);
    }
    mysql_free_result(mysql_res);
    return vec;
}
