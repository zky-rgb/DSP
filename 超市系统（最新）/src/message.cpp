#include"Message.h"
#include"Warhouse.h"
#include"UI.h"
//消息类实现
//递增时间
void Message::msg_increasetime()
{
    msg_ctime.lock();
	++msg_time;
	if(msg_time==13)
	{
		msg_time=1;
		++msg_year;
	}	
	std::string s="Mon:"+to_string(msg_time);
	if(msg_time<10)
		s+=" ";
	s+=" Year:"+to_string(msg_year);
	msg_ctime.unlock();
	//更新时间栏
	PView *p=new PView(TIMETABLE,0,0,&s,NULL);
	msg_ui->UI_Print(p,1,0);//刷新
	delete p;
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
void Message::msg_add(std::string s,bool warn)
{
	msg_ctime.lock();
	MsgNode mnode(s,(msg_time + 1) % 12+1,warn);
	msg_ctime.unlock();	
	msg.Insert(mnode);//插入节点
	for(int i=0;i<8;i++){
	    s[i]=' ';
	}

	s[0]+= (char)' ';
	for(int i(1),size(msg.Size());i<size+1;++i)
	{
		
	}
	//更新消息栏
	msg_cui.lock();
	msg_ui->UI_clear(MSGBAR,0,7);//清空消息栏
	
	msg_cui.unlock();
}
//控制与用户交互
void Message::msg_control()
{
	int page(0);
	int lightline(-1);
	bool quit(false);
	while(!quit)
	{
		switch (page)
		{
		case 0:
			msg_ui->UI_WelCome();//欢迎界面
			++page;//页数递增
			break;
		case 1://主菜单
		{
			lightline=1;//设置高亮行
			msg_ui->UI_clear(MAINVIEW,0,14);//刷新面板
			msg_ui->UI_Menu(0,5,&lightline);//菜单栏初始化
			do
			{
				char c=msg_ui->UI_get(NULL,0);//捕获用户输入
				if(c==LINEDOWN)//行数向下
					if(lightline!=5)
					{
						++lightline;
						msg_ui->UI_Menu(lightline-1,lightline,&lightline);
					}
				else if(c==LINEUP)//行数向上
					if(lightline!=1)
					{
						--lightline;
						msg_ui->UI_Menu(lightline,lightline+1,&lightline);
					}
				else if(c==ENTER)
				{
					page=lightline+1;
					lightline=1;
					break;
				}
			} while (1);
			msg_ui->UI_clear(MAINVIEW,0,5);//刷新面板
			lightline=(page%2==0)?1:-1;
			break;
		}
		case 2://商品检查界面
		{	
			//预载所有商品信息
			msg_commondity();//加载商品检查界面
			page=1;
			break;
		}
		case 3://商品购买界面
		{
			msg_ui->UI_Menu_Purchase();
			page=1;
			break;
		}
		case 4://出售列表
		{
			msg_ui->UI_SellList();
			page=1;
			break;
		}
		case 5://显示日志
		{
			msg_ui->UI_Daily();
			page=1;
			break;
		}
		case 6://退出程序
		{
			quit=true;
			break;
		}
		}
	}
}
//弹出队首消息,成功则返回true,
bool Message::msg_pop()
{
	if(msg.Size()==0)
		return false;
	else
	{
		msg.Remove();
		return true;
	}
}
//获得消息队列队首的消息,如果失败，则返回false
bool Message::msg_front(MsgNode& node)
{
	if(msg.Size()==0)
		return false;
	else
	{
		node=msg.Search(0);
		return true;
	}
}
//对于
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
