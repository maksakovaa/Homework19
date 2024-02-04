#pragma once
#include "UserBased.h"
#include <ctime>

struct Message
{
	string msgTo;
	string msgFrom;
	string msgDate;
	string msg;
	Message(string msgTo_, string msgFrom_, string msg_) : msgTo(msgTo_), msgFrom(msgFrom_), msg(msg_)
	{
		time_t now = time(NULL);
		char str[26];
#if defined (_WIN32) || (_WIN64)
		ctime_s(str, 26, &now);
#elif defined (__linux__)
        ctime_r(&now, str);
#endif
		msgDate = str;
		msgDate.pop_back();
	}
	Message(string msgTo_, string msgFrom_, string msgDate_, string msg_) : msgTo(msgTo_), msgFrom(msgFrom_), msgDate(msgDate_), msg(msg_) {}
};

class Chat
{
public:
	Chat();
	~Chat();
	int getMsgCount();
	void sendMsg(Message newMsg);
	string packMsg(int msgId);
	Message splitMsgPkg(string& msgPkg);
	void saveMsgBase();
private:
	string delim = "<|>";
	std::vector<Message>* msgBase;
#if defined (_WIN32) || defined (_WIN64)
	string MBPath = "srv_msg_base.dat";
#elif defined (__linux__)
string MBPath = "/var/lib/chatd/msg_base.dat";
#endif
};
