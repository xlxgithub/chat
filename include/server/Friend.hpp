#ifndef FRIEND_H
#define FRIEND_H
class Friend{
public:
    bool insert(int& userid,int& friendid);
    bool remove(int& userid,int& friendid);
private:
    int userid;
    int friendid;
};


#endif