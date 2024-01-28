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
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        cout << "ERROR: Ошибка создания сокета." << endl;
        exit(1);
    }
    srvaddress.sin_addr.s_addr = inet_addr(SERVER);
    srvaddress.sin_port = htons(PORT);
    srvaddress.sin_family = AF_INET;
    connection = connect(socket_fd, (struct sockaddr*)&srvaddress, sizeof(srvaddress));
    if(connection == -1)
    {
        cout << "ERROR: Ошибка подключения к серверу." << endl;
        exit(1);
    }
    bzero(package, sizeof(package));
    ssize_t bytes = write(socket_fd, "GET_MSGBASE", sizeof("GET_MSGBASE"));
    bzero(package, sizeof(package));

	std::ofstream msgbase_file(MBPath, std::ios::trunc);

	while (strncmp("MSGBASE_END", package, 11) != 0)
	{
		read(socket_fd, package, sizeof(package));
		
		if (strncmp("MSGBASE_END", package, 11) == 0)
		{
			ssize_t bytes = write(socket_fd, "DISCONNECT", sizeof("DISCONNECT"));		
			break;
		}
		string temp = package;
		if(!msgbase_file.is_open())
		{
			cout << "ERROR: Ошибка открытия файла!" << endl;
		}
		else
		{
			msgbase_file << temp;
		}
	}
	msgbase_file.close();
	close(socket_fd);
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