#include"huffman.h"

int main(void)
{
    using namespace std;
    
    string str;
    cout<<"input the string"<<endl;
    cin>>str;
    Huffman h(str);
    cout<<"input the id:";
    cin>>str;
    h.Decoding(str);
}