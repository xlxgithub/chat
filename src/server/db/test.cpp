// #include<iostream>
// #include<unistd.h>
// #include<chrono>
// #include<vector>
// #include<algorithm>
// #include<numeric>
// #include"dbConnectPool.hpp"
// #include<memory>
// using namespace std;


// void ConnectPool(int count){
//     vector<int> v1;
//     DbPool* pool = DbPool::getinstance();
//     chrono::system_clock::time_point begin = chrono::system_clock::now();
//     for (int i = 0; i < count; i++)
//     {
//         shared_ptr<Mysql> conn = pool->get();
//         char sql[1024] = {0};
//         // sprintf(sql,"insert into user(name,password) values('%s','%s')",'a','a');
//         // bool flag = conn->update(sql);
//     }
//     // pool = nullptr;
//     // if(pool==nullptr){
//     //     cout<<"pool为空"<<endl;
//     // }
//     chrono::system_clock::time_point end = chrono::system_clock::now();
//     cout<<"花费时间:"<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms"<<endl;
    
// }
// void Connect(int count){
//     vector<int> v1;
//     chrono::system_clock::time_point begin = chrono::system_clock::now();
//     Mysql conn;
//     char sql[1024] = {0};
//    // sprintf(sql,"insert into user(id,name,password) values('%s','%s')",'a','a');
//    //std::string ip, unsigned short port, std::string user, std::string passsword, std::string dbname
//     conn.connect();
//     //bool flag = conn.update(sql);

//     chrono::system_clock::time_point end = chrono::system_clock::now();
//     cout<<"花费时间:"<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms"<<endl;
// }
// int main(){

//     int count=25;
//     chrono::system_clock::time_point begin = chrono::system_clock::now();
//     thread t1(ConnectPool,count);
//     thread t2(ConnectPool,count);
//     thread t3(ConnectPool,count);
//     thread t4(ConnectPool,count);
//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();
// Connect(5);
//     chrono::system_clock::time_point end = chrono::system_clock::now();
//     cout<<"main花费时间:"<<chrono::duration_cast<chrono::milliseconds>(end-begin).count()<<"ms"<<endl;
//     this_thread::sleep_for(chrono::seconds(10));
    
//     /*
//                 1000
//     单无连接池：8476ms     连接池 7628          四线程无连接池 6477    四线程连接池 1270
//                 5000
//     单无连接池：42046      连接池 37830         四线程无连接池 32075    四线程连接池 6255
//                 10000
//     单无连接池：91388      连接池 75694         四线程无连接池 64119    四线程连接池 12212
//     */
//     return 0;
// }