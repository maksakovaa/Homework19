#include "chat.h"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PACKAGE_LENGTH 1024
#define PORT 9999

UserBase* Users = new UserBase();
Chat* mainChat = new Chat();
string delim = "<|>";

struct sockaddr_in srvaddress, client;
socklen_t length;
int socket_fd, connection, bind_status, connect_status;
char package[PACKAGE_LENGTH];

void sendUsrBase()
{
    cout << "GET_USRBASE request accepted" << endl;
    for (int i = 0; i < Users->getUserCount(); i++)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, Users->packUser(i).data());
        ssize_t bytes = write(connection, package, sizeof(package));
        if (bytes >= 0)
        {
            cout << "USRBASE package " << i << " sent. Contains: " << package << endl;
        }
        if (i == Users->getUserCount() - 1)
        {
            ssize_t bytes2 = write(connection, "USRBASE_END", sizeof("USRBASE_END"));
            if (bytes2 >= 0)
            {
                cout << "USRBASE sended" << endl;
            }
        }
    }
}

void sendMsgBase()
{
    cout << "GET_MSGBASE request accepted" << endl;
    
    if (mainChat->getMsgCount() == 0)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, "MSGBASE_EMPTY");
        ssize_t bytes = write(connection, package, sizeof(package));
        if (bytes >= 0)
        {
            cout << "MSGBASE EMPTY" << endl;
        }
        return;
    }
    
    for (int i = 0; i < mainChat->getMsgCount(); i++)
    {
        bzero(package, PACKAGE_LENGTH);
        strcpy(package, mainChat->packMsg(i).data());
        ssize_t bytes = write(connection, package, sizeof(package));
        if (bytes >= 0)
        {
            cout << "MSGBASE package " << i << " sent" << endl;
        }
        if(i == mainChat->getMsgCount()-1)
        {
            ssize_t bytes = write(connection, "MSGBASE_END", sizeof("MSGBASE_END"));
            if (bytes >= 0)
            {
                cout << "MSGBASE sended" << endl;
            }
        }
    }
}

void regUser()
{
    cout << "REG_USER request accepted" << endl;
    string temp = package;
    string array[2];
    temp.erase(0, temp.find(delim) + delim.length());
    User newUser = Users->splitUsrPkg(temp); 
	Users->addUsers(newUser);
    cout << "User " << array[0] << " registered" << endl;
}

void regMSG()
{
    cout << "SND_MSGBASE request accepted" << endl;
    string temp = package;
    mainChat->sendMsg(mainChat->splitMsgPkg(temp));
}

int main()
{
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
    cout << "Сервер приложения Chat запущен." << endl;
    while (true)
    {
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
        if (strncmp("GET_USRBASE", package, sizeof("GET_USRBASE")-1) == 0)
        {
            sendUsrBase();
        }
        else if (strncmp("GET_MSGBASE", package, sizeof("GET_MSGBASE")-1) == 0)
        {
            sendMsgBase();
        }
        else if (strncmp("REG_USER", package, sizeof("REG_USER")-1) == 0)
        {
            regUser();   
        }
        else if (strncmp("SND_MSG", package, sizeof("SND_MSG")-1) == 0)
        {
            regMSG();
        }
    }
    close(socket_fd);
    delete mainChat;
    delete Users;
    return 0; 
}