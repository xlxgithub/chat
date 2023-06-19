#include"OfflineMessage.hpp"

bool OfflineMessage::insert(const int& userid, const std::string& msg,const std::string& time,const std::string& msgtype)
{
    //1.组装sql语句
    char sql[1024]={0};
    std::sprintf(sql,"insert into offlineMessage(userid,message,time,msgtype) values('%d','%s','%s','%s')",userid,msg.c_str(),time.c_str(),msgtype.c_str());
    if(DbPool::getinstance()->get()->update(sql)){
        return true;
    }
    return false;
}

bool OfflineMessage::remove(const int& id)
{
    //1.组装sql语句
    char sql[1024]={0};
    sprintf(sql,"delete from offlineMessage where userid=%d",id);
    if(DbPool::getinstance()->get()->update(sql)){
        LOG_INFO<<"删除离线消息成功"<<"userid"<<id;
        return true;
    }
    return false;
}

std::vector<std::string> OfflineMessage::query(const int& userid)
{
    //1.组装sql语句
    std::vector<std::string> vec;
    char sql[1024]={0};
    std::sprintf(sql,"select message,time,msgtype from offlineMessage where userid='%d'",userid);
    MYSQL_RES* mysql_res = DbPool::getinstance()->get()->query(sql);
    if(mysql_res==nullptr){
        LOG_INFO<<"查询离线信息失败";
        return vec;
    }
    MYSQL_ROW row;
    while((row= mysql_fetch_row(mysql_res)) !=nullptr){
        vec.push_back(row[0]);
        vec.push_back(row[1]);
        vec.push_back(row[2]);
    }
    mysql_free_result(mysql_res);
    return vec;
}
