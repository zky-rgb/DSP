#pragma once
class Message;
class Handle;
class Time
{
public:
	void Time_clock();//��ʱ��
	Time(Message* m, Handle* handle) :T_m(m), T_handle(handle) {};//���캯��
private:
	Message* T_m;//��Ϣ��ָ��
	Handle* T_handle;//���ݴ�����ָ��
};