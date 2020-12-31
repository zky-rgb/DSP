#include"Message.h"
//消息队列的操作
//插入操作
void Message_queue::Insert(const MsgNode& m)
{
	Msq_m.lock();	
	int size = (end == head) ? 0 : ((7 + end - head) % 7 + 1);
	if(end==head&&size!=0)
		change(head);
	dataqueue[end]=m;
	change(end);

	Msq_m.unlock();
}	
//修改head和end的位置
void Message_queue::change(int &i)
{
	++i;
	if(i==7)
		i=0; 
}
//弹出队首的元素
void Message_queue::Remove()
{
	Msq_m.lock();
		change(head);
	Msq_m.unlock();
}
//搜索第i个消息的信息
MsgNode Message_queue::Search(const int & i)
{
	Msq_m.lock();
	MsgNode j=dataqueue[(head+i)%7];
	Msq_m.unlock();
	return j;
}

