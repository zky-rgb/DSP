#ifndef HANDLE_H
#define HANDLE_H

class Message;
class Logs;
class BTree;
//处理消息类
class Handle
{
public:
    Handle(Message* m, BTree* bt,Logs *l) :h_msg(m), h_btree(bt) ,log(l){}
    void H_createsell();//生成交易信息
    void H_handlemsg();//处理消息队列
    void H_handleOverdue();//处理过期
private:
    Message* h_msg;//指向消息对象的指针
    BTree* h_btree;//指向B树仓库的指针   
    Logs* log;
};

#endif