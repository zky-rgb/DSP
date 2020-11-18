#include"io.h"
int main(void)
{
    Huffman h;
    IO io(&h);
    io.interface();//进入交互界面
    
    return 0;
}