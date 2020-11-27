#include"class.h"
#include<time.h>
#include<cstdlib>
#include<stack>
//处理类实现
//生成交易信息
void Handle::H_createsell()
{
    using namespace std;
    int sum=h_msg->msg_getsum();//获取商品的总数
    int random_id1,random_id2;//保存两个商品的id
    int random_sum1,random_sum2;//保存两个商品的数量
    int random_year1,random_year2;//保存两个随机年份
    int random_mon1,random_mon2;//保存两个随机月
    int year,mon;//保存现在的年份和月份

    if(sum==0)//如果没有商品则直接退出，不进行生成
    {
        return;
    }
    else if(sum==1)//只有一个商品,直接生成一个购买请求
    {
        h_msg->msg_ctime.lock();
        year = h_msg->msg_year;
        mon = h_msg->msg_time;
        h_msg->msg_ctime.unlock();

        srand(time(NULL));
        random_sum1=rand()%2+1;
        random_year1=rand()%3+year;
        random_mon1=(rand()%12+mon)%12+1;
        //插入队列
        h_msg->msg_pushdealask(h_btree->Wh_getid(0),random_sum1,random_year1,random_mon1);
    }
    else//有多个商品
    {
        //获取现在的年份信息
        h_msg->msg_gettime(mon,year);
        //生成随机数
        srand(time(NULL));
        random_id1 = rand() % sum;
        random_sum1=rand()%2+1;//每次最多买两个
        random_year1=rand()%3+year;
        random_mon1=(rand()%12+mon)%12+1;
        chrono::milliseconds dura(1000);//2000毫秒进行一次刷新
        this_thread::sleep_for(dura);
        srand(time(NULL));
        random_id2 = rand() % sum;
        random_sum2=rand()%2+1;  
        random_year2=rand()%3+year;
        random_mon2=(rand()%12+mon)%12+1;
        //插入队列

        h_msg->msg_pushdealask(h_btree->Wh_getid(random_id1),random_sum1,random_year1,random_mon1);
        h_msg->msg_pushdealask(h_btree->Wh_getid(random_id2),random_sum2,random_year2,random_mon2);

    }
}
//处理消息队列
//该函数主要用来分析进出口
void Handle::H_handlemsg()
{
    int min=999999;//用来保存最小值
    int minid;
    int m;
    int max=-1;//用来保存最大值
    int maxid;
    h_msg->msg_common.lock();
    if (h_msg->msg_io.size() > 0)
    {    
        bool i(true);
        //遍历进出口哈希表，并分析数据
        for (auto& x : h_msg->msg_io)
        {
            if (i)
            {
                minid = maxid = x.first;
                min=max= h_btree->Wh_profit(x.first) * (x.second.msg_out - x.second.msg_in);
                i = false;
            }
            m = h_btree->Wh_profit(x.first) * (x.second.msg_out - x.second.msg_in);
            if (m < min)
            {
                min = m;
                minid = x.first;
            }
            if (m > max)
            {
                max = m;
                maxid = x.first;
            }
        }
        //将得到的消息加入到消息队列
        std::string minstr;//最大消息
        std::string maxstr;//最小消息
        minstr = "H:[Suggest]buy less " + std::to_string(minid) + "(id)";
        maxstr = "H:[Suggest]buy more " + std::to_string(maxid) + "(id)"; 
        std::string ps = "H:[Suggest]Other kinds of commondities has not been selled";
        h_msg->msg_common.unlock();
        h_msg->msg_add(minstr, true);
        h_msg->msg_add(maxstr, true);
        h_msg->msg_add(ps, true);
        h_msg->msg_common.lock();
    }
    h_msg->msg_common.unlock();

}
//处理是否过期或者过季
void Handle::H_handleOverdue()
{
    using namespace std;
    
    int mon, year;//保存现有的年份和月份
    Type_Season season;
    Type_Lable type;
    h_msg->msg_gettime(mon, year,&season);//获取目前时间
    stack<BNode*> s;
    BNode* q;
    Wh_Chain* ptr;
    bool ifdate;//是否过期
    bool ifup;//是否需要涨价/降价

    BNode* p = h_btree->Wh_reroot();//获取根节点
    if (p != NULL)//只要B树不为空，则遍历 B树
    {
        s.push(p);
        h_btree->Wh_C.lock();
        while (!s.empty())//只要栈不为空
        {
            q = s.top();//访问栈顶指针
            s.pop();
            for (int i(0); i < 2; ++i)
            {
                if (q->P_data[i] != NULL)
                {  
                    ptr = q->P_data[i];
                    type = ptr->WhC_Rekind();
                    switch (type)
                    {       
                    case Type_Lable::Clothe:
                        if (season == ptr->WhC_ReSeason())
                        {
                            ptr->WhC_turnup(true);//切换状态
                            string str = "H:[msg] The Clothe (id)" + to_string(ptr->WhC_REID()) + "now meets the right season";
                            h_msg->msg_add(str, true);//将涨价消息传递
                        }
                        else if(ptr->WhC_ifup())
                        {
                            ptr->WhC_turnup(false);
                            string str="H:[warn]The Clothe (id)"+ to_string(ptr->WhC_REID())+"now pass the right season";
                            h_msg->msg_add(str,false);
                        }
                    case Type_Lable::Food:                   
                    case Type_Lable::Electrical:
                        if (ptr->WhC_ReDate(year, mon))//如果有过期的商品
                        {
                            string str = "H:[warn]Some commondity (id)" + to_string(ptr->WhC_REID()) + " will overdue!";
                            h_msg->msg_add(str, false);
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            for(int i(0);i<3;++i)
            {
                if(q->kids_ptr[i]!=NULL)
                {
                    s.push(q->kids_ptr[i]);
                }
                else
                {
                    break;
                }
            }
        }
        h_btree->Wh_C.unlock();                           
    }
}
