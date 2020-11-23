#include <string>
#ifndef __DNF__
#define __DNF__

class Tree;
struct Node{
    std::string data;
    std::string ip; //仅最底层叶子结点需要
    Node* parent;   //上层节点
    int pos;//父节点的位置,从0开始
    Node* firstchild;
    Node* nextsibling;
    Node(std::string d="\0",std::string i="\0"):data(d),
    ip(i),firstchild(NULL),nextsibling(NULL){}
};
struct Tetrad//搜索节点	
{
   Node* p;//搜索到的节点的地址
   bool tag;//是否搜索到,true为搜索到
   int position;//所在层的位置,从零开始时
   int deep;//当前深度
};
class Tree{
public:
    Tree();
    void CreateTree(std::string dNamePart[],std::string ip);//s建树
    Tetrad searchPath(std::string *str);//用来进行搜索，如果搜索到了路径，则tag为1,否则为0
    bool deletePath(std::string *str);//删除已有的路径
    void deleteNode(Node *p,int i);//删除对应的节点
    void PrintTree();   //屏幕打印树整个域名树
private:
    Node* root;  
    Node* level[4]; //保存每一层首个元素地址
    Node* pre[4];   //建树插入过程中间变量
};




#endif
