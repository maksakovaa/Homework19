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
	std::ifstream userbase_file(UBPath);
	if (!userbase_file.is_open())
	{
		User admin("Administartor", "admin", sha256("admin2023"));
		addUsers(admin);
	}
	if(userbase_file.is_open())
	{
		string s;
		string delim = "<|>";
		while (getline(userbase_file, s))
		{
			size_t pos = 0;
			int i = 0;
			string array[2];
			while((pos = s.find(delim)) != string::npos)
			{
				array[i++] = s.substr(0,pos);
				s.erase(0, pos + delim.length());
			}
			User newUser(array[0], array[1], s);
			addUsers(newUser);
		}
		userbase_file.close();
	}

}

UserBase::~UserBase()
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
			userbase_file << usrBase->at(i).name;
			userbase_file << "<|>";
			userbase_file << usrBase->at(i).login;
			userbase_file << "<|>";
			userbase_file << usrBase->at(i).pwd;
			userbase_file << "\n";
		}
		userbase_file.close();
	}
	delete usrBase;
}

void UserBase::showUsers()
{
	cout << setw(17) << "ID пользовател¤" << setw(20) << "Имя пользователя" << setw(10) << "Логин" << endl;
	for (int i = 0; i < usrBase->size(); i++)
	{
		cout << setw(17) << i << setw(20) << usrBase->at(i).name << setw(10) << usrBase->at(i).login << endl;
	}
}

string UserBase::getUser(int userId)
{
	string result = "";
	result.append(usrBase->at(userId).name);
	result.append("<|>");
	result.append(usrBase->at(userId).login);
	result.append("<|>");
	result.append(usrBase->at(userId).pwd);
	return result;
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
	usrBase->at(userId).pwd = sha256(pwd);
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
	usrBase->erase(usrBase->begin() + userId);
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
