#ifndef GROUPUSER_H
#define GROUPUSER_H
#include"User.hpp"
#include<string>
class Groupuser:public User{
public:
    void setRole(const std::string& role) { this->m_role = role; }
    std::string getRole() { return this->m_role; }

private:
    std::string m_role;
};
#endif