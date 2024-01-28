#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 
using namespace std;
 
#define PACKAGE_LENGTH 1024
#define PORT 9999
#define SERVER "192.168.0.1"

int socket_fd, connection;
struct sockaddr_in srvaddress, client;

char package[PACKAGE_LENGTH];