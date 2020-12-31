#include"Time.h"
#include"Handle.h"
#include "Message.h"
#include "UI.h"
#include "Warhouse.h"
#include "password.h"
#include"Logs.h"
int main(void)
{
	using namespace std;

	
	//password demo;
	//demo.assembly();

	Message message;
	UI ui;
	BTree btree;
	//Logs logs;
	message.msg_getui(&ui);
	message.msg_getbtree(&btree);	
	Handle handle(&message,&btree,NULL);
	Time clock(&message, &handle);

	//插入节点
	Wh_Chain* p1 = new Wh_Chain(1, "apple", "food", 10, 12);
	Wh_Chain* p2 = new Wh_Chain(2, "ab", "clothe", 10, 12);
	Wh_Chain* p3 = new Wh_Chain(3, "ase", "clothe", 10, 12);
	Wh_Chain* p4 = new Wh_Chain(4, "bsbf", "food", 10, 12);
	/*Wh_Chain* p4 = new Wh_Chain(5, "gag", "food", 10, 12);
	Wh_Chain* p5 = new Wh_Chain(6, "gab", "food", 10, 12);
	Wh_Chain* p6 = new Wh_Chain(7, "bsb", "game", 10, 12);
	Wh_Chain* p7 = new Wh_Chain(8, "ocd", "clothe", 10, 12);
	Wh_Chain* p8 = new Wh_Chain(9, "cldoe", "food", 10, 12);
	Wh_Chain* p9 = new Wh_Chain(10, "enfa", "game", 10, 12);
	Wh_Chain* p10 = new Wh_Chain(11, "pfemag", "food", 10, 12);
	Wh_Chain* p11 = new Wh_Chain(12, "faome", "food", 10, 12);
	Wh_Chain* p12 = new Wh_Chain(13, "gaon", "game", 10, 12);
	Wh_Chain* p13 = new Wh_Chain(14, "eg", "food", 10, 12);
	Wh_Chain* p14 = new Wh_Chain(15, "ngeg", "clothe", 10, 12);*/

	btree.Wh_BTreeInsert(p1);
	btree.Wh_BTreeInsert(p2);
	btree.Wh_BTreeInsert(p3);
	btree.Wh_BTreeInsert(p4);
	/*btree.Wh_BTreeInsert(p5);
	btree.Wh_BTreeInsert(p6);
	btree.Wh_BTreeInsert(p7);
	btree.Wh_BTreeInsert(p8);
	btree.Wh_BTreeInsert(p9);
	btree.Wh_BTreeInsert(p10);
	btree.Wh_BTreeInsert(p11);
	btree.Wh_BTreeInsert(p12);
	btree.Wh_BTreeInsert(p13);
	btree.Wh_BTreeInsert(p14);
	btree.Wh_BTreeInsert(p15);*/

	Commondity a1("orange", 2020, 2020, 1, 1, 10);
	Commondity a2("o", 2020, 2020, 1, 1, 10);
	Commondity a3("dsagg", 2020, 2020, 1, 1, 10);
	Commondity a4("sdafdsffd", 2020, 2020, 1, 1, 10);
	btree.Wh_InsertdataNode(1, a1);
	btree.Wh_InsertdataNode(2, a2);
	btree.Wh_InsertdataNode(3, a3);
	btree.Wh_InsertdataNode(4, a4);
	thread clockthread(&Time::Time_clock, &clock);
	message.msg_control();
	clockthread.join();
	return 0;
}