#include "net.h"

net::net()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1)
    {
        cout << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    else
    {
        cout << "Socket created" << endl;
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
    else
    {
        cout << "Подключение установлено" << endl;
    }
}

net::~net()
{
    close(socket_fd);
    cout << "Socket closed" << endl;
}

void net::sendReq(char* package, int size)
{
    ssize_t bytes = write(socket_fd, package, size);
    if (bytes >= 0)
    {
        cout << package << " request sent. Size: " << size << endl;
    }
}

char* net::readmsg()
{
    bzero(package, PACKAGE_LENGTH);
    read(socket_fd, package, sizeof(package));
    return package;
}

void net::getUsrBase(std::vector<string>& users)
{
    users.clear();
    cout << "net getUsrBase start" << endl;
    bzero(package, PACKAGE_LENGTH);
    while (strncmp("USRBASE_END", readmsg(), sizeof("USRBASE_END")) != 0)
	{
        users.push_back(package);
        cout << users.size() << package << endl;
	}
    if (strncmp("USRBASE_END", package, sizeof("USRBASE_END")) == 0)
    {
        cout << "USRBASE_END" << endl;
        cout << "net getUsrBase complete" << endl;
        return;
    }
}

void net::regUser(string& usrPkg)
{
    bzero(package, PACKAGE_LENGTH);
    strcpy(package, usrPkg.data());
    sendReq(package, usrPkg.length());
}

void net::getMsgBase()
{
    bzero(package, PACKAGE_LENGTH);
    #if defined (_WIN32) || defined (_WIN64)
	string MBPath = "msg_base.dat";
    #elif defined (__linux__)
    string MBPath = "/var/lib/Chat/msg_base.dat";
    #endif
    std::ofstream msgbase_file(MBPath, std::ios::trunc);
    while (strncmp("MSGBASE_END", readmsg(), 11) != 0)
	{
        if (strncmp("MSGBASE_EMPTY", package, sizeof("MSGBASE_EMPTY")) == 0)
        {
            break;
        }
		if (strncmp("MSGBASE_END", package, sizeof("MSGBASE_END")) == 0)
		{
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

void net::regMsg(string msgPkg)
{
    bzero(package, PACKAGE_LENGTH);
    strcpy(package, msgPkg.data());
    sendReq(package, msgPkg.length());
}