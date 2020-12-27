#ifndef HANDLE_H
#define HANDLE_H

#include <iostream>
#include <vector>
#include<fstream>
#include "Warhouse.h"

using namespace std;
class Message;



class Logs{
public:
    Logs(); //构造函数
    void ShowLogs(int *id,int *num,int *year,int *month,int i);    //输出全部日志信息 i为当前商品序号
    void SaveLogs();    //保存日志信息到文件
    void InputLog(int id,int num,int year,int month);    //保存单条日志信息到日志中
    int ReSize();



private:
    vector<int> ids;    //以int类型的vector数组存储日志信息
    vector<int> nums;
    vector<int> years;
    vector<int> months;
};


//处理消息类
class Handle
{
    public:
    Handle(Message *m,BTree *bt):h_msg(m),h_btree(bt){};
    void H_createsell();//生成交易信息
    void H_handlemsg();//处理消息队列
    void H_handleOverdue();//处理过期
    Logs* log;

    private:
    Message* h_msg;//指向消息对象的指针
    BTree* h_btree;//指向B树仓库的指针
};



#endif
