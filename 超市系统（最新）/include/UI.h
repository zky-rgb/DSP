#ifndef UIC
#define UIC

#include<thread>
#include<windows.h>
#include<mutex>

#define PAGEDOWN -1
#define PAGEUP -2
#define ENTER -3
#define PAGEKEEP 0

class Message;
class BTree;
class Wh_Chain;
class UI;
struct Triple;

//UI界面类
class UI
{
public:
    UI(Message* m,BTree *wh);
    ~UI();
    char UI_getinput(const int& high,const int& low,bool upper,bool lower,char key[],const int& n);//获取用户的输入信息
    void UI_Printing(const int& begin,const int& end,const int& pbegin,const int &pend);//主界面的刷新
    void UI_Printing(const int&line);//主界面的刷新，针对lightline的优化版本
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
    int UI_LightLine;//高亮的行数
    HANDLE handle;//程序句柄
	CONSOLE_CURSOR_INFO CursorInfo;//控制光标结构体
    COORD coord;//控制光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi;//缓冲区信息
    std::string UI_view[15];//用来显示主界面的二维数组
    Message* UI_m;//消息类指针
    BTree *UI_wh;//指向仓库的指针
    bool UI_reall;//是否刷新主界面
};
#endif UIC