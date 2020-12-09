#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<stack>
#include<list>

struct WNode
{
    char c;//所保存的字符
    int weight;//权值
    bool codenum;//结点对应值
    WNode *lchild,*rchild;//左右节点
    WNode *parent;//父节点
    WNode (const char c1='\0',const int i=1):c(c1),weight(i),lchild(NULL),rchild(NULL){}
    bool operator < (const WNode & a) const//重载运算符
	{
		return weight<a.weight;
	}
};
//Huffman树类,链式结构
class Huffman 
{
    public:
        Huffman() :root(NULL) {}
    ~Huffman();//析构函数
    void getMessage(const std::string s)//获取用来建树的字符串
    {
        HuffmanStr=s;
    }
    void handling();//计算字符串中的加权
    void coding(std::unordered_map<char,int>& weight);//生成编码
    void creatCodetable();//生成对应的huffmam
    void Huffmandestroy(WNode*r);//销毁二叉树
    void Decoding(std::string &c,std::string &s);//将编码转换为字符串
    std::vector<bool> OutputCode()//输出编码
    {
        return encode;
    }
    private:
    std::vector<bool> encode;//最终编码
    std::unordered_map<char,std::vector<bool> > CharCode;//用来存储每个字符所对应的编码
    std::unordered_map<std::vector<bool>,char> CodeChar;//反向编码的表
    std::string HuffmanStr;//用来接收字符串
    WNode *root;//huffman树的根节点
};
