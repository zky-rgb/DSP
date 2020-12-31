#include"Warhouse.h"

//向链表中插入数据
void Wh_Chain::WhC_insert(Commondity data)
{
    bool insert(false);
    WhC_m.lock();
    for(std::vector<Commondity>::iterator iter=wh_list.begin();iter!=wh_list.end();++iter)
    {
        if (data < (*iter))
        {
            wh_list.insert(iter,data);
            insert = true;
            break;

        }

    }
    if(!insert)
        wh_list.push_back(data);
    WhC_sum+=data.get_sum();
    WhC_m.unlock();
}
//删除节点
void Wh_Chain::WhC_delete(const int &i)
{
    WhC_m.lock();
    std::vector<Commondity>::iterator iter=wh_list.begin()+i;
    WhC_sum-=(*iter).get_sum();
    wh_list.erase(iter);
    WhC_m.unlock();
}
//将链表的总数减少s,成功则返回true
bool Wh_Chain::WhC_decrease(int s)
{
    //判断数量是否满足要求
    if(WhC_ReSum()<s)
    {
        //数量不足
        return false;
    }
    else
    {       
        //总数减少
        WhC_m.lock();
        WhC_sum-=s;
        //进行删除操作
        while(s!=0)
        {
            if(wh_list[0].get_sum()<=s)
            {
                s-=wh_list[0].get_sum();
                wh_list.erase(wh_list.begin());//删除第一个节点
            }
            else
            {
                s=0;
                wh_list[0].c_decrease(s);
            }
        }
        WhC_m.unlock();
        return true;
    }
}
//将节点的基本信息返回
INFChain Wh_Chain::WhC_ReInf()
{
    
    WhC_m.lock();
    INFChain inf(WhC_id,WhC_name,WhC_sum,WhC_price_in,WhC_price_get,WhC_ifseason,WhC_season,lab);
    WhC_m.unlock();
    return inf;
}
//将结点中的数据节点信息,i为打印的第几个节点
INFCom Wh_Chain::WhC_NodeInf(const int&i)
{
    //获取时间
    int m,y,min,yin;
    Commondity a(wh_list[i]);
    a.get_out_date(m,y);
    a.get_in_date(min,yin);
    INFCom inf(wh_list[i].get_mau(),min,yin,m,y,wh_list[i].get_sum());
    return inf;
};
