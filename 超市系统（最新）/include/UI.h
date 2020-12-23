#ifndef UIC
#define UIC

#include<thread>
#include<windows.h>
#include<mutex>

#define LINEDOWN -1
#define LINEUP -2
#define ENTER -3
#define MAINVIEW 1
#define MSGBAR 2
#define TIMETABLE 3

typedef int INTERFACE;

struct PView//打印信息结结构体
{
    INTERFACE inf;//打印的页面是那个
    int begin;//开始行数，第一行为0
    int end;//结束行数
    std::string *view;//指向打印的界面的指针
    int *Light;//需要染色的行
    PView(INTERFACE i,int b,int e,std::string *v,int *l):inf(i),begin(b),end(e),view(v),Light(l){}
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
	    std::cout << "System Out" << std::endl;
    }
    char UI_get(char key[],const int& n);//获取用户的输入信息(char版本)
    void UI_get(std::string key[],int x[],int y[],const int&n);//获取用户的输入信息(string版本)
    void UI_Print(PView pview[],const int n);//界面打印
    void UI_Setcursor(const int& x,const int &y)
    {
        coord.X=x;
	    coord.Y=y;
	    SetConsoleCursorPosition(handle, coord);//设置光标位置
    }
    void UI_WelCome();//欢迎界面
    void UI_Menu();//菜单界面
    void UI_Msg();//消息弹窗
    void UI_ShowCommondiy(const int &sum,int &p,const int &sum_p);//打印仓库的物品
    void UI_ShowDetail(const int &sum, int &p,const  int &sum_p,Triple t);//展示详细信息
    int UI_SC_Control(const int& p);//打开仓库页面的控制
    int UI_SD_Control(const int& p, Wh_Chain* ptr);//对于详细信息界面的控制
    bool UI_Delete(Wh_Chain* ptr,const int i);//删除界面
    void UI_Changepice(Wh_Chain* ptr);//更新价格
    void UI_Update(const int &num);//对商品进行更新,传入商品的顺序
    void UI_SellList(const int &sum,int &p,const int &sum_p);//预出售列表
    int UI_SellControl(const int&p);//出售控制
    void UI_CheckSell(const int &num);//对于出售列表的展开,传入物品位于请求出售容器中的位置
    void UI_Purchase(Wh_Chain *p);//进购界面
    void UI_Menu_Purchase();//进购菜单
    void UI_Refresh();//刷新
    void UI_Refresh_Time();//时序刷新
    std::mutex UI_C;//防止同时刷新
private:
    HANDLE handle;//程序句柄    
    COORD coord;//控制光标位置
	CONSOLE_CURSOR_INFO CursorInfo;//控制光标结构体
	CONSOLE_SCREEN_BUFFER_INFO csbi;//缓冲区信息
    std::string UI_view[15];//用来显示主界面的二维数组
    bool UI_reall;//是否刷新主界面
};
#endif UIC
