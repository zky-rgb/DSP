#include"huffman.h"
#include<cmath>
//析构函数
Huffman::~Huffman()
{
    //析构函数     
    Huffmandestroy(root);

}
//销毁二叉树
void Huffman::Huffmandestroy(WeightNode *p)
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
//返回最大的树身
int Huffman::maxheigh(int i)
{
    if(v[i]->heigh>v[i+1]->heigh)
        return v[i]->heigh;
    else
        return v[i+1]->heigh;
}
//将两个子树合并,i,j是其在vector容器中的角标
void Huffman::combine(int i)
{
    //将两个结点合并
    WeightNode *p =new WeightNode;
    //将两个结点合并
    p->lchild=v[i];
    p->rchild=v[i+1];
    //将两个点的权值相加
    p->weight=v[i]->weight+v[i+1]->weight;
    //写入p的高度
    p->heigh=maxheigh(i)+1;
    v[i]->parent=p;
    v[i+1]->parent=p;
    //将原来的两个节点删除
    v.erase(v.begin()+i,v.begin()+i+2);
    //将新的结点插入vector
    HuffmanInsert(p);
}
//将结点插入
void Huffman::HuffmanInsert(WeightNode *p)
{
    if(v.size()>0)//有节点
    {        
        //判断插入种类
        if(p->c!='\0')//插入的是叶节点
        {
            if(m.find(p->c)==m.end())//没有该字符
            {
                v.insert(v.begin(),p);//将节点插入vector
                p->posection=0-negative;//设置节点的位置
                ++negative;
                m[p->c]=p;//加入节点
            }
            else//有该字符
            {
                WeightNode *q;
                q=m[p->c];//用哈希表找到该字符
                ++(v[q->posection+negative-1]->weight);//增加其权值
                if(v.size()>1)
                {
                    for (int i(q->posection+negative); i < v.size(); ++i)
                    {
                        if (v[i]->weight < v[i - 1]->weight)
                        {
                            q = v[i];
                            v[i] = v[i - 1];
                            v[i - 1] = q;
                            int i1 = v[i]->posection;
                            v[i]->posection = v[i - 1]->posection;
                            v[i - 1]->posection = i1;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                delete p;//将p的空间释放
            }
        }
        else//是空字符的情况
        {
            bool ifinsert(false);
            for(int i(0);i<v.size();++i)
            {
                if(v[i]->weight>p->weight)
                {
                    v.insert(v.begin()+i,p);
                    ifinsert=true;
                    break;
                }
                else if(v[i]->weight==p->weight)
                {
                    if(p->heigh<=v[i]->heigh)
                    {
                        v.insert(v.begin()+i,p);
                        ifinsert=true;
                        break;
                    }
                    else
                    {
                        continue;
                    }      
                }
                else
                {
                    continue;
                }   
            }
            if(!ifinsert)
            {
                v.push_back(p);
            }
        }
    }
    else//没有节点，则直接插入
    {
        v.push_back(p);//将节点加入到vector中
        if(p->c!='\0')
        {
            m[p->c]=p;//将值插入哈希表
            p->posection=0;
            ++negative;
        }
    }    
}
//huffman树的初始化
void Huffman::initialization()
{
    handling();
    while(v.size()>1)
    {
        combine(0);//合并
    }
    root=v[0];
    coding();//编码
}
//计算字符串的加权
void Huffman::handling()
{
    WeightNode *p;
    for(int i(0);i<HuffmanStr.length();++i)//读取字符串
    {
        p=new WeightNode(HuffmanStr[i]);
        HuffmanInsert(p);//插入数据
    }
}
//生成编码
void Huffman::coding()//生成加权
{
    using namespace std;
    
    stack<WeightNode*> s;
    if(root!=NULL)//节点不为空
    {
        int d(0);
        int h(-1);
        int l(0);
        WeightNode *q,*r;
        Codedata *p;
        //进行后序遍历
        bool flag(false);
        q=root;
        do 
        {
            //循环遍历p的左子树
            while(q!=NULL)
            {
                s.push(q);
                q=q->lchild;
                if(d==0)
                {                 
                  ++h;//递增前面的零位                                                   
                }
                else
                {
                    d*=2;//d的位数加一
                }
                ++l;
            }
            //初始化
            r=NULL;
            flag=true;
            while(!s.empty()&&flag)
            {
                q=s.top();
                if(q->rchild==r)//如果其右子节是刚刚访问的节点或者为空
                {
                    s.pop();
                    if(q->c!='\0')//有效字符
                    {
                        p=new Codedata(d,h,l);
                        cm[q->c]=p;//将字符添加至哈希表
                        r=q;
                    }
                    else
                    {
                        r=q;
                        continue;
                    }
                    if(d!=0)
                        d/=2;//d退位
                    else
                    {
                        --h;
                    }
                    --l;
                }
                else
                {
                    //访问其右节点
                    q=q->rchild;
                    flag=false;
                    d=d*2+1;//编码加一位
                    ++l;
                }   
            }
        } while (!s.empty());
    }
    else
    {
        cout<<"The huffman tree is empty"<<endl;
    }
}
//将字符串编码
void Huffman::Encoding()
{
    Codedata *p;
    for(int i(0);i<HuffmanStr.length();++i)//遍历字符串
    {
        p=cm[HuffmanStr[i]];//查询
        if(recode==0)
        {
            Ehead+=p->headempty;
            recode+=p->data;
        }
        else
        {
            recode*=pow(2,p->length);
            recode+=p->data;
        }
    }
}
//将编码输出
void Huffman::OutputCode(std::string &code)
{
    using namespace std;
    
    cout<<"The string is:";
    cout<<HuffmanStr<<endl;
    cout<<"The huffman code is:"<<endl;
    stack<int> s;
    int j(0);   
    for(int i(0);i<HuffmanStr.length();++i)
    {
        for(int i1(0);i1<cm[HuffmanStr[i]]->headempty;++i1)
        {
            code.push_back('0');
            cout<<"0";
        }
        j=cm[HuffmanStr[i]]->data;
        while(j!=0)
        {
            s.push(j%2);
            j/=2;//退位
        }
        while (!s.empty())
        {
            if(s.top()==1){
                code.push_back('1');
            } else{
                code.push_back('0');
            }
            cout<<s.top();  
            s.pop();
        }
    }
}
//译码
void Huffman::Decoding(std::string c, std::string& s)
{
    Huffmancode = "";
    int d(0);//存储数值
    int h(0);//存储零的数量
    for (int i(0); i < c.length(); ++i)//遍历编码
    {
        switch (c[i])
        {
        case '0'://如果为零
        {
            if (d == 0)
            {
                h++;
            }
            else
            {
                d = d * 2;
            }
            for (auto& x : cm)
            {
                if (x.second->data == d && x.second->headempty == h)
                {
                    Huffmancode += x.first;
                    //重置
                    d = 0;
                    h = 0;
                    break;
                }
            }
            break;
        }
        case '1'://如果为1
        {
            d = d * 2 + 1;//增加一位
            for (auto& x : cm)
            {
                if (x.second->data == d && x.second->headempty == h)
                {
                    Huffmancode += x.first;
                    //重置
                    d = 0;
                    h = 0;
                    break;
                }
            }
            break;

        }
        }
        
    }
    /*std::cout << "string is:" << std::endl;
    std::cout << Huffmancode << std::endl;
    s = Huffmancode*/;
}
//获取字符串
void Huffman::getMessage(const std::string s)
{
    HuffmanStr=s;
}


//打印哈夫曼树
void Huffman ::PrintHuffmanTree(WeightNode* T,int level) {

    if(T==NULL){
        return;
    }
    PrintHuffmanTree(T->rchild,level+1);
    for(int i=0;i<level;i++){
        std::cout<<" ";
    }
    std::cout<<T->c<<std::endl;
    PrintHuffmanTree(T->lchild,level+1);


}

void Huffman ::PrintHuffmanTree() {
    PrintHuffmanTree(root,1);
}
