#include"Handle.h"
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

