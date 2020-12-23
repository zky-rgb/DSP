#include"class.h"

//时刻函数，用来计时
void Time::Time_clock()
{
	using namespace std;
	int times(0);//计数
	while(!T_m->msg_quit())
	{
		chrono::milliseconds dura(2000);//2000毫秒进行一次刷新
		this_thread::sleep_for(dura);
		if(times==5)
		{
			T_m->msg_bool.lock();
			T_m->msg_remsg=true;
			T_m->msg_bool.unlock();
			T_m->msg_increasetime();//给消息类传递时间递增信号
			times=0;//重置时间
		}
		T_ui->UI_Refresh_Time();//给UI类刷新的请求
		++times;//计数器递增
	}
}
