// password.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<conio.h>
#include<fstream>
#include"password.h"
bool password::checkUsername() {
    usernameLine = 0;
    std::ifstream in("username.txt", std::ios::in);
    std::string checkName;
    do {
        if (in.eof()) {
            return 0;
        }
        in >> checkName;
        usernameLine++;
    } while (checkName != username);
    return 1;
}
bool password::checkPassword() {
    passwordLine = 0;
    std::ifstream in("password.txt", std::ios::in);
    std::string checkPass ;
    
    do {
        if (in.eof()) {
            return 0;
        }
        in >> checkPass;
        passwordLine++;
    } while (checkPass != password);
    return 1;
}
void password::login() {
    std::cout << "登录" << std::endl;
    std::cout << "请输入用户名和密码" << std::endl;
    std::cin >> username;
    char temp = _getch();
    while (temp != '\r')
    {
        _putch('*');
        password += temp;
        temp = _getch();
    }
    std::cout << std::endl;
    /*if (checkUsername() && checkPassword() && usernameLine == passwordLine) {
        std::cout << "成功" << std::endl;//进入主界面
    }
    else {
        std::cout << "用户名或密码错误" << std::endl;
        system("pause");
        system("cls");
        login();
    }*/
}
bool password::dChecking() {
    std::ifstream in("username.txt", std::ios::in);
    std::string checkName;
    do {
        if (in.eof()) {
            return 1;//未有重复
        }
        in >> checkName;
    } while (checkName != username);
    return 0;//重复
}
void password::myregister() {
    std::cout << "注册" << std::endl;
    std::cout << "请输入用户名和密码" << std::endl;
    std::cin >> username;
    char temp = _getch();
    while (temp != '\r')
    {
        _putch('*');
        password += temp;
        temp = _getch();
    }
    while(!dChecking()) {
        system("cls");
        std::cout << "用户名已注册" << std::endl;
        std::cout << "请输入用户名和密码" << std::endl;
        std::cin >> username;
        temp = _getch();
        while (temp != '\r')
        {
            _putch('*');
            password += temp;
            temp = _getch();
        }
        std::cout << std::endl;
        system("cls");
    }
    std::ofstream outUsername("username.txt", std::ios::app);
    outUsername << std::endl;
    outUsername << username;
    outUsername.close();
    std::ofstream outPassword("password.txt", std::ios::app);
    outPassword << std::endl;
    outPassword << password;
    outPassword.close();
    std::cout << "注册成功，即将进入系统" << std::endl;
    system("pause");
    //进入系统
}
void password::assembly() {
    int choice;
    std::cout << "这里是身份验证部分" << std::endl;
    std::cout << "1.登录" << std::endl;
    std::cout << "2.注册" << std::endl;
    std::cout << "请输入选择" << std::endl;
    std::cin >> choice;
    switch (choice) {
    case 1:
        system("cls");
        login();
        return;
        break;
    case 2:
        system("cls");
        myregister();
        break;
    }
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
