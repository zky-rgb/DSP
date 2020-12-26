#include"userio.h"
void souredfishIO::load(){
	std::ifstream inDomain("domain.txt",std::ios::in);
	std::ifstream inIp("ip.txt", std::ios::in);
	while (1) {
		if (inDomain.eof()) {
			break;
		}
		inDomain >> dName;
		divide(dName, dNamePart);
		tr.InData(dNamePart);
		inIp >> ip;
		tr.CreateTree(dNamePart, ip);

	}
	std::cout<<"文件中域名：";
    tr.Print();
	inDomain.close();
	inIp.close();
}
void souredfishIO::save(){//存储树
	std::ofstream outDomain("domain.txt", std::ios::app);
	outDomain << std::endl;
	outDomain << dName;
	outDomain.close();
	std::ofstream outIp("ip.txt", std::ios::app);
	outIp << std::endl;
	outIp << termip;
	outIp.close();
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
	//system("cls");
	std::cout << "请输入域名" << std::endl;//请输入域名
	std::cin >> dName;
	divide(dName,dNamePart);

    //Tetrad Tet;

	/*if(tr.JudgeTreeEmpty()==0){
        Tet=t->searchPath(dNamePart);
	} else{
	    Tet.tag=0;
	}*/

	if(tr.Search(dNamePart)==0){
		char choice;
		std::cout << "未搜索到该域名，是否添加？(yY/Nn)" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
		case 'Y':
		    std::cout<<"请输入IP"<<std::endl;
		    std::cin >> termip;
		    tr.InData(dNamePart);
			tr.CreateTree(dNamePart,termip);
			save();
			tr.Print();
			system("pause");
		case 'N':
		case 'n':

            char judge;
            std::cout<<"Do you want to continue?"<<std::endl;
                std::cin>>judge;
                if(judge=='y'){
                    domainName();
                } else{
                    exit(0);
                }
		}
	}
	else {
	    char judge;
		std::cout << "ip:" << tr.Reip() << std::endl;
		std::cout<<"done!"<<std::endl;
		std::cout<<"Do you want to continue?"<<std::endl;
		std::cin>>judge;
		if(judge=='y'){
            domainName();
		} else{
		    exit(0);
		}

	}



}
void souredfishIO::readIP(){
	std::cout<<"请输入IP"<<std::endl;
	std::cin >> ip;
}