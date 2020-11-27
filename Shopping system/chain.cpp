#include"class.h"


Wh_Chain::Wh_Chain()
{
    WhC_head=NULL;
    int i=1;
}
Wh_Chain::Wh_Chain(const int &i,const std::string &str,Type_Lable t,int p_in,int p_out,Type_Season*s):
WhC_id(i),ifup(false),WhC_name(str),WhC_type(t),WhC_price_in(p_in),WhC_price_get(p_out),WhC_length(0)
{
    WhC_head=new Commondity;
    WhC_head->next=NULL;
    WhC_end=WhC_head;
    if(s!=NULL)
    {
        WhC_season=*s;
    }
    else
    {
        WhC_season=Type_Season::Spr;
    }
}
//仓库链表的实现
Wh_Chain::~Wh_Chain()
{
	Commondity *p(WhC_head),*q1;
    while(p!=NULL)
    {
        q1=p;
        p=p->next;
        delete q1;
    }
}
//向链表中插入数据
void Wh_Chain::WhC_insert(Commondity* data)
{
    if(WhC_type==Type_Lable::Food)//食物是有保质期的
    {
        Commondity *p=WhC_head;
        bool i(false);
        while(p->next!=NULL)
        {
            if(p->next->c_date_year*100+p->next->c_date_m>data->c_date_year*100+data->c_date_m)
            {
                data->next=p->next;
                p->next=data;
                i=true;
                break;
            }
            p=p->next;
        }
        if(!i)
        {
	        WhC_end->next= data;
            WhC_end = WhC_end->next;
        }
    }
    else
    {
	    WhC_end->next= data;	
        WhC_end = WhC_end->next;
    }

	++WhC_length;
    WhC_sum+=data->c_sum;//增加总数
}
//删除节点
void Wh_Chain::WhC_delete(int i)
{
	Commondity *m_ptr = WhC_head;
	for (int j = 0; j < i; ++j)		
    {
		m_ptr = m_ptr->next;
	}
	Commondity* p;
	p = m_ptr->next;
	m_ptr->next = m_ptr->next->next;
	if (i == WhC_length-1)
	{
		WhC_end = m_ptr;
	}    
    WhC_sum-=p->c_sum;
	delete p;
	--WhC_length;

}
//调整数据，单参数版
void Wh_Chain::WhC_Update(int s)
{
    Commondity *m_ptr=WhC_head->next;

    while(m_ptr!=NULL)
    {
        if(m_ptr->c_sum>=s)
        {
            m_ptr->c_sum-=s;
            if (m_ptr->c_sum == 0)
            {
                WhC_delete(0);//删除该结点
            }
            break;
        }
        else
        {
            s-=m_ptr->c_sum;
            m_ptr->c_sum=0;                
            WhC_delete(0);//删除该结点      
            m_ptr=m_ptr->next;//指向下一个
        }
  
    }
    WhC_sum-=s;
}
//将节点的基本信息返回
std::string Wh_Chain::WhC_ReInf()
{
    using namespace std;
    string str("");
    str="id:"+to_string(WhC_id)+" name:"+WhC_name+" sum:"+to_string(WhC_sum)+" profit:"+to_string(WhC_price_get);
    return str;
}
//将结点中的数据节点信息,i为打印的第几个节点
std::string Wh_Chain::WhC_NodeInf(Commondity* ptr)
{
    using namespace std;
    std::string re="";
    re+="Maufacter:"+ptr->c_maufacter+" Shelf Life:"+to_string(ptr->c_date_m)+" "+to_string(ptr->c_date_year);
    re+=" In Time:"+to_string(ptr->c_date_m_in)+" "+to_string(ptr->c_date_year_in);
    re+=" sum:"+to_string(ptr->c_sum);
    return re;
};
//返回对应位置的节点
Commondity *Wh_Chain::WhC_RePtr(const int &i)
{
    Commondity *p=WhC_ReChain();
    for(int j(0);j<i;++j)
    {
        p=p->next;
    }
    return p->next;
}
//返回数量,i为其所在位置
int Wh_Chain::WhC_ReSum(int i)
{
    Commondity *m_ptr = WhC_head;
    //找到节点
	for (int j = 0; j < i; ++j)		
    {
		m_ptr = m_ptr->next;
	}

    return m_ptr->next->c_sum;
}
//返回商品是否过期,如果有过期商品则返回true，否则返回false
bool Wh_Chain::WhC_ReDate(const int &year,const int & day)
{
    Commondity* p = WhC_head->next;
    while (p != NULL)
    {
        if (p->c_date_year == year && ( p->c_date_m-day) <= 2 && (p->c_date_m-day) >= 0)
        {
            return true;
        }
        else
        {
            break;
        }
    }
    return false;
}
