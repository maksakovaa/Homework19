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
	std::ifstream msgbase_file(MBPath);
	if(msgbase_file.is_open())
	{
		string s;
		string delim = "<|>";
		while (getline(msgbase_file, s))
		{
			size_t pos = 0;
			int i = 0;
			string array[3];
			while((pos = s.find(delim)) != string::npos)
			{
				array[i++] = s.substr(0,pos);
				s.erase(0, pos + delim.length());
			}
			Message newMsg(array[0], array[1], array[2], s);
			msgBase->push_back(newMsg);
		}
		msgbase_file.close();
	}
}

Chat::~Chat()
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
			msgbase_file << msgBase->at(i).msgTo;
			msgbase_file << "<|>";
			msgbase_file << msgBase->at(i).msgFrom;
			msgbase_file << "<|>";
			msgbase_file << msgBase->at(i).msgDate;
			msgbase_file << "<|>";
			msgbase_file << msgBase->at(i).msg;
			msgbase_file << "\n";
		}
		msgbase_file.close();
	}
	delete msgBase;
}

void Chat::getMsgBase()
{
	net start;
	start.sendmsg("GET_MSGBASE");
	start.getMsgBase();
	start.~net();
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
	Message newMsg(msgTo, msgFrom, msg);
	msgBase->push_back(newMsg);
}

string Chat::getMBPath()
{
	return MBPath;
}