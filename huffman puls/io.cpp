//
// Created by 杨业卿 on 2020/11/30.
//

#include <iostream>
#include<fstream>
#include <vector>
#include <sstream>
#include"HIO.h"

void PIO ::input() {
    std::ifstream in(fileName,std::ios::in|std::ios::binary); //创建二进制文件的读写

    std::stringstream buffer;//利用stringsteam字符流来临时存储读到的txt文件内容
    buffer<<in.rdbuf();
    std::string str(buffer.str());
    message = str;//将读到的内容传递给私有数据乘员message
    std::cout << "原始数据";
    std::cout<<message;
    huffmantree->getMessage(message);
    huffmantree->handling();
    in.close();

}

void PIO ::outputCode() {
    std::ofstream out("out.dat", std::ios::out);//采用二进制输出方式
    for (bool i : huffmantree->OutputCode()) {//输出编码后的二进制信息
        if (i) {
            out << 1;
        }
        else
            out << 0;
    }
    out.close();
}

void PIO::inputProcess() {
    std::ifstream in("out.dat", std::ios::in);
    in >> recover;//读入待解码的二进制编码
    huffmantree->Decoding(recover, decode);//解码
    in.close();
    std::cout << "操作之后的数据" << std::endl;
    std::cout << decode;
}
void PIO ::interface() {
    
    using namespace std;
    
    bool quit(false);
    do
    {
        char c;
        cout<<"1.encoding"<<endl;
        cout<<"2.decoding"<<endl;  
        cout<<"3.quit"<<endl;
        c=cin.get();
        while(cin.get()!='\n')
            continue;
        switch (c)
        {
        case '1':
            cout << "请输入文件名" << std::endl;
            std::cin >> fileName;
            input();
            outputCode();
            inputProcess();  
            break;
        case '2':
            
        default:
            break;
        }
    } while (quit);
    


}

