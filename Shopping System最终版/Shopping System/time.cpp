#include"Time.h"
#include "Handle.h"
#include"Message.h"
#include<thread>
#include<iostream>

//时刻函数，用来计时
void Time::Time_clock()
{
	using namespace std;
	int times(0);//计数
	while(!T_m->msg_quit())
	{
		chrono::milliseconds dura(3000);//2000毫秒进行一次刷新
		this_thread::sleep_for(dura);
		if(times==3)
		{
			T_m->msg_increasetime();//给消息类传递时间递增信号
			times=0;//重置时间
			T_handle->H_createsell();
			T_handle->H_handlemsg();
			T_handle->H_handleOverdue();
		}			
		++times;//计数器递增
	}
}
