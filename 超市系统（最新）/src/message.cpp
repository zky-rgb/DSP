#include"Message.h"
#include"Warhouse.h"
//消息类实现
//消息类构造函数
Message::Message()
{
	msg_time = 1;
	msg_year = 2020;
	msg_remsg=true;
	msg_uiquit = false;
	msg_handle=NULL;
}
//递增时间
void Message::msg_increasetime()
{
	bool monchange(false);
    msg_ctime.lock();
	++msg_time;
	if(msg_time==13)
		msg_time=1;
	switch (msg_time)
	{
	case 1:
		++msg_year;//年份递增
		break;
	case 3:
		msg_season=Type_Season::Spr;
		monchange = true;
		break;
	case 6:
		msg_season=Type_Season::Sum;
		monchange = true;
		break;
	case 9:
		msg_season=Type_Season::Aut;
		monchange = true;
		break;
	case 12:
		msg_season=Type_Season::Win;
		monchange = true;
		break;
	default:
		break;
	}
	msg_ctime.unlock();
	msg_pop();//删除过期的消息
	////调用处理类来生成对应的消息
	if (monchange)
	{
		msg_handle->H_handlemsg();
		msg_handle->H_createsell();
		msg_handle->H_handleOverdue();
		msg_clearIO();//重置
	}
}
//获取时间
void Message::msg_gettime(int &mon,int &year,Type_Season *s)
{
	msg_ctime.lock();
	mon=msg_time;
	year=msg_year;
	msg_ctime.unlock();
	if (s != NULL)
	{
		switch (mon)
		{
		case 12:
		case 1:
		case 2:
			*s=Type_Season::Win;
			break;
		case 11:
		case 10:
		case 9:
			*s=Type_Season::Aut;
			break;
		case 8:
		case 7:
		case 6:
			*s=Type_Season::Sum;
			break;
		case 5:
		case 4:
		case 3:
			*s=Type_Season::Spr;
			break;
		}
	}
}
//增加消息，b为真时是普通消息
void Message::msg_add(std::string s,bool b)
{
	msg_ctime.lock();
	int times = (msg_time + 2) % 12;
	msg_ctime.unlock();	
	if (times == 0)
	{
		times=12;
	}
	MsgNode* p=new MsgNode(s,times);//创建消息节点
	if(b)
	{
		msg_common.lock();
		msg_normalmsg.push_back(p);//在消息队列中添加队列
		if(msg_normalmsg.size()>6)//如果队列的消息过多
		{
			msg_normalmsg.pop_front();//弹出队首的消息
		}
		msg_common.unlock();
		msg_bool.lock();
		msg_remsg=true;
		msg_bool.unlock();
	}
	else
	{
		msg_waring.lock();
		msg_warningmsg.push_back(p);//在消息队列中添加消息
		if(msg_warningmsg.size()>6)
		{
			msg_warningmsg.pop_back();//弹出队首消息
		}
		msg_waring.unlock();
		msg_bool.lock();
		msg_remsg=true;
		msg_bool.unlock();
	}
}
//删除过期的消息
void Message::msg_pop()
{
	//检查普通消息队列
	msg_common.lock();
	if(!msg_normalmsg.empty())//消息队列不为空
	{
		do
		{
			MsgNode *p=msg_normalmsg.front();
			msg_ctime.lock();
			if(p->msg_lifetime==msg_time)
			{
				msg_normalmsg.pop_front();//过期消息出队
				delete p;//删除掉这个消息节点
			}
			else
			{
				msg_ctime.unlock();
				break;
			}
			msg_ctime.unlock();
		}while(!msg_normalmsg.empty());
	}
	msg_common.unlock();
	//检查警告消息队列
	msg_waring.lock();
	if(!msg_warningmsg.empty())//消息队列不为空
	{
		do
		{
			MsgNode *p=msg_warningmsg.front();
			msg_ctime.lock();
			if(p->msg_lifetime==msg_time)
			{
				msg_warningmsg.pop_front();//过期消息出队
				delete p;//删除掉这个消息节点
			}
			else
			{
				msg_ctime.unlock();
				break;
			}
			msg_ctime.unlock();
		}while(!msg_warningmsg.empty());
	}
	msg_waring.unlock();
	//检查出售请求队列
	msg_ctime.lock();
	int year(msg_time);
	int mon(msg_year);
	msg_ctime.unlock();
	msg_common.lock();
	if(!msg_dealask.empty())
	{
		std::vector<MsgSellNode>::iterator iter(msg_dealask.begin());
		do
		{
			iter=msg_dealask.begin();
			if((*iter).msg_ddl_year*100+(*iter).msg_ddl_mon==year*100+mon)
			{
				msg_dealask.erase(iter);//队首出列
			}
			else
			{
				break;//直接退出循环
			}
		} while (!msg_dealask.empty());//只要还有元素，则继续
	}
	msg_common.unlock();
}
//向进出口哈希表中添加项/更新项
void Message::msg_pushdeal(const int& id,int in,int out)
{
	msg_common.lock();
	if(msg_io.find(id)==msg_io.end())//如果没有节点，则建立一个节点
	{
		MsgSellreNode p(in,out);
		msg_io[id]=p;
	}
	else//有节点，则直接修改现在节点的值
	{
		msg_io[id].msg_in+=in;
		msg_io[id].msg_out+=out;	
	}
	msg_common.unlock();
}
//从进出口表中删除项
void Message::msg_deletedeal(const int& id)
{
	std::unordered_map<int ,MsgSellreNode>::iterator iter;
	iter=msg_io.find(id);
	msg_io.erase(iter);
}
//向出售请求队列中添加消息
void Message::msg_pushdealask(int id,int sum,int mon,int year)
{
	msg_common.lock();
	bool isinsert(false);// 判断是否插入成功
	MsgSellNode m(id,sum,mon,year);
	if(msg_dealask.empty())//如果为空，则直接添加
	{
		msg_dealask.push_back(m);
	}
	else//如果不为空，则需要比较
	{
		//遍历整个链表
		for(std::vector<MsgSellNode>::iterator iter=msg_dealask.begin();iter!=msg_dealask.end();++iter)
		{
			if(((*iter).msg_ddl_year*100+(*iter).msg_ddl_mon)>(mon+year*100))
			{
				msg_dealask.insert(iter,m);//插入到合适位置
				isinsert = true;
				break;
			}
		}
		if (!isinsert)
		{
			msg_dealask.push_back(m);//直接在队尾插入
		}
	}
	msg_common.unlock();
}
//从出售队列删除消息
void Message::msg_deletedealask(const int& i)
{
	msg_common.lock();
	std::vector<MsgSellNode>::iterator iter=msg_dealask.begin();
	for(int j(0);j<i;j++)
		++iter;
	msg_dealask.erase(iter);//删除这条信息
	msg_common.unlock();
}
//获取商品总数
int Message::msg_getsum()
{
	int i;
	msg_common.lock();
	i=msg_sum;
	msg_common.unlock();
	return i;
}
//插入操作
void Message_queue::Insert(const MsgNode& m)
{
	Msq_m.lock();	
	if(end==head)
		change(head);
	dataqueue[end]=m;
	change(end);

	Msq_m.unlock();
}	
//修改head和end的位置
void Message_queue::change(int &i)
{
	++i;
	if(i==6)
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
	MsgNode j=dataqueue[(head+i)%6];
	Msq_m.unlock();
	return j;
}
