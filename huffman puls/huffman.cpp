#include"huffman.h"
//析构函数
Huffman::~Huffman()
{
    //析构函数     
    Huffmandestroy(root);
}
//销毁二叉树
void Huffman::Huffmandestroy(WNode *p)
{   
    if(p==NULL)
    {
        return;
    }
    else
    {
        //后续遍历来销毁哈夫曼树
        Huffmandestroy(p->lchild);
        Huffmandestroy(p->rchild);
        delete p;
    }
}
//计算字符串的加权
void Huffman::handling()
{
    char c;
    std::unordered_map<char,int> weight;//用来记录加权
    for(int i(0);i<HuffmanStr.length();++i)//读取字符串
    {
        c=HuffmanStr[i];
        if(weight.find(c)==weight.end())//如果不存在
        {
            weight[c]=1;//将节点加入
        }
        else
        {
            ++weight[c];//加权递增
        }
    }
    coding(weight);
}
//生成编码
void Huffman::coding(std::unordered_map<char,int>& weight)//生成加权
{
    using namespace std;
    
    list<WNode> l1;
    for(auto &a:weight)//将权值入队
    {
        WNode w(a.first,a.second);   
        l1.push_front(w);
    }
    weight.clear();//释放空间
    l1.sort();//进行排序
    list<WNode* > l2;//使用归并来排序
    
    //对于l2进行初始化
    WNode* p1=new WNode(l1.front().c,l1.front().weight);
    l1.pop_front();//弹出第一个
    WNode* p2=new WNode(l1.front().c,l1.front().weight);
    l1.pop_front();//弹出第一个
    WNode* p3=new WNode;
    p3->weight=p1->weight+p2->weight;
    p3->lchild=p1;
    p3->rchild=p2;
    p1->parent=p3;
    p1->codenum=false;
    p2->parent=p3;
    p2->codenum=true;
    l2.push_back(p3);
    
    //循环插入
    while(!(l2.size()==1&&l1.empty()))//只要没有擦插入完
    {
        p3=new WNode;//用来插入的节点
        for(int i(0);i<2;i++)//循环两次
        {
            if(!l1.empty())
            {
                if(l1.front().weight<=l2.front()->weight)
                {
                    p1=new WNode(l1.front().c,l1.front().weight);
                    if(i==0)
                    {
                        p3->weight=p1->weight;
                        p3->lchild=p1;
                        p1->codenum=false;
                    }
                    else
                    {
                        p3->weight+=p1->weight;
                        p3->rchild=p1;
                        p1->codenum=true;
                    }
                    p1->parent=p3;
                    l1.pop_front();
                }   
                else
                {
                    if(i==0)
                    {
                        p3->weight=l2.front()->weight;
                        p3->lchild=l2.front();
                        l2.front()->codenum=false;
                    }
                    else
                    {
                        p3->weight+=l2.front()->weight;
                        p3->rchild=l2.front();
                        l2.front()->codenum=true;
                    }
                    l2.front()->parent=p3;
                    l2.pop_front();
                }
            }
            else
            {
                if(i==0)
                {
                    p3->weight=l2.front()->weight;
                    p3->lchild=l2.front();
                    l2.front()->codenum=false;
                    
                }
                else
                {
                    p3->weight+=l2.front()->weight;
                    p3->rchild=l2.front();
                    l2.front()->codenum=true;

                }
                l2.front()->parent=p3;
                l2.pop_front();
            }
        }
        l2.push_back(p3);//加入到l2中
    }
    //将最后的跟结点保存
    root=l2.front();
    creatCodetable();//生成编码表
    for(char c:HuffmanStr)
    {
        encode.insert(encode.end(),CharCode[c].begin(),CharCode[c].end());
    }
}
//生成对应的字符编码表和译码表
void Huffman::creatCodetable()
{
    std::vector<bool> v;
    WNode *ptr(root),*r;
    WNode *q;
    //从根节点开始遍历
    std::stack<WNode*> s;
    bool flag;//标志位
    s.push(ptr);
    if(ptr!=NULL)
    {
        do
        {
            while(ptr!=NULL)
            {
                s.push(ptr); 
                if(ptr!=root)
                    v.push_back(ptr->codenum);
                ptr=ptr->lchild;
            }
            flag=true;
            r=NULL;
            while(!s.empty()&&flag)
            {
                q=s.top();
                if(q->c!='\0')//不为空节点
                {
                    CharCode[q->c]=v;//写入表中
                    CodeChar[v]=q->c;
                }
                if(q->rchild==r)
                {
                    s.pop();
                    if(!v.empty())
                        v.pop_back();//退位
                    r=q;
                }
                else
                {
                    ptr=q->rchild;
                    flag=false;
                }
            }
        } while (!s.empty());
    }
}
void Huffman::Decoding(std::string &in,std::string &out)
{
    std::vector<bool> bin;//一个容器，用来比较
    out="";
    for(char c:in)//遍历给予的编码
    {
        c=='1'?bin.push_back(true):bin.push_back(false);
        if(CodeChar.find(bin)!=CodeChar.end())
        {
            out+=CodeChar[bin];
            bin.clear();
        }
    }
}
