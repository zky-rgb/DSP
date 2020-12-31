#include<iostream>
#include<fstream>
#include"Warhouse.h"
#include"Message.h"
class storage {
public:
	void save();
	void load();
	void log();
private:
	int id;
	std::string name;
	int sum;
	float cost;
	float profit;
	float total;
	bool ifseason;
	Type_Season season;
	std::string tempseason;
	std::string lab;//标签
	std::string c_maufacter;//厂商
	int c_date_year;//过期年份
	int c_date_year_in;//进货年份
	int c_date_m;//过期月份
	int c_date_m_in;//进货日期
	int c_sum;//该批商品的数量
	Type_Season seas;
	Wh_Chain chain;
	Commondity com;
	BTree tree;
	Message mes;
	int nowmon;
	int nowyear;
	Type_Season nowsea;
	std::string tempnowseason;
};

void storage::save() {
	std::ofstream out("data.txt", std::ios::out);
	mes.msg_gettime(nowmon, nowyear, &nowsea);
	out << nowmon << "\t" << nowyear << "\t";
	switch (nowsea) {
	case Type_Season::Spr:
		out << "Spring";
		break;
	case Type_Season::Sum:
		out << "Summer";
		break;
	case Type_Season::Aut:
		out << "Autumn";
		break;
	case Type_Season::Win:
		out << "Winter";
		break;
	}
	out << std::endl;
	for (int i = 1; i <= tree.Wh_num(); i++) {
		Wh_Chain* chain = tree.Wh_list();
		INFChain zongxinxi = chain->WhC_ReInf();
		out << zongxinxi.id;
		out << "\t";
		out << zongxinxi.name;
		out << "\t";
		out << zongxinxi.sum;
		out << "\t";
		out << zongxinxi.cost;
		out << "\t";
		out << zongxinxi.profit;
		out << "\t";
		out << zongxinxi.ifseason;
		out << "\t";
		if (zongxinxi.ifseason) {
			switch (zongxinxi.season) {
			case Type_Season::Spr:
				out << "Spring";
				break;
			case Type_Season::Sum:
				out << "Summer";
				break;
			case Type_Season::Aut:
				out << "Autumn";
				break;
			case Type_Season::Win:
				out << "Winter";
				break;
			default:
				out << "none";
			}
			out << "\t";
		}
		out << zongxinxi.lab << "\t";
		if (!chain->WhC_ifempty()) {
			for (int j = 0; i < chain->WhC_len(); j++) {
				INFCom picixinxi = chain->WhC_NodeInf(j);
				out << "第" << j << "批次" << "\t";
				out << picixinxi.maufacter << "\t";
				out << picixinxi.in_m << "\t";
				out << picixinxi.in_y << "\t";
				out << picixinxi.out_m << "\t";
				out << picixinxi.out_y << "\t";
				out << picixinxi.sum << "\t";
			}
		}
		out << std::endl;
	}
	out.close();
}
void storage::load() {
	int temp;
	std::ifstream in("data.txt", std::ios::in);
	in >> nowmon >> nowyear >> tempnowseason;
	if (tempnowseason == "Spring") {
		nowsea = Type_Season::Spr;
	}
	else if (tempnowseason == "Summer") {
		nowsea = Type_Season::Sum;
	}
	else if (tempnowseason == "Autumn") {
		nowsea = Type_Season::Aut;
	}
	else if (tempnowseason == "Winter") {
		nowsea = Type_Season::Win;
	}
	mes.msg_settime(nowmon, nowyear);
	while (!in.eof()) {
		in >> id;
		in >> name;
		in >> sum;
		in >> cost;
		in >> profit;
		total = cost + profit;
		in >> ifseason;
		in >> tempseason;
		if (tempseason == "Spring") {
			seas = Type_Season::Spr;
		}
		else if (tempseason == "Summer") {
			seas = Type_Season::Sum;
		}
		else if (tempseason == "Autumn") {
			seas = Type_Season::Aut;
		}
		else if (tempseason == "Winter") {
			seas = Type_Season::Win;
		}
		in >> lab;
		Wh_Chain chain(id, name, lab, cost, total);
		if (ifseason) {
			chain.WhC_InitSeason(seas);
		}
		while (in >> temp && temp != '\n') {
			in >> c_date_m_in;
			in >> c_maufacter;
			in >> c_date_year_in;
			in >> c_date_m;
			in >> c_date_year;
			in >> c_sum;
			Commondity com(c_maufacter, c_date_year, c_date_year_in, c_date_m, c_date_m_in, sum);
			chain.WhC_insert(com);
		}
		tree.Wh_BTreeInsert(&chain);
	}
}