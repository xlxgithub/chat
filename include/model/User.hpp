#ifndef USER_H
#define USER_H
#include <string>

class User{
public:
    User(int id=-1,std::string name="",std::string password="",std::string state="offline"){
        m_id=id;
        m_name=name;
        m_password=password;
        m_state=state;
    }
    void setId(const int& id) { this->m_id = id; }
    void setName(const std::string& name) { this->m_name = name; }
    void setPwd(const std::string& pwd) { this->m_password = pwd; }
    void setState(const std::string& state) { this->m_state = state; }

    const int& getId() { return this->m_id; }
    const std::string& getName() { return this->m_name; }
    const std::string& getPwd() { return this->m_password; }
    const std::string& getState() { return this->m_state; }

private:
    int m_id;               // 用户名
    std::string m_name;     // 用户名
    std::string m_password; // 用户密码
    std::string m_state;    // 用户登陆状态
};

#endif