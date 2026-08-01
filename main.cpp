using namespace std;
#include "MainMenu.h"
int main()
{
    arcadeSystem.loadUsers();
    MainMenu();
    arcadeSystem.saveUsers();
    return 0;
}