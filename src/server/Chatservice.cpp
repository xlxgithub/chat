#include"Chatservice.hpp"


//单例模式
Chatservice* Chatservice::getinstance(){
    static Chatservice instance;
    return &instance;
}
Chatservice::Chatservice(){
    //初始化消息id对应的处理器
    m_handler.insert({MSGID::LOG_ID,std::bind(&Chatservice::login,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    m_handler.insert({MSGID::REG_ID,std::bind(&Chatservice::reg,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    m_handler.insert({MSGID::ONECHAT,std::bind(&Chatservice::OneChat,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}

void Chatservice::login(const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp time)
{
    //获取用户id和密码
    int id = js["id"];
    std::string passwd = js["passwd"];
    //根据用户名查找用户密码
    User user = m_usermodel.query(id);
    if(user.getId()!=-1 && user.getPwd()==passwd){
        if (user.getState() == "online")
        {
            // 该用户已经登录，不允许重复登录
            nlohmann::json response;
            response["msgid"] = MSGID::LOG_ACK;
            response["errno"] = 1;
            response["errmsg"] = "this account is using, input another!";
            conn->send(response.dump());
        }else{
            //登陆成功
            //将用户插入在线信息表中
            {
                std::lock_guard<std::mutex> locker(m_mutex);
                m_ConMap.insert({id,conn});
            }
            //修改状态信息 online
            user.setState("online");
            m_usermodel.updateState(user);

            //如果离线表中存在信息 则输出
            nlohmann::json response;
            std::vector<std::string> vec_offMessage=m_offlineMessage.query(user.getId());
            if(!vec_offMessage.empty()){
                response["offlinemsg"] = vec_offMessage;
                // 读取该用户的离线消息后，把该用户的所有离线消息删除掉
                m_offlineMessage.remove(id);
            }

            response["msgid"] = MSGID::LOG_ACK;
            response["errno"] = 0;
            response["errmsg"] = "登陆成功";
            conn->send(response.dump());
        }
    }
    if(user.getId()==-1){
        nlohmann::json response;
        response["msgid"] = MSGID::LOG_ACK;
        response["errno"] = 2;
        response["errmsg"] = "没有该用户 请先注册!";
        conn->send(response.dump());
        return;
    }
    if(user.getPwd()!=passwd){
        nlohmann::json response;
        response["msgid"] = MSGID::LOG_ACK;
        response["errno"] = 3;
        response["errmsg"] = "密码错误!";
        conn->send(response.dump());
        return;
    }



}

void Chatservice::reg(const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp)
{
    LOG_INFO<<"!!!注册成功";
    std::string name = js["name"];
    std::string passwd = js["passwd"];
    User user;
    user.setName(name);
    user.setPwd(passwd);
    if(!m_usermodel.insert(user)){
       LOG_INFO<<"注册用户失败"; 
       return;
    }
    js["msgid"]=MSGID::LOG_ACK;
    js["id"]=user.getId();
    conn->send(js.dump());

}

void Chatservice::clinetCloseException(const muduo::net::TcpConnectionPtr &conn)
{
    User user;
    // LOG_INFO<<"连接断开 开始恢复offline";
    for(auto it=m_ConMap.begin();it!=m_ConMap.end();it++){
        if(it->second=conn){
            {
                // LOG_INFO<<"找到对应id 开始恢复offline";
                std::lock_guard<std::mutex> locker(m_mutex);
                user.setId(it->first);
                m_ConMap.erase(it);
                // LOG_INFO<<"找到对应id 删除了";
                break;
            }
        }
    }
    // LOG_INFO<<"找到对应id for之后 开始恢复offline";
    if(user.getId()!=-1){
        user.setState("offline");
        m_usermodel.updateState(user);
    }
}

void Chatservice::OneChat(const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp)
{
    int toid = js["toid"];
    std::string message = js["message"];

    //先看用户在不在线
    User user = m_usermodel.query(toid);
    if(user.getId()==-1){
        LOG_INFO<<"不存在这个用户";
        return;
    }
    //1.用户在线
    if(user.getState()=="online"){
        //用户在线
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_ConMap.find(toid);
        if (it != m_ConMap.end())
        {
            // toid在线，转发消息   服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }
    //2.用户不在线
    if(user.getState()=="offline"){
        //用户不在线
        m_offlineMessage.insert(user.getId(),message);
    }
}

void Chatservice::reset()
{
    m_usermodel.resetState();
}

MsgHandler Chatservice::getHandler(int id)
{
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = m_handler.find(id);
    if (it == m_handler.end())
    {
        // 返回一个默认的处理器，空操作
        return [=](const muduo::net::TcpConnectionPtr &conn, nlohmann::json &js, muduo::Timestamp) {
            LOG_ERROR << "msgid:" << id << " can not find handler!";
        };
    }
    else
    {
        return m_handler[id];
    }
}
