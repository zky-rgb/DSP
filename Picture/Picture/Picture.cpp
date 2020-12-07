// Picture.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<iostream>
#include"PictureProcess.h"
#include<string>
#include<windows.h>
#include<fstream>
#include<iomanip>

using namespace std;

int main()
{
    std::cout << "Hello World!\n";


    Picture pi;
    string a;

    int marker = 0;
    do {
        system("cls");
        cout << "1.读取图像" << endl;
        cout << "2.保存图像" << endl;
        cout << "3.锐化" << endl;
        cout << "4.pinghua" << endl;
        cout << "5.展示处理后图像" << endl;
        cin >> marker;
        switch (marker)
        {
        case 1:
            system("cls");
            pi.setPicture();
            pi.PrintPicture();
            cin >>a;
            break;

        case 2:
            pi.SavePicture();
            system("pause");
            break;

        case 3:
            pi.Sharpness();
            system("pause");
            break;


        case 4:
            pi.Smooth();
            system("pause");
            break;

        case 5:
            pi.PrintPicture();
            cin >> a;
            break;

        default:
            break;
        }
    } while (marker != 0);

}
