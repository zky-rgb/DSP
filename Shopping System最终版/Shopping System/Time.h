#pragma once
class Message;
class Handle;
class Time
{
public:
	void Time_clock();//计时器
	Time(Message* m, Handle* handle) :T_m(m), T_handle(handle) {};//构造函数
private:
	Message* T_m;//消息类指针
	Handle* T_handle;//数据处理类指针
};