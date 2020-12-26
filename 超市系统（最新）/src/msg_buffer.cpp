#include"Warhouse.h"
//插入数据
void MSG_BUFFER::insert(INFChain i)
{
    buffer.push_back(i);
}
//排序
void MSG_BUFFER::sort(Type_Sort type)
{
    for(int i(size/2);i>0;--i)
    {
        switch (type)
        {
        case Type_Sort::id:
            sift_id(i,size);
            break;
        case Type_Sort::name:
            sift_name(i,size);
            break;
        case Type_Sort::lab:
            sift_lab(i,size);
            break;
        }
    }
    for(int i(size);i>1;--i)
    {
        swap(i,1);
        switch (type)
        {
        case Type_Sort::id:
            sift_id(1,i-1);
            break;
        case Type_Sort::name:
            sift_name(1,i-1);
            break;
        case Type_Sort::lab:
            sift_lab(1,i-1);
            break;
        }
    }
}
//交换数值
void MSG_BUFFER::swap(int i,int j)
{
    INFChain tmp(buffer[i-1]);
    buffer[i-1]=buffer[j-1];
    buffer[j-1]=tmp;
}
//根据id来进行调整
void MSG_BUFFER::sift_id(int low,int high)
{
    int i(low),j(low*2);
    INFChain tmp=buffer[i-1];
    while(j<=high)
    {
        if(j<high&&buffer[j-1].id<buffer[j].id)
            ++j;
        if(tmp.id<buffer[j-1].id)
        {
            buffer[i-1]=buffer[j-1];
            i=j;
            j=i*2;
        }
        else
            break;
    }
    buffer[i-1]=tmp;
}
//根据名称进行调整
void MSG_BUFFER::sift_name(int low,int high)
{
    int i(low),j(low*2);
    INFChain tmp=buffer[i-1];
    while(j<=high)
    {
        if(j<high&&buffer[j-1].name<buffer[j].name)
            ++j;
        if(tmp.name<buffer[j-1].name)
        {
            buffer[i-1]=buffer[j-1];
            i=j;
            j=i*2;
        }
        else
            break;
    }
    buffer[i-1]=tmp;
}
//根据标签进行调整
void MSG_BUFFER::sift_lab(int low,int high)
{
    int i(low),j(low*2);
    INFChain tmp=buffer[i-1];
    while(j<=high)
    {
        if(j<high&&buffer[j-1].lab<buffer[j].lab)
            ++j;
        if(tmp.lab<buffer[j-1].lab)
        {
            buffer[i-1]=buffer[j-1];
            i=j;
            j=i*2;
        }
        else
            break;
    }
    buffer[i-1]=tmp;
}

