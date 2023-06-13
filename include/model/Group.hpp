#ifndef GROUP_H
#define GROUP_H
#include<string>

class Group{
public:
    Group(int id=-1,std::string groupname="",std::string groupdesc=""){
        m_id=id;
        m_groupname = groupname;
        m_groupdesc = groupdesc;
    }
    void setId(const int& id) { this->m_id = id; }
    void setName(const std::string& name) { this->m_groupname = name; }
    void setDesc(const std::string& desc) { this->m_groupdesc = desc; }

    int getId() { return this->m_id; }
    std::string getName() { return this->m_groupname; }
    std::string getDesc() { return this->m_groupdesc; }
private:
    int m_id;
    std::string m_groupname;
    std::string m_groupdesc;
};

#endif