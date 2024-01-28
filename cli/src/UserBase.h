#pragma once
#include <iomanip> 
#include <new>
#include "sha256.h"
#include "net.h"

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
    void showUsers();
	void addUsers(User& usrStr);
	void addUsers(string& name, string& login, string& pwd);
	void addUsers(const char name[], const char login[], const char pwd[]);
	void chgPwd(int userId, std::string& pwd);
	int getUserCount();
	int getUserId(string& login);
	string getUserName(int userId);
	void delUser(int userId);
	bool loginCheck(string& login);
	bool pwdCheck(int userId, string& pwd);

private:
	std::vector<User>* usrBase;
	SHA256 sha256;
#if defined (_WIN32) || defined (_WIN64)
	string UBPath = "users.dat";
#elif defined (__linux__)
	string UBPath = "/var/lib/Chat/users.dat";
#endif
};