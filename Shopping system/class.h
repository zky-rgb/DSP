#include<thread>
#include<mutex>
#include<string>
#include<windows.h>
#include<list>
#include<unordered_map>
#include<vector>

class UI;
class Message;
class BTree;
class Handle;
struct BNode;
struct Triple;


//季节枚举类
enum class Type_Season { Spr, Sum, Aut, Win };
//标签类
enum class Type_Lable { Food, Clothe, Electrical };

//时间类
class Time
{
public:
    void Time_clock();//计时器
    Time(Message*m,UI* ui) :T_m(m),T_ui(ui){};//构造函数
private:
    Message* T_m;//消息类指针
    UI* T_ui;//UI类指针
};
//商品结构体
struct  Commondity
{
    std::string c_maufacter;//厂商
    int c_date_year;//过期年份
    int c_date_year_in;//进货年份
    int c_date_m;//过期月份
    int c_date_m_in;//进货日期
    int c_sum;//该批商品的数量
    Commondity *next;//后一个节点的指针
    Commondity(std::string mau,int year,int in_year,int mon,int in_mon,int sum):
    c_maufacter(mau),c_date_year(year),c_date_year_in(in_year),c_date_m(mon),
    c_date_m_in(in_mon),c_sum(sum),next(NULL){}//构造函数
    Commondity(){}
};
//仓库链表类
class Wh_Chain
{
public:
    friend BTree;
    Wh_Chain(const int &i,const std::string &str="",Type_Lable t=Type_Lable::Food,int p_in=0,int p_out=0,Type_Season*s=NULL);
    Wh_Chain();
    ~Wh_Chain();//析构函数
    std::string WhC_ReInf();//将节点的信息打印
    std::string WhC_NodeInf(Commondity *ptr);//将结点中的数据节点信息,ptr为指向该节点的指针
    void WhC_insert(Commondity* data);//插入数据
    void WhC_delete(int i);//删除数据
    int WhC_ReSum(int i);//返回数量,i为其所在位置
    void WhC_Update(int s);//s为减少的值
    Type_Lable WhC_Rekind()//返回产品的种类
    {
        return WhC_type;
    }
    bool WhC_ReDate(const int& year,const int & day);//判断是否有产品即将过期
    Type_Season WhC_ReSeason()//返回商品合适的季节
    {
        return WhC_season;
    }
    std::string WhC_ReName()
    {
        return WhC_name;
    }
    int WhC_REID()//返回id;
    {
        return WhC_id;
    }
    Commondity * WhC_RePtr(const int &i);//返回对应位置的节点
    Commondity* WhC_ReEnd()//将尾指针返回
    {
        return WhC_end;
    }
    Commondity * WhC_ReChain()//将链表返回
    {
        return WhC_head;
    }
    int WhC_len()//查询表长
    {
        return WhC_length;
    }
    bool WhC_ifempty()//是否为空
    {
        return (WhC_length==0)?true:false;
    }
    bool WhC_ifup()//判断商品是否涨价
    {
        return ifup;
    }
    void WhC_turnup(bool i)//切回商品的涨价的情况
    {
        ifup = i;
    }
private:
    int WhC_id;//产品的iD
    std::string WhC_name;//产品的名称    
    Type_Lable WhC_type;//产品的种类
    int WhC_price_in;//进价
    int WhC_price_get;//利润
    int WhC_length;//链表长度
    int WhC_sum;//产品的总数
    bool ifup;//商品是否有涨价过
    Type_Season WhC_season;//产品适合的季节
    Commondity* WhC_head;//头指针
    Commondity* WhC_end;//尾指针
};
//定义搜索三元组
struct  Triple
{
    BNode* r;//所在节点
    int id;//结点中的关键码序号i
    int tag;//tag=0,搜索不成功，tag=1，搜索成功
    Triple() :r(NULL), id(1), tag(0) {}
};
//B树节点结构体
struct BNode
{
    int par_id;//其在父结点上的第几个空
    int n;//关键码个数
    BNode* parent;//父节点指针
    BNode* kids_ptr[4];//子树节点指针
    Wh_Chain* P_data[3];//监视单元所对应的节点
    BNode(BNode* p, Wh_Chain* Wp = NULL) :parent(p)//构造函数
    {
        for (int i = 0; i < 3; ++i)
        {
            P_data[i] = NULL;
            kids_ptr[i] = NULL;
        }
        par_id = 0;
        kids_ptr[3] = NULL;
        n = 0;
        P_data[0] = Wp;
    }
};
//B树仓库
class BTree
{
public:
    friend UI;
    friend Message;
    BTree(Message *m);//构造函数
    ~BTree();//析构函数
    void Wh_BTreeInsert(const int &i,const std::string &str,
    Type_Lable t,int p_in,int p_out,Type_Season*p=NULL);//在Btree中插入新的种类的商品,参数传入data
    //先生成数据保存的数据节点，然后查询
    void Wh_BTreeInsertKey(BNode* p, int j, Wh_Chain* p_data, BNode* rp);//插入关键码
    void Wh_BTreeRemoveKey(BNode* p, int j, bool kids = true);
    bool Wh_BTreeRemove(const int& d);//在Btree中移除商品
    Triple Wh_BTreeSearch(const Wh_Chain& x);//搜索
    void Wh_Changeprice(Wh_Chain *p,const int i);//修改地址为p的节点的价格
    bool Wh_UpdateNode(int id,int sum);//将编号为id的商品的数量减少
    void Wh_InsertdataNode(int id,std::string mau,int year,int in_year,int mon,int in_mon,int sum);//将单个商品添加到仓库中
    bool Wh_deletedataNode(int id,int i);//删除某一特定的商品，i为要删除的位置
    int Wh_profit(const int &id);//返回对应商品所对应的利润
    int Wh_getid(const int & i);//随机获取id 
    BNode* Wh_reroot()//返回根节点
    {
        BNode* p;
        Wh_C.lock();
        p = root;
        Wh_C.unlock();
        return p;
    }   
    std::mutex Wh_C;//仓库锁，用来保护仓库的数据被同时访问
private:

    Message *Wh_msg;//消息类指针
    BNode* root;//根节点指针
    int B_n;//节点总数
    int B_m;//度数
    std::vector<int> Wh_idlist;//用来保存id
};
//UI界面类
class UI
{
public:
    UI(Message* m,BTree *wh);
    ~UI();
    void UI_WelCome();//欢迎界面
    void UI_Clearview();//操作界面刷新
    void UI_Menu();//菜单界面
    int UI_MenuControl();//菜单栏获取,返回页面数
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
//消息节点
struct MsgNode
{
    std::string msg_data;//存储要打印的消息
    int msg_lifetime;//存储消息的到期时间
    MsgNode(std::string s,int t):msg_data(s),msg_lifetime(t){}
};
//出售处理结构体
struct MsgSellNode
{
    int msg_id;//出售的产品编号
    int msg_sum;//出售的数量
    int msg_ddl_mon;//请求到期时间月份
    int msg_ddl_year;//请求到期时间年份
    MsgSellNode(int id,int sum,int mon,int year):msg_id(id),msg_sum(sum),
    msg_ddl_mon(mon),msg_ddl_year(year){}
};
//销售记录节点
struct MsgSellreNode
{
    int msg_in;//进货的数量
    int msg_out;//出售的数量
    MsgSellreNode(const int  in=0,const int out=0):msg_in(in),msg_out(out){}
};
//消息类
class Message
{
public:
    friend UI;
    friend Handle;
    friend Time;
    Message();//构造函数
    ~Message();//析构函数
    void msg_increasetime();//递增时间
    void msg_gettime(int &mon,int&year,Type_Season *s=NULL);//获取时间
    void msg_add(std::string s,bool b=true);//加入消息,b为true时为普通消息队列
    void msg_pop();//检测消息队列，进行更新
    void msg_pushdeal(const int& id,int in=0,int out=0);//向进出口哈希表中添加项/更新项
    void msg_deletedeal(const int&id);//从进出口表中删除项
    void msg_pushdealask(int id,int sum,int mon,int year);//向出售请求队列中添加消息
    void msg_deletedealask(const int &i);//从出售队列删除消息
    void msg_updatasum(bool i)//修改商品总数，true为递增，false为递减
    {
        msg_common.lock();
        if (i)
        {
            ++msg_sum;
        }
        else
        {
            --msg_sum;
        }
        msg_common.unlock();
    }
    void msg_clearIO()//清空进出口表
    {
        msg_common.lock();
        msg_io.clear();//清空进出口表
        msg_common.unlock();
    }
    int msg_getsum();//获取商品总数
    bool msg_ifretime()//是否刷消息列表
    {
        msg_bool.lock();
        bool a=msg_remsg;
        msg_bool.unlock();
        return a;
    }
    bool msg_quit()//程序是否退出
    {
        msg_bool.lock();
        bool i=msg_uiquit;
        msg_bool.unlock();
        return i;
    }
    void msg_gethandle(Handle *h)//获取处理类对象的地址
    {
        msg_handle=h;
    }
    std::mutex msg_waring;//警报消息互斥量
    std::mutex msg_common;//普通消息互斥量
    std::mutex msg_bool;//刷新警告互斥量
    std::mutex msg_ctime;//时间消息互斥量
private:
    int msg_year;//存储年份
    int msg_time;//储存月份
    Type_Season msg_season;//存储的季节
    std::list<MsgNode*> msg_normalmsg;//普通消息队列
    std::list<MsgNode*> msg_warningmsg;//警告消息队列
    std::unordered_map<int,MsgSellreNode> msg_io;//进出口哈希表
    std::vector<MsgSellNode>msg_dealask;//出售请求
    int msg_sum;//商品总数
    bool msg_uiquit;//是否关闭UI界面
    bool msg_remsg;//是否刷新消息栏
    Handle* msg_handle;//是否需要处理消息
};
//处理消息类
class Handle
{
    public:
    Handle(Message *m,BTree *bt):h_msg(m),h_btree(bt){};
    void H_createsell();//生成交易信息
    void H_handlemsg();//处理消息队列
    void H_handleOverdue();//处理过期
    private:
    Message* h_msg;//指向消息对象的指针
    BTree* h_btree;//指向B树仓库的指针
};