#include"class.h"
#include<iostream>
#include<conio.h>
#include<stack>

//UI类的实现
//UI类构造函数
UI::UI(Message* m,BTree *wh):UI_m(m),UI_wh(wh),UI_LightLine(0)
{
    //初始化几个消息框
    UI_Clearview();
    handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取程序的句柄
	GetConsoleCursorInfo(handle, &CursorInfo);//获取光标信息
	CursorInfo.bVisible = false; //将光标设置为不可见
	SetConsoleCursorInfo(handle, &CursorInfo);
}
//UI类析构函数
UI::~UI()
{
	system("cls");
	std::cout << "System Out" << std::endl;
}
//操作界面刷新
void UI::UI_Clearview()
{
	using namespace std;
	UI_C.lock();
	coord.X=0;
	coord.Y=0;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
    for(int i(0);i<15;++i)
    {
		cout<<"                                                                                                                    "<<endl;	
    }
	coord.X=0;
	coord.Y=0;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	for(int i(0);i<15;++i)
	{
		if(i==UI_LightLine&&UI_LightLine>=0)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);//设置红色和绿色相加
			cout<<UI_view[i]<<endl;//逐行刷新
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
		}
		else
			cout<<UI_view[i]<<endl;//逐行刷新
	}
	UI_C.unlock();
}
//欢迎界面
void UI::UI_WelCome()
{
	UI_view[4] ="       *    ***    *     *****       **         ";
	UI_view[5] ="       *   ** **   *    **   **      **         ";
	UI_view[6] ="        * **   ** *     * ****       **         ";
	UI_view[7] ="         **     **       *****       *****      ";
	UI_view[8] ="                                                ";
	UI_view[9] ="      *****     ****    **      **    *****     "; 
	UI_view[10]="     **        *    *   * *    * *   **   **    ";
	UI_view[11]="     **        *    *   *  *  *  *   * ****     ";
	UI_view[12]="      *****     ****    *   **   *    *****     ";
	UI_view[13]="                  by ZKY 2020                   ";
}
//菜单界面
void UI::UI_Menu()
{
	UI_view[0] ="MENU:" ;
	UI_view[1] ="  Show the commondiys";
	UI_view[2] ="  purchase the commondiy";
	UI_view[3] ="  sell list";
	UI_view[4] ="  quit";
	for(int i=(5);i<15;++i)//其他的行清空
	{
		UI_view[i]="";
	}
}
//菜单的控制
int UI::UI_MenuControl()
{
	int c1,c2;//存储输入

	c1=_getch();
	if(c1==224)//是方向键
	{
		c2=_getch();
		if(c2==80)//方向键下
		{
			UI_LightLine++;//高亮行增加
		}
		else if(c2==72)//方向键上
		{
			UI_LightLine--;//高亮行减少
		}
		if(UI_LightLine==5)
		{
			UI_LightLine=1;
		}
		else if(UI_LightLine==0)
		{
			UI_LightLine=4;
		}
	}
	else if(c1==13)//如果是回车
	{
		switch (UI_LightLine)//检查高亮行
		{
		case 1:
			return 2;
		case 2:
			return 3;
		case 3:
			return 4;
		case 4:
			return 5;
		}		
	}
	return 1;//页数不改变
}
//信息栏的打印
void UI::UI_Msg()
{
	UI_C.lock();//防止同时刷新
	using namespace std;
	//获取缓冲区的光标信息
	GetConsoleScreenBufferInfo(handle, &csbi);
	int x,y;//分别保存x,y的值
	x=csbi.dwCursorPosition.X;//获取x的值
	y=csbi.dwCursorPosition.Y;//获取y的值
	//设置x,y的信息
	coord.X=0;
	coord.Y=16;
	//移动光标
	SetConsoleCursorPosition(handle, coord);
	for(int i(0);i<15;++i)//将区域刷新
		cout<<"                                                                 "<<endl;   
	//设置x,y的信息
	coord.X=0;
	coord.Y=16;
	//移动光标
	SetConsoleCursorPosition(handle, coord);
	cout<<"Message List:"<<endl;
	UI_m->msg_common.lock();
	if(UI_m->msg_normalmsg.size()>0)
	{
		for(list<MsgNode*>::iterator iter=UI_m->msg_normalmsg.begin();iter!=UI_m->msg_normalmsg.end();++iter)
		{
			cout<<(*iter)->msg_data<<endl;
		}
	}
	for(int i(0);i<6-UI_m->msg_normalmsg.size();++i)
	{
		cout<<endl;
	}
	UI_m->msg_common.unlock();
	cout<<"Warnning List:"<<endl;
	UI_m->msg_waring.lock();
	if(UI_m->msg_warningmsg.size()>0)
	{
		for(list<MsgNode*>::iterator iter=UI_m->msg_warningmsg.begin();iter!=UI_m->msg_warningmsg.end();++iter)
		{
			cout<<(*iter)->msg_data<<endl;
		}
	}
	for(int i(0);i<6-UI_m->msg_warningmsg.size();++i)
	{
		cout<<endl;
	}
	UI_m->msg_waring.unlock();
	UI_m->msg_ctime.lock();
	cout<<"Season:";
	switch (UI_m->msg_season)
	{
	case Type_Season::Spr:
		cout << "Spr";
		break;
	case Type_Season::Sum:
		cout << "Sum";
		break;
	case Type_Season::Aut:
		cout << "Aut";
		break;
	case Type_Season::Win:
		cout << "Win";
		break;
	}
	cout<<" Mon:"<<UI_m->msg_time<<"  ";
	cout<<"year:"<<UI_m->msg_year<<endl;
	UI_m->msg_ctime.unlock();
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(handle, coord);//将光标还会
	UI_C.unlock();
}
//用来检测刷新，刷新界面
void UI::UI_Refresh()
{
	using namespace std;
	int page(0);
	bool quit(true);
	while (!UI_m->msg_quit())//检查是否退出
	{
		switch (page)//选择打印的界面
		{
		case 0:
			UI_LightLine=-1;//不打印高亮
			UI_WelCome();//欢迎界面
			UI_Clearview();//刷新
			_getch();//等待用户输入
			page++;
			break;
		case 1:
			quit=false;
			UI_LightLine=1;//初始化第一行高亮
			do
			{	
				UI_Menu();//菜单界面
				UI_Clearview();//刷新
				page=UI_MenuControl();//获取
				if(page!=1)//页数变了
				{
					quit=true;//退出
				}
			}while(!quit);
			break;
		case 2:
		{
			int in_page(0);//当前所在的页数
			quit=false;
			UI_LightLine=1;//初始化第一行
			UI_wh->Wh_C.lock();
			int sum=UI_wh->Wh_idlist.size();//获得id的总数
			UI_wh->Wh_C.unlock();
			int sum_p;//总页数
			sum_p=sum/12;
			int judgepage;//如果为true，则回到menu页
			if((sum%12)!=0)
			{
				++sum_p;
			}
			do
			{
				UI_ShowCommondiy(sum,in_page,sum_p);//展示商品
				UI_Clearview();//刷新
				judgepage=UI_SC_Control(in_page);//获取控制
				if(judgepage==1)
				{
					page=1;
					quit=true;
				}
				else
				{
					if(judgepage==2)//重新初始化界面
					{
						in_page=0;//当前所在的页数
						quit=false;
						UI_LightLine=1;//初始化第一行
						UI_wh->Wh_C.lock();
						sum=UI_wh->Wh_idlist.size();//获得id的总数
						UI_wh->Wh_C.unlock();
						sum_p=sum/12;
						if((sum%12)!=0||sum==0)
						{
							++sum_p;
						}
					}
				}
			}while(!quit);
			break;
		}
		case 3:
			UI_Menu_Purchase();//购买界面
			page = 1;
			break;
		case 4:
		{
			int in_page(0);//当前所在的页数
			quit=false;
			UI_LightLine=1;//初始化第一行
			UI_m->msg_common.lock();
			int sum=UI_m->msg_dealask.size();
			UI_m->msg_common.unlock();
			int sum_p;//总页数
			sum_p=sum/12;
			int judgepage;//如果为true，则回到menu页
			if((sum%12)!=0||sum==0)
			{
				++sum_p;
			}
			do
			{
				UI_SellList(sum,in_page,sum_p);//出售列表
				UI_Clearview();//刷新
				judgepage=UI_SellControl(in_page);//获取控制
				if(judgepage==1)
				{
					page=1;
					quit=true;
				}
				else
				{
					if(judgepage==2)//重新初始化界面
					{
						in_page=0;//当前所在的页数
						quit=false;
						UI_LightLine=1;//初始化第一行
						UI_m->msg_common.lock();
						sum=UI_m->msg_dealask.size();
						UI_m->msg_common.unlock();
						sum_p=sum/12;
						if((sum%12)!=0)
						{
							++sum_p;
						}
					}
				}
			}while(!quit);
			break;	
		}
		case 5:
			UI_m->msg_bool.lock();
			UI_m->msg_uiquit=true;//退出程序
			UI_m->msg_bool.unlock();
			break;
		}
	}
}
//时序刷新
void UI::UI_Refresh_Time()
{
	if(UI_m->msg_ifretime())
	{
		UI_Msg();
	}	
}
//展示商品界面,sum为商品总量，p为当前所在的子页面,sum_p为总页数
void UI::UI_ShowCommondiy(const int &sum,int &p,const int &sum_p)
{
	using namespace std;
	int p_end;//目前页的行数
	if (p == sum_p - 1)
	{
		p_end = (sum - p * 12) % 12;
		if (p_end == 0&&sum!=0)
		{
			p_end = 12;
		}
	}
	else
	{
		p_end = 12;
	}
	if(UI_LightLine==0)//需要向上翻页
	{
		UI_LightLine=1;//重置到第一个
		if(p!=0)
		{
			--p;//页面减少
		}
	}
	else if(UI_LightLine==p_end+1)//需要向下翻页
	{
		if(p!=sum_p-1)//目前不是最后一页
		{
			++p;//页面增加
			UI_LightLine=1;
		}
		else
		{
			UI_LightLine=p_end;
		}
	}
	if (p == sum_p - 1)
	{
		p_end = (sum - p * 12) % 12;
		if (p_end == 0&&sum!=0)
		{
			p_end = 12;
		}
	}
	else
	{
		p_end = 12;
	}
	UI_view[0]="SHOWCOMMONDITY:";

	Triple t;
	for(int i(1);i<13;++i)
	{
		UI_wh->Wh_C.lock();
		if(p_end<i)
		{
			//将该行置空
			UI_view[i]="";
		}
		else
		{
			//搜索到id对应的节点
			Wh_Chain chain(UI_wh->Wh_idlist[i-1+p*12]);
			t=UI_wh->Wh_BTreeSearch(chain);
			//将节点的信息打印
			UI_view[i]=t.r->P_data[t.id]->WhC_ReInf();
		}
		UI_wh->Wh_C.unlock();
	}
	UI_view[13]="'B' to the Menu";
	UI_view[14]="'Enter' to Choise ";
}
//商品界面的控制，如果返回menu，则返回true
int UI::UI_SC_Control(const int &p)
{
	int c1,c2;//存储输入

	c1=_getch();
	if(c1==224)//是方向键
	{
		c2=_getch();
		if(c2==80)//方向键下
		{
			UI_LightLine++;//高亮行增加
		}
		else if(c2==72)//方向键上
		{
			UI_LightLine--;//高亮行减少
		}
		return 0;
	}
	else if(c1==13)//如果是回车
	{
		UI_Update(UI_LightLine+p*12-1);//对该特定项的展开   
		return 2;//依然在目前界面
	}
	else if(c1==66||c1==98)//如果用户按了b或B
	{
		return 1;//返回true，返回menu界面
	}
	else
	{
		return 0;
	}
}
//对商品进行更新,传入商品的顺序
void UI::UI_Update(const int &num)
{
	//访问仓库
	Triple t;	
	int page(0);//当前所在页

	UI_wh->Wh_C.lock();
	Wh_Chain x(UI_wh->Wh_idlist[num]);
	t=UI_wh->Wh_BTreeSearch(x);//搜索该结点
	int sum=t.r->P_data[t.id]->WhC_len();//获取表长	
	Commondity *ptr=t.r->P_data[t.id]->WhC_ReChain();	
	UI_wh->Wh_C.unlock();

	int pagechoice;
	int sum_p=sum/10;
	if((sum%10)!=0)
	{
		++sum_p;
	}
	UI_LightLine=1;//初始化光亮
	bool quit(false);
	do
	{
		UI_ShowDetail(sum,page,sum_p,t);
		UI_Clearview();//打印界面
		pagechoice=UI_SD_Control(page,t.r->P_data[t.id]);
		if(pagechoice==1)//回到前一页
		{
			page=2;
			quit=true;
		}
		else
		{
			UI_wh->Wh_C.lock();
			Wh_Chain x(UI_wh->Wh_idlist[num]);
			sum=t.r->P_data[t.id]->WhC_len();//获取表长	
			Commondity *ptr=t.r->P_data[t.id]->WhC_ReChain();	
			UI_wh->Wh_C.unlock();
			page=0;
			sum_p=sum/10;
			if((sum%10)!=0)
			{
				++sum_p;
			}
			if(pagechoice!=2)
				UI_LightLine=1;//初始化光亮
		}
	} while (!quit);
}
//展示详细信息
void UI::UI_ShowDetail(const int &sum, int &p,const  int &sum_p,Triple t)
{
	using namespace std;
	Commondity *ptr;
	int p_end;//目前页的行数
	if (p ==sum_p - 1)
	{
		p_end = (sum - p * 10) % 10;
		if (p_end == 0)
		{
			p_end = 10;
		}
	}
	else
	{
		p_end = 10;
	}
	if(UI_LightLine==0)//需要向上翻页
	{
		UI_LightLine=1;//重置到第一个
		if(p!=0)
		{
			--p;//页面减少
		}
	}
	else if(UI_LightLine==p_end+1)//需要向下翻页
	{
		if(p!=sum_p-1)//目前不是最后一页
		{
			++p;//页面增加
			UI_LightLine=1;
		}
		else
		{
			UI_LightLine=p_end;
		}
	}
	if (p ==sum_p - 1)
	{
		p_end = (sum - p * 10) % 10;
		if (p_end == 0)
		{
			p_end = 10;
		}
	}
	else
	{
		p_end = 10;
	}
	UI_wh->Wh_C.lock();
	ptr=t.r->P_data[t.id]->WhC_RePtr(p*10);
	UI_view[0]=t.r->P_data[t.id]->WhC_ReInf();
	Commondity * pt= t.r->P_data[t.id]->WhC_ReEnd();
	UI_wh->Wh_C.unlock();

	for(int i(1);i<11;++i)
	{
		UI_wh->Wh_C.lock();
		if(ptr!=NULL&&(ptr==pt||ptr->next != NULL))
		{
			UI_view[i]=t.r->P_data[t.id]->WhC_NodeInf(ptr);//将节点信息打印
			ptr=ptr->next;
		}
		else
		{
			UI_view[i]="";//否则置空
		}
		UI_wh->Wh_C.unlock();
	}
	UI_view[11]="C to Change the profit";
	UI_view[12]="U to Buy";
	UI_view[13]="'B' to Back";
	UI_view[14]="'Enter' to Choise ";
}
//详细信息界面的控制，如果要返回展示商品界面，则返回true
int UI::UI_SD_Control(const int &p,Wh_Chain *ptr)
{
	int c1,c2;//存储输入

	c1=_getch();
	if(c1==224)//是方向键
	{
		c2=_getch();
		if(c2==80)//方向键下
		{
			UI_LightLine++;//高亮行增加
		}
		else if(c2==72)//方向键上
		{
			UI_LightLine--;//高亮行减少
		}
		return 2;
	}
	else if(c1==13)//如果是回车
	{
		bool j;
		j=UI_Delete(ptr,p*10+UI_LightLine-1);//对该特定项的展开   
		if (!j)
			return 1;
		else
			return 0;//依然在目前界面
	}
	else if(c1==66||c1==98)//如果用户按了b或B
	{
		return 1;//返回true，返回menu界面
	}
	else if(c1==99||c1==67)//如果用户按了c或C
	{
		UI_Changepice(ptr);//进入修改价格界面
		return 0;//依然在目前界面
	}
	else if(c1==117||c1==85)//如果用户按了U或者u
	{
		UI_Purchase(ptr);//进入购买界面
		return 0;//依然在目前界面
	}
	else
	{
		return 0;
	}
}
//删除节点
bool UI::UI_Delete( Wh_Chain* ptr,const int i)
{
	using namespace std;
	
	//搜索节点的信息
	UI_wh->Wh_C.lock();
	Commondity* p = ptr->WhC_RePtr(i);
	string inf = ptr->WhC_NodeInf(p);
	UI_wh->Wh_C.unlock();

	UI_LightLine = -1;//没有高亮
	UI_view[5] = "----------------------------------------";
	UI_view[6] = "";
	UI_view[7] = inf;
	UI_view[8] = "";
	UI_view[9] = "Do you want delete this Node?";
	UI_view[10] = "Y(yes)                         C(cancel)";
	UI_view[11]= "----------------------------------------";
	UI_Clearview();//打印界面
	char c;
	do
	{
		c = _getch();
		if (c == 'y' || c == 'Y')
		{
			UI_wh->Wh_C.lock();
			int id = ptr->WhC_REID();
			bool judge;
			judge=UI_wh->Wh_deletedataNode(id, i);//删除节点信息
			UI_wh->Wh_C.unlock();
			if (!judge)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (c == 'c' || c == 'C')
		{
			break;//直接退出循环
		}
		else
		{
			continue;
		}
	} while (1);
	return true;
}
//更新价格
void UI::UI_Changepice(Wh_Chain *ptr)
{
	UI_LightLine = -1;//没有高亮
	UI_view[5] = "----------------------------------------";
	UI_view[6] = "";
	UI_view[7] = "please input the new profit:";	
	UI_view[8] = "";
	UI_view[9] = "----------------------------------------";
	UI_Clearview();//打印界面
	coord.X=27;
	coord.Y=7;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	CursorInfo.bVisible = true; //将光标设置为可见
	SetConsoleCursorInfo(handle, &CursorInfo);
	int profit;
	std::cin>>profit;
	CursorInfo.bVisible = false; //将光标设置为不可见
	SetConsoleCursorInfo(handle, &CursorInfo);
	UI_LightLine = -1;//没有高亮
	UI_view[5] = "----------------------------------------------";
	UI_view[6] = "Are you sure to set this new price("+std::to_string(profit)+")";
	UI_view[7] = "";	
	UI_view[8] = "Y(yes)                                         C(cancel)";
	UI_view[9] = "----------------------------------------------";
	UI_Clearview();//打印界面
	char c;
	do
	{
		c = _getch();
		if (c == 'y' || c == 'Y')
		{
			UI_wh->Wh_Changeprice(ptr, profit);//删除节点信息
			break;//退出循环
		}
		else if (c == 'c' || c == 'C')
		{
			break;//直接退出循环
		}
		else
		{
			continue;
		}
	} while (1);
}
//销售列表
void UI::UI_SellList(const int &sum,int &p,const int &sum_p)
{
	using namespace std;
	int p_end;//目前页的行数
	if (p == sum_p - 1)
	{
		p_end = (sum - p * 12) % 12;
		if (p_end == 0&&sum!=0)
		{
			p_end = 12;
		}
		else if (sum == 0)
		{
			p_end = 0;
		}
	}
	else
	{
		p_end = 12;
	}
	if(UI_LightLine==0)//需要向上翻页
	{
		UI_LightLine=1;//重置到第一个
		if(p!=0)
		{
			--p;//页面减少
		}
	}
	else if(UI_LightLine==p_end+1)//需要向下翻页
	{
		if(p!=sum_p-1)//目前不是最后一页
		{
			++p;//页面增加
			UI_LightLine=1;
		}
		else
		{
			UI_LightLine=p_end;
		}
	}
	if (p == sum_p - 1)
	{
		p_end = (sum - p * 12) % 12;
		if (p_end == 0 && sum != 0)
		{
			p_end = 12;
		}
		else if (sum == 0)
		{
			p_end = 0;
		}
	}
	else
	{
		p_end = 12;
	}
	UI_view[0]="SELL LIST:";

	for(int i(1);i<13;++i)
	{
		if(p_end<i)
		{
			//将该行置空
			UI_view[i]="";
		}
		else
		{
			string str="";
			UI_m->msg_common.lock();
			str="ID:"+to_string(UI_m->msg_dealask[i+p*12-1].msg_id)+" sum:"+to_string(UI_m->msg_dealask[i+p*12-1].msg_sum);
			str+=" DDL:"+to_string(UI_m->msg_dealask[i+p*12-1].msg_ddl_mon)+" "+to_string(UI_m->msg_dealask[i+p*12-1].msg_ddl_year);
			UI_m->msg_common.unlock();
			UI_view[i]=str;
		}
	}
	UI_view[13]="'B' to the Menu";
	UI_view[14]="'Enter' to Choise ";
}
//出售控制
int UI::UI_SellControl(const int&p)
{
	int c1,c2;//存储输入
	
	c1=_getch();
	if(c1==224)//是方向键
	{
		c2=_getch();
		if(c2==80)//方向键下
		{
			UI_LightLine++;//高亮行增加
		}
		else if(c2==72)//方向键上
		{
			UI_LightLine--;//高亮行减少
		}
		return 0;
	}
	else if(c1==13)//如果是回车
	{
		UI_CheckSell(UI_LightLine+p*12-1);//对该特定项的展开   
		return 2;//依然在目前界面
	}
	else if(c1==66||c1==98)//如果用户按了b或B
	{
		return 1;//返回true，返回menu界面
	}
	else
	{
		return 0;
	}
}
//对于出售列表的展开,传入物品位于请求出售容器中的位置
void UI::UI_CheckSell(const int &num)
{
	using namespace std;
	UI_m->msg_common.lock();
	int id=UI_m->msg_dealask[num].msg_id;
	int sum=UI_m->msg_dealask[num].msg_sum;
	UI_m->msg_common.unlock();
	
	Wh_Chain ch(id);
	Triple t;
	UI_wh->Wh_C.lock();
	t=UI_wh->Wh_BTreeSearch(ch);
	string name=t.r->P_data[t.id]->WhC_ReName();
	UI_wh->Wh_C.unlock();
	
	UI_view[4]="--------------------------";
	UI_view[5]="ID :"+to_string(id);
	UI_view[6]="NAME:"+name;
	UI_view[7]="SUM:"+to_string(sum);
	UI_view[8]="Would you want to sell it?";
	UI_view[9]="Y(yes)           C(Cancel)";
	UI_view[10]="--------------------------";
	UI_Clearview();//打印界面
	char c;
	do
	{
		c = _getch();
		if (c == 'y' || c == 'Y')
		{
			UI_wh->Wh_C.lock();
			UI_wh->Wh_UpdateNode(id,sum);//更新节点
			UI_m->msg_deletedealask(num);
			UI_wh->Wh_C.unlock();
			break;//退出循环
		}
		else if (c == 'c' || c == 'C')
		{
			break;//直接退出循环
		}
		else
		{
			continue;
		}
	} while (1);
}
//商品购买界面
void UI::UI_Purchase(Wh_Chain *p)
{
	using namespace std;

	int id;
	std::string name;
	UI_wh->Wh_C.lock();
	id=p->WhC_REID();
	name=p->WhC_ReName();
	UI_wh->Wh_C.unlock();
	
	UI_LightLine=-1;//不打印高亮行
	UI_view[3]="--------------------------------";
	UI_view[4]="Purcharse";
	UI_view[5]="  ID:"+to_string(id);
	UI_view[6]="  Name:"+name;
	UI_view[7]="  Sum:";
	UI_view[8]="  Manufacturer:";
	UI_view[9]="  Shelf Life(year):";
	UI_view[10]="  Shelf Life(mon):";
	UI_view[11]="--------------------------------";
	UI_Clearview();//打印界面

	CursorInfo.bVisible = true; //将光标设置为可见
	SetConsoleCursorInfo(handle, &CursorInfo);
	int sum,sl_year,sl_mon;
	string man;
	Type_Season season;

	UI_C.lock();
	coord.X=7;
	coord.Y=7;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	UI_C.unlock();
	cin>>sum;

	UI_C.lock();
	coord.X=16;
	coord.Y=8;
	SetConsoleCursorPosition(handle, coord);//设置光标位置	
	UI_C.unlock();
	cin.get();
	getline(cin,man);

	UI_C.lock();
	coord.X=20;
	coord.Y=9;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	UI_C.unlock();
	cin>>sl_year;
	
	UI_C.lock();
	coord.X=20;
	coord.Y=10;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	UI_C.unlock();
	cin>>sl_mon;

	CursorInfo.bVisible = false; //将光标设置为不可见
	SetConsoleCursorInfo(handle, &CursorInfo);

	int in_year,in_mon;
	
	UI_m->msg_ctime.lock();
	in_year=UI_m->msg_year;
	in_mon=UI_m->msg_time;
	UI_m->msg_ctime.unlock();
	
	UI_wh->Wh_C.lock();
	UI_wh->Wh_InsertdataNode(id,man,sl_year,in_year,sl_mon,in_mon,sum);
	UI_wh->Wh_C.unlock();

}
//进购菜单
void UI::UI_Menu_Purchase()
{
	using namespace std;
	
	UI_LightLine=-1;//没有高亮
	UI_view[0]="Purcharse:";
	UI_view[1]="  please input the id: ";
	for(int i(2);i<15;++i)
	{
		UI_view[i]="";//其他行置空
	}
	UI_Clearview();//打印界面
	
	int id;
	//读取用户输入的id
	CursorInfo.bVisible = true; //将光标设置为可见
	SetConsoleCursorInfo(handle, &CursorInfo);
	UI_C.lock();
	coord.X=23;
	coord.Y=1;
	SetConsoleCursorPosition(handle, coord);//设置光标位置
	UI_C.unlock();
	cin>>id;
	CursorInfo.bVisible = false; //将光标设置为不可见
	SetConsoleCursorInfo(handle, &CursorInfo);

	//在已有的列表中搜索id
	Wh_Chain node(id);
	Triple t;
	UI_wh->Wh_C.lock();
	t=UI_wh->Wh_BTreeSearch(node);//搜索
	UI_wh->Wh_C.unlock();
	

	if(t.tag==0)//没有该节点
	{

		Type_Lable Kind;
		UI_view[4]="  -----------------------------------";
		UI_view[5]="  This ID has not been insert";
		UI_view[6]="  Would you want to add this id?";
		UI_view[7]="  Y(yes)                   C(Cancel)";
		UI_view[8]="  -----------------------------------";
		UI_Clearview();//打印界面
		char c;
		do
		{
			c = _getch();
			if (c == 'y' || c == 'Y')
			{		
				int cost,profit;
				string name;
				char kind;
				Type_Season season;
				UI_view[4]="  -----------------------------------------";
				UI_view[5]="  please input the name:";
				UI_view[6]="  please input the cost:";
				UI_view[7]="  please input the profit:";
				UI_view[8]="  please input the kind(F/C/E):";
				UI_view[9]="  -----------------------------------------";
				UI_Clearview();//打印界面
				
				CursorInfo.bVisible = true; //将光标设置为可见
				SetConsoleCursorInfo(handle, &CursorInfo);

				UI_C.lock();
				coord.X=25;
				coord.Y=5;
				SetConsoleCursorPosition(handle, coord);//设置光标位置
				UI_C.unlock();
				cin>>name;

				UI_C.lock();
				coord.X=25;
				coord.Y=6;
				SetConsoleCursorPosition(handle, coord);//设置光标位置
				UI_C.unlock();
				cin>>cost;

				UI_C.lock();
				coord.X=27;
				coord.Y=7;
				SetConsoleCursorPosition(handle, coord);//设置光标位置
				UI_C.unlock();
				cin>>profit;

				UI_C.lock();
				coord.X=32;
				coord.Y=8;
				SetConsoleCursorPosition(handle, coord);//设置光标位置
				UI_C.unlock();
				cin>>kind;

				if(kind='c')
				{
					UI_view[5]="---------------------------------------------";
					UI_view[6]="Suit Season(1(spr),2(sum),3(aut),4(win)):";
					UI_view[7]="---------------------------------------------";
				}
				UI_Clearview();//打印界面

				switch(kind)
				{
					case 'f':
					case 'F':
						Kind=Type_Lable::Food;
						break;
					case 'c':
					case 'C':
						Kind=Type_Lable::Clothe;
						break;
					case 'e':
					case 'E':
						Kind=Type_Lable::Electrical;
						break;
				}
				if(Kind==Type_Lable::Clothe)
				{
					UI_C.lock();
					coord.X=42;
					coord.Y=6;
					SetConsoleCursorPosition(handle, coord);//设置光标位置
					UI_C.unlock();
					char c;
					cin>>c;
					switch(c)
					{
					case '1':
						season=Type_Season::Spr;
						break;
					case '2':
						season=Type_Season::Sum;
						break;
					case '3':
						season=Type_Season::Aut;
						break;
					case '4':
						season=Type_Season::Win;
						break;
					}
				}
				CursorInfo.bVisible = false; //将光标设置为不可见
				SetConsoleCursorInfo(handle, &CursorInfo);
				UI_wh->Wh_BTreeInsert(id,name,Kind,cost,profit,&season);
				UI_Purchase(t.r->P_data[t.id]);
				break;//退出循环
			}
			else if (c == 'c' || c == 'C')
			{
				break;//直接退出循环
			}
			else
			{
				continue;
			}
		} while (1);
	}
	else//有该节点
	{
		UI_view[4]="-------------------------------------------------";
		UI_view[5]="  This ID has been inserted";
		UI_view[6]="  Would you want add more Commondities?";
		UI_view[7]="      Y(yes)                            C(Cancel)";
		UI_view[8]="-------------------------------------------------";
		UI_Clearview();//打印界面
		char c;
		do
		{
			c = _getch();
			if (c == 'y' || c == 'Y')
			{
				UI_Purchase(t.r->P_data[t.id]);
				break;//退出循环
			}
			else if (c == 'c' || c == 'C')
			{
				break;//直接退出循环
			}
			else
			{
				continue;
			}
		} while (1);
	}
}
