#pragma once
#include <iostream>
#include <string>
#include<fstream>
#include"huffman.h"
class IO
{
public:
	IO(Huffman* h):huffmantree(h){}
	void readCharacter();//��ȡ�ַ���
	void weight();//ȡ�ַ������������ȡȨֵ
	void getHuffman();//��ȡ��������
	void getHuffman(WeightNode* T, int level);
	void readMessage();//��ȡ���������͵���ԭ��
	void readySend();//ȡ�ñ����ĵ���
	void writeMessage();//ȡ�ý�������
	void writeHuffmantree();//��ӡ��������
	void interface();//���û��Ľ�������
	void readPassword();//��ȡ�����������
	void finishDecode();//�洢����������
	void FR();
private:
	Huffman* huffmantree;//�����huffman����
	std::string message;//�����û�����Ľ����ַ���
	std::string encode;//��ñ����ĵ���
	std::string decode;//�������ַ���
	std::string outputtree;
};
	
