#include "net.h"

net::net()
{
#if defined (_WIN32) || defined (_WIN64)
    bzero(package, PACKAGE_LENGTH);
    WSADATA wsaData;
    int iResult;
    struct addrinfo hints;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "ERROR: WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(SERVER, PORT, &hints, &result);
    if (iResult != 0)
    {
        cout << "ERROR: getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return;
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
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

#elif defined (__linux__)
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

void net::sendReq(const char* package)
{
#if defined (_WIN32) || defined (_WIN64)
    int iResult = send(ConnectSocket, package, (int)strlen(package), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
#elif defined (__linux__)
    ssize_t bytes = write(socket_fd, package, (int)strlen(package));
    if (bytes >= 0)
    {
        cout << "Send " << package << endl;
    }
    
#endif
}

char* net::readmsg()
{
    bzero(package, PACKAGE_LENGTH);
#if defined (_WIN32) || defined (_WIN64)
    int iResult = recv(ConnectSocket, package, sizeof(package), 0);
    if (iResult == 0)
    {
        cout << "Connection closed" << endl;
    }
    else if (iResult > 0)
    {
        cout << "Recived " << iResult << " bytes." << endl;
    }
    else
    {
        cout << "ERROR: recv failed with error: " << WSAGetLastError();
    }
#elif defined (__linux__)
    read(socket_fd, package, sizeof(package));
#endif
    return package;
}

void net::getUsrBase(std::vector<string>& users)
{
    users.clear();
    bzero(package, PACKAGE_LENGTH);
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
    bzero(package, PACKAGE_LENGTH);
    strcpy(package, usrPkg.data());
    sendReq(package);
}

void net::getMsgBase(std::vector<string>* sMsg)
{
    while (strncmp("MSGBASE_END", readmsg(), 11) != 0)
	{
        if (strncmp("MSGBASE_EMPTY", package, sizeof("MSGBASE_EMPTY")) == 0)
        {
            cout << "MSGBASE_EMPTY" << endl;
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
    bzero(package, PACKAGE_LENGTH);
    strcpy(package, msgPkg.data());
    sendReq(package);
}
