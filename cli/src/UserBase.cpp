#include "UserBase.h"

UserBase::UserBase()
{
	try
	{
		usrBase = new std::vector<User>;
	}
	catch (std::bad_alloc& ex)
	{
		cout << "ERROR: caught bad_alloc: " << ex.what() << endl;
	}
	getUserBase();
}

UserBase::~UserBase()
{
	delete usrBase;
}

string UserBase::packUsr(string& name, string& login, string& pwd)
{
	string result;
	result.append(name);
	result.append(delim);
	result.append(login);
	result.append(delim);
	result.append(sha256(pwd));
	return result;
}

User UserBase::splitUsrPkg(string& usrPkg)
{
	size_t pos = 0;
	int i = 0;
	string sa[2];
	while ((pos = usrPkg.find(delim)) != string::npos)
	{
		sa[i++] = usrPkg.substr(0, pos);
		usrPkg.erase(0, pos + delim.length());
	}
	User newUser(sa[0], sa[1], usrPkg);
	return newUser;
}

void UserBase::getUserBase()
{
	usrBase->clear();
	net* start = new net;
	const char* pkg = "GET_USRBASE";
	start->sendReq(pkg);
	std::vector<string> users;
	start->getUsrBase(users);
	for (int i = 0; i < users.size(); i++)
	{
		User newUser = splitUsrPkg(users.at(i));
		addUsers(newUser);
	}
	delete start;
	cout << "Users: " << getUserCount() << endl;
}

void UserBase::regUsers(string& name, string& login, string& pwd)
{
	net* start = new net;
	string usrPkg = "REG_USER<|>";
	usrPkg.append(packUsr(name, login, pwd));
	start->regUser(usrPkg);
	delete start;
	getUserBase();
}

void UserBase::showUsers()
{
	cout << setw(17) << "ID пользовател¤" << setw(20) << "Имя пользователя" << setw(10) << "Логин" << endl;
	for (int i = 0; i < usrBase->size(); i++)
	{
		cout << setw(17) << i << setw(20) << usrBase->at(i).name << setw(10) << usrBase->at(i).login << endl;
	}
}

void UserBase::addUsers(const char name[], const char login[], const char pwd[])
{
	string sname = name;
	string slogin = login;
	string spwd = pwd;
	addUsers(sname, slogin, spwd);
}

void UserBase::addUsers(string& name, string& login, string& pwd)
{
	string pwdsha = sha256(pwd);
	User newUser(name, login, pwdsha);
	addUsers(newUser);
}

void UserBase::addUsers(User& newUser)
{
	usrBase->push_back(newUser);
}

void UserBase::chgPwd(int userId, string& pwd)
{
	net* start = new net;
	string usrPkg = "CHG_PWD";
	usrPkg.append(delim);
	usrPkg.append(std::to_string(userId));
	usrPkg.append(delim);
	usrPkg.append(sha256(pwd));
	start->sendReq(usrPkg.data());
	delete start;
	getUserBase();
}

int UserBase::getUserCount()
{
	return usrBase->size();
}

int UserBase::getUserId(string& login)
{
	for (int i = 0; i < usrBase->size(); i++)
	{
		if (usrBase->at(i).login == login)
		{
			return i;
		}
	}
	return -1;
}

string UserBase::getUserName(int userId)
{
	return usrBase->at(userId).name;
}

void UserBase::delUser(int userId)
{
	net* start = new net;
	string usrPkg = "DEL_USER";
	usrPkg.append(delim);
	usrPkg.append(std::to_string(userId));
	start->sendReq(usrPkg.data());
	delete start;
	getUserBase();
}

bool UserBase::loginCheck(string& login)
{
	for (int i = 0; i < usrBase->size(); i++)
	{
		if (usrBase->at(i).login == login)
		{
			return true;
		}
	}
	return false;
}

bool UserBase::pwdCheck(int userId, string& pwd)
{
	string temp = sha256(pwd);
	if (usrBase->at(userId).pwd == temp)
	{
		return true;
	}
	else
	{
		return false;
	}
}