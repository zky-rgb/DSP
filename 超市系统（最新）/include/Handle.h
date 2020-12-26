#ifndef HANDLE_H
#define HANDLE_H

#include <iostream>
#include <vector>
#include<fstream>

using namespace std;
class Message;



class Logs{
public:
    Logs(); //构造函数
    void ShowLogs();    //输出全部日志信息
    void SaveLogs();    //保存日志信息到文件
    void InputLog(int id,int num,int year,int month);    //保存单条日志信息到日志中



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
    Logs log;

    private:
    Message* h_msg;//指向消息对象的指针
    BTree* h_btree;//指向B树仓库的指针
};




Logs ::Logs() {
    string example = "时间 货物名称 货物数量";
}

void Logs ::ShowLogs() {
    cout<<"----日志----"<<endl;
    for(int i=0;i<ids.size();i++){
        cout<<ids[i]<<" "<<nums[i]<<" "<<years[i]<<" "<<months[i]<<endl;
    }
    cout<<"------------"<<endl;
}

void Logs ::SaveLogs() {
    std::ofstream out("Logs.txt",std::ios::out);
    for(int i=0;i<ids.size();i++){
        out<<ids[i]<<" "<<nums[i]<<" "<<years[i]<<" "<<months[i]<<endl;
    }
    cout<<"Saved to file!"<<endl;
}

void Logs ::InputLog(int id,int num,int year,int month) {
    ids.push_back(id);
    nums.push_back(num);
    years.push_back(year);
    months.push_back(month);
}


#endif
