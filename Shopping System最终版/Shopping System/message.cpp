#include "Message.h"
#include "Warhouse.h"
#include "UI.h"
#include"Logs.h"
//消息类实现
//递增时间
void Message::msg_increasetime()
{
	msg_ctime.lock();
	++msg_time;
	if (msg_time == 13)
	{
		msg_time = 1;
		++msg_year;
	}
	
	std::string s = "Mon:" + std::to_string(msg_time);
	if (msg_time < 10)
		s += " ";
	s += " Year:" + std::to_string(msg_year);
	msg_ctime.unlock();
	int l = -1;
	//更新时间栏
	PView p(TIMETABLE, 0, 0, &s, &l);
	msg_ui->UI_Print(p, 0); //刷新
}
//获取时间
void Message::msg_gettime(int &mon, int &year, Type_Season *s)
{
	msg_ctime.lock();
	mon = msg_time;
	year = msg_year;
	msg_ctime.unlock();
	if (s != NULL)
	{
		switch (mon)
		{
		case 12:
		case 1:
		case 2:
			*s = Type_Season::Win;
			break;
		case 11:
		case 10:
		case 9:
			*s = Type_Season::Aut;
			break;
		case 8:
		case 7:
		case 6:
			*s = Type_Season::Sum;
			break;
		case 5:
		case 4:
		case 3:
			*s = Type_Season::Spr;
			break;
		}
	}
}
//增加消息，b为真时是普通消息
void Message::msg_add(std::string s, bool warn)
{
	msg_ctime.lock();
	MsgNode mnode(s, (msg_time + 1) % 12 + 1, warn);
	msg_ctime.unlock();
	msg.Insert(mnode); //插入节点
	int lightline[7] = {-1, -1, -1, -1, -1, -1, -1};
	std::string str[8] = {"", "  ", "  ", "  ", "  ", "  ", "  ", "  "};
	str[0] += "MESSAGE TABLE:";
	for (int i(1), j(0), size(msg.Size()); i < size + 1; ++i)
	{
		str[i] += msg.Search(i - 1).data;
		if (msg.Search(i - 1).warning) //警告消息，设置高亮行
		{
			lightline[j] = i;
			++j;
		}
	}
	PView view(MSGBAR, 0, 7, str, lightline);
	//更新消息栏
	msg_cui.lock();
	msg_ui->UI_clear(MSGBAR, 0, 7); //清空消息栏
	msg_ui->UI_Print(view, 0);
	msg_cui.unlock();
}
//控制与用户交互
void Message::msg_control()
{
	int page(0);
	bool quit(false);
	while (!quit)
	{
		switch (page)
		{
		case 0:
			msg_ui->UI_WelCome(); //欢迎界面
			++page;				  //页数递增
			break;
		case 1: //主菜单
		{
			int lightline = 1;				   //设置高亮行
			msg_ui->UI_clear(MAINVIEW, 0, 14); //刷新面板
			msg_ui->UI_Menu(0, 5, &lightline); //菜单栏初始化
			do
			{
				char c = msg_ui->UI_get(NULL, 0); //捕获用户输入
				if (c == LINEDOWN)				  //行数向下
				{
					if (lightline != 5)
					{
						++lightline;
						msg_ui->UI_Menu(lightline - 1, lightline, &lightline);
					}
				}
				else if (c == LINEUP) //行数向上
				{
						if (lightline != 1)
						{
							--lightline;
							msg_ui->UI_Menu(lightline, lightline + 1, &lightline);
						}
				}
				else if (c == ENTER)
				{
							page = lightline + 1;
							lightline = 1;
							break;
				}
			} while (1);
			msg_ui->UI_clear(MAINVIEW, 0, 5); //刷新面板
			break;
		}
		case 2: //商品检查界面
		{
			//预载所有商品信息
			int id=msg_commondity(); //加载商品检查界面
			if(id>=0)
				msg_detail(id);
			page = 1;//回到主界面
			break;
		}
		case 3: //商品购买界面
		{
			msg_purcharse();
			page = 1;
			break;
		}
		case 4: //出售列表
		{
			msg_selllist();//出售列表
			page = 1;
			break;
		}
		case 5: //显示日志
		{
			msg_logs();
			page = 1;
			break;
		}
		case 6: //退出程序
		{
			quit = true;
			msg_bool.lock();
			msg_uiquit = true;
			msg_bool.unlock();
			break;
		}
		}
	}
}
//弹出队首消息,成功则返回true,
bool Message::msg_pop()
{
	if (msg.Size() == 0)
		return false;
	else
	{
		msg.Remove();
		return true;
	}
}
//获得消息队列队首的消息,如果失败，则返回false
bool Message::msg_front(MsgNode &node)
{
	if (msg.Size() == 0)
		return false;
	else
	{
		node = msg.Search(0);
		return true;
	}
}
//加载商品到缓冲区,返回要详细展开的项
int Message::msg_commondity()
{
	//将仓库的信息保存到缓冲区内
	MSG_BUFFER buffer(msg_btree->Wh_num());
	msg_common.lock();
	msg_btree->Wh_list(true); //重置
	for (int i(0), j(buffer.Size()); i < j; ++i)
		buffer.insert(msg_btree->Wh_list()->WhC_ReInf());
	msg_common.unlock();
	//初始化相关信息
	Type_Sort s(Type_Sort::id);
	bool quit(false);
	bool resort(true);
	bool ifprint(true);
	int lightline(3); //高亮行
	int page(0);	  //当前页数
	char c[2] = {'b', 's'};
	std::string str[15] = {""};
	str[0] = "COMMONDITY";
	str[14] = "|TAB-change sort,Enter-show detail,b-back-,s-search|";
	str[2] = "ID    NAME    LAB    SUM";
	//显示
	while (!quit)
	{
		//根据用户选择类型进行排序
		if (resort)
		{
			buffer.sort(s);
			switch (s)
			{
			case Type_Sort::id:
				str[1] = "SORT TYPE:ID";
				break;
			case Type_Sort::name:
				str[1] = "SORT TYPE:NAME";
				break;
			case Type_Sort::lab:
				str[1] = "SORT TYPE:LAB";
				break;
			}
			resort=false;
		} //从buffer中读取数据
		if (ifprint)
		{
			for (int i(page * 11);i < 11 * (page + 1); ++i)
			{
				if (i < buffer.Size())
				{
					INFChain inf = buffer.output(i);
					str[i - page * 11 + 3] = std::to_string(inf.id) + "    " + inf.name + "    " + inf.lab + "    " + std::to_string(inf.sum);
				}
				else
					str[i - page * 11 + 3] = "";
			}
			//显示界面
			PView view(MAINVIEW, 0, 14, str, &lightline);
			msg_ui->UI_clear(MAINVIEW, 1, 13); //清空界面
			msg_ui->UI_Print(view, 0);		   //打印界面
			ifprint=false;
		}
		switch (msg_ui->UI_get(c, 2))
		{
		case 'b': //返回主菜单
			return -1;
		case 's': //进入搜哦
		{
			Triple t=msg_btree->Wh_BTreeSearch(msg_ui->UI_Search());//搜索对应的id
			if (t.tag)
			{
				//搜素成功
				std::string tip[4] = {
					"----------------------------",
					"          Search Success",
					"ENTER-Detail       c-Cancel",
					"----------------------------"
				};
				if (msg_ui->UI_ToolTip(tip, 4, 5))
						return t.id;
				else
					ifprint = true;
			}
			else
			{
				//搜素失败
				std::string tip[4] = {
				"----------------------------",
				"        Can't Find This ID",
				"        Enter to continue",
				"----------------------------"
				};
				msg_ui->UI_ToolTip(tip, 4, 5);
				ifprint = true;
			}
			break;
		}
		case LINEUP: //向上移动光标
		{
			if (lightline != 3)
			{
				--lightline;
				PView view(MAINVIEW, lightline, lightline + 1, str, &lightline);
				msg_ui->UI_Print(view, lightline);
			}
			break;
		}
		case LINEDOWN: //向下移动光标
		{
			if (lightline != 13&&(lightline-2+page*11)!=(buffer.Size()))
			{
				++lightline;
				PView view(MAINVIEW, lightline - 1, lightline, str, &lightline);
				msg_ui->UI_Print(view, lightline - 1);
			}
			break;
		}
		case TAB: //tab键
		{
			resort = true;
			ifprint = true;
			switch (s)
			{
			case Type_Sort::id:
				s = Type_Sort::name;
				break;
			case Type_Sort::name:
				s = Type_Sort::lab;
				break;
			case Type_Sort::lab:
				s = Type_Sort::id;
				break;
			}
			break;
		}
		case ENTER: //回车
			return (buffer.output(lightline+page*11-3).id);
		case LIFT: //向左翻页
		{
			//页面减少
			if (page != 0)
			{
				--page;
				ifprint=true;
				lightline = 3;
			}
			break;
		}
		case RIGHT: //向右翻页
		{
			//页面增加
			if((page+1)*11<buffer.Size())
			{
				page++;
				ifprint=true;
				lightline = 3;
			}
			break;
		}
		}
	}
}
//显示日志
void Message::msg_logs()
{
	int size(msg_log->ReSize());//获取日志大小

	//初始化打印的信息
	std::string str[15];
	str[0] = "LOGS";
	str[1] = "ID     SUM     MON    YEAR";
	str[14] = "Enter to Back";
	int lightline(-1);
	int page(0);
	bool quit(false);
	bool ifprint(true);
	char c[2] = { 'b', 's' };
	//显示
	while (!quit)
	{
		if (ifprint)
		{
			for (int i(page * 12); i < size&& i < 12 * (page + 1); ++i)
			{
				LogNode node(msg_log->ShowLogs(i));
				str[i + 2] = std::to_string(node.id) + "   " + std::to_string(node.num) + "   " + std::to_string(node.month) + "   " + std::to_string(node.year);
			}
			//显示界面
			PView view(MAINVIEW, 0, 14, str, &lightline);
			msg_ui->UI_clear(MAINVIEW, 2, 13); //清空界面
			msg_ui->UI_Print(view, 2);		   //打印界面
			ifprint = false;
		}
		switch (msg_ui->UI_get(NULL, 0))
		{
		case ENTER: //回车
			quit = true;
		case LIFT: //向左翻页
		{
			//页面减少
			if (page != 0)
			{
				--page;
				ifprint = true;
			}
			break;
		}
		case RIGHT: //向右翻页
		{
			//页面增加
			if ((page + 1) * 12 <size)
			{
				page++;
				ifprint = true;
			}
			break;
		}
		}
	}
}
//向进出口哈希表中添加项/更新项
void Message::msg_pushdeal(const int &id, int in, int out)
{
	msg_common.lock();
	if (msg_io.find(id) == msg_io.end()) //如果没有节点，则建立一个节点
	{
		MsgSellreNode p(in, out);
		msg_io[id] = p;
	}
	else //有节点，则直接修改现在节点的值
	{
		msg_io[id].msg_in += in;
		msg_io[id].msg_out += out;
	}
	msg_common.unlock();
}
//从进出口表中删除项
void Message::msg_deletedeal(const int &id)
{
	std::unordered_map<int, MsgSellreNode>::iterator iter;
	iter = msg_io.find(id);
	msg_io.erase(iter);
}
//向出售请求队列中添加消息
void Message::msg_pushdealask(int id, int sum, int mon, int year)
{
	msg_common.lock();
	bool isinsert(false); // 判断是否插入成功
	MsgSellNode m(id, sum, mon, year);
	if (msg_dealask.empty()) //如果为空，则直接添加
	{
		msg_dealask.push_back(m);
	}
	else //如果不为空，则需要比较
	{
		//遍历整个链表
		for (std::vector<MsgSellNode>::iterator iter = msg_dealask.begin(); iter != msg_dealask.end(); ++iter)
		{
			if (((*iter).msg_ddl_year * 100 + (*iter).msg_ddl_mon) > (mon + year * 100))
			{
				msg_dealask.insert(iter, m); //插入到合适位置
				isinsert = true;
				break;
			}
		}
		if (!isinsert)
		{
			msg_dealask.push_back(m); //直接在队尾插入
		}
	}
	msg_common.unlock();
}
//从出售队列删除消息
void Message::msg_deletedealask(const int &i)
{
	msg_common.lock();
	std::vector<MsgSellNode>::iterator iter = msg_dealask.begin();
	for (int j(0); j < i; j++)
		++iter;
	msg_dealask.erase(iter); //删除这条信息
	msg_common.unlock();
}
//显示商品的详细信息
void Message::msg_detail(const int id)
{
	Triple t(msg_btree->Wh_BTreeSearch(id));//搜索对应的id

	int size;
	std::vector<INFCom> buffer;//建立缓冲区
	Wh_Chain* p = t.r->P_data[t.id];

	//初始化相关信息
	bool quit(false);
	bool reset(true);
	int lightline(3); //高亮行
	int page(0);	  //当前页数
	bool ifprint(true);
	char c[3] = { 'b', 'y' ,'c'};
	std::string str[15] = { "" };
	str[0] = "DETAIL";	
	str[2] = "MAU   IN_DATE   SHELF LIFE   SUM";
	str[14] = "|Enter-delete,b-back-,y-buy|";
	//显示
	while (!quit)
	{
		if (reset)
		{
			//将节点信息载入缓冲区
			size= p->WhC_len();
			buffer.reserve(size);
			for (int i(0); i < size; ++i)
				buffer.push_back(p->WhC_NodeInf(i));
			reset = false;
		}
		if (ifprint)
		{			
			INFChain infc = p->WhC_ReInf();
			str[1] = std::to_string(infc.id) + " " + infc.name + " " + std::to_string(infc.profit);
			for (int i(page * 11); i < 11 * (page + 1); ++i)
			{
				if (i <size)
				{
					INFCom inf = buffer[i];
					str[i - page * 11 + 3] = inf.maufacter+ "    " +std::to_string( inf.in_y )+"."+std::to_string(inf.in_m)+ "   " +std::to_string(inf.out_y) + "." + std::to_string(inf.out_m)+"   " + std::to_string(inf.sum);
				}
				else
					str[i - page * 11 + 3] = "";
			}
			//显示界面
			PView view(MAINVIEW, 0, 14, str, &lightline);
			msg_ui->UI_clear(MAINVIEW, 0, 14); //清空界面
			msg_ui->UI_Print(view, 0);		   //打印界面
			ifprint = false;
		}
		switch (msg_ui->UI_get(c, 3))
		{
		case 'b': //返回主菜单
			quit = true;
			break;
		case 'y': //进入购买界面
		{
			msg_ui->UI_Purchase();//购买
			//获取用户输入
			std::string key[4] = { "" };
			int x[4] = {7,16,20,21};
			int y[4] = { 6,7,8,9 };
			msg_ui->UI_get(key, x, y, 4);
			int  sum, year, mon;
			turn_string(key[0], sum);
			turn_string(key[2], year);
			turn_string(key[3], mon);
			int in_y, in_m;
			msg_gettime(in_m, in_y, NULL);
			Commondity com(key[1], year, in_y, mon, in_m, sum);
			msg_btree->Wh_InsertdataNode(id, com);
			msg_add("[msg]:new node added", false);
		}
		case LINEUP: //向上移动光标
		{
			if (lightline != 3)
			{
				--lightline;
				PView view(MAINVIEW, lightline, lightline + 1, str, &lightline);
				msg_ui->UI_Print(view, lightline);
			}
			break;
		}
		case LINEDOWN: //向下移动光标
		{
			if (lightline != 13 && (lightline - 2 + page * 11) != size)
			{
				++lightline;
				PView view(MAINVIEW, lightline - 1, lightline, str, &lightline);
				msg_ui->UI_Print(view, lightline - 1);
			}
			break;
		}
		case 'c':
		{
			float np = msg_ui->UI_Changepice();
			if (np > 0)
			{
				msg_btree->Wh_ChaProfit(id, np);//修改对应的价格
				msg_add("[msg]:Change success", false);
				ifprint = true;
			}
			break;
		}			
		case ENTER: //删除商品
		{
			std::string dview[4] = {
				"----------------------------------------",
				 "Do you want delete this Node?",
				 "Enter(yes)                         c(cancel)",
				 "----------------------------------------"
			};
			bool choise=msg_ui->UI_ToolTip(dview, 4, 6);//打印删除界面
			if (choise)
			{
				//将节点删除
				msg_btree->Wh_deletedataNode(id, lightline - 3 + page * 11);
				msg_add("[msg]:remove success", false);
				reset = true;
			}
			ifprint = true;
			break;
		}
		case LIFT: //向左翻页
		{
			//页面减少
			if (page != 0)
			{
				--page;
				ifprint = true;
				lightline = 3;
			}
			break;
		}
		case RIGHT: //向右翻页
		{
			//页面增加
			if ((page + 1) * 11 < size)
			{
				page++;
				ifprint = true;
				lightline = 3;
			}
			break;
		}
		}
	}
}
//购买商品页面
void Message::msg_purcharse()
{
	int lightline(-1);
	std::string str[6] = {
		"---------------",
		"ID:",
		"---------------"
	};
	PView view(MAINVIEW, 0, 2, str, &lightline);
	msg_ui->UI_clear(MAINVIEW, 5, 7);
	msg_ui->UI_Print(view, 5);
	std::string ca;
	int x(4), y(6);
	msg_ui->UI_get(&ca, &x, &y, 1);
	turn_string(ca, x);
	//搜索对应的id
	Triple t = msg_btree->Wh_BTreeSearch(x);
	if (t.tag)
	{
		//有该id
		str[0] = "----------------------";
		str[1] = "Find id success";
		str[2] = "Enter(buy)   c(Cancel)";
		str[3] = "----------------------";
		if (!msg_ui->UI_ToolTip(str, 4, 5))
			return;//取消购买。直接结束函数
	}
	else
	{
		//没有该id
		str[0] = "-------------------------";
		str[1] = "Find id Failed";
		str[2] = "Enter(add id)   c(Back)";
		str[3] = "--------------------------";
		if (!msg_ui->UI_ToolTip(str, 4, 5))
			return;//取消购买。直接结束函数
		else
		{
			str[0] = "-----------------------------------------";
			str[1] = "name:";
			str[2] = "cost:";
			str[3] = "price:";
			str[4] = "lable:";
			str[5] = "-----------------------------------------";
			PView pv(MAINVIEW, 0, 5, str, &lightline);
			msg_ui->UI_clear(MAINVIEW, 4, 4 + 6 - 1);
			msg_ui->UI_Print(pv, 4);
			int x1[4] = { 6,6,7,7 };
			int y2[4] = { 5,6,7,8 };
			msg_ui->UI_get(str, x1, y2, 4);
			float cost, price;
			turn_string(str[1], cost);
			turn_string(str[2], price);
			Wh_Chain* p = new Wh_Chain(x, str[0], str[3], cost, price);
			msg_btree->Wh_BTreeInsert(p);
			msg_add("[msg]new commondity add", false);
			str[0] = "-----------------------------------------";
			str[1] = "If you want set the suit season?";
			str[2] = "Enter-yes                              c-no    ";
			str[3] = "-----------------------------------------";
			bool choice=msg_ui->UI_ToolTip(str, 4, 5);
			if (choice)
			{
				str[0] = "-----------------------------------------";
				str[1] = "Season(Spr/Sum/Aut/Win):";
				str[2] = "-----------------------------------------";
				int x2(24), y2(6);
				PView pv(MAINVIEW, 0, 2, str, &lightline);
				msg_ui->UI_clear(MAINVIEW, 5, 4 + 3 - 1);
				msg_ui->UI_Print(pv, 5);
				msg_ui->UI_get(str,&x2,& y2, 1);
				if (str[0] == "Spr" || str[0] == "spr")
					p->WhC_InitSeason(Type_Season::Spr);
				else if (str[0] == "Sum" || str[0] == "sum")
					p->WhC_InitSeason(Type_Season::Spr);
				else if (str[0] == "Aut" || str[0] == "aut")
					p->WhC_InitSeason(Type_Season::Spr);
				else if (str[0] == "Win" || str[0] == "win")
					p->WhC_InitSeason(Type_Season::Spr);
			}
		}
	}
	msg_ui->UI_Purchase();
	//获取用户输入
	std::string key[4] = { "" };
	int x3[4] = { 7,16,20,21 };
	int y3[4] = { 6,7,8,9 };
	msg_ui->UI_get(key, x3, y3, 4);
	int  sum, year, mon;
	turn_string(key[0], sum);
	turn_string(key[2], year);
	turn_string(key[3], mon);
	int in_y, in_m;
	msg_gettime(in_m, in_y, NULL);
	Commondity com(key[1], year, in_y, mon, in_m, sum);
	msg_btree->Wh_InsertdataNode(x, com);
	msg_add("[msg]:new node added", false);
}
//出售列表
void Message::msg_selllist()
{
	//初始化相关信息
	msg_common.lock();
	int size = msg_dealask.size();
	msg_common.unlock();
	bool quit(false);
	bool resort(true);
	bool ifprint(true);
	int lightline(2); //高亮行
	int page(0);	  //当前页数
	char c = 'b';
	std::string str[15] = { "" };
	str[0] = "SELLLIST";
	str[14] = "Enter to choise, b to back";
	str[1] = "ID   SUM";
	//显示
	while (!quit)
	{
		if (ifprint)
		{
			lightline = 2;
			for (int i(page * 12); i < 12 * (page + 1); ++i)
			{
				if (i < size)
				{
					msg_common.lock();
					str[i - page * 12 + 2] = std::to_string(msg_dealask[i].msg_id) + "    " + std::to_string(msg_dealask[i].msg_sum);
					msg_common.unlock();
				}
				else
					str[i - page * 12 + 2] = "";
			}
			//显示界面
			PView view(MAINVIEW, 0, 14, str, &lightline);
			msg_ui->UI_clear(MAINVIEW, 1, 13); //清空界面
			msg_ui->UI_Print(view, 0);		   //打印界面
			ifprint = false;
		}
		switch (msg_ui->UI_get(&c, 1))
		{
		case 'b': //返回主菜单
			quit = true;
			break;
		case LINEUP: //向上移动光标
		{
			if (lightline != 2)
			{
				--lightline;
				PView view(MAINVIEW, lightline, lightline + 1, str, &lightline);
				msg_ui->UI_Print(view, lightline);
			}
			break;
		}
		case LINEDOWN: //向下移动光标
		{
			if (lightline != 13 && (lightline - 1 + page * 12) !=size)
			{
				++lightline;
				PView view(MAINVIEW, lightline - 1, lightline, str, &lightline);
				msg_ui->UI_Print(view, lightline - 1);
			}
			break;
		}
		case ENTER: //同意消息
		{
			msg_common.lock();
			MsgSellNode node = msg_dealask[lightline - 2 + page * 12];
			msg_common.unlock();
			int n_mon, n_year;
			msg_gettime(n_mon, n_year, NULL);
			if (n_mon + n_year > node.msg_ddl_mon + node.msg_ddl_year)
			{
				//过期请求处理
				std::string tip[4] = {
					"--------------------------------",
					"This sell require has overdued",
					"Enter to continue",
					"--------------------------------"
				};
				msg_ui->UI_ToolTip(tip, 4, 4);
				msg_deletedealask(lightline - 2 + page * 12);
				ifprint = true;//刷新页面
			}
			else
			{
				std::string tip[4] = {
					"--------------------------------",
					"       Are you sure to sell",
					"Enter-yes                  c-Cancel",
					"--------------------------------"
				};
				if (msg_ui->UI_ToolTip(tip, 4, 4))
				{
					//处理消息
					if (msg_btree->Wh_deletedataNode(node.msg_id, node.msg_sum))
					{
						//购买成功
						msg_add("[msg]sell success", false);
						msg_deletedealask(lightline - 2 + page * 12);
						ifprint = true;
					}
					else
					{
						//失败处理
						std::string tip[4] = {
							"--------------------------------",
							"This sell sum is too large!",
							"Enter to continue",
							"--------------------------------"
						};
						msg_ui->UI_ToolTip(tip, 4, 4);
						ifprint = true;//刷新页面
					}
				}
				else
				{
					ifprint = true;
				}
			}
		}
		case LIFT: //向左翻页
		{
			//页面减少
			if (page != 0)
			{
				--page;
				ifprint = true;
			}
			break;
		}
		case RIGHT: //向右翻页
		{
			//页面增加
			if ((page + 1) * 12 < size)
			{
				page++;
				ifprint = true;
			}
			break;
		}
		}
	}
}
