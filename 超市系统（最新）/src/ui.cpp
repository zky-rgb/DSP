#include"UI.h"
#include<iostream>
#include<conio.h>
#include<stack>

//UI类的实现

//获取用户的输入信息,high为上限，low为下限
char UI::UI_get(char key[],const int& n)
{
	int c1;//存储输入
	do
	{
		c1=_getch();
		if(c1==224)//是方向键
		{
			int c2 =_getch();
			if(c2==80)//方向键下
				return LINEDOWN;
			else if(c2==72)//方向键上
				return LINEUP;
			else if(c2==75)
				return LIFT;
			else if(c2==77)
				return RIGHT;
		}
		else if(c1==13)//回车键
			return ENTER;
		else if(c1==9)//tab键
			return TAB;
		else
			for(int i(0);i<n;i++)
				if(c1==key[i])
					return c1;
	}while(1);
}
//获取用户的输入信息(string版本)
void UI::UI_get(std::string key[],int x[],int y[],const int&n)
{
	for(int i(0);i<n;++i)
	{
		UI_Setcursor(x[i],y[i]);
		std::cin>>key[i];
	}
}
//界面的刷新
void UI::UI_Print(PView pview,int begin)
{
	UI_C.lock();
	//初始化开始的行数
	switch (pview.inf)
	{
		case MAINVIEW://调整到主界面
			begin+=0;
			break;
		case MSGBAR://调整到消息栏
			begin+=15;
			break;
		case TIMETABLE://调整时间栏
			begin+=15+8;
			break;
	}
	UI_Setcursor(0,begin);//设置光标位置
	//打印
	for(int j(pview.begin),u(0);j<(pview.end+1);++j)
	{
		if(pview.Light[u]==j)
		{
			if(pview.inf==MAINVIEW)//显示黄色
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);//设置红色和绿色相加
			else if(pview.inf==MSGBAR)//显示红色	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
		std::cout<<pview.view[j]<<std::endl;
		if(pview.Light[u]==j)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
			++u;//递增
		}
	}
	UI_C.unlock();
}
//清空界面
void UI::UI_clear(INTERFACE i,const int begin,const int end)
{
	int line;
	//初始化开始的行数
	switch (i)
	{
	case MAINVIEW://调整到主界面
		line=0;
		break;
	case MSGBAR://调整到消息栏
		line=15;
		break;
	case TIMETABLE://调整时间栏
		line=15+8;
		break;
	}
	UI_C.lock();
	UI_Setcursor(0,line);
	for(int j(0);j<(end-begin+1);++j)
	{
		std::cout<<"                                                                                                "<<std::endl;
	}
	UI_C.unlock();
}
//欢迎界面
void UI::UI_WelCome()
{
	system("cls");//清屏
	std::string s[10];
	s[0] ="       *    ***    *     *****       **         ";
	s[1] ="       *   ** **   *    **   **      **         ";
	s[2] ="        * **   ** *     * ****       **         ";
	s[3] ="         **     **       *****       *****      ";
	s[4] ="                                                ";
	s[5] ="      *****     ****    **      **    *****     "; 
	s[6] ="     **        *    *   * *    * *   **   **    ";
	s[7] ="     **        *    *   *  *  *  *   * ****     ";
	s[8] ="      *****     ****    *   **   *    *****     ";
	s[9] ="                       2020                     ";
	int *Light;
	*Light=-1;
	PView p(MAINVIEW,0,9,s,Light);
	UI_Print(p,4);
	_getch();
}
//菜单界面
void UI::UI_Menu(const int begin,const int end,int *light)
{
	std::string s[6]={"MENU:",
						"  Show the commondiys",
						"  purchase the commondiy",
						"  sell list",
						"  daily record",
						"  quit"};
	PView p(MAINVIEW,begin,end,&s[begin],light);
	UI_Print(p,0);
}
//展示商品界面,sum为商品总量，p为当前所在的子页面,sum_p为总页数
int UI::UI_Search()
{
	std::string str[4]={
		"--------------------",
		"-Search-",
		"ID:",
		"--------------------"
	};
	UI_clear(MAINVIEW,5,8);//清空
	int lightline(-1);
	int x(4),y(7);
	PView view(MAINVIEW,0,3,str,&lightline);
	UI_Print(view,5);
	UI_get(str,&x,&y,1);//获取用户的输入
	turn_string(str[0],lightline);//将输入转换
	return lightline;
}
//提示框界面
bool UI::UI_ToolTip(std::string str[], const int n, const int begin)
{
	UI_clear(MAINVIEW, begin, n-1);//清空
	int lightline(-1);
	//打印提示框
	PView view(MAINVIEW, 0, n - 1, str, &lightline);
	char c('c');
	char gc;
	while (1)
	{
		//获取用户输入
		gc = UI_get(&c, 1);
		if (gc == c)
			return false;
		else if (gc == ENTER)
			return true;
		else
			continue;
	}
}
void UI_Daily()
{
	
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
