#include"user io.h""
#include<iostream>
int main(int argc, char *argv[])
{
	souredfishIO sio;
	UserInterface ui;
	ui.GetIO(&sio);
	sio.load();
}