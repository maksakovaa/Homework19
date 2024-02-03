#include "chatd.h"

Chat::Chat()
{
	try
	{
		msgBase = new std::vector<Message>;
	}
	catch (std::bad_alloc& ex)
	{
		cout << "ERROR: caught bad_alloc: " << ex.what() << endl;
	}
	std::ifstream msgbase_file(MBPath);
	if(msgbase_file.is_open())
	{
		string s;
		while (getline(msgbase_file, s))
		{
			msgBase->push_back(splitMsgPkg(s));
		}
		msgbase_file.close();
	}
}

Chat::~Chat()
{
	if (getMsgCount() > 0)
	{
		saveMsgBase();
	}
	delete msgBase;
}

int Chat::getMsgCount()
{
	return msgBase->size();
}

void Chat::sendMsg(Message newMsg)
{
	msgBase->push_back(newMsg);
	saveMsgBase();
}

string Chat::packMsg(int msgId)
{
	string result;
	result.append(msgBase->at(msgId).msgTo);
	result.append(delim);
	result.append(msgBase->at(msgId).msgFrom);
	result.append(delim);
	result.append(msgBase->at(msgId).msgDate);
	result.append(delim);
	result.append(msgBase->at(msgId).msg);
    return result;
}

Message Chat::splitMsgPkg(string& msgPkg)
{
	size_t pos = 0;
	int i = 0;
	string sa[3];
	while((pos = msgPkg.find(delim)) != string::npos)
	{
		sa[i++] = msgPkg.substr(0,pos);
		msgPkg.erase(0, pos + delim.length());
	}
	Message newMsg(sa[0], sa[1], sa[2], msgPkg);
	return newMsg;
}

void Chat::saveMsgBase()
{
	std::ofstream msgbase_file(MBPath, std::ios::trunc);
	if(!msgbase_file.is_open())
	{
		cout << "Ошибка открытия файла!" << endl;
	}
	else
	{
		for (int i = 0; i < msgBase->size(); i++)
		{
			msgbase_file << packMsg(i);
			msgbase_file << "\n";
		}
		msgbase_file.close();
	}
}