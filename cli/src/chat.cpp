#include "chat.h"

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
	getMsgBase();
}

Chat::~Chat()
{
	saveMsgBase();
	delete msgBase;
}

string Chat::packMsg(int msgId)
{
	string result;
	result.append(packMsg(msgBase->at(msgId)));
    return result;
}
string Chat::packMsg(Message msgb)
{
	string result;
	result.append(msgb.msgTo);
	result.append(delim);
	result.append(msgb.msgFrom);
	result.append(delim);
	result.append(msgb.msgDate);
	result.append(delim);
	result.append(msgb.msg);
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

int Chat::getMsgCount()
{
	return msgBase->size();
}

std::vector<int> Chat::getMsgToCount(string msgFrom = "ALL", string msgTo = "ALL")
{
	std::vector<int> msgFromTo;
	if (msgFrom == "ALL" && msgTo == "ALL")
	{
		for (int i = 0; i < getMsgCount(); i++)
		{
			msgFromTo.push_back(i);
		}
	}
	else
	{
		for (int i = 0; i < getMsgCount(); i++)
		{
			if (msgBase->at(i).msgTo == msgTo && msgBase->at(i).msgFrom == msgFrom || msgBase->at(i).msgTo == msgFrom && msgBase->at(i).msgFrom == msgTo)
			{
				msgFromTo.push_back(i);
			}
		}
	}
	return msgFromTo;
}

void Chat::emptyMsg()
{
	std::cout << "Пока здесь еще никто ничего не писал" << std::endl;
}

void Chat::printMsg(std::vector<int>& msgIndex)
{
	int j = 0;
	for (int i = 0; i < msgIndex.size(); i++)
	{
		j = msgIndex[i];
		cout << msgBase->at(j).msgFrom << "\t" << msgBase->at(j).msgDate << "\n" << msgBase->at(j).msg << "\n" << endl;
	}
}

void Chat::showMain()
{
	if (msgBase->empty())
	{
		emptyMsg();
		return;
	}
	std::vector<int> msgToAll = getMsgToCount();
	if (msgToAll.empty())
	{
		emptyMsg();
		return;
	}
	else
	{
		printMsg(msgToAll);
		return;
	}
}

void Chat::showPersonal(string user1, string user2)
{
	if (msgBase->empty())
	{
		emptyMsg();
		return;
	}
	std::vector<int> msgPersonal = getMsgToCount(user1, user2);
	if (msgPersonal.empty())
	{
		emptyMsg();
		return;
	}
	else
	{
		printMsg(msgPersonal);
		return;
	}
}

void Chat::sendMsg(string msgTo, string msgFrom, string& msg)
{
	net* online = new net;
	Message newMsg(msgTo, msgFrom, msg);
	string req = "SND_MSG";
	req.append(delim);
	req.append(packMsg(newMsg));
	online->regMsg(req);
	delete online;
	getMsgBase();
}

string Chat::getMBPath()
{
	return MBPath;
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

void Chat::getMsgBase()
{
	net* start = new net;
	char pkg[] = {"GET_MSGBASE"};
	start->sendReq(pkg, sizeof(pkg)-1);
	start->getMsgBase();
	std::ifstream msgbase_file(MBPath);
	if(msgbase_file.is_open())
	{
		string s;
		while (getline(msgbase_file, s))
		{
			Message newMsg = splitMsgPkg(s);
			msgBase->push_back(newMsg);
		}
		msgbase_file.close();
	}
	delete start;
}