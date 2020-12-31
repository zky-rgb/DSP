
#ifndef MUTEX
#define MUTEX
#include<mutex>
#endif // !MUTEX

#ifndef UIC
#define UIC

#include<thread>
#include<windows.h>
#include <sstream>

#define LINEDOWN -1
#define LINEUP -2
#define ENTER -3
#define LIFT -4
#define RIGHT -5
#define TAB -6
#define MAINVIEW 1
#define MSGBAR 2
#define TIMETABLE 3


struct PView//打印信息结结构体
{
    int inf;//打印的页面是那个
    int begin;//开始行数，第一行为0
    int end;//结束行数
    std::string *view;//指向打印的界面的指针
    int *Light;//需要染色的行
    PView(int i,int b,int e,std::string *v,int *l):inf(i),begin(b),end(e),view(v),Light(l){}
};
//UI界面类
class UI
{
public:
    UI()
    {
        //初始化几个消息框
        handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取程序的句柄
	    GetConsoleCursorInfo(handle, &CursorInfo);//获取光标信息
	    CursorInfo.bVisible = false; //将光标设置为不可见
	    SetConsoleCursorInfo(handle, &CursorInfo);
    }
    ~UI()
    {
	    system("cls");
    }
    char UI_get(char key[],const int& n);//获取用户的输入信息(char版本)
    void UI_get(std::string key[],int x[],int y[],const int&n);//获取用户的输入信息(string版本)
    void UI_Print(PView pview,const int begin);//界面打印
    void UI_Setcursor(const int& x,const int &y)
    {
        coord.X=x;
	    coord.Y=y;
	    SetConsoleCursorPosition(handle, coord);//设置光标位置
    }
    void UI_clear(int i,const int begin,const int end);//清空界面
    void UI_WelCome();//欢迎界面
    void UI_Menu(const int begin,const int end,int *light);//菜单界面
    int UI_Search();//搜索界面
    bool UI_ToolTip(std::string str[],const int n,const int begin);//提示框
    float UI_Changepice();//更新价格
    void  UI_Purchase();//进购界面
private:    
    std::mutex UI_C;//防止同时刷新
    HANDLE handle;//程序句柄    
    COORD coord;//控制光标位置
	CONSOLE_CURSOR_INFO CursorInfo;//控制光标结构体
	CONSOLE_SCREEN_BUFFER_INFO csbi;//缓冲区信息
};
template<typename T>
void turn_string(std::string str,T &i)
{
    std::stringstream S;
    S<<str;
    S>>i;
}
#endif 