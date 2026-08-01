#include "MainMenu.h"

#include "Shop.h"



void MainMenu()
{

    //register games first
    arcadeSystem.registerGame<TriviaTacToe>();
    arcadeSystem.registerGame<TypingGame>();
    arcadeSystem.registerGame<PlayHopscotchGame>();
    arcadeSystem.registerGame<RPGGame>();

    while (true) {
        cout << "\n\t\t\t\t\t\t=================================\n";
        cout << "\t\t\t\t\t\t  Welcome to the Arcade System!  \n";
        cout << "\t\t\t\t\t\t=================================\n";

        int choice;
        cout << "\n\t\t\t\t\t\tAre you:\n";
        cout << "\t\t\t\t\t\t1. User\n";
        cout << "\t\t\t\t\t\t2. Admin\n";
        cout << "\t\t\t\t\t\t3. Exit\n";
        cout << "\n\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;
        cout << endl;

        Admin admin;
        if (choice == 2) {
            if (Admin::isAdminExists()) {
                if (admin.login()) {
                    admin.adminMenu(admin);
                } else {
                    cout << "Admin login failed.\n";
                }
            } else {
                cout << "No admin found. Creating new admin...\n";
                Admin::createAdmin(admin);
                admin.adminMenu(admin);
            }
        } else if (choice == 1) {
            // User mode
            User* currentUser = User::registerUser();
            int gameChoice;
            do {
                cout << "\n\t\t\t\t\t\t------------- Game Menu -------------\n";
                cout << "\n\t\t\t\t\t\t1. Play TriviaTacToe\n";
                cout << "\t\t\t\t\t\t2. Play Typing Game\n";
                cout << "\t\t\t\t\t\t3. Play Hopscotch Game\n";
                cout << "\t\t\t\t\t\t4. Play RPG Game\n";
                cout << "\t\t\t\t\t\t5. View Global Leaderboard\n";
                cout << "\t\t\t\t\t\t6. View your Global Points and Score\n";
                cout << "\t\t\t\t\t\t7. Browse Arcade Shop\n";
                cout << "\t\t\t\t\t\t8. Logout\n";
                cout << "\n\t\t\t\t\t\tEnter your choice: ";
                cin >> gameChoice;

                switch (gameChoice) {
                    case 1: {
                            arcadeSystem.selectGame("TriviaTacToe")->gameMenu(*currentUser);
                            break;
                    }
                    case 2: {
                            arcadeSystem.selectGame("TypingGame")->gameMenu(*currentUser);
                            break;
                    }
                    case 3: {
                            arcadeSystem.selectGame("HopscotchGame")->gameMenu(*currentUser);
                            break;
                    }
                    case 4: {
                            arcadeSystem.selectGame("RPGGame")->gameMenu(*currentUser);
                            break;
                    }
                    case 5: {
                        GlobalLeaderBoard globalLB;
                        vector<User> users = globalLB.loadUsersFromJsonForGlobalLB("users.json");
                        globalLB.bubbleSortForGlobalLeaderBoard(users.data(), users.size());
                        globalLB.displayGlobalLeaderboard(users.data(), users.size(), 8);
                        break;
                    }
                    case 6:
                        currentUser->profileViewer();
                        break;
                    case 7:
                        ArcadeShop::getInstance().shopMenu(*currentUser);
                        break;
                    case 8:
                        cout << "\n\t\t\t\t\t\tLogging out. Thanks for playing!\n";
                        break;
                    default:
                        cout << "\n\t\t\t\t\t\tInvalid choice. Try again.\n";
                }

            } while (gameChoice != 8);
        } else if (choice == 3) {
            cout << "\n\t\t\t\t\t\t\t    Goodbye!\n";
            break;
        } else {
            cout << "\n\t\t\t\t\t\tInvalid choice. Select 1, 2, or 3.\n\n";
        }
    }
}
