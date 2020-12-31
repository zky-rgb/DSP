#ifndef MUTEX
#define MUTEX
#include<mutex>
#endif // !MUTEX

#ifndef MESSAGE
#define MESSAGE

#include<array>
#include<unordered_map>
#include<vector>

//时间类
class UI;
class BTree;
class Logs;
enum class Type_Season;

//消息节点
struct MsgNode
{
    std::string data;//消息内容
    int mon;//保质期（月）
    bool warning;//是否为警告消息,警告消息为true
    MsgNode() {};
    MsgNode(std::string s,int m,bool w):data(s),mon(m),warning(w){}
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
    Message_queue() :head(0), end(0) {};
    void Insert(const MsgNode& m);//插入操作
    void Remove();//从队首弹出
    int Size()//返回当前队列长度
    {
       
        Msq_m.lock();
        int i=(end==head)?0:((7+end-head)%7+1);
        Msq_m.unlock();
        return i;
    }
    MsgNode Search(const int &i);//返回第i个消息的信息
private:
    void change(int &i);//修改位置
    std::mutex Msq_m;
    std::array<MsgNode,7> dataqueue;//用来存储消息的array容器
    int head;//队首
    int end;//队尾
};
//消息类
class Message
{
public:
    Message():msg_time(1),msg_year(2020),msg_uiquit(false){}//构造函数
    void msg_increasetime();//递增时间
    void msg_gettime(int &mon,int&year,Type_Season* sea=NULL);//获取时间
    void msg_add(std::string s,bool warn);//向消息栏添加新的消息
    void msg_control();//控制与用户交互
    void msg_logs();//展示日志信息
    void msg_purcharse();//购买商品页面
    int  msg_commondity();//商品界面的封装,返回需要展开的id
    void msg_selllist();//出售列表
    void msg_detail(const int id);//显示商品详细信息
    bool msg_pop();//弹出队首的消息，成功返回true
    bool msg_front(MsgNode& node);//返回队首的消息信息
    void msg_pushdeal(const int& id,int in=0,int out=0);//向进出口哈希表中添加项/更新项
    void msg_deletedeal(const int&id);//从进出口表中删除项
    void msg_pushdealask(int id,int sum,int mon,int year);//向出售请求队列中添加消息
    void msg_deletedealask(const int &i);//从出售队列删除消息
    bool msg_settime(const int mon,const int year)//初始化系统时间,如果是非法时间，则返回false
    {
        if(mon<=0||mon>12||year<=0)
            return false;
        else
        {
            msg_ctime.lock();
            msg_time=mon;
            msg_year=year;
            msg_ctime.unlock();
            return true;
        }
    }
    void msg_clearIO()//清空进出口表
    {
        msg_common.lock();
        msg_io.clear();//清空进出口表
        msg_common.unlock();
    }
    bool msg_quit()//程序是否退出
    {
        msg_bool.lock();
        bool i=msg_uiquit;
        msg_bool.unlock();
        return i;
    }
    void msg_getui(UI*ui)//获取ui类指针
    {
        msg_ui=ui;
    }
    void msg_getbtree(BTree *btree)
    {
        msg_btree=btree;
    }    
    void msg_getlogs(Logs* log)
    {
        msg_log= log;
    }
    std::unordered_map<int,MsgSellreNode> msg_io;//进出口哈希表    
    std::vector<MsgSellNode>msg_dealask;//出售请求   
    std::mutex msg_common;//普通变量锁
    Message_queue msg;//消息队列
private:    

    std::mutex msg_bool;//退出消息锁
    std::mutex msg_ctime;//时间消息互斥量
    std::mutex msg_cui;//ui互斥量
    int msg_year;//存储年份
    int msg_time;//储存月份
    
    BTree *msg_btree;//BTree类指针
    UI *msg_ui;//UI类指针    
    Logs* msg_log;//日志指针
    bool msg_uiquit;//是否关闭UI界面

};

#endif