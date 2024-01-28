#include "functions.h"

void logOut()
{
	auth = false;
	userId = -1;
}

void inputCleaner()
{
	cin.clear();
	cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void incorrect()
{
	inputCleaner();
	cout << "Некорректный ввод, повторите:" << endl;
}

bool nonLatinChk(string& text)
{
	string letters = "абвгдеёжщийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	if (text.find_first_of(letters) != string::npos)
	{
		cout << "ERROR: Некорректный ввод. Используйте латинницу." << endl;
		return false;
	}
	return true;
}

bool noSpacesChk(string& text)
{
	if (text.find_first_of(" ") != string::npos)
	{
		cout << "ERROR: Некорректный ввод. Не используйте пробел." << endl;
		return false;
	}
	return true;
}

bool lengthChk(string& text)
{
	if (text.size() < 4)
	{
		cout << "ERROR: Длина должна быть не менее 4 символов." << endl;
		return false;
	}
	return true;
}

bool regLoginChk(string& _login)
{
	int errCount = 0;
	if (!nonLatinChk(_login))
		errCount++;
	if (!noSpacesChk(_login))
		errCount++;
	if (!lengthChk(_login))
		errCount++;

	if (Users->loginCheck(_login))
	{
		cout << "ERROR: Имя пользователя уже занято." << endl;
		errCount++;
	}

	if (errCount == 0)
	{
		return true;
	}
	else
	{
		cout << "Повторите ввод:" << endl;
		return false;
	}
}

bool regPwdChk(string& _pwd)
{
	int errCount = 0;
	if (!nonLatinChk(_pwd))
		errCount++;
	if (!noSpacesChk(_pwd))
		errCount++;
	if (!lengthChk(_pwd))
		errCount++;

	if (errCount == 0)
	{
		return true;
	}
	else
	{
		cout << "Повторите ввод:" << endl;
		return false;
	}
}

bool authPwdChk(string& _login, string& _pwd)
{
	int id = Users->getUserId(_login);
	return Users->pwdCheck(id, _pwd);
}

bool authStatChk()
{
	if (auth && userId >= 0 && userId < Users->getUserCount())
	{
		return true;
	}
	return false;
}

bool authAdminStatChk()
{
	if (auth && userId == 0)
		return true;

	return false;
}

bool exit()
{
	return 0;
}

string getOsName()
{
#if defined(_WIN32) || defined (_WIN64)
	return "Windows";
#elif defined(__linux__)
	return "Linux";
#endif
}

int getProcId()
{
#if defined(__linux__)
	return getpid();
#elif defined(_WIN32) || defined(_WIN64)
	return GetCurrentProcessId();
#endif
}
