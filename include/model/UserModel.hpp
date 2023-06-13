#ifndef USERMODEL_H
#define USERMODEL_H


#include"User.hpp"
#include"dbConnectPool.hpp"
#include<memory>


class UserModel{
public:

    static DbPool* getDbpool();
    // User表的增加方法
    bool insert(User &user);

    // 根据用户号码查询用户信息
    const User query(const int& id);

    // 更新用户的状态信息
    bool updateState(User& user);

    // 重置用户的状态信息
    void resetState();
private:
    DbPool* m_mysql_pool;
};

#endif