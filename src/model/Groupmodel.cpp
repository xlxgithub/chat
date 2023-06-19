#include"Groupmodel.hpp"

bool Groupmodel::create_group(Group& group)
{
    //1.组装sql语句 更新两个表
    char sql[1024]={0};
    sprintf(sql,"insert into allgroup(groupname,groupdesc) values('%s','%s')",group.getName().c_str(),group.getDesc().c_str());

    //2.执行
    std::shared_ptr<Mysql> m1 = DbPool::getinstance()->get();
    if(m1->update(sql)){
        group.setId(mysql_insert_id(m1->get()));
        return true;
    }
    LOG_INFO<<__FILE__<<"创建群组失败";
    return false;
}

bool Groupmodel::add_group(const int &groupid, const int &ueserid,const std::string& groupprole)
{
    //如果用户已经在群组 则不创建 新增函数 查询用户是否在群组
    if(isin_Group(groupid,ueserid)){
        LOG_INFO<<"用户已经在群组 不要重复加入";
        return false;
    }

    //1.组装sql语句 更新表
    char sql[1024]={0};
    sprintf(sql,"insert into groupuser(groupid,userid,grouprole) values(%d,%d,'%s')",groupid,ueserid,groupprole.c_str());

    //2.执行
    std::shared_ptr<Mysql> m1 = DbPool::getinstance()->get();
    if(m1->update(sql)){
        return true;
    }
    LOG_INFO<<__FILE__<<"加入群组失败";
    return false;
}

bool Groupmodel::isin_Group(const int &groupid, const int &ueserid)
{
    char sql[1024]={0};
    sprintf(sql,"select userid from groupuser where groupid=%d",groupid);
    MYSQL_RES* my_res = DbPool::getinstance()->get()->query(sql);
    if(my_res!=nullptr){
        MYSQL_ROW row;
        while((row = mysql_fetch_row(my_res))!=nullptr){
            if(atoi(row[0])==ueserid){
                return true;
            }
        }
    }
    return false;
}

std::vector<Group> Groupmodel::queryGroups(const int &userid)
{
    //1.组装sql语句
    /*
     1.根据userid在groupuser表中查询出该用户所属的群组信息
     2.根据群组信息，查询属于该群组的所有用户id 和user表进行联合查询 查出用户详细信息
    */
    std::vector<Group> res;
    char sql[1024]={0};
    sprintf(sql,"select a.id,a.groupname,a.groupdesc from allgroup a inner join groupuser b on a.id = b.groupid where b.userid=%d",userid);
    MYSQL_RES* my_res = DbPool::getinstance()->get()->query(sql);
    if(my_res!=nullptr){
        MYSQL_ROW row;
        while((row = mysql_fetch_row(my_res))!=nullptr){
            Group group;
            group.setId(atoi(row[0]));
            group.setName(row[1]);
            group.setDesc(row[2]); 
            res.push_back(group);
        }
    }
    return res;
}

std::vector<Groupuser> Groupmodel::queryUser(const int &groupid)
{

    std::vector<Groupuser> res;
    char sql[1024]={0};
    sprintf(sql,"select a.id,a.name,a.state,b.grouprole from user a inner join groupuser b on a.id=b.userid where b.groupid=%d",groupid);
    MYSQL_RES* my_res = DbPool::getinstance()->get()->query(sql);
    if(my_res!=nullptr){
        MYSQL_ROW row;
        while((row = mysql_fetch_row(my_res))!=nullptr){
            Groupuser guser;
            guser.setId(atoi(row[0]));
            guser.setName(row[1]);
            guser.setRole(row[2]);
            guser.setState(row[3]);
            res.push_back(guser);
        }
    }
    return res;
}

std::vector<int> Groupmodel::queryUserid(int &groupid)
{
    std::vector<int> res;
    char sql[1024]={0};
    sprintf(sql,"select userid from groupuser where groupid=%d",groupid);
    MYSQL_RES* my_res = DbPool::getinstance()->get()->query(sql);
    if(my_res!=nullptr){
        MYSQL_ROW row;
        while((row = mysql_fetch_row(my_res))!=nullptr){
            res.push_back(atoi(row[0]));
        }
    }
    return res;
}
