#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include"Group.hpp"
#include"Groupuser.hpp"
#include"dbConnectPool.hpp"
class Groupmodel
{
public:
    //创建群组
    bool create_group(Group& group);
    //加入群组
    bool add_group(const int& groupid,const int& ueserid,const std::string& groupprole);
    //删除群组
    bool remove_group();

    bool isin_Group(const int& groupid,const int& ueserid);

    // 查询用户所在群组信息用于登陆时候的显示
    std::vector<Group> queryGroups(const int& userid);

    // 根据组id找到所有用户
    std::vector<Groupuser> queryUser(const int& groupid);

    // 根据组id找到所有用户id
    std::vector<int> queryUserid(int& groupid);
private:
};
#endif