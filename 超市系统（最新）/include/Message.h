
#ifndef MESSAGE
#define MESSAGE

#include<array>
#include<unordered_map>
#include<mutex>
#include"handle.h"

//时间类
class UI;
enum class Type_Season;
class Time
{
public:
    void Time_clock();//计时器
    Time(Message*m,UI* ui) :T_m(m),T_ui(ui){};//构造函数
private:
    Message* T_m;//消息类指针
    UI* T_ui;//UI类指针
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
//保存消息的队列
class Message_queue
{
public:
    Message_queue();
    void Insert(const MsgNode& m);//插入操作
    void Remove();//从队首弹出
    int Size()//返回当前队列长度
    {
        int i;
        Msq_m.lock();
        (end-head)==0?i=0:i=(6+end-head)%6+1;
        Msq_m.unlock();
    }
    MsgNode Search(const int &i);//返回第i个消息的信息
private:
    void change(int &i);//修改位置
    std::mutex Msq_m;
    std::array<MsgNode,6> dataqueue;//用来存储消息的array容器
    int head;//队首
    int end;//队尾
};
//消息类
class Message
{
public:
    Message();//构造函数
    ~Message();//析构函数
    void msg_increasetime();//递增时间
    void msg_gettime(int &mon,int&year,Type_Season* sea);//获取时间
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
            ++msg_sum;
        else
            --msg_sum;
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
    Message_queue msg_normalmsg;//普通消息队列
    Message_queue msg_warningmsg;//警告消息队列
    std::unordered_map<int,MsgSellreNode> msg_io;//进出口哈希表
    std::vector<MsgSellNode>msg_dealask;//出售请求
    int msg_sum;//商品总数
    bool msg_uiquit;//是否关闭UI界面
    bool msg_remsg;//是否刷新消息栏
    Handle* msg_handle;//是否需要处理消息
};

#endif