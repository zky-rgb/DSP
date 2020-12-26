#include "DNF.h"
#include <iostream>
#include <queue>

Tree ::Tree() {
    Node* a = new Node;
    a->data=' ';
    a->firstchild = a->nextsibling = NULL;
    root = a;

    /*for(int i=0;i<4;i++){
        level[i] = new Node;
        pre[i] = level[i];
        level[i]->data = " ";
        a->firstchild = level[i];
        a = level[i];
    }*/

    pre = root;
}


void Tree ::CreateTree(std::string dNamePart[],std::string ip) {
    int depth=0;
    pre = root;
    for(int i=dNamePart->size()-1;i>-1;i--){
        /*if(searchPath(dNamePart).tag==0){
            Node* t = new Node;
            t->data = dNamePart[i];

        }*/

        if(CSearch(dNamePart[i],&depth,4-i)==NULL){
            continue;
        } else{
            /*if(depth==0){
                Node* n = new Node;
                n->Depth = 4-i;
                n->data = dNamePart[i];
                pre->nextsibling = n;
                pre = n;
            }*/
            if(depth==3-i){
                Node* n = new Node;
                n->Depth = 4-i;
                n->data = dNamePart[i];
                pre->nextsibling = n;
                pre = n;
                if(i==0){
                    n->ip = ip;
                }
            } else{
                Node* n = new Node;
                n->Depth = 4-i;
                n->data = dNamePart[i];
                pre->firstchild = n;
                pre = n;
                if(i==0){
                    n->ip = ip;
                }
            }


        }


    }

}

bool Tree ::JudgeTreeEmpty() {
    if(root->nextsibling==NULL){
        return 1;
    } else{
        return 0;
    }
}

void Tree ::Printt(Node *p) {
    if(p==NULL){
        return;
    }






    Printt(p->nextsibling);
    std::cout<<p->data<<" ";
    std::cout<<std::endl;
    Printt(p->firstchild);



}

void Tree ::PrintTree() {

    if(root!=NULL){
        /*while (pre->nextsibling != NULL || pre->firstchild != NULL) {



        }*/

        Printt(root);


    }
}


//用来搜索需要的节点
Tetrad Tree::searchPath(std::string str[4])
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

    if(ptr==NULL){

    }


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

Node* Tree ::CSearch(std::string key,int *depth,int d) {

    //Node* N;
    //pre = pre->firstchild;
    Node *p1, *p2;

    if (pre->nextsibling == NULL && pre->firstchild == NULL) {
        return pre;
    }   //退出查找条件


    p1 = pre->firstchild;
    p2 = pre->nextsibling;

    if (d==*depth) {
        while (pre->nextsibling != NULL || pre->firstchild != NULL) {

            /*if (pre->firstchild == NULL) {
                pre = pre->nextsibling;
            } else {
                pre = pre->firstchild;
                ++*depth;
            }*/
            /*if(pre->nextsibling==NULL){
                return pre;
            }*/
            pre = pre->nextsibling;

            if (pre->data == key) {
                return NULL;
            }

        }

        if(pre->nextsibling==NULL && pre->firstchild==NULL){
            return pre;
        }

    }

    if (p1 != NULL && d>pre->Depth) {
        while (pre->nextsibling != NULL || pre->firstchild != NULL) {

            if (pre->firstchild == NULL) {
                pre = pre->nextsibling;
            } else {
                pre = pre->firstchild;
                ++*depth;

            }

            if (pre->data == key) {
                return NULL;
            }

        }

        if(pre->nextsibling==NULL && pre->firstchild==NULL){
            return pre;
        }

    }


}

bool Tree ::Search(std::string dNamePart[]){
    int depth=0;
    pre = root;
    int mark=0;

    for(int i=dNamePart->size()-1;i>-1;i--){
        if(CSearch(dNamePart[i],&depth,4-i)==NULL) {
            mark++;
        }
    }

    if(mark == dNamePart->size()){
        return 1;
    } else{
        return 0;
    }
}

void Tree ::InData(std::string *dNamePart) {

    domin[0].push_back(dNamePart[0]);
    domin[1].push_back(dNamePart[1]);
    domin[2].push_back(dNamePart[2]);
    domin[3].push_back(dNamePart[3]);
}

void Tree ::Print() {
    std::cout<<domin[3][1]<<" ";
    for(int i=0;i<domin[3].size();i++){
        if(domin[3][i]!=domin[3][i-1]&&i>1)
            std::cout<<domin[3][i]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<domin[2][1]<<" ";
    for(int i=0;i<domin[2].size();i++){
        if(domin[2][i]!=domin[2][i-1]&&i>1)
            std::cout<<domin[2][i]<<" ";
    }
    std::cout<<std::endl;
    for(int i=0;i<domin[1].size();i++){
        if(domin[1][i]!=domin[1][i-1]&&i>1)
            std::cout<<domin[1][i]<<" ";
    }
    std::cout<<std::endl;


    for(int i=0;i<domin[0].size();i++){
            std::cout<<domin[0][i]<<" ";
    }
    std::cout<<std::endl;

}