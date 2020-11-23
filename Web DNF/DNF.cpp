#include "DNF.h"

Tree ::Tree() {
    Node* a = new Node;
    a->data='0';
    a->firstchild = a->nextsibling = NULL;
    root = a;

    for(int i=0;i<4;i++){
        level[i] = new Node;
        pre[i] = level[i];
        level[i]->data = "0";
        a->firstchild = level[i];
        a = level[i];
    }
}


void Tree ::CreateTree(std::string dNamePart[],std::string ip) {
    for(int i=3;i>-1;i--){
        Node* t = new Node;
        pre[i]->nextsibling = t;
        t->data = dNamePart[i];
        pre[i] = t;

        if(i==0){
            t->ip = ip;
        }
    }

}
//用来搜索需要的节点
Tetrad Tree::searchPath(std::string *str)
{

    Node* ptr(root);//游走指针
    int effectiveSum;//有效数据段的数量
    //确定末尾的有效位
    for(effectiveSum=3;effectiveSum>0;--effectiveSum)
    {
        if(str[effectiveSum]=="\0")
            continue;
        else
            break;
    }
    Tetrad t;
    int i(0),j(0);
    while(effectiveSum>-1)//有需要处理的节点
    {
        if(ptr->firstchild!=NULL)//搜索到的该节点有子节点
        {
            Node*p=ptr;
            ptr=ptr->firstchild;//到下一层
            i=0;
            bool iffind(false);
            while(ptr!=NULL)
            {
                if(ptr->data==str[effectiveSum])//可以对应到
                {
                    iffind=true;
                    break;//退出循环
                }
                else//没有，则接着往下找
                {
                    i++;
                    ptr=ptr->nextsibling;
                }
            }
            if(!iffind)
            {
                t.deep=effectiveSum+1;
                t.p=p;
                t.position=j;
                t.tag=false;
                return t;
            }
        }
        else//没有该节点,直接退出
        {
            t.deep=4-(effectiveSum+1);//当前深度
            t.p=ptr;
            t.position=j;
            t.tag=false;
            return t;
        }
        j=i;//保存上一层的位置
        --effectiveSum;
    }
    t.deep=4-(effectiveSum+1);//当前深度
    t.p=ptr;
    t.position=j;
    t.tag=true;
    return t;//寻找成功
}
//删除一个域名，如果删除失败，则返回true;
bool Tree::deletePath(std::string *str)
{
    Tetrad t=searchPath(str);//搜索节点
    
    if(!t.tag)//如果没有搜索到
    {
        return false;//
    }
    else
    {
        deleteNode(t.p,t.position);
        return true;
    }
}
//删除节点
void Tree::deleteNode(Node *p,int pos)
{
    if(pos==0)//为该层的第一个
    {
        if(p->nextsibling!=NULL)//还有别的兄弟节点
        {
            p->parent->firstchild=p->nextsibling;
            Node *q(p->nextsibling);
            while(q!=NULL)
            {
                if(q->firstchild!=NULL)
                {
                    Node *q1(q->firstchild);
                    while(q1!=NULL)
                    {
                        ++q1->pos;
                        q1=q1->nextsibling;
                    }
                }
                q=q->nextsibling;
            }
            delete p;//删除p      
        }
        else
        {
            deleteNode(p->parent,p->pos);
            delete p;
        }
    }
    else
    {
        Node *q1(p->nextsibling);
        while(q1!=NULL)
        {
            if(q1->firstchild!=NULL)
            {
                Node *q2(q1->firstchild);
                while(q2!=NULL)
                {
                    ++q2->pos;
                    q2=q2->nextsibling;
                }
            }
            q1=q1->nextsibling;
        }
        Node *q(p->parent->firstchild);
        for(int i(0);i<pos-1;++i)
        {
            q=q->nextsibling;
        }
        q->nextsibling=p->nextsibling;
        delete p;//删除节点
    }
    
}
/*void Tree ::PrintTree() {
    Node* p=new Node;
    for(int i=3;i>-1;i--){
        p=level[i];
        while (p!=NULL){
            if(p->data=="0"){

            } else{
                std::cout<<p->data<<" ";
            }
            p = p->nextsibling;

        }
        std::cout<<std::endl;
    }

}*/
