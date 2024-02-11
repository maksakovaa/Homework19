#include "navigation.h"

void menu()
{
	if (authStatChk())
	{
		menuAuth();
		return;
	}
	consoleClear();
	cout << "Добро пожаловать в консольный чат!\nДля просмотра и отправки сообщений необходимо войти или зарегистрироваться. " << endl;
	cout << "Выберите действие:\n 1 - Регистрация пользователя \n 2 - Авторизация \n 3 - Список пользователей \n 4 - О программе \n 5 - Выход из программы" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice !=5)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
	case 1:
	{
		menuRegUser();
		break;
	}
	case 2:
	{
		menuLogin();
		break;
	}
	case 3:
	{
		menuUserList();
		break;
	}
	case 4:
	{
		menuAbout();
		break;
	}
	case 5:
	{
		exit();
		break;
	}
	default:
		break;
	}
}

void menuRegUser()
{
	string name, login, pwd;
	consoleClear();
	cout << "Введите имя пользователя:" << endl;
	cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	getline(cin, name);
	cout << "Введите логин пользователя:" << endl;
	getline(cin, login);
	while (!regLoginChk(login))
	{
		getline(cin, login);
	}
	cout << "Введите желаемый пароль:" << endl;
	getline(cin, pwd);
	while (!regPwdChk(pwd))
	{
		getline(cin, pwd);
	}
	Users->regUsers(name, login, pwd);
	consoleClear();
	cout << "Пользователь " << name << " успешно зарегистрирован. \n 1 - Главное меню 2 - Авторизация" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
	case 1:
	{
		if (auth && userId == 0)
		{
			menuAuth();
			break;
		}
		else
		{
			menu();
			break;
		}
		break;
	}
	case 2:
	{
		menuLogin();
		break;
	}
	default:
		break;
	}
}

void menuLogin()
{
	string name, login, pwd;
	consoleClear();
	cout << "Вход для зарегистрированных пользователей" << endl;
	cout << "Введите логин:" << endl;
	inputCleaner();
	getline(cin, login);
	if (Users->loginCheck(login))
	{
		cout << "Введите пароль:" << endl;
		getline(cin, pwd);
		while (!authPwdChk(login, pwd))
		{
			cout << "Неверный пароль, повторите:" << endl;
			getline(cin, pwd);
		}
		if (authPwdChk(login, pwd))
		{
			auth = true;
			userId = Users->getUserId(login);
			menuAuth();
		}
	}
	else
	{
		cout << "Неверное имя пользователя" << endl;
		cout << "1 - вернуться в главное меню, 2 - выход из программы, 3 - повторить" << endl;
		cin >> choice;
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			cin >> choice;
		}
		switch (choice)
		{
		case 1:
		{
			menu();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		case 3:
		{
			menuLogin();
			break;
		}
		default:
			break;
		}
	}
}

void menuUserList()
{
	consoleClear();
	Users->showUsers();
	cout << "1 - вернуться в главное меню, 2 - выход из программы";
	if (authStatChk())
		cout << " 3 - Просмотр сообщений";
	cout << endl;

	cin >> choice;
	if (authStatChk())
	{
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			cin >> choice;
		}
	}
	else
	{
		while (choice != 1 && choice != 2)
		{
			incorrect();
			cin >> choice;
		}
	}
	switch (choice)
	{
	case 1:
	{
		if (authStatChk())
		{
			menuAuth();
		}
		else
		{
			menu();
		}
		break;
	}
	case 2:
	{
		exit();
		break;
	}
	case 3:
	{
		menuPrivateSendMsg();
		break;
	}
	default:
		break;
	}
}

void menuUserMgmt()
{
	if (!authAdminStatChk())
	{
		menuAuth();
		return;
	}
	consoleClear();
	cout << "Управление пользователями. \n";
	Users->showUsers();
	cout << "\n Выберите действие: \n 1 - Добавить пользователя \n 2 - Удалить пользователя \n 3 - Сменить пароль пользователя \n 4 - Главное меню" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
	case 1:
	{
		menuRegUser();
		break;
	}
	case 2:
	{
		menuDelUser();
		break;
	}
	case 3:
	{
		menuUsrMgmtSetPwd();
		break;
	}
	case 4:
	{
		menuAuth();
		break;
	}
	default:
		break;
	}
}

void chkUsersAv()
{
	if (Users->getUserCount() <= 1)
	{
		cout << "Нет доступных для измениения пользователей. Продолжить? \n 1 - Да 2 - нет" << endl;
		cin >> choice;
		while (choice != 1 && choice != 2)
		{
			incorrect();
			cin >> choice;
		}
		switch (choice)
		{
		case 1:
		{
			menuUserMgmt();
			break;
		}
		case 2:
		{
			menuAuth();
			break;
		}
		default:
			break;
		}
	}
}

void menuDelUser()
{
	if (!authAdminStatChk())
	{
		menuAuth();
		return;
	}
	if (Users->getUserCount() > 1)
	{
		cout << "Учётную запись \"Администратор\" удалить нельзя. Для удаления пользователя введите его ID : " << endl;
		cin >> choice;
		while (choice <= 0 || choice >= Users->getUserCount())
		{
			incorrect();
			cin >> choice;
		}
		cout << "Вы уверены что хотите удалить " << Users->getUserName(choice) << "? \n 1 - да, 2 - нет" << endl;
		int temp;
		cin >> temp;
		while (temp != 1 && temp != 2)
		{
			incorrect();
			cin >> temp;
		}
		switch (temp)
		{
		case 1:
		{
			string curUser = Users->getUserName(userId);
			Users->delUser(choice);
			if (choice < userId)
			{
				userId = Users->getUserId(curUser);
			}
			menuUserMgmt();
			break;
		}
		case 2:
		{
			menuUserMgmt();
			break;
		}
		default:
			break;
		}
	}
	else
	{
		chkUsersAv();
	}
}

void menuUsrMgmtSetPwd()
{
	if (Users->getUserCount() > 1)
	{
		cout << "Изменить пароль учётной записи \"Администратор\" нельзя. Для изменения пароля пользователя введите его ID : " << endl;
		cin >> choice;
		while (choice <= 0 || choice >= Users->getUserCount())
		{
			incorrect();
			cin >> choice;
		}
		menuChgPwd(choice);
	}
	else
	{
		chkUsersAv();
	}
}

void menuChgPwd(int choice = userId)
{
	string pwd;
	cout << "Введите новый пароль:" << endl;
	inputCleaner();
	getline(cin, pwd);
	while (!regPwdChk(pwd))
	{
		getline(cin, pwd);
	}
	Users->chgPwd(choice, pwd);
	cout << "Пароль пользователя " << Users->getUserName(choice) << " успешно изменен. Введите 1 чтобы продолжить, 2 чтобы выйти из программы" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
		case 1:
		{
			if(userId == 0)
				menuUserMgmt();
			if (userId > 0)
				menuAuth();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		default:
			break;
	}
}

void menuAuth()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	consoleClear();
	cout << Users->getUserName(userId) << " добро пожаловать в консольный чат!" << endl;
	cout << "Выберите действие:\n 1 - Общий чат\n 2 - Личные сообщения" << endl;
	if (userId == 0)
	{
		cout << " 3 - Управление пользователями" << endl;
	}
	else if (userId > 0)
	{
		cout << " 3 - Изменить пароль" << endl;
	}
	cout << " 4 - Выйти из учетной записи\n 5 - О программе\n 6 - Выход из программы" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
	case 1:
	{
		menuMainChat();
		break;
	}
	case 2:
	{
		menuUserList();
		break;
	}
	case 3:
	{
		if (userId == 0)
		{
			menuUserMgmt();
			break;
		}
		else if (userId > 0)
		{
			menuChgPwd();
			break;
		}
	}
	case 4:
	{
		logOut();
		menu();
		break;
	}
	case 5:
	{
		menuAbout();
		break;
	}
	case 6:
	{
		exit();
		break;
	}
	default:
		break;
	}
}

void menuPrivateSendMsg()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	int temp;
	cout << "Введите ID пользователя:" << endl;
	cin >> temp;
	while (cin.fail() || temp < 0 || temp >= Users->getUserCount())
	{
		incorrect();
		cin >> temp;
	}
	while (choice == 3)
	{
		consoleClear();
		mainChat->showPersonal(Users->getUserName(userId), Users->getUserName(temp));
		cout << "1 - Вернуться в главное меню, 2 - Выход из программы 3 - Отправка сообщения 4 - Вернуться к списку пользователей" << endl;
		cin >> choice;
		while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
		{
			incorrect();
			cin >> choice;
		}
		switch (choice)
		{
		case 1:
		{
			menuAuth();
			break;
		}
		case 2:
		{
			exit();
			break;
		}
		case 3:
		{
			cout << "Введите сообщение:" << endl;
			string msgText;
			inputCleaner();
			getline(cin, msgText);
			mainChat->sendMsg(Users->getUserName(temp), Users->getUserName(userId), msgText);
			break;
		}
		case 4:
		{
			menuUserList();
			break;
		}
		default:
			break;
		}
	}
}

void menuMainChat()
{
	if (!authStatChk())
	{
		menu();
		return;
	}
	while (choice == 1)
	{
		consoleClear();
		mainChat->showMain();
		cout << " 1 - Отправить сообщение\n 2 - Вернуться в главное меню\n 3 - Выход из программы" << endl;
		cin >> choice;
		while (choice != 1 && choice != 2 && choice != 3)
		{
			incorrect();
			cin >> choice;
		}
		switch (choice)
		{
		case 1:
		{
			cout << "Введите сообщение:" << endl;
			string msgText;
			inputCleaner();
			getline(cin, msgText);
			mainChat->sendMsg("ALL", Users->getUserName(userId), msgText);
			break;
		}
		case 2:
		{
			menuAuth();
			break;
		}
		case 3:
		{
			exit();
			break;
		}
		default:
			break;
		}
	}
}

void menuAbout()
{
	consoleClear();
	cout << "Программа консольный чат. [0.59]\n© Максаков А.А., 2024\nСборка для " << getOsName() << endl;
	cout << "PID процесса: " << getProcId() << endl;
	cout << "1 - Вернуться в главное меню\n2 - Выход из программы" << endl;
	cin >> choice;
	while (choice != 1 && choice != 2)
	{
		incorrect();
		cin >> choice;
	}
	switch (choice)
	{
	case 1:
	{
		menu();
		break;
	}
	case 2:
	{
		exit();
		break;
	}
	default:
		break;
	}
}
