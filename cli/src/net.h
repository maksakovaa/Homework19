#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "sha256.h"
#if defined (_WIN32) || defined (_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define bzero(b,len) (memset((b), '0', (len)), (void) 0)
#pragma warning(disable : 4996)
#elif defined (__linux__)
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

using std::cout;
using std::endl;
using std::string;

class net
{
private:
#if defined (_WIN32) || defined (_WIN64)
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, *ptr = NULL;
    struct addrinfo hints;
    string netCfgPath = "net_settings.ini";
#elif defined (__linux__)
    string netCfgPath = "/var/lib/Chat/net_settings.ini";
    int socket_fd, connection;
    struct sockaddr_in srvaddress, client;
#endif
    static const int package_length = 1024;
    char package[package_length];
    string delim = " = ";
    string server_ip;
    string chat_port;
public:
    net();
    ~net();
    void readConfig();
    void saveConfig();
    void sendReq();
    void sendReq(string& usrRequest);
    void readmsg();
    void getUsrBase(std::vector<string>& users);
    void getMsgBase(std::vector<string>& sMsg);
};