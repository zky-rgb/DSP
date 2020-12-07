//
// Created by 杨业卿 on 2020/11/30.
//

#include <iostream>
#include<fstream>
#include <vector>
#include <sstream>

class Huffman;
class WeightNode;

//using namespace std;

class PIO{
public:
    PIO(){
        huffmantree = NULL;
    };
    void interface(); //用户界面信息
    void input();    //从文件读取要解压缩的txt文件 并建立哈夫曼树
    void outputCode();//将编码压缩后字符串存储到文件
    void inputProcess();//将输入的哈夫曼树建立并将二进制字符串译码
    void outputTree(WeightNode* T,int level=1);//递归遍历并将哈夫曼树中缀输出到文件



private:
    Huffman *huffmantree;
    std::string message;//保存用户输入的或文件读入的语句
    std::string encode;//编码后的信息
    std::string decode;//解码后信息
    WeightNode* inroot; //从文件读取后建立哈夫曼树的树根

};



void PIO ::input() {
    std::ifstream in("tree.txt",std::ios::in); //创建文件读写

    std::stringstream buffer;//利用stringsteam字符流来临时存储读到的txt文件内容
    buffer<<in.rdbuf();
    std::string str(buffer.str());
    message = str;//将读到的内容传递给私有数据乘员message
    std::cout<<message;

    in.close();

}

void PIO ::outputCode() {
    std::ofstream out("out.dat",std::ios::out);//采用二进制输出方式
    out<<encode;//输出编码后的二进制信息
    out.close();
}


void PIO ::interface() {
    
}

