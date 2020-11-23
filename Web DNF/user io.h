#include<string>
#include"DNF.h"
class souredfishIO 
{
private:
	std::string tree;
	std::string dName;
	std::string dNamePart[4];
	std::string ip;
	std::string ipPart[4];
	Tree *t;
public:
	void load();
	void save();
	void domainName();
	void readIP();
	void divide(std::string &example,std::string examplePart[]);
};
//界面类
class UserInterface{
public:
    UserInterface();
    void MainInterFace();
	void GetIO( souredfishIO * p1)
	{
		p=p1;
	}
private:
    int key=-1;
    souredfishIO * p;
};