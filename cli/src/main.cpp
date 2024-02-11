#include "navigation.h"

static void setConsLocale() {
	setlocale(LC_ALL, "Russiаn");
#if defined (_WIN32) || defined (_WIN64)
	system("chcp 65001");
#endif
}

UserBase* Users = new UserBase();
Chat* mainChat = new Chat();
int choice{ 0 }, userId{ -1 };
bool auth{ false };

int main()
{
	setConsLocale();
	menu();
	delete mainChat;
	delete Users;
	return 0;
}
