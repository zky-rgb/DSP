#include"userio.h"
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
		pos = example.find('.', i);
		if (pos < example.size()&&pos!=-1) {
			examplePart[part++] = example.substr(i, pos - i);
			i = pos;
		}
		else {
			examplePart[part] = example.substr(i, example.size()-i);
			break;
		}
	}
}
void souredfishIO::domainName() {//读取域名
	std::cout << "请输入域名" << std::endl;//请输入域名
	std::cin >> dName;
	divide(dName,dNamePart);
    Tetrad Tet;

	if(tr.JudgeTreeEmpty()==0){
        Tet=t->searchPath(dNamePart);
	} else{
	    Tet.tag=0;
	}

	if(Tet.tag==0){
		char choice;
		std::cout << "未搜索到该域名，是否添加？(yY/Nn)" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
		case 'Y':
			readIP();
			tr.CreateTree(dNamePart,ip);
			tr.PrintTree();
		case 'N':
		case 'n':
			domainName();
		}
	}
	else {
		std::cout << "ip" << Tet.p->ip << std::endl;
	}



}
void souredfishIO::readIP(){
	std::cout<<"请输入IP"<<std::endl;
	std::cin >> ip;
}