

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<stack>
//保存数据权值的节点
struct WeightNode
{
    char c;//所保存的字符
    int weight;//权值
    int heigh;//以其为根的子树的高度 
    int posection;//自己在容器中的位置（用于数据的插入）
    WeightNode *lchild,*rchild,*parent;//左右子树，以及父节点
    WeightNode(const char &c1='\0'):c(c1),weight(1),heigh(1),lchild(NULL),rchild(NULL),parent(NULL){}//\0表示空的节点
};
//编码节点
struct Codedata
{
    unsigned int data;//编码
    int headempty;//0的数量
    int length;//位数
    Codedata(int d,int e,int l):data(d),headempty(e),length(l){}
};
//Huffman树类,链式结构
class Huffman 
{
    public:
        Huffman() :negative(0), recode(0), Ehead(0), root(NULL) {}
    ~Huffman();//析构函数
    void getMessage(const std::string s);//获取用来建树的字符串
    void combine(int i);//将两个结点合并
    void handling();//计算字符串中的加权
    void initialization();//huffman树的初始化
    int maxheigh(int i);//返回最大的树深
    void coding();//生成编码
    void HuffmanInsert(WeightNode *p);//将结点插入加入到权值的结合中
    void Huffmandestroy(WeightNode *r);//销毁二叉树
    void Encoding();//将字符串转换为编码
    void Decoding(std::string c,std::string &s);//将编码转换为字符串
    void OutputCode(std::string &code);//输出编码
    void OutputStr();//输出字符串
    void PrintHuffmanTree(WeightNode* T,int level=1);
    void PrintHuffmanTree();
    friend void getHuffman();
    private:
    int negative;//负值数
    unsigned long int recode;//最终的编码
    int Ehead;//开头零的数目
    std::vector<WeightNode *> v;//用来保存huffman树的子树们
    std::unordered_map<char,WeightNode*> m;//用哈希表来建立字典来搜索已有字符
    std::unordered_map<char,Codedata*> cm;//用来查询编码
    std::string HuffmanStr;//用来接收字符串
    std::string Huffmancode;//用来接收编码
    WeightNode *root;//huffman树的根节点
};
