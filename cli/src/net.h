#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
  
#define PACKAGE_LENGTH 1024
#define PORT 9999
#define SERVER "192.168.0.1"

using std::cout;
using std::endl;

class net
{
private:
    int socket_fd, connection;
    struct sockaddr_in srvaddress, client;
    char package[PACKAGE_LENGTH];
public:
    net();
    ~net();
    void sendmsg(const char* msg);
    void getUsrBase();
    void getMsgBase();
    User parsingUsrPkg(string& pkg);
};