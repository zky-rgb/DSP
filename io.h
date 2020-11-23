#pragma once
#include <iostream>
#include <string>
#include<fstream>
#include"huffman.h"
class IO
{
public:
	IO(Huffman* h):huffmantree(h){}
	void readCharacter();//读取字符串
	void weight();//取字符，创建数组读取权值
	void getHuffman();//获取哈夫曼树
	void getHuffman(WeightNode* T, int level);
	void readMessage();//读取哈夫曼树和电文原文
	void readySend();//取得编码后的电文
	void writeMessage();//取得解码后电文
	void writeHuffmantree();//打印哈夫曼树
	void interface();//打开用户的交互界面
	void readPassword();//读取待解码的密码
	void finishDecode();//存储解码后的密码
	void FR();
private:
	Huffman* huffmantree;//保存的huffman树类
	std::string message;//保存用户输入的建树字符串
	std::string encode;//获得编码后的电文
	std::string decode;//译码后的字符串
	std::string outputtree;
};
	