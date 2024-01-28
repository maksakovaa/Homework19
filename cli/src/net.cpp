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

void net::sendmsg(char* command)
{
    cout << sizeof(command) << endl;
    cout << sizeof("GET_USRBASE") << endl;
    ssize_t bytes = write(socket_fd, command, 12);
    if (bytes >= 0)
    {
        cout << command << " request sent" << endl;
    }
}
void net::readmsg()
{
    bzero(package, PACKAGE_LENGTH);
    read(socket_fd, package, sizeof(package));
}

void net::getUsrBase(std::vector<string>* users)
{
    while (strncmp("USRBASE_END", package, 11) != 0)
	{
        readmsg();
		if (strncmp("USRBASE_END", package, 11) == 0)
		{
			ssize_t bytes = write(socket_fd, "DISCONNECT", sizeof("DISCONNECT"));
			break;
		}
        users->push_back(package);
	}
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
    while (strncmp("MSGBASE_END", package, 11) != 0)
	{
		readmsg();		
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