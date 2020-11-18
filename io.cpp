// io.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"io.h"
	void IO::readCharacter() {//读取字符集
		using namespace std;
		cout<<"建立Huffman树:";
		cout<<"请输入字符串以建立huffman树"<<endl;
		message = "hello";
		//getline(cin,message);
		
		cout<<"读取成功！";
		huffmantree->getMessage(message);
		huffmantree->initialization();
		huffmantree->OutputCode(encode);
		huffmantree->Decoding(encode,decode);
	}
	
	void IO::getHuffman() {//存储哈夫曼树
		std::ofstream out("huffman.txt", std::ios::out);
		out<< outputtree <<std::endl;
		out.close();
		system("pause");
		system("cls");
		interface();
	}
	void IO::readMessage(){//读取未编码的发送电文
		std::cout << "ReadMessage" << std::endl;
		std::ifstream in("huffman.txt", std::ios::in);
		in >> outputtree;
		in.close();
		std::ifstream rein("tobetrans.dat", std::ios::in);
		in >> message;
		//std::cin.sync()
		in.close();
		system("pause");
		system("cls");
		interface();
	}
	void IO::readySend() {//存储编码后的电文
		std::ofstream out("codefile.txt", std::ios::out);
		out << encode<<std::endl;
		out.close();
	}
	void IO::readPassword() {//读取未解码的密码
		std::ifstream in("codefile.txt", std::ios::in);
		in >> encode ;
		in.close();
	}
	void IO::finishDecode() {
		std::ofstream out("txtfile.txt", std::ios::out);
		out << decode << std::endl;
		out.close();
	}
	void IO::writeMessage() {//输出电文
		std::cout << "输出电文" << std::endl;
		std::ofstream reout("codeprint.txt", std::ios::out);
		for (int i = 0; encode[i] != '\0'; i++) {
			if (i % 50 == 0) {
				std::cout << std::endl;
				reout << std::endl;
			}
			std::cout << encode[i];
			reout << encode[i];
		}
		reout.close();
	}
	void IO::writeHuffmantree() {//打印哈夫曼树
			std::cout<<"printHuffmantree"<<std::endl;
			std::ofstream out("treeprint.txt", std::ios::out);

			

			std::cout << outputtree << std::endl;
			out << outputtree << std::endl;
			out.close();
		system("pause");
		system("cls");
		interface();
	}
	
void IO::interface() {
	int choice;
	std::cout << "欢迎使用哈夫曼解编码器" << std::endl;
	std::cout << "1.建立哈夫曼树,并创建哈夫曼编码" << std::endl;
	std::cout << "2.对文件进行编码" << std::endl;
	std::cout << "3.对文件进行译码" << std::endl;
	std::cout << "4.打印密码文件" << std::endl;
	std::cout << "5.打印哈夫曼树" << std::endl;
	std::cin >> choice;
	system("cls");
	switch (choice) {
	case 1:
		readCharacter();
		getHuffman();
		break;
	case 2:
		readMessage();
		readySend();
		break;
	case 3:
		readPassword();
		finishDecode();
		break;
	case 4:
		writeMessage();
		break;
	case 5:
		writeHuffmantree();
		break;
	}
}
 