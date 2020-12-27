#include"Logs.h"
#include<fstream>
#include<string>

using namespace std;

void Logs::ShowLogs(int* id, int* num, int* year, int* month, int i) {

    *id = ids[i];
    *num = nums[i];
    *year = years[i];
    *month = months[i];
}

void Logs::SaveLogs() {
    std::ofstream out("Logs.txt", std::ios::out);
    for (int i = 0; i < ids.size(); i++) {
        out << ids[i] << " " << nums[i] << " " << years[i] << " " << months[i] << endl;
    }
}

void Logs::InputLog(int id, int num, int year, int month) {
    ids.push_back(id);
    nums.push_back(num);
    years.push_back(year);
    months.push_back(month);
}

int Logs::ReSize() {
    return ids.size();
}

