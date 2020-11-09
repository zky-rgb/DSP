//
// Created by 杨业卿 on 2020/11/5.
// tree1-4 H


#include <iostream>
#include <string>
#include <cmath>
using namespace std;
class HTree;

class HTreeNode{
public:
    friend class HTree;
private:
    int Nweight;
    int parent,lchild,rchild;

};


class HTree{
public:
    HTree();
    void input();
    void countWeight();
    void CreateHTree();
    void Select(int pos,int &s1,int &s2);
    void PrintHTree();

    string Encode();

private:
    int a_length;
    char array[50];
    char alpabet[26];
    int weight[26]={0};
    int totalnum=0;
    int leaf=0;
    HTreeNode *HN;


};


int main(){

    HTree htr;
    htr.input();
    htr.countWeight();
    htr.CreateHTree();
    htr.PrintHTree();


    return 0;
}

void HTree ::input() {
    string input;
    cout<<"enter the sentence:";
    cin>>input;

    a_length = input.length();
    strcpy(array,input.c_str());

    /*for(int i=0;i<a_length;i++){
        cout<<array[i];
    }
    cout<<endl;*/
}

void HTree ::countWeight() {

    for(int i=0;i<26;i++){
        for(int j=0;j<a_length;j++){
            /*if(array[j]==' '){
                continue;
            }*/
            if(alpabet[i]==array[j]){
                weight[i]++;
            }
        }
    }

    char a='a';
    for(int i=0;i<26;i++){
        cout<<a<<" ";
        a++;
    }
    cout<<endl;
    for(int i=0;i<26;i++){
        if(weight[i]!=0){
            totalnum++;
        }
        cout<<weight[i]<<" ";
    }
    cout<<endl;

    totalnum=totalnum*2-1;
    //cout<<totalnum;

}

HTree ::HTree() {
    int a='a';
    for(int i=0;i<26;i++){
        alpabet[i]=a;
        a++;
    }
}

void HTree ::Select(int pos, int &s1, int &s2) {
    int min1=20,min2=20;
    for(int i=1;i<=pos;i++){
        if(HN[i].parent==0){
            if(HN[i].Nweight<min1){
                min1=HN[i].Nweight;
                s1=i;
            }
        }
    }

    for(int i=1;i<=pos;i++){
        if(i==s1){
            continue;
        }
        if(HN[i].parent==0){
            if(HN[i].Nweight<=min2){
                min2=HN[i].Nweight;
                s2=i;
            }
        }
    }
}

void HTree ::CreateHTree() {
    int s1=-1,s2=-1;
    leaf = (totalnum+1)/2;
    int TWeight[leaf];
    HN = new HTreeNode[totalnum+1];
    for(int i=0;i<=totalnum;i++){
        HN[i].parent=0;
        HN[i].lchild=0;
        HN[i].rchild=0;
    }

    int n=1;
    for(int i=0;i<26;i++){
        if(weight[i]!=0){
            HN[n].Nweight=weight[i];
            n++;
        }
    }

    /*for(int i=0;i<leaf;i++){
        cout<<HN[i].Nweight<<" ";
    }
    cout<<endl;*/
    for(int i=leaf+1;i<=totalnum;i++){
        Select(i-1,s1,s2);
        HN[s1].parent=i;
        HN[s2].parent=i;

        HN[i].lchild=s1;
        HN[i].rchild=s2;
        HN[i].Nweight=HN[s1].Nweight+HN[s2].Nweight;
    }

    //cout<<s1<<" "<<s2<<endl;


}

void HTree::PrintHTree() {
    for(int i=1;i<totalnum+1;i++){
        cout<<HN[i].Nweight<<" ";
        cout<<HN[i].parent<<" ";
        cout<<HN[i].lchild<<" ";
        cout<<HN[i].rchild<<" ";
        cout<<endl;
    }
    cout<<endl;

}




