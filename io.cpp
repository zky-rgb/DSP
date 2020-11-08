// io.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>
namespace Souredfish {
	typedef struct {
		int n;
		int m;
		std::string huffmanTree;
		int* weight = new int[m];
		std::string letter;
		std::string message;
		std::string encode;
		std::string decode;
	}data;
	data d;
	void readCharacter() {//读取字符集
		int alphabet;
		std::cout << "请输入字符集大小" << std::endl;
		std::cin >> d.n;
		std::cout << "请输入字符集" << std::endl;
		for (int i = 0; i < d.n; i++) {
			alphabet=getchar();
			d.letter += alphabet;
		}
	}
	void weight() {//读取权值
		printf("请输入%d个权值", d.m);
		for (int i = 0; i < d.m; i++ ) {
			std::cin >> d.weight[i];
		}
	}
	void huffman() {//存储哈夫曼树
		{std::cin >> d.huffmanTree; }
		std::ofstream out("huffman.txt", std::ios::out);
		out<< d.huffmanTree<<std::endl;
		out.close();
	}
	void readMessage(){//读取未编码的发送电文
		std::ifstream in("huffman.txt", std::ios::in);
		in >> d.huffmanTree;
		in.close();
		std::ifstream in("tobetrans.dat", std::ios::in);
		in >> d.message;
		in.close();
	}
	void readySend() {//存储编码后的电文
		{std::cin >> d.encode; }
		std::ofstream out("codefile.txt", std::ios::out);
		out << d.encode<<std::endl;
		out.close();
	}
	void writeMessage() {//输出电文
		{std::cin >> d.decode; }
		std::ofstream out("txtfile.txt", std::ios::out);
		out << d.decode<<std::endl;
		out.close();
		{std::cin >> d.encode; }
		std::ofstream out("codeprint.txt", std::ios::out);
		for (int i = 0; d.encode[i]!='\0'; i++) {
			if (i%50==0) {
				std::cout << std::endl;
				out << std::endl;
			}
			std::cout << d.encode[i];
			out << d.encode[i];
		}
		out.close();
		{std::cin >> d.huffmanTree; }
		std::ofstream out("treeprint.txt", std::ios::out);
		std::cout << d.huffmanTree<<std::endl;
		out << d.huffmanTree << std::endl;
		out.close();
	}
}
int main()
{
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
