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
	getUsrBase();
}

UserBase::~UserBase()
{
	delete usrBase;
}

User UserBase::parsingUsrPkg(string& pkg)
{
	size_t pos = 0;
	int i = 0;
	string array[2];
	string delim = "<|>";
	while ((pos = pkg.find(delim)) != string::npos)
	{
		array[i++] = pkg.substr(0,pos);
		pkg.erase(0, pos + delim.length());
	}
	User newUser(array[0], array[1], pkg);
	return newUser;
}

void UserBase::getUsrBase()
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
	if(socket_fd == -1)
    {
        cout << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    
	srvaddress.sin_addr.s_addr = inet_addr(SERVER);
    srvaddress.sin_port = htons(PORT);
    srvaddress.sin_family = AF_INET;

    connection = connect(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
 
    if(connection == -1)
    {
        cout << "ERROR: Ошибка подключения к серверу." << endl;
        exit(1);
    }

    bzero(package, sizeof(package));
    ssize_t bytes = write(socket_fd, "GET_USRBASE", sizeof("GET_USRBASE"));
    bzero(package, sizeof(package));

	while (strncmp("USRBASE_END", package, 11) != 0)
	{
		read(socket_fd, package, sizeof(package));
		if (strncmp("USRBASE_END", package, 11) == 0)
		{
			ssize_t bytes = write(socket_fd, "DISCONNECT", sizeof("DISCONNECT"));
			break;
		}	
		string temp = package;
		User newUser = parsingUsrPkg(temp);
		addUsers(newUser);
	}
	close(socket_fd);
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