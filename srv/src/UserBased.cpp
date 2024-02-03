#include "UserBased.h"

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
	std::ifstream userbase_file(UBPath);
	if (!userbase_file.is_open())
	{
		User admin("Administartor", "admin", sha256("admin2023"));
		addUsers(admin);
	}
	if(userbase_file.is_open())
	{
		string s;
		while (getline(userbase_file, s))
		{
			usrBase->push_back(splitUsrPkg(s));
		}
		userbase_file.close();
	}
}

UserBase::~UserBase()
{
	saveUsrBase();
	delete usrBase;
}

string UserBase::packUser(int userId)
{
	string result;
	result.append(usrBase->at(userId).name);
	result.append(delim);
	result.append(usrBase->at(userId).login);
	result.append(delim);
	result.append(usrBase->at(userId).pwd);
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

void UserBase::addUsers(string& name, string& login, string& pwd)
{
	string pwdsha = sha256(pwd);
	User newUser(name, login, pwdsha);
	addUsers(newUser);
}

void UserBase::addUsers(User& newUser)
{
	usrBase->push_back(newUser);
	saveUsrBase();
}


int UserBase::getUserCount()
{
	return usrBase->size();
}

void UserBase::saveUsrBase()
{
	std::ofstream userbase_file(UBPath, std::ios::trunc);
	if(!userbase_file.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else
	{
		for (int i = 0; i < usrBase->size(); i++)
		{
			userbase_file << packUser(i);
			userbase_file << "\n";
		}
		userbase_file.close();
	}
}


void UserBase::chgPwd(int userId, string& pwd)
{
	usrBase->at(userId).pwd = sha256(pwd);
}

void UserBase::delUser(int userId)
{
	usrBase->erase(usrBase->begin() + userId);
}