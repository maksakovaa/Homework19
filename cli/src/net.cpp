#include "net.h"
#include "UserBase.h"
#include "chat.h"

extern UserBase* Users;
extern Chat* mainChat;

net::net()
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
}

net::~net()
{
    close(socket_fd);
}

void net::sendmsg(const char* msg)
{
    ssize_t bytes = write(socket_fd, msg, sizeof(msg));
}

void net::getUsrBase()
{
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
		Users->addUsers(newUser);
	}
}

void net::getMsgBase()
{
    std::ofstream msgbase_file(mainChat->getMBPath(), std::ios::trunc);
    while (strncmp("MSGBASE_END", package, 11) != 0)
	{
		read(socket_fd, package, sizeof(package));
		
		if (strncmp("MSGBASE_END", package, 11) == 0)
		{
			ssize_t bytes = write(socket_fd, "DISCONNECT", sizeof("DISCONNECT"));		
			break;
		}
		string temp = package;
		if(!msgbase_file.is_open())
		{
			cout << "ERROR: Ошибка открытия файла!" << endl;
		}
		else
		{
			msgbase_file << temp;
			msgbase_file << "\n";
		}
	}
   	msgbase_file.close();
}

User net::parsingUsrPkg(string& pkg)
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