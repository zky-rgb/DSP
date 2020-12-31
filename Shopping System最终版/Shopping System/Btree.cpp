#include"Warhouse.h"
#include"Message.h"
#include<stack>
#include <algorithm>
//B树类的实现

//析构函数
BTree::~BTree()
{
	using namespace std;

	BNode* p = root;
	BNode* q;
	stack<BNode*> s;
	if (p != NULL)
	{
		s.push(p);
		while (!s.empty())
		{
			q = s.top();
			for (int i(0); i < q->n; ++i)
			{
				delete q->P_data[i];
			}
			s.pop();
			for (int i(0); i < q->n + 1; ++i)
			{
				if (q->kids_ptr[i] != NULL)
				{
					s.push(q->kids_ptr[i]);
				}
			}
			delete q;
		}
	}
}
//将数据节点插入到B树中
void BTree::Wh_BTreeInsert(Wh_Chain* p1)
{
	Triple loc = Wh_BTreeSearch(p1->WhC_REID());//检查是否已经存在
    if (loc.tag)//说明节点存在
        delete p1;
	else//节点不存在
	{
		BNode* p = loc.r;//获取应该插入的节点指针
        if(p==NULL)//如果p为空节点
        {
            p=new BNode(NULL);
            p->parent=p;
            p->par_id=0;
            root=p;
            B_n++;
        }
        Wh_C.lock();
        ++B_num;
        Wh_C.unlock();
		BNode* rp = NULL;//对应的右侧的子节点指针
		int j = loc.id;//获取插入位置
        Wh_C.lock();
		Wh_BTreeInsertKey(p, j, p1, rp);//插入关键码
        Wh_C.unlock();
	}
}
//插入关键码
void BTree::Wh_BTreeInsertKey(BNode* p, int j, Wh_Chain* p_data, BNode* rp)
{
	//为插入的节点腾出位置
	for (int i(B_m - 1); i > j; --i)
	{
		p->kids_ptr[i + 1] = p->kids_ptr[i];
		p->P_data[i] = p->P_data[i - 1];
	}
	//插入节点
	p->P_data[j] = p_data;
	p->kids_ptr[j + 1] = rp;
	++p->n;
	//检查节点是否溢出
	if (p->n == B_m)//如果已经溢出
	{
		//检查是否为根节点
		if (p == p->parent)
		{
			//是根节点
			root = new BNode(NULL);
			++B_n;
			root->parent = root;
			root->par_id = 0;
			p->parent = root;
			root->kids_ptr[0] = p;
		}
		//分裂节点
		BNode* q1 = new BNode(p->parent);
		++B_n;
		//将节点分为两半
		for (int i(B_m / 2 + 1); i < B_m; ++i)
		{
			q1->P_data[i - B_m / 2 - 1] = p->P_data[i];
			q1->kids_ptr[i - B_m / 2] = p->kids_ptr[i + 1];
			if (q1->kids_ptr[i - B_m / 2] != NULL)
			{
				q1->kids_ptr[i - B_m / 2]->parent = q1;
                q1->kids_ptr[i-B_m/2]->par_id=i-B_m/2;
			}
			++q1->n;
			--p->n;
		}
		q1->kids_ptr[0] = p->kids_ptr[B_m / 2 + 1];
		if (q1->kids_ptr[0] != NULL)
		{
			q1->kids_ptr[0]->parent = q1;
			q1->kids_ptr[0]->par_id = 0;
		}
		//调整子节点所在位置
		q1->par_id = p->par_id + 1;
		j = p->par_id;
		p_data = p->P_data[B_m / 2];
		--p->n;
		for (int i(B_m / 2); i < B_m; ++i)
		{
			p->P_data[i] = NULL;
			p->kids_ptr[i + 1] = NULL;
		}

		p = p->parent;
		Wh_BTreeInsertKey(p, j, p_data, q1);
	}
}
//搜索节点
Triple BTree::Wh_BTreeSearch(const int &id)
{
    Triple t;
    if(root==NULL)//根节点为空
    {
        t.id=0;
        t.r=root;
        t.tag=0;
        return t;   
    }
    Wh_C.lock();
	BNode* p = root;//获取根节点
    Wh_Chain* q(root->P_data[0]);
	while (p!=NULL)//对于B树进行搜索
	{
		for (int i = 0; i < B_m; i++)//遍历B树，进行搜索
		{
			q = p->P_data[i];
			if (q != NULL)//如果该节点不为空
			{
				if (q->WhC_REID() == id)//等于，说明找到
				{
					t.r = p;
					t.id = i;
					t.tag = 1;//标记找到
                    Wh_C.unlock();
					return t;
				}
				else if (q->WhC_REID() < id)//如果节点id大，则继续
					continue;
				else//到下一层或则退出
				{
					if (p->kids_ptr[i] != NULL)
					{
						p = p->kids_ptr[i];//到子节点
						break;
					}
					else
					{
						t.r = p;
						t.id = i;
						t.tag = 0;//没有找到节点
                        Wh_C.unlock();
						return t;
					}
				}
			}
			else//该结点为空
			{
				if (p->kids_ptr[i] != NULL)
				{
					p = p->kids_ptr[i];//将指针移动到子结点
					break;
				}
				else
				{
					t.r = p;
					t.id = i;
					t.tag = 0;//tag=0，没有找到子结点
                    Wh_C.unlock();
					return t;//ֱ返回搜索结果
				}
			}
		}
	}
}
//将关键码移除
void BTree::Wh_BTreeRemoveKey(BNode* p, int j, bool kids) 
{
    //直接delete 要删除的结点
    delete p->P_data[j];
    --p->n;
    BNode* q;
    //如果kids不为false的话，则需要考虑其子结点
    if (kids)
    {
        if (p->kids_ptr[j + 1] != NULL)
        {
            q = p->kids_ptr[j + 1];
            p->P_data[j] = q->P_data[0];
            q->P_data[0] = NULL;
            ++p->n;
            Wh_BTreeRemoveKey(q, 0);
            return;
        }
        else if (p->kids_ptr[j] != NULL)
        {
            q = p->kids_ptr[j];
            p->P_data[j] = q->P_data[q->n - 1];
            q->P_data[q->n - 1] = NULL;
            ++p->n;
            Wh_BTreeRemoveKey(q, q->n - 1);
            return;
        }
        else
        {
            //直接将结点置空，并调整关键码的位置
            p->P_data[j] = NULL;
            for (int i(j); i < B_m - 1; ++i)
            {
                p->P_data[j] = p->P_data[j + 1];
                p->kids_ptr[j + 1] = p->kids_ptr[j + 2];
                if (p->kids_ptr[j + 1] != NULL)
                {
                    --p->kids_ptr[j + 1]->par_id;
                }
            }
        }
    }
   else
    {
        //不处理子节点，直接合并
        p->P_data[j] = NULL;
        if (p->kids_ptr[j] == NULL)
        {
            p->kids_ptr[j] = p->kids_ptr[j + 1];
            if (p->kids_ptr[j] != NULL)
            {
                --p->kids_ptr[j]->par_id;
            }
        }
        for (int i(j); i < B_m - 1; ++i)
        {
            p->P_data[j] = p->P_data[j + 1];
            p->kids_ptr[j + 1] = p->kids_ptr[j + 2];
            if (p->kids_ptr[j + 1] != NULL)
            {
                --p->kids_ptr[j + 1]->par_id;
            }
        }
    }
    //判断操作之后节点是否贫瘠
    if (p->n < B_m / 2 && p != root)
    {
        //如果该节点贫瘠的话
        
        //判断左右兄弟节点是否丰满
        int rb, lb;
        if (p->par_id == 0)
        {
            //节点本身没有左节点
            lb = -1;
            rb = p->parent->kids_ptr[p->par_id + 1]->n;
        }
        else if (p->par_id == p->parent->n)
        {
            //节点本身没有右节点
            rb = -1;
            lb = p->parent->kids_ptr[p->par_id - 1]->n;
        }
        else
        {
            //左右节点都存在的
            lb = p->parent->kids_ptr[p->par_id - 1]->n;
            rb = p->parent->kids_ptr[p->par_id + 1]->n;
        }
        if (rb > B_m / 2 || lb > B_m / 2)
        {
            //左右节点至少有一个丰满
            //向丰满的兄弟节点要一个数据节点
            if (rb > B_m / 2)
            {
                //右兄兄弟丰满
                q = p->parent->kids_ptr[p->par_id + 1];
                p->P_data[p->n] = p->parent->P_data[p->par_id];
                p->kids_ptr[p->n + 1] = q->kids_ptr[0];
                if (p->kids_ptr[p->n + 1] != NULL)
                {
                    p->kids_ptr[p->n+1]->parent=p;
                    p->kids_ptr[p->n + 1]->par_id = p->n + 1;
                }
                ++p->n;
                p->parent->P_data[p->par_id] = q->P_data[0];
                for (int i(0); i < q->n; ++i)
                {
                    q->P_data[i] = q->P_data[i + 1];
                    q->kids_ptr[i] = q->kids_ptr[i + 1];
                    if (q->kids_ptr[i] != NULL)
                    {
                        --q->kids_ptr[i]->par_id;
                    }
                }
                q->kids_ptr[q->n] = NULL;
                --q->n;
            }
            else
            {
                //左兄弟丰满
                q = p->parent->kids_ptr[p->par_id - 1];
                //为节点腾出地方
                for(int i(p->n);i>0;--i)
                {
                    p->P_data[i]=p->P_data[i-1];
                    p->kids_ptr[i+1]=p->kids_ptr[i];
                    if(p->kids_ptr[i+1]!=NULL)
                    {
                        p->kids_ptr[i+1]->par_id++;
                    }
                }

                p->kids_ptr[1] = p->kids_ptr[0];
                if (p->kids_ptr[1] != NULL)
                {
                    p->kids_ptr[1]->par_id = 1;
                }
                //将父节点下移
                p->P_data[0] = p->parent->P_data[q->par_id];
                p->kids_ptr[0] = q->kids_ptr[q->n];
                if (p->kids_ptr[0] != NULL)
                {
                    p->kids_ptr[0]->par_id=0;
                    p->kids_ptr[0]->parent=p;
                }
                p->parent->P_data[q->par_id] = q->P_data[q->n - 1];
                ++p->n;
                q->kids_ptr[q->n] = NULL;
                q->P_data[q->n - 1] = NULL;
                --q->n;
            }
        }
        else
        {
            //左右兄弟节点都不丰满
            //需要合并
            if (rb != -1)
            {
                //和右节点进行合并
                q = p->parent->kids_ptr[p->par_id + 1];
               //将父结点中的数据节点借下来
                p->P_data[p->n]=p->parent->P_data[p->par_id];
                if(p->kids_ptr[p->n]!=NULL)
                {
                    p->kids_ptr[p->n]->par_id=p->n;
                }
                p->kids_ptr[p->n+1]=q->kids_ptr[0];
                ++p->n;
                if(p->kids_ptr[p->n]!=NULL)
                {
                    p->kids_ptr[p->n]->parent=p;
                    p->kids_ptr[p->n]->par_id=p->n;
                }
                //处理父结点
                p->parent->kids_ptr[p->par_id + 1] = NULL;
                //合并
                for (int i(0); i < q->n; ++i)
                {
                    p->P_data[i + p->n] = q->P_data[i];
                    p->kids_ptr[i + 1 + p->n] = q->kids_ptr[i + 1];
                    if (q->kids_ptr[i + 1] != NULL)
                    {
                        q->kids_ptr[i + 1]->parent = p;
                        q->kids_ptr[i + 1]->par_id = i + 1 + p->n;
                    }
                }
                p->n = p->n + q->n;
                delete q;
                --B_n;
                p->parent->P_data[p->par_id] = NULL;
                Wh_BTreeRemoveKey(p->parent, p->par_id, false);
                return;
                //完成
            }
            else
            {
                //和左节点进行合并
                q = p->parent->kids_ptr[p->par_id - 1];
                //将父结点的数据节点移动到左兄弟节点
                //将父结点中的数据节点借下来
                q->P_data[q->n] = q->parent->P_data[q->par_id];
                q->kids_ptr[q->n + 1] = p->kids_ptr[0];
                ++q->n;
                if (q->kids_ptr[q->n] != NULL)
                {
                    q->kids_ptr[q->n]->parent = q;
                    q->kids_ptr[q->n]->par_id = q->n;
                }
                //处理父结点
                q->parent->kids_ptr[q->par_id + 1] = NULL;
                //合并
                for (int i(0); i < p->n; ++i)
                {
                    q->P_data[i + q->n] = p->P_data[i];
                    q->kids_ptr[i + 1 + p->n] = p->kids_ptr[i + 1];
                    if (p->kids_ptr[i + 1] != NULL)
                    {
                        p->kids_ptr[i + 1]->parent = p;
                        p->kids_ptr[i + 1]->par_id = i + 1 + q->n;
                    }
                }
                q->n = q->n + p->n;
                delete p;
                --B_n;
                q->parent->P_data[q->par_id] = NULL;
                Wh_BTreeRemoveKey(q->parent, q->par_id, false);
                return;
                //完成
            }

        }
    }
    //根节点或者是没有贫瘠，则不进行处理
    else
    {
        return;
    }

}
//将Btree的结点移除
bool BTree::Wh_BTreeRemove(const int& d)
{
    Triple loc = Wh_BTreeSearch(d);//搜索要删除的结点
    if (!loc.tag)//结点不存在
        return false;
    else//如果结点存在
    {
        BNode* p = loc.r;
        int j = loc.id;
        Wh_C.lock();
        --B_num;
        Wh_C.unlock();
        Wh_C.lock();
        Wh_BTreeRemoveKey(p, j);
        Wh_C.unlock();
        return true;
    }
}
//更新结点,如果更新成功，则返回true
bool BTree::Wh_UpdateNode(const int &id,const int& sum)
{
    Triple t=Wh_BTreeSearch(id);//搜素结点
    if(t.tag==0)//没有找到对应结点
    {
        return false;
    }
    else//找到结点
    {
        if(!t.r->P_data[t.id]->WhC_decrease(sum))//如果数量超过的话，则返回false
        {
            return false;
        }
        else
        {
            if(t.r->P_data[t.id]->WhC_ReSum()==0)//如果没有商品了，则直接删除这个节点
                Wh_BTreeRemove(id);
            return true;
        }
    }
}
//将单个商品添加到仓库中
void BTree::Wh_InsertdataNode(const int& id,Commondity c)
{
    Triple t=Wh_BTreeSearch(id);//搜索结点
    t.r->P_data[t.id]->WhC_insert(c);//插入数据
}
//删除某一个特定的商品，i为要删除的位置，如果整个节点都被消除则返回false
bool BTree::Wh_deletedataNode(const int& id,const int& i)
{
    Triple t=Wh_BTreeSearch(id);//搜索结点 
    t.r->P_data[t.id]->WhC_delete(i);//删除结点
    if(t.r->P_data[t.id]->WhC_len()==0)
    {
        Wh_BTreeRemove(id);//直接将节点移除
        return false;
    }
    return true;
}
//返回对应商品的利润
float BTree::Wh_profit(const int &id)
{
    Triple p=Wh_BTreeSearch(id);//搜索位置
    return p.r->P_data[p.id]->WhC_Reprofit();//返回该id下的利润
}
//修改对应商品的利润,成功则返回true
void BTree::Wh_ChaProfit(const int &id,const float &i)
{
    Triple p=Wh_BTreeSearch(id);//搜索位置
    p.r->P_data[p.id]->WhC_ChaProfit(i);
}
//用来遍历整个树
Wh_Chain* BTree::Wh_list(bool restrat)
{
    static int place=0;
    static std::stack<BNode*> s;
    if(restrat)
    {
        //重置
        place=0;
        while (!s.empty())
            s.pop();
        s.push(root);
        return NULL;
    }
    else
    {
        BNode *q=s.top();
        Wh_Chain*p= q->P_data[place];
        if(q->P_data[place+1]==NULL)
        {
            s.pop();
            for(int i(0);i<place+2;++i)
            {
                if(q->kids_ptr[i]!=NULL)
                    s.push(q->kids_ptr[i]);
            }
            place=0;//重置为0
        }
        else
            ++place;
        return p;
    }
}
