#include"Friend.hpp"

bool Friend::insert(const int &userid, const int &friendid)
{
    //1.组装sql语句
    char sql[1024];
    sprintf(sql, "insert into friend(userid, friendid) values('%d', '%d')",userid,friendid);
    if(DbPool::getinstance()->get()->update(sql)){
        return true;
    }
    LOG_INFO<<"添加朋友失败";
    return false;
    
}

bool Friend::remove(const int &userid, const int &friendid)
{
    return false;
}

std::vector<User> Friend::query(const int& userid)
{
    std::vector<User> v_user;
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where b.userid=%d", userid);

    MYSQL_RES* mysql_res=DbPool::getinstance()->get()->query(sql);
    if(mysql_res==nullptr){
        LOG_INFO<<"返回朋友列表失败";
        return v_user;
    }
    MYSQL_ROW row;
    while ((row=mysql_fetch_row(mysql_res))!=nullptr)
    {
        User user;
        user.setId(atoi(row[0]));
        user.setName(row[1]);
        user.setState(row[2]);
        v_user.push_back(user);
    }
    mysql_free_result(mysql_res);
    return v_user;
}
