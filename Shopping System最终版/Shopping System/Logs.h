#pragma once
#include <vector>
#include<mutex>
struct LogNode
{
    int id;
    int num;
    int year;
    int month;
    LogNode(int i, int n, int y, int m) :id(i), num(n), year(y), month(m){}
};

class Logs {
public:
    LogNode ShowLogs(int i);    //输出全部日志信息 i为当前商品序号
    void SaveLogs();    //保存日志信息到文件
    void InputLog(int id, int num, int year, int month);    //保存单条日志信息到日志中
    int ReSize();
private:
    std::mutex  log_c;//线程互斥量
    std::vector<int> ids;    //以int类型的vector数组存储日志信息
    std::vector<int> nums;
    std::vector<int> years;
    std::vector<int> months;
};