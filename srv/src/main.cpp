#include "netd.h"

UserBase* Users = new UserBase();
Chat* mainChat = new Chat();

int main()
{
#if defined (_WIN32) || defined (_WIN64)
    setlocale(LC_ALL, "Russiаn");
    system("chcp 65001");
#endif
    net* online = new net();
    online->netGateway();

    delete online, mainChat, Users;
    return 0;
}