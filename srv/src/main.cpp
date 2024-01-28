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

struct sockaddr_in srvaddress, client;
socklen_t length;
int socket_fd, connection, bind_status, connect_status;
char package[PACKAGE_LENGTH];

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
    while (1)
    {
        connect_status = listen(socket_fd, 10);
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
        if (strncmp("CRC_USRBASE", package, 11) == 0)
        {
            
        }     
        else if (strncmp("GET_USRBASE", package, 11) == 0)
        {
            for (int i = 0; i < Users->getUserCount(); i++)
            {
                strcpy(package, Users->getUser(i).data());
                ssize_t bytes = write(connection, package, sizeof(package));
                if (bytes >= 0)
                {
                    cout << "USRBASE package " << i << " sent" << endl;
                }
                if(i == Users->getUserCount()-1)
                {
                    ssize_t bytes = write(connection, "USRBASE_END", sizeof("USRBASE_END"));
                    if (bytes >= 0)
                    {
                        cout << "USRBASE sended" << endl;
                    }
                }
            }
        }
        else if (strncmp("CRC_MSGBASE", package, 11))
        {
            
        }
        else if (strncmp("GET_MSGBASE", package, 11) == 0)
        {
            for (int i = 0; i < mainChat->getMsgCount(); i++)
            {
                strcpy(package, mainChat->getMsg(i).data());
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
        else if (strncmp("REG_USER", package, 8) == 0)
        {

        }
        else if (strncmp("SND_MSG", package, 7) == 0)
        {
            
        }
    }
    close(socket_fd);
    delete mainChat;
    delete Users;
    return 0; 
}