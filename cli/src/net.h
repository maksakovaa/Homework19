#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#if defined (_WIN32) || defined (_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#pragma warning(disable : 4996)
#define PORT "9999"
#elif defined (__linux__)
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 9999
#endif

#define PACKAGE_LENGTH 1024
#define SERVER "127.0.0.1"

using std::cout;
using std::endl;
using std::string;

class net
{
private:
#if defined (_WIN32) || defined (_WIN64)
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, *ptr = NULL;
#elif defined (__linux__)
    int socket_fd, connection;
    struct sockaddr_in srvaddress, client;
#endif
    char package[PACKAGE_LENGTH];
public:
    net();
    ~net();
    void sendReq(const char* package);
    char* readmsg();
    void getUsrBase(std::vector<string>& users);
    void getMsgBase(std::vector<string>* sMsg);
    void regUser(string& usrPkg);
    void regMsg(string msgPkg);
};