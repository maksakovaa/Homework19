#include "netd.h"
net::net()
{
    readConfig();
    bzero(package, package_length);
#if defined (_WIN32) || (_WIN64)
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << curDateTime() << "ERROR: WSAStartup failed with error: " << iResult << endl;
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
        cout << curDateTime() << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    srvaddress.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddress.sin_port = htons(std::stoi(chat_port));
    srvaddress.sin_family = AF_INET;

    bind_status = bind(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
    if (bind_status == -1)
    {
        cout << curDateTime() << "ERROR: Ошибка привязки сокета." << endl;
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

void net::readConfig()
{
    std::ifstream config(netCfgPath);
    if (config.is_open())
    {
        string str;
        while (std::getline(config, str))
        {
            if (strncmp("server_ip", str.data(), (int)strlen("server_ip")) == 0)
                server_ip = str.erase(0, str.find(delim_settings) + delim_settings.length());
            if (strncmp("chat_port", str.data(), (int)strlen("chat_port")) == 0)
                chat_port = str.erase(0, str.find(delim_settings) + delim_settings.length());
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

void net::netGateway()
{
    cout << curDateTime() << "Сервер приложения Chat запущен." << endl;
    while (true)
    {
#if defined (_WIN32) || defined (_WIN64)
        iResult = getaddrinfo(NULL, chat_port.data(), &hints, &result);
        if (iResult != 0)
        {
            cout << curDateTime() << "ERROR: getaddrinfo failed with error: " << iResult << endl;
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
            cout << curDateTime() << "ERROR: Ошибка при постановке на приём данных: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
            cout << curDateTime() << "ERROR: Ошибка принятия покдключения: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }

        closesocket(ListenSocket);
        iResult = recv(ClientSocket, package, package_length, 0);
#elif defined (__linux__)
        connect_status = listen(socket_fd, 20);
        if (connect_status == -1)
        {
            cout << curDateTime() << "ERROR: Ошибка при постановке на приём данных." << endl;
            exit(1);
        }
        else
        {
            cout << curDateTime() << "Ожидание подключений..." << endl;
        }
        length = sizeof(client);
        connection = accept(socket_fd, (struct sockaddr*)&client, &length);
        if (connection == -1)
        {
            cout << curDateTime() << "ERROR: Сервер несмог принять данные от клиента." << endl;
            exit(1);
        }
        bzero(package, package_length);
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
        cout << curDateTime() << "ERROR: Ошибка отправки запроса: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }
    else
    {
        cout << curDateTime() << reqName << " send" << endl;
    }
#elif defined (__linux__)
    ssize_t bytes = write(connection, package, sizeof(package));
    if (bytes >= 0)
    {
        cout << curDateTime() << reqName << " send" << endl;
    }
#endif
}

void net::sendUsrBase()
{
    cout << curDateTime() << "GET_USRBASE request accepted" << endl;
    for (int i = 0; i < Users->getUserCount(); i++)
    {
        bzero(package, package_length);
        strcpy(package, Users->packUser(i).data());
        reqName = "USRBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == Users->getUserCount() - 1)
        {
            bzero(package, package_length);
            strcpy(package, "USRBASE_END");
            reqName = "USRBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::sendMsgBase()
{
    cout << curDateTime() << "GET_MSGBASE request accepted" << endl;

    if (mainChat->getMsgCount() == 0)
    {
        bzero(package, package_length);
        strcpy(package, "MSGBASE_EMPTY");
        reqName = "MSGBASE_EMPTY";
        sendRequest(reqName);
        return;
    }

    for (int i = 0; i < mainChat->getMsgCount(); i++)
    {
        bzero(package, package_length);
        strcpy(package, mainChat->packMsg(i).data());
        reqName = "MSGBASE package ";
        sendRequest(reqName.append(std::to_string(i)));
        if (i == mainChat->getMsgCount() - 1)
        {
            bzero(package, package_length);
            strcpy(package, "MSGBASE_END");
            reqName = "MSGBASE_END";
            sendRequest(reqName);
        }
    }
}

void net::regUser()
{
    cout << curDateTime() << "REG_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    User newUser = Users->splitUsrPkg(temp);
    Users->addUsers(newUser);
    cout << curDateTime() << "User " << newUser.name << " registered" << endl;
}

void net::regMSG()
{
    cout << curDateTime() << "SND_MSG request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    Message newMsg = mainChat->splitMsgPkg(temp);
    mainChat->sendMsg(newMsg);
    cout << curDateTime() << "Message " << newMsg.msg << " send" << endl;
}

void net::delUsr()
{
    cout << curDateTime() << "DEL_USER request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    int userId = stoi(temp);
    Users->delUser(userId);
    cout << curDateTime() << "User with ID " << userId << " deleted." << endl;
}

void net::chgPwd()
{
    cout << curDateTime() << "CHG_PWD request accepted" << endl;
    string temp = package;
    temp.erase(0, temp.find(delim) + delim.length());
    int userId = stoi(temp.substr(0, temp.find(delim)));
    string pwd = temp.erase(0, temp.find(delim) + delim.length());
    Users->chgPwd(userId, pwd);
    cout << curDateTime() << "Password for user with ID " << userId << " changed" << endl;
}
