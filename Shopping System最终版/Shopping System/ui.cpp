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
	CursorInfo.bVisible = true; //将光标设置为可见
	SetConsoleCursorInfo(handle, &CursorInfo);
	for(int i(0);i<n;++i)
	{
		UI_Setcursor(x[i],y[i]);

		std::cin>>key[i];

	}
	CursorInfo.bVisible = false; //将光标设置为可见
	SetConsoleCursorInfo(handle, &CursorInfo);
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
	GetConsoleScreenBufferInfo(handle, &csbi);
	int x, y;//分别保存x,y的值
	x = csbi.dwCursorPosition.X;//获取x的值
	y = csbi.dwCursorPosition.Y;//获取y的值
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
	UI_Setcursor(x,y);//设置光标位置
	UI_C.unlock();
}
//清空界面
void UI::UI_clear(int i,const int begin,const int end)
{
	int line(begin);	
	GetConsoleScreenBufferInfo(handle, &csbi);
	int x, y;//分别保存x,y的值
	x = csbi.dwCursorPosition.X;//获取x的值
	y = csbi.dwCursorPosition.Y;//获取y的值
	//初始化开始的行数
	switch (i)
	{
	case MAINVIEW://调整到主界面
		line+=0;
		break;
	case MSGBAR://调整到消息栏
		line+=15;
		break;
	case TIMETABLE://调整时间栏
		line+=15+8;
		break;
	}
	UI_C.lock();
	UI_Setcursor(0,line);
	for(int j(begin);j<end+1;++j)
	{
		std::cout<<"                                                                                                                "<<std::endl;
	}
	UI_Setcursor(x, y);//设置光标位置
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
	int Light=-1;
	PView p(MAINVIEW,0,9,s,&Light);
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
	PView p(MAINVIEW,begin,end,s,light);
	UI_Print(p,begin);
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
//提示框界面,begin为开始行数
bool UI::UI_ToolTip(std::string str[], const int n, const int begin)
{
	UI_clear(MAINVIEW, begin, begin+n-1);//清空
	int lightline(-1);
	//打印提示框
	PView view(MAINVIEW, 0, n - 1, str, &lightline);
	UI_Print(view, begin);
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
//更新价格
float UI::UI_Changepice()
{
	int lightline = -1;//没有高亮
	std::string str[4] = {
		"----------------------------------------",
		"please input the new profit:",
		"----------------------------------------"
	};
	PView view(MAINVIEW, 0, 2, str, &lightline);
	UI_clear(MAINVIEW, 4, 4 + 3 - 1);//清空界面
	UI_Print(view, 4);
	std::string n = "";
	int x(29), y(5);
	UI_get(&n, &x, &y, 1);

	str[0] = "----------------------------------------------";
	str[1] = "Are you sure to change the  profit";
	str[2] = "Enter(yes)                              c(cancel)";
	str[3] = "----------------------------------------------";
	bool choice = UI_ToolTip(str, 4, 4);
	if (choice)
	{
		//确定更改
		float np;
		turn_string(n, np);
		return np;
	}
	else
		return -1;
}
//商品购买界面
void UI::UI_Purchase()
{
	using namespace std;
	
	string str[7] = {
		"--------------------------------",
		"Purcharse",
		"  Sum:",
		"  Manufacturer:",
		"  Shelf Life(year):",
		"  Shelf Life(mon):",
		"--------------------------------"
	};
	int lightline = -1;
	PView view(MAINVIEW, 0, 6, str, &lightline);
	UI_clear(MAINVIEW,4,4+7-1);//清空界面
	UI_Print(view, 4);
}
