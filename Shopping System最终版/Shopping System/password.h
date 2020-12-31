#pragma once
#include<string>
class password {
public:
    bool checkUsername();
    bool checkPassword();
    void assembly();
    bool dChecking();
    void login();
    void myregister();
private:
    std::string username;
    std::string password;
    int usernameLine = 0;
    int passwordLine = 0;
};