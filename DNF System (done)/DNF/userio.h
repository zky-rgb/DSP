#include<string>
#include<iostream>
#include<fstream>
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
    Tree tr;
public:
	void load();
	void save();
	void domainName();
	void readIP();
	void divide(std::string &example,std::string examplePart[]);
    std::string termip;
};
//界面类
class UserInterface{
public:
	UserInterface() {
		key = -1;
	}
    void MainInterFace();
	void GetIO( souredfishIO * p1)
	{
		p=p1;
	}
private:
    int key=-1;
    souredfishIO * p;

};