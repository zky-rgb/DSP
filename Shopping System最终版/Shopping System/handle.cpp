#include<time.h>
#include<cstdlib>
#include<stack>
#include "Message.h"
#include "Warhouse.h"
#include "Handle.h"
#include"Logs.h"
//处理类实现
//生成交易信息

void Handle::H_createsell()
{
    using namespace std;
    int sum = h_btree->Wh_num();//获取商品的总数
    int random_id1, random_id2;//保存两个商品的id
    int random_sum1, random_sum2;//保存两个商品的数量
    int random_year1, random_year2;//保存两个随机年份
    int random_mon1, random_mon2;//保存两个随机月
    int year, mon;//保存现在的年份和月份
    Type_Season now;//保存当前生成的商品季节信息
    Wh_Chain* ptr1;
    Wh_Chain* ptr2;


    if (sum == 0)//如果没有商品则直接退出，不进行生成
    {
        return;
    }
    else if (sum == 1)//只有一个商品,直接生成一个购买请求
    {
        h_msg->msg_gettime(mon, year, &now);   //获取当前月份和年份

        srand(time(NULL));
        random_sum1 = rand() % 2 + 1;
        random_year1 = rand() % 3 + year;
        random_mon1 = (rand() % 12 + mon) % 12 + 1;

        //插入队列
        //h_msg->msg_pushdealask(h_btree->Wh_getid(0),random_sum1,random_year1,random_mon1);
        //插入日志记录
        log->InputLog(h_btree->Wh_list()->WhC_REID(), random_sum1, random_year1, random_mon1);

    }
    else//有多个商品
    {
        h_btree->Wh_list(1);
        //获取现在的年份信息
        h_msg->msg_gettime(mon, year, &now);
        //生成随机数
        int i = 0;
        srand(time(NULL));
        ptr1 = h_btree->Wh_list();
        random_id1 = ptr1->WhC_REID();

        random_sum1 = rand() % 2 + 1;//每次最多买两个
        random_year1 = rand() % 3 + year;
        random_mon1 = (rand() % 12 + mon) % 12 + 1;

        if (random_mon1 > 2 && random_mon1 < 6) {
            now = Type_Season::Spr;
        }
        else if (random_mon1 > 5 && random_mon1 < 9) {
            now = Type_Season::Sum;
        }
        else if (random_mon1 > 8 && random_mon1 < 12) {
            now = Type_Season::Aut;
        }
        else {
            now = Type_Season::Win;
        }
        /*do {
            srand(time(NULL));
            ptr1 = h_btree->Wh_list();
            random_id1 = ptr1->WhC_REID();

            random_sum1 = rand() % 2 + 1;//每次最多买两个
            random_year1 = rand() % 3 + year;
            random_mon1 = (rand() % 12 + mon) % 12 + 1;

            if (random_mon1 > 2 && random_mon1 < 6) {
                now = Type_Season::Spr;
            }
            else if (random_mon1 > 5 && random_mon1 < 9) {
                now = Type_Season::Sum;
            }
            else if (random_mon1 > 8 && random_mon1 < 12) {
                now = Type_Season::Aut;
            }
            else {
                now = Type_Season::Win;
            }
            i++;
           // chrono::milliseconds dura(1000);//2000毫秒进行一次刷新 
            //this_thread::sleep_for(dura);

        } while (ptr1->WhC_ReSeason() == now || !ptr1->WhC_ReIfSeason() || i == sum);   //当now与当前id货物季节相同或货物无需季节 结束循环
        */

        h_btree->Wh_list(1);
        i = 0;
        ptr2 = h_btree->Wh_list();
        random_id2 = ptr2->WhC_REID();

        random_sum2 = rand() % 2 + 1;
        random_year2 = rand() % 3 + year;
        random_mon2 = (rand() % 12 + mon) % 12 + 1;

        if (random_mon2 > 2 && random_mon2 < 6) {
            now = Type_Season::Spr;
        }
        else if (random_mon2 > 5 && random_mon2 < 9) {
            now = Type_Season::Sum;
        }
        else if (random_mon2 > 8 && random_mon2 < 12) {
            now = Type_Season::Aut;
        }
        else {
            now = Type_Season::Win;
        }
        /*do {
            srand(time(NULL));

            ptr2 = h_btree->Wh_list();
            random_id2 = ptr2->WhC_REID();

            random_sum2 = rand() % 2 + 1;
            random_year2 = rand() % 3 + year;
            random_mon2 = (rand() % 12 + mon) % 12 + 1;

            if (random_mon2 > 2 && random_mon2 < 6) {
                now = Type_Season::Spr;
            }
            else if (random_mon2 > 5 && random_mon2 < 9) {
                now = Type_Season::Sum;
            }
            else if (random_mon2 > 8 && random_mon2 < 12) {
                now = Type_Season::Aut;
            }
            else {
                now = Type_Season::Win;
            }
            i++;


        } while (ptr2->WhC_ReSeason() == now || !ptr2->WhC_ReIfSeason() || i == sum);   //当now与当前id货物季节相同或货物无需季节 结束循环
        */

        //插入队列

        h_msg->msg_pushdealask(random_id1, random_sum1, random_year1, random_mon1);
        h_msg->msg_pushdealask(random_id2, random_sum2, random_year2, random_mon2);

        //插入日志记录

        //log->InputLog(random_id1, random_sum1, random_year1, random_mon1);
        //log->InputLog(random_id2, random_sum2, random_year2, random_mon2);

    }
}

//处理消息队列
//该函数主要用来分析进出口
void Handle::H_handlemsg()
{
    int min = 999999;//用来保存最小值
    int minid=0;
    int m;
    int max = -1;//用来保存最大值
    int maxid=0;
    //h_msg->msg_common.lock();
    if (h_msg->msg_pop() == 1)
    {
        bool i(true);
        //遍历进出口哈希表，并分析数据
        for (auto& x : h_msg->msg_io)
        {
            if (i)
            {
                minid = maxid = x.first;
                min = max = h_btree->Wh_profit(x.first) * (x.second.msg_out - x.second.msg_in);
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
        //h_msg->msg_common.unlock();
        h_msg->msg_add(minstr, true);
        h_msg->msg_add(maxstr, true);
        h_msg->msg_add(ps, true);
        //h_msg->msg_common.lock();
    }
    //h_msg->msg_common.unlock();

}


//处理是否过期或者过季
void Handle::H_handleOverdue()
{
    using namespace std;
    h_msg->msg_common.lock();
    h_btree->Wh_list(1);
    int mon, year;//保存现有的年份和月份
    Type_Season season;
    h_msg->msg_gettime(mon, year, &season);//获取目前时间
    BNode* q;
    Wh_Chain* ptr;
    int dmon, dyear;//保存商品过期日期
    bool ifdate;//是否过期
    bool ifup;//是否需要涨价/降价

    int idnum = h_btree->Wh_num();
    int nowid = 0;

    for (int i = 0; i < idnum; i++) {
        ptr = h_btree->Wh_list();
        nowid = ptr->WhC_len();

        for (int j = 0; j < nowid; j++) {
            dmon = ptr->WhC_NodeInf(j).out_m;   //存储当前商品过期日期
            dyear = ptr->WhC_NodeInf(j).out_y;

            if (year > dyear) {
                string str = "H:[warn]Some commondity (id)" + to_string(ptr->WhC_REID()) + " will overdue!";
                h_msg->msg_add(str, false);
            }
            else if (year == dyear && mon > dmon) {
                string str = "H:[warn]Some commondity (id)" + to_string(ptr->WhC_REID()) + " will overdue!";
                h_msg->msg_add(str, false);
            }
            else {
                //没有过期商品 无需操作
            }


            if (season == ptr->WhC_ReSeason()) {    //如果当前季节为货物应季
                ptr->WhC_turnup();//切换状态为涨价
                string str = "H:[msg] The (id)" + to_string(ptr->WhC_REID()) + "now meets the right season";
                h_msg->msg_add(str, true);//将涨价消息传递
            }
            else {
                ptr->WhC_turnup(); //切换为非应季 商品降价
            }
        }


       
    }
    h_msg->msg_common.unlock();
}
