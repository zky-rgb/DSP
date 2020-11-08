#include <iostream>

using namespace std;
class HTree;

class HNode{
public:
    friend class HTree;
    HNode(){
        wei=0;
        data=0;
        lchild = rchild=NULL;
    }
private:
    int wei;
    char data;
    HNode* lchild,*rchild,*parent;


};

class HTree{
public:
    HTree();
    bool CreateHTree(int weight[26]);
    void mergeTree(HNode *&first,HNode *&second,HNode*&parent);

private:
    HNode* root;
    int leafnum=26;

};

int main() {

    return 0;
}


bool HTree ::CreateHTree(int weight[26]) {

}

void HTree ::mergeTree(HNode *&first, HNode *&second, HNode *&parent) {
    parent = new HNode;
    parent->lchild = first;
    parent->rchild = second;
    parent->data = first->data+second->data;
    first->parent = second->parent = parent;
}