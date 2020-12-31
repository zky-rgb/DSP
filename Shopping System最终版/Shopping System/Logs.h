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
    LogNode ShowLogs(int i);    //���ȫ����־��Ϣ iΪ��ǰ��Ʒ���
    void SaveLogs();    //������־��Ϣ���ļ�
    void InputLog(int id, int num, int year, int month);    //���浥����־��Ϣ����־��
    int ReSize();
private:
    std::mutex  log_c;//�̻߳�����
    std::vector<int> ids;    //��int���͵�vector����洢��־��Ϣ
    std::vector<int> nums;
    std::vector<int> years;
    std::vector<int> months;
};