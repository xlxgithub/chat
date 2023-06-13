#!/usr/bin/zsh

                                        
PORT="3306"
USERNAME="root"
PASSWORD="xlx0220@"




DBNAME="chat"   
#表名                                                    
TABLENAME="user" 
TABLENAME2="friend"
TABLENAME3="allgroup" 
TABLENAME4="groupuser"
TABLENAME5="offlineMessage"



#创建使用数据库
create_database="create database IF NOT EXISTS ${DBNAME}"
mysql -u${USERNAME} -p${PASSWORD}  -e"${create_database}"


create_table_sql1="create table IF NOT EXISTS ${TABLENAME} ( 
id int primary key auto_increment,
name varchar(50) not null unique,
password varchar(50) not null,
state enum('online','offline') default 'offline'
)"

create_table_sql2="create table IF NOT EXISTS ${TABLENAME2} ( 
userid int not null,
friendid int not null,
primary key(userid,friendid)
)"
create_table_sql3="create table IF NOT EXISTS ${TABLENAME3} ( 
id int primary key auto_increment,
groupname varchar(50) not null unique,
groupdesc varchar(50)
)"

create_table_sql4="create table IF NOT EXISTS ${TABLENAME4} ( 
groupid int not null,
userid int not null,
primary key(groupid,userid),
grouprole enum('creator','normal') default 'normal'
)"
create_table_sql5="create table IF NOT EXISTS ${TABLENAME5} ( 
userid int not null,
message varchar(50) not null 
)"


for ((i=1; i<=5; i++))
do
str="create_table_sql"$i
str1=`eval echo '$'"$str"`
#echo $str1
mysql  -h${IP}  -u${USERNAME} -p${PASSWORD} ${DBNAME} -e"${str1}"
done