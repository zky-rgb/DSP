#include"Logs.h"
#include<fstream>
#include<string>

using namespace std;

LogNode Logs::ShowLogs (int i) 
{
    log_c.lock();
    LogNode node(ids[i], nums[i], years[i], months[i]);
    log_c.unlock();
    return node;
}

void Logs::SaveLogs() {
    std::ofstream out("Logs.txt", std::ios::out);
    for (int i = 0; i < ids.size(); i++) {
        out << ids[i] << " " << nums[i] << " " << years[i] << " " << months[i] << endl;
    }
}

void Logs::InputLog(int id, int num, int year, int month) {
    log_c.lock();
    ids.push_back(id);
    nums.push_back(num);
    years.push_back(year);
    months.push_back(month);
    log_c.unlock();
}

int Logs::ReSize() {
    log_c.lock();
    int size(ids.size());
    log_c.unlock();
    return size;
}

