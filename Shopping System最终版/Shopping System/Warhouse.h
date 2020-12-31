#ifndef MUTEX
#define MUTEX
#include<mutex>
#endif // !MUTEX

#ifndef WAR_HOUSE
#define WAR_HOUSE

#include<vector>
#include<queue>
#include<string>

//季节枚举类
enum class Type_Season { Spr, Sum, Aut, Win };
enum class Type_Sort{id,name,lab};
class Message;
//类和结构体的声明
struct BNode;
class Wh_Chain;
//商品结构体
class  Commondity
{
public:
    Commondity(std::string mau,int year,int in_year,int mon,int in_mon,int sum):
    c_maufacter(mau),c_date_year(year),c_date_year_in(in_year),c_date_m(mon),
    c_date_m_in(in_mon),c_sum(sum){}//构造函数
    bool c_decrease(const int& i)//减少数量
    {
        if (i > c_sum)
            return false;
        else
        {            
            c_sum -= i;
            return true;
        }
    }
    bool empty()//判断是否为空
    {
        return c_sum==0;
    }
    void get_in_date(int &m,int &y)//获得进货日期
    {
        m=c_date_m_in;
        y=c_date_year_in;
    }
    void get_out_date(int &m,int &y)//获得过期日期
    {
        m=c_date_m;
        y=c_date_year;
    }
    int get_sum()//获得大小
    {
        return c_sum;
    }
    std::string get_mau()//获得厂商
    {
        return c_maufacter;
    }
    bool operator <(Commondity &c)
    {
        return (c_date_m+c_date_year*100)<(c.c_date_year*100+c.c_date_m);
    }
private:
    std::string c_maufacter;//厂商
    int c_date_year;//过期年份
    int c_date_year_in;//进货年份
    int c_date_m;//过期月份
    int c_date_m_in;//进货日期
    int c_sum;//该批商品的数量
};
//定义搜索三元组
struct  Triple
{
    BNode* r;//所在节点
    int id;//结点中的关键码序号i
    int tag;//tag=0,搜索不成功，tag=1，搜索成功
    Triple() :r(NULL), id(1), tag(0) {}
};
//用来返回链表的结构体
struct INFChain
{
    int id;
    std::string name;
    int sum;
    float cost;
    float profit;
    bool ifseason;
    Type_Season season;
    std::string lab;//标签
    INFChain(const int & i,std::string n,const int & s,const float &c,const float &p,bool ifs,Type_Season t,std::string l):id(i),
    name(n),sum(s),cost(c),profit(p),ifseason(ifs),season(t),lab(l){}
};
//用来返回商品信息的结构体
struct INFCom
{
    std::string maufacter;
    int in_m;  
    int in_y;
    int out_m;
    int out_y;
    int sum;
    INFCom(std::string m,const int & inm,const int & iny,const int & outm,const int & outy,const int & s):
    maufacter(m),in_m(inm),in_y(iny),out_m(outm),out_y(outy),sum(s){}
};
//B树仓库
class BTree
{
public:
    BTree():B_n(0),B_m(3),B_num(0),root(NULL){}//构造函数
    ~BTree();//析构函数
    void Wh_BTreeInsert(Wh_Chain *p1);//在Btree中插入新的种类的商品,参数传入data
    bool Wh_BTreeRemove(const int& d);//在Btree中移除商品
    Triple Wh_BTreeSearch(const int &id);//搜索
    bool Wh_UpdateNode(const int& id,const int &sum);//将编号为id的商品的数量减少
    void Wh_InsertdataNode(const int& id,Commondity C);//将单个商品添加到仓库中
    bool Wh_deletedataNode(const int& id,const int& i);//删除id为id的某一特定的商品批次，i为要删除的批次
    float Wh_profit(const int &id);//返回对应商品所对应的利润
    void Wh_ChaProfit(const int &id,const float &i);//修改对应商品的利润,成功则返回true
    Wh_Chain *Wh_list(bool restart=false);//用来遍历整个数
    int Wh_num()//返回仓库存储的商品的类别数
    {
        Wh_C.lock();
        int num(B_num);
        Wh_C.unlock();
        return num;
    }
private:
    void Wh_BTreeInsertKey(BNode* p, int j, Wh_Chain* p_data, BNode* rp);//插入关键码
    void Wh_BTreeRemoveKey(BNode* p, int j, bool kids = true);//删除关键码
    std::mutex Wh_C;//仓库锁
    BNode* root;//根节点指针
    int B_n;//节点总数
    int B_m;//度数
    int B_num;//所有id的数量
};
//仓库链表类
class Wh_Chain
{
public:
    Wh_Chain(const int &i,const std::string &str,const std::string &l,const float& p_in,const float& p_out):
    WhC_id(i),WhC_name(str),lab(l),WhC_price_in(p_in),WhC_price_get(p_out-p_in),WhC_ifseason(false){}
    INFChain WhC_ReInf();//将节点的信息打印
    INFCom WhC_NodeInf(const int & i);//将结点中的数据节点信息,ptr为指向该节点的指针
    void WhC_insert(Commondity data);//插入数据
    void WhC_delete(const int &i);//删除数据
    bool WhC_decrease(int s);//s为减少的值
    int WhC_ReSum()//返回商品的总数
    {
        WhC_m.lock();
        int i=WhC_sum;
        WhC_m.unlock();
        return i;
    }
    void WhC_InitSeason(Type_Season s)//初始化适合季节
    {
        WhC_m.lock();
        WhC_ifseason=true;
        WhC_season=s;
        WhC_m.unlock();
    }
    Type_Season WhC_ReSeason()//返回商品合适的季节
    {
        WhC_m.lock();
        Type_Season t=WhC_season;
        WhC_m.unlock();
        return t;
    }
    std::string WhC_ReName()//返回名称
    {
        return WhC_name;
    }
    int WhC_REID()//返回id;
    {
        return WhC_id;
    }
    int WhC_len()//查询表长
    {
        WhC_m.lock();
        int i= wh_list.size();
        WhC_m.unlock();
        return i;
    }
    bool WhC_ifempty()//是否为空
    {
        WhC_m.lock();
        bool i=wh_list.empty();
        WhC_m.unlock();
        return i;
    }
    bool WhC_ifup()//判断商品是否涨价
    {
        return ifup;
    }
    void WhC_turnup()//切回商品的涨价的情况
    {
        ifup=!ifup;
    }
    float WhC_Reprofit()//返回商品的利润
    {
        return WhC_price_get;
    }
    void WhC_ChaProfit(const float & i)//将商品的利润赋值
    {
        WhC_price_get=i;
    }
    bool WhC_ReIfSeason()//返回是否有季节的属性
    {
        return WhC_ifseason;
    }
    std::vector<Commondity> wh_list;//保存的优先队列
private:
    std::mutex WhC_m;//链表锁
    
    std::string lab;//商品的标签
    int WhC_id;//产品的iD
    std::string WhC_name;//产品的名称    
    float WhC_price_in;//进价
    float WhC_price_get;//利润
    int WhC_sum;//产品的总数
    bool ifup;//商品是否处于涨价状态
    bool WhC_ifseason;//商品是否需要合适季节这个属性
    Type_Season WhC_season;//产品适合的季节
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
//缓冲区类
class MSG_BUFFER
{
public:
    MSG_BUFFER(const int s):size(s)//构造函数
    {
        buffer.reserve(s);//扩容
    }
    void insert(INFChain i);//插入
    void sort(Type_Sort type);//排序
    void swap(int i,int j);//交换
    void sift_id(int i,int j);//根据id进行调整
    void sift_name(int i,int j);//根据名称进行调整
    void sift_lab(int i,int j);//根据标签进行调整
    int Size()//返回缓冲区大小
    {
        return size;
    }
    INFChain output(const int i)//将缓冲区中的第i个元素返回,从0开始
    {
        return buffer[i];
    }
private:  
    std::vector<INFChain> buffer;
    int size;//缓冲区大小
};
#endif
