#include "net.h"

net::net()
{
    readConfig();
#if defined (_WIN32) || defined (_WIN64)
    bzero(package, package_length);
    WSADATA wsaData;
    struct addrinfo hints;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << curDateTime() << "ERROR: WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(server_ip.data(), chat_port.data(), &hints, &result);
    if (iResult != 0)
    {
        cout << curDateTime() << "ERROR: getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        exit(1);
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            cout << curDateTime() << "ERROR: Socket failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            exit(1);
        }

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << curDateTime() << "ERROR: Не удалось подключиться к серверу." << endl;
        WSACleanup();
        exit(1);
    }

#elif defined (__linux__)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1)
    {
        cout << curDateTime() << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
       
    srvaddress.sin_addr.s_addr = inet_addr(server_ip.data());
    srvaddress.sin_port = htons(stoi(chat_port));
    srvaddress.sin_family = AF_INET;

    connection = connect(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
    
    if(connection == -1)
    {
        cout << curDateTime() << "ERROR: Ошибка подключения к серверу." << endl;
        exit(1);
    }
#endif
}

net::~net()
{
#if defined (_WIN32) || defined (_WIN64)
    closesocket(ConnectSocket);
    WSACleanup();
#elif defined (__linux__)
    close(socket_fd);
#endif
}

void net::readConfig()
{
    std::ifstream config(netCfgPath);
    if (config.is_open())
    {
        string str;
        while (std::getline(config, str))
        {
            if (strncmp("server_ip", str.data(), (int)strlen("server_ip")) == 0)
                server_ip = str.erase(0, str.find(delim) + delim.length());
            if (strncmp("chat_port", str.data(), (int)strlen("chat_port")) == 0)
                chat_port = str.erase(0, str.find(delim) + delim.length());
        }
        config.close();
    }
    else
    {
        server_ip = "127.0.0.1";
        chat_port = "9999";
        saveConfig();
    }
}

void net::saveConfig()
{
    std::ofstream config(netCfgPath, std::ios::trunc);
    if (!config.is_open())
    {
        cout << curDateTime() << "Ошибка открытия файла!" << endl;
    }
    else
    {
        config << "server_ip = ";
        config << server_ip;
        config << "\n";
        config << "chat_port = ";
        config << chat_port;
        config << "\n";
        config.close();
    }
}

void net::sendReq(const char* package)
{
#if defined (_WIN32) || defined (_WIN64)
    int iResult = send(ConnectSocket, package, (int)strlen(package), 0);
    if (iResult == SOCKET_ERROR)
    {
        cout << curDateTime() << "ERROR: Ошибка отправки: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        cout << curDateTime() << "ERROR: Ошибка выключения: " << WSAGetLastError() <<endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
#elif defined (__linux__)
    ssize_t bytes = write(socket_fd, package, (int)strlen(package));  
#endif
}

char* net::readmsg()
{
    bzero(package, package_length);
#if defined (_WIN32) || defined (_WIN64)
    int iResult = recv(ConnectSocket, package, sizeof(package), 0);
    if (iResult == 0)
    {
        cout << curDateTime() << "Connection closed" << endl;
    }
    else if (iResult > 0) {}
    else
    {
        cout << curDateTime() << "ERROR: recv failed with error: " << WSAGetLastError();
        exit(1);
    }
#elif defined (__linux__)
    read(socket_fd, package, sizeof(package));
#endif
    return package;
}

void net::getUsrBase(std::vector<string>& users)
{
    users.clear();
    bzero(package, package_length);
    while (strncmp("USRBASE_END", readmsg(), sizeof("USRBASE_END")) != 0)
	{
        users.push_back(package);
	}
    if (strncmp("USRBASE_END", package, sizeof("USRBASE_END")) == 0)
    {
        return;
    }
}

void net::regUser(string& usrPkg)
{
    bzero(package, package_length);
    strcpy(package, usrPkg.data());
    sendReq(package);
}

void net::getMsgBase(std::vector<string>* sMsg)
{
    while (strncmp("MSGBASE_END", readmsg(), 11) != 0)
	{
        if (strncmp("MSGBASE_EMPTY", package, sizeof("MSGBASE_EMPTY")) == 0)
        {
            cout << curDateTime() << "MSGBASE_EMPTY" << endl;
            break;
            return;
        }
		if (strncmp("MSGBASE_END", package, sizeof("MSGBASE_END")) == 0)
		{
			return;
		}
		string temp = package;
        sMsg->push_back(package);
	}
}

void net::regMsg(string msgPkg)
{
    bzero(package, package_length);
    strcpy(package, msgPkg.data());
    sendReq(package);
}