#include"userio.h""
using namespace std;
int main(void)
{
    string a;

    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"-----Domain name resolution System-----"<<endl;
    cout<<"developed by group: zky yyq ffy cyz xcx"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"press any to continue"<<endl;
    cin>>a;

	souredfishIO sio;
	UserInterface ui;
	ui.GetIO(&sio);
	sio.load();
	sio.domainName();

	return 0;
}