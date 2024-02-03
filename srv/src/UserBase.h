#pragma once
#include <iostream>
#include <vector>
#include <iomanip> 
#include <new>
#include <fstream>
#include "sha256.h"

using std::cout;
using std::string;
using std::cin;
using std::endl;
using std::setw;

struct User
{
	string name;
	string login;
	string pwd;
	User(string name_, string login_, string pwd_) : name(name_), login(login_), pwd(pwd_) {}
};

class UserBase
{
public:
	UserBase();
	~UserBase();
	string packUser(int userId);
	User splitUsrPkg(string& usrPkg);
	void addUsers(User& usrStr);
	void addUsers(string& name, string& login, string& pwd);
	void chgPwd(int userId, std::string& pwd);
	int getUserCount();
	void delUser(int userId);
	void saveUsrBase();
private:
	string delim = "<|>";
	std::vector<User>* usrBase;
	SHA256 sha256;
#if defined (_WIN32) || defined (_WIN64)
	string UBPath = "users.dat";
#elif defined (__linux__)
	string UBPath = "/var/lib/Chat/users.dat";
#endif
};