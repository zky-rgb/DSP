#include"class.h"

int main(void)
{
	using namespace std;
	
	Message message;

	BTree btree(&message);	
	Handle handle(&message,&btree);
	message.msg_gethandle(&handle);
	UI ui(&message,&btree);
	//插入节点
	btree.Wh_BTreeInsert(1, "apple", Type_Lable::Food, 10, 2);
	btree.Wh_BTreeInsert(2, "clothe1", Type_Lable::Clothe, 120, 20);
	btree.Wh_BTreeInsert(3, "banana", Type_Lable::Food, 15, 3);
	//btree.Wh_BTreeInsert(4, "fridge", Type_Lable::Electrical, 1500, 300);
	//btree.Wh_BTreeInsert(5, "green apple", Type_Lable::Food, 15, 3);
	//btree.Wh_BTreeInsert(6, "big banana", Type_Lable::Food, 15, 5);
	//btree.Wh_BTreeInsert(7, "vodka", Type_Lable::Food, 150, 30);
	//btree.Wh_BTreeInsert(8, "XiaoMi TV", Type_Lable::Electrical, 2000, 1000);
	//btree.Wh_BTreeInsert(9, "HUAWEI PHONE", Type_Lable::Electrical, 3000, 1000);
	//btree.Wh_BTreeInsert(10, "noodles", Type_Lable::Food, 10, 3);
	//btree.Wh_BTreeInsert(11, "bread", Type_Lable::Food, 15, 3);
	//btree.Wh_BTreeInsert(12, "pie", Type_Lable::Food, 15, 30);
	//btree.Wh_BTreeInsert(13, "clothe2", Type_Lable::Clothe, 150, 30);
	//btree.Wh_BTreeInsert(14, "clothe3", Type_Lable::Clothe, 150, 30);
	//btree.Wh_BTreeInsert(15, "clothe4", Type_Lable::Clothe , 150, 30);
	//btree.Wh_BTreeInsert(16, "Iphon", Type_Lable::Electrical, 10000, 3000);
	//btree.Wh_BTreeInsert(17, "cabbage", Type_Lable::Food, 22, 3);
	//btree.Wh_BTreeInsert(18, "adidas", Type_Lable::Clothe, 150, 30);
	//btree.Wh_BTreeInsert(19, "LIning", Type_Lable::Clothe, 100, 30);
	//btree.Wh_BTreeInsert(20, "wine", Type_Lable::Food, 150, 30);
	//btree.Wh_BTreeInsert(21, "bayehelie", Type_Lable::Food, 150, 30);
	//btree.Wh_InsertdataNode(1, "ChangPing", 2023, 2020, 3, 1, 20);
	//btree.Wh_InsertdataNode(1, "PinGu", 2021, 2020, 5, 1, 11);
	//btree.Wh_InsertdataNode(1, "DaXing", 2021, 2020, 7, 1, 21);
	//btree.Wh_InsertdataNode(1, "TongZhou", 2022, 2020, 8, 1, 40);
	//btree.Wh_InsertdataNode(1, "YanQing", 2021, 2020, 2, 1, 50);
	btree.Wh_InsertdataNode(1, "HeBei", 2021, 2020, 2, 1, 20);
	//btree.Wh_InsertdataNode(2, "Beijing Clothe mau", 2021, 2020, 1, 1, 10);
	btree.Wh_InsertdataNode(2, "USA Clothe mau", 2026, 2020, 5, 1, 2);
	//btree.Wh_InsertdataNode(2, "UK Clothe mau", 2024, 2020, 6, 1, 2);
	//btree.Wh_InsertdataNode(3, "Beijing", 2023, 2020, 10, 1, 20);
	btree.Wh_InsertdataNode(3, "Japan", 2022, 2020, 11, 1, 30);

	Time t(&message, & ui);
	thread myobj(&Time::Time_clock, &t);
	thread refreshobj(&UI::UI_Refresh, &ui);
	myobj.join();
	refreshobj.join();
	
	return 0;
}