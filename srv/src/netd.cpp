#include "netd.h"
net::net()
{
    bzero(package, PACKAGE_LENGTH);
#if defined (_WIN32) || (_WIN64)
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "ERROR: WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

#elif defined (__linux__)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        cout << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    srvaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddress.sin_port = htons(PORT);
    srvaddress.sin_family = AF_INET;

    bind_status = bind(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
    if (bind_status == -1)
    {
        cout << "ERROR: Ошибка привязки сокета." << endl;
        exit(1);
    }
#endif
}

net::~net()
{
#if defined (_WIN32) || defined (_WIN64)
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("ERROR: shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }

    closesocket(ClientSocket);
    WSACleanup();
#elif defined (__linux__)
    close(socket_fd);
#endif
}

void net::netGateway()
{
    cout << "Сервер приложения Chat запущен." << endl;
    while (true)
    {
#if defined (_WIN32) || defined (_WIN64)
        iResult = getaddrinfo(NULL, PORT, &hints, &result);
        if (iResult != 0)
        {
            cout << "ERROR: getaddrinfo failed with error: " << iResult << endl;
            WSACleanup();
            exit(1);
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            exit(1);
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            cout << "ERROR: Ошибка при постановке на приём данных: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            cout << "ERROR: Ошибка принятия покдключения: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        closesocket(ListenSocket);
        iResult = recv(ClientSocket, package, PACKAGE_LENGTH, 0);
#elif defined (__linux__)
        connect_status = listen(socket_fd, 20);
        if (connect_status == -1)
        {
            cout << "ERROR: Ошибка при постановке на приём данных." << endl;
            exit(1);
        }
        else
        {
            cout << "Ожидание подключений..." << endl;
        }
        length = sizeof(client);
        connection = accept(socket_fd, (struct sockaddr*)&client, &length);
        if (connection == -1)
        {
            cout << "ERROR: Сервер несмог принять данные от клиента." << endl;
            exit(1);
        }
        bzero(package, PACKAGE_LENGTH);
        read(connection, package, sizeof(package));
#endif
        if (strncmp("GET_USRBASE", package, sizeof("GET_USRBASE") - 1) == 0)
        {
            sendUsrBase();
        }
        else if (strncmp("GET_MSGBASE", package, sizeof("GET_MSGBASE") - 1) == 0)
        {
            sendMsgBase();
        }
        else if (strncmp("REG_USER", package, sizeof("REG_USER") - 1) == 0)
        {
            regUser();
        }
        else if (strncmp("SND_MSG", package, sizeof("SND_MSG") - 1) == 0)
        {
            regMSG();
        }
        else if (strncmp("DEL_USER", package, sizeof("DEL_USER") - 1) == 0)
        {
            delUsr();
        }
        else if (strncmp("CHG_PWD", package, sizeof("CHG_PWD") - 1) == 0)
        {
            chgPwd();
        }

    }
}

void net::sendRequest(string& reqName)
{
#if defined (_WIN32) || defined (_WIN64)
    int iSendResult = send(ClientSocket, package, sizeof(package), 0);
    if (iSendResult == SOCKET_ERROR)
    {
        cout << "ERROR: Ошибка отправки запроса: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }
    else
    {
        cout << reqName << " send" << endl;
    }
#elif defined (__linux__)
    ssize_t bytes = write(connection, package, sizeof(package));
    if (bytes >= 0)
    {
        cout << reqName << " send" << endl;
    }
#endif
}

void net::sendUsrBase()
{
    cout << "GET_USRBASE request accepted" << endl;
    for (int i = 0; i < Users->getUserCount(); i++)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, Users->packUser(i).data());
        reqName = "USRBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == Users->getUserCount() - 1)
        {
            bzero(package, PACKAGE_LENGTH);
            strcpy(package, "USRBASE_END");
            reqName = "USRBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::sendMsgBase()
{
    cout << "GET_MSGBASE request accepted" << endl;

    if (mainChat->getMsgCount() == 0)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, "MSGBASE_EMPTY");
        reqName = "MSGBASE_EMPTY";
        sendRequest(reqName);
        return;
    }

    for (int i = 0; i < mainChat->getMsgCount(); i++)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, mainChat->packMsg(i).data());
        reqName = "MSGBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == mainChat->getMsgCount() - 1)
        {
            bzero(package, PACKAGE_LENGTH);
            strcpy(package, "MSGBASE_END");
            reqName = "MSGBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::regUser()
{
    cout << "REG_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    User newUser = Users->splitUsrPkg(temp);
    Users->addUsers(newUser);
    cout << "User " << newUser.name << " registered" << endl;
}

void net::regMSG()
{
    cout << "SND_MSG request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    Message newMsg = mainChat->splitMsgPkg(temp);
    mainChat->sendMsg(newMsg);
    cout << "Message " << newMsg.msg << " send" << endl;
}

void net::delUsr()
{
    cout << "DEL_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    int userId = stoi(temp);
    Users->delUser(userId);
    cout << "User with ID " << userId << " deleted." << endl;
}

void net::chgPwd()
{
    cout << "CHG_PWD request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    int userId = stoi(temp.substr(0, temp.find(delim)));
    string pwd = temp.erase(0, temp.find(delim) + delim.length());
    Users->chgPwd(userId, pwd);
    cout << "Password for user with ID " << userId << " changed" << endl;
}