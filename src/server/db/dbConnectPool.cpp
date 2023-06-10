#include"dbConnectPool.hpp"

DbPool *DbPool::getinstance()
{
    static DbPool instance;
    return &instance;
}

std::shared_ptr<Mysql> DbPool::get()
{
    //先判断队列是否为空
    std::unique_lock<std::mutex> locker(m_queue_mutex);
    while (m_queue.empty())
    {
        if(std::cv_status::timeout==cv.wait_for(locker,std::chrono::milliseconds(m_connectionTimeout))){
            if(m_queue.empty()){
                LOG_INFO<<"获取空闲连接超时，获取连接失败";
                return nullptr;
            }
        }
    }
    /*
        智能指针析构时会直接delete 我们需要让他归还到连接池
    */
    std::shared_ptr<Mysql> sp(m_queue.front(),
        [&](Mysql *pcon){
            std::unique_lock<std::mutex> locker(m_queue_mutex);
            //pcon->refreshAlivetime();
            m_queue.push(pcon);
        }
    );
    m_queue.pop();
    cv.notify_one();//消费完成后通知生产者检查一下
    return sp;
}

bool DbPool::ReleaseConnection(Mysql *conn)
{
    if(conn==nullptr)
    return false;
    
    std::unique_lock<std::mutex> locker(m_queue_mutex);
    m_queue.push(conn);
    conn->refreshAlivetime();
    return true;
}

void DbPool::DestroyPool()
{
    std::unique_lock<std::mutex> locker(m_queue_mutex);
    while (!m_queue.empty())
    {
        Mysql* temp = m_queue.front();
        m_queue.pop();
        delete temp;
    }
}

DbPool::DbPool()
{
    //首先加载配置文件获取 数据库登陆需要的信息
    if(!loadConfigfile()){
        LOG_INFO<<"数据库配置文件加载失败"<<__FILE__;
    }
    m_cur_queue_count = 0;
    for(int i =0;i<m_initSize;i++){
        Mysql* conn = new Mysql();
        if(!conn->connect(m_ip,m_port,m_username,m_password,m_dbname)){
            LOG_INFO<<"数据库连接失败"<<__FILE__;
        }
        m_queue.push(conn);
        m_cur_queue_count++;
    }
    //启动一个新的线程作为连接的生产者 
    std::thread produce(std::bind(&DbPool::produceConnectionTask,this));
    produce.detach();

    //启动一个新的线程作为超时连接的管理者 
    std::thread scan(std::bind(&DbPool::ScanConnectionTask,this));
    scan.detach();
}

DbPool::~DbPool()
{
    DestroyPool();
}

void DbPool::produceConnectionTask()
{
    for(;;){
        std::unique_lock<std::mutex> locker(m_queue_mutex);
        while (!m_queue.empty())
        {
            cv.wait(locker); //队列不空 此处生产者线程进入等待状态
        }
        //判断连接数量是否达到上限
        if(m_cur_queue_count < m_MaxSize){
            Mysql* conn = new Mysql();
            if(!conn->connect(m_ip,m_port,m_username,m_password,m_dbname)){
                LOG_INFO<<"数据库连接失败"<<__FILE__;
            }
            m_queue.push(conn);
            m_cur_queue_count++;
        }
        //通知消费者线程可以连接了
        cv.notify_all();
    
    }
}

bool DbPool::loadConfigfile()
{
    FILE* fp = fopen("./mysql.ini","r");
    if(fp==nullptr){
        LOG_INFO<<"数据库加载文件读取失败"<<__FILE__;
        return false;
    }
    while (!feof(fp))
    {
        //feof文件结束符 判断文件是否为空
        //读取一行
        char line[1024] = {0};
        fgets(line,1024,fp);
        std::string  str = line;
        // 根据字符串提供的函数找=所在位置
        int index = str.find('=',0);
        if(index==-1){
            continue;
        }
       //找结束位置
        int endindex = str.find('\n',index);
        std::string key = str.substr(0,index);
        std::string value = str.substr(index+1,endindex-index-1);
        if(key=="ip"){
            m_ip = value;
        }else if(key=="port"){
            m_port = atoi(value.c_str());
        }else if(key=="username"){
            m_username = value;
        }else if(key=="password"){
            m_password = value;
        }else if(key=="dbname"){
            m_dbname = value;
        }else if(key=="initSize"){
            m_initSize=atoi(value.c_str());
        }else if(key=="maxSize"){
            m_MaxSize = atoi(value.c_str());
        }else if(key=="maxIdleTime"){
            m_MaxidleTime=atoi(value.c_str());
        }else if(key=="connectionTimeOut"){
            m_connectionTimeout = atoi(value.c_str());
        }
    }
    

}

void DbPool::ScanConnectionTask()
{
        for(;;){
        std::this_thread::sleep_for(std::chrono::seconds(m_MaxidleTime));
        std::unique_lock<std::mutex> locker(m_queue_mutex);
        while (m_cur_queue_count>m_initSize)
        {
            Mysql *p = m_queue.front();
            if(p->getAliceTime()>=(m_MaxidleTime*1000)){
                m_queue.pop();
                m_cur_queue_count--;
                delete p;
            }else{
                break;
            }
        }
        
    }
}
