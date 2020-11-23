#include"user io.h"
#include<iostream>
#include<fstream>
void souredfishIO::load(){
	std::ifstream in("tree.txt",std::ios::in);
	in >>  tree;
	in.close();
}
void souredfishIO::save(){//存储树
	std::ofstream out("tree", std::ios::out);
	out << tree;
	out.close();
}
void souredfishIO::divide(std::string &example,std::string examplePart[]){
	int pos;
	int part = 0;
	for (int i = 0; i < example.size(); i++) {
		pos = ip.find('.', i);
		if (pos < example.size()&&pos!=-1) {
			examplePart[part++] = example.substr(i, pos - i);
			i = pos;
		}
		else {
			examplePart[part] = example.substr(i, ip.size()-i);
		}
	}
}
void souredfishIO::domainName() {//读取域名
	std::cout<<"请输入域名"<<std::endl;
	std::cin >> dName;
	divide(dName,dNamePart);
	Tetrad Tet=t->searchPath(dNamePart);
	if(Tet.tag==0){
		int choice;
		std::cout << "未搜索到该域名，是否添加？(yY/Nn)" << std::endl;
		std::cin >> choice;
		switch(choice){
			case 'y':
			case 'Y':
				readIP();
			case 'N':
			case 'n':
				
		}
	}
}
void souredfishIO::readIP(){
	std::cout<<"请输入IP"<<std::endl;
	std::cin >> ip;
}