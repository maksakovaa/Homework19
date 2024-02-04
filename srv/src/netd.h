#pragma once
#include "chatd.h"
#if defined (_WIN32) || defined (_WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
#include <string.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#pragma warning(disable : 4996)
#define PORT "9999"
#elif defined (__linux__)
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 9999
#endif

#define PACKAGE_LENGTH 1024
extern UserBase* Users;
extern Chat* mainChat;

class net
{
public:
	net();
	~net();
	void netGateway();
	void sendRequest(string& reqName);
	void sendUsrBase();
	void sendMsgBase();
	void regUser();
	void regMSG();
	void delUsr();
	void chgPwd();
private:
	string reqName;
	string delim = "<|>";
	char package[PACKAGE_LENGTH];
#if defined (_WIN32) || defined (_WIN64)
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

#elif defined (__linux__)
	struct sockaddr_in srvaddress, client;
	socklen_t length;
	int socket_fd, connection, bind_status, connect_status;
#endif
};