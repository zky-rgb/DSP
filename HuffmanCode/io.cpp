// io.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"io.h"
void IO::FR() {//检查树是否存在
	std::ifstream in("huffman.txt", std::ios::in);
	in >> outputtree;
	in.close();
	if (outputtree == "") {
		std::cout << "请先建树再进行其他操作" << std::endl;
		interface();
	}
}
	void IO::readCharacter() {//读取字符集
		using namespace std;
		cout<<"建立Huffman树:";
		cout<<"请输入字符串以建立huffman树"<<endl;
		
		message="";
		cin.get();
		getline(cin, message);
		
		cout<<"读取成功！";
		huffmantree->getMessage(message);
		huffmantree->initialization();
		
		
		cout << endl;
	}
	
	void IO::getHuffman() {//存储哈夫曼树
		std::ofstream out("huffman.txt", std::ios::out);
		out<< outputtree <<std::endl;
		out.close();
		system("pause");
		system("cls");
		interface();
	}

	void IO::getHuffman(WeightNode* T, int level) {
		std::ofstream out("huffman.txt", std::ios::out);
		if (T == NULL) {
			return;
		}
		getHuffman(T->rchild, level + 1);
		for (int i = 0; i < level; i++) {
			out << " ";
		}
		out << T->c << std::endl;
		getHuffman(T->lchild, level + 1);
	}

	void IO::readMessage(){//读取未编码的发送电文
		FR();
		std::cout << "ReadMessage" << std::endl;
		std::ifstream in("huffman.txt", std::ios::in);
		in >> outputtree;
		in.close();
		std::ifstream rein("tobetrans.dat", std::ios::in);
		if (!rein.is_open()) {
			std::cout << "文件不存在,请手动输入未编码的发送电文" << std::endl;
			std::cin >> message;
		}
		in >> message;
		if (message == "") {
			std::cout << "文件为空，请手动输入未编码的电文" << std::endl;
			std::cin >> message;
		}
		//std::cin.sync()
		in.close();
		huffmantree->OutputCode(encode);
		system("pause");
		system("cls");
	}
	void IO::readySend() {//存储编码后的电文
		std::ofstream out("codefile.txt", std::ios::out);
		out << encode<<std::endl;
		out.close();
		system("pause");
		system("cls");
		interface();
	}
	void IO::readPassword() {//读取未解码的密码
		FR();
		std::ifstream in("codefile.txt", std::ios::in);
		if (!in.is_open()) {
			std::cout << "文件不存在,请手动输入未解码的密码" << std::endl;
			std::cin >> message;
		}
		in >> encode;
		if (encode == "") {
			std::cout << "文件为空，请手动输入未解码的密码" << std::endl;
			std::cin >> message;
		}
		in.close();
	}
	void IO::finishDecode() {
		huffmantree->Decoding(encode, decode);
		std::ofstream out("txtfile.txt", std::ios::out);
		out << decode << std::endl;
		out.close();
	}
	void IO::writeMessage() {//输出电文
		FR();
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
		FR();
			std::cout<<"printHuffmantree"<<std::endl;
			std::ofstream out("treeprint.txt", std::ios::out);

			huffmantree->PrintHuffmanTree();

			//std::cout << outputtree << std::endl;
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
 
