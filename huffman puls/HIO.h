#include"huffman.h"
class Huffman;
class WeightNode;
class PIO {
public:
    PIO(Huffman* h):huffmantree(h){};
    void interface(); //与用户的接口
    void input();    //将数据传入到huffman树里
    void outputCode();//输出字符
    void inputProcess();//从文件中读取二进制编码

private:
    Huffman* huffmantree;
    std::string message;//
    std::string encode;//��������Ϣ
    std::string decode;//�������Ϣ
    WeightNode* inroot; //���ļ���ȡ������������������
    std::string fileName;
    std::string recover;//��Ҫ�ָ��Ķ����Ʊ���
};
