#pragma once
#include "UserBased.h"

struct Message
{
	string msgTo;
	string msgFrom;
	string msgDate;
	string msg;
	Message(string msgTo_, string msgFrom_, string msg_) : msgTo(msgTo_), msgFrom(msgFrom_), msg(msg_)
	{
		msgDate = curDateTime();
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
