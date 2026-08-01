#include "Admin.h"
#include "Arcade.h"

using namespace std;
using json = nlohmann::json;

// Constructor
Admin::Admin() {
    json data = readAdminJsonFile("admin.json");
    if (!data.empty()) {
        setAdminUsername(data["adminUsername"]);  
        setAdminPassword(data["adminPassword"]);
    }
}

// Function to read JSON file
json Admin::readAdminJsonFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return json();
    }
    json data;
    file >> data;
    file.close();
    return data;
}

// Function to write JSON file
void Admin::writeAdminJsonFile(const string &filename, const json &data) {
    ofstream outFile(filename);
    if (outFile) {
        outFile << data.dump(4);
        outFile.close();
    }
}

// return true if admin already exisits
bool Admin::isAdminExists() {
    json data = readAdminJsonFile("admin.json");
    return data.contains("adminUsername");
}

// create an admin if it doesn't exist
void Admin::createAdmin(Admin& admin) {
    if (isAdminExists()) {
        cout << "\n\t\t\t\t\t\tAn admin already exists!" << endl;
        return;
    }
    
    string username, password;
    cout << "\t\t\t\t\t\tEnter Admin Username: ";
    cin >> username;
    admin.setAdminUsername(username);
    cout << "\t\t\t\t\t\tEnter Admin Password: ";
    cin >> password;
    admin.setAdminPassword(password);
    
    json adminData = {
        {"adminUsername", admin.getAdminUsername()},
        {"adminPassword", admin.getAdminPassword()}
    };

    // write the admin data to json file
    writeAdminJsonFile("admin.json", adminData);
    cout << "\t\t\t\t\t\tAdmin account created successfully!" << endl;
}

// Admin Login Function
bool Admin::login() {
    json data = readAdminJsonFile("admin.json");

    if (data.empty()) {
        string choice;
        while (true) {  
            cout << "\t\t\t\t\t\tNo admin found. Would you like to create one? (yes/no): ";
            cin >> choice;
            cout << endl;

            if (choice == "yes") {
                createAdmin(*this);
                return true;
            } 
            else if (choice == "no") {
                cout << "\t\t\t\t\t\tReturning to the main menu.\n";
                return false;
            } 
            else {
                cout << "\t\t\t\t\tInvalid input. Please type 'yes' or 'no'.\n";
            }
        }
    }

    // Allow multiple login attempts
    int attempts = 3;
    while (attempts > 0) {
        cout << "\t\t\t\t\t\tEnter admin username: ";
        cin >> adminUsername;
        cout << "\t\t\t\t\t\tEnter admin password: ";
        cin >> adminPassword;

        if (data["adminUsername"] == adminUsername && data["adminPassword"] == adminPassword) {
            setAdminUsername(adminUsername);  
            setAdminPassword(adminPassword); 
            cout << "\n\t\t\t\t\t\t     Admin login successful!\n";
            return true;
        } else {
            attempts--;
            cout << "\t\t\t\t\tIncorrect admin credentials. Attempts left: " << attempts << endl;
        }
    }

    cout << "\t\t\t\t\t\tToo many failed attempts. Exiting.\n";
    User user;
    user.registerUser();
    return false;  
}

// Function to Modify User Score
void Admin::modifyUserScore() {
    json usersData = readAdminJsonFile("users.json");
    string username;
    int newScore;

    cout << "\t\t\t\t\tEnter the username whose score you want to modify: ";
    cin >> username;

    if (usersData.contains(username)) {
        cout << "\n\t\t\t\t\t\t     Enter the new score: ";
        cin >> newScore;
        usersData[username]["globalPoints"] = newScore;
        writeAdminJsonFile("users.json", usersData);
        cout << "\n\t\t\t\t\t\tUser score updated successfully!\n";
    } else {
        cout << "\n\t\t\t\t\t\tUser not found!\n";
    }
}

// Function to Modify User Coins
void Admin::modifyUserCoins() {
    json data = readAdminJsonFile("users.json");
    string username;
    int newCoins;

    cout << "\t\t\t\t\tEnter the username whose coins you want to modify: ";
    cin >> username;

    if (data.contains(username)) {
        cout << "\n\t\t\t\t\t\t    Enter the number of coins: ";
        cin >> newCoins;
        data[username]["coins"] = newCoins;
        writeAdminJsonFile("users.json", data);
        cout << "\n\t\t\t\t\t\tUser coins updated successfully!\n";
    } else {
        cout << "\n\t\t\t\t\t\tUser not found!\n";
    }
}

void Admin::wipeGame(const string& gameName) {
    Game* game = arcadeSystem.selectGame(gameName); 

    if (game) { 
        game->wipeGame(); 
        cout << "\t\t\t\t\t\tGame data wiped successfully for " << gameName << endl;
    } else {
        cout << "\t\t\t\t\t\tGame not found. Try again.\n";
    }
}


// Function to Delete a User
void Admin::deleteUser(const string& username) {
    User* user = arcadeSystem.getUser(username);

    if (user != nullptr && user->getName() == username) {
        for (Game* game : arcadeSystem.getGames()) {
            game->deletePlayer(*user);
        }
        arcadeSystem.removeUser(username);
        arcadeSystem.saveUsers();
        cout << "\n\t\t\t\t\t\tUser deleted successfully!\n";
    } else {
        cout << "\n\t\t\t\t\t\tUser not found!\n";
    }
}

// Function to Logout Admin
void Admin::logout() {
    setAdminUsername(""); 
    setAdminPassword("");
}

void Admin::deleteAdminAccount() {
    json data = readAdminJsonFile("admin.json");

    // Check if admin's section exists in the JSON
    if (data.contains("adminPassword") && data.contains("adminUsername")) {
        data.erase("adminPassword"); 
        data.erase("adminUsername");
        writeAdminJsonFile("admin.json", data);
        cout << "\n\t\t\t\t\t\tAdmin account deleted successfully!\n";
    } else {
        cout << "\n\t\t\t\t\t\tNo admin account found to delete!\n";
    }
}

void Admin::adminMenu(Admin& admin) {
    int choice;
    do {
        cout << "\n\t\t\t\t\t\t     ===== ADMIN MENU =====\n";
        cout << "\n\t\t\t\t\t\t     1. Modify User Score\n";
        cout << "\t\t\t\t\t\t     2. Modify User Coins\n";
        cout << "\t\t\t\t\t\t     3. Wipe Game Data\n";
        cout << "\t\t\t\t\t\t     4. Delete User\n";
        cout << "\t\t\t\t\t\t     5. Delete Admin Account\n";
        cout << "\t\t\t\t\t\t     6. Logout\n";
        cout << "\n\t\t\t\t\t\t     Enter your choice: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
            case 1:
                admin.modifyUserScore();
                break;
            case 2:
                admin.modifyUserCoins();
                break;
            case 3: {
                string gameName;
                cout << "\t\t\t\t\tEnter the game name you want to wipe: ";
                cin >> gameName;
                admin.wipeGame(gameName);
                cout << "\t\t\t\t\t\tGame Wiped" << endl;
                break;
            }
            case 4: {
                string username;
                cout << "\t\t\t\t\t\t Enter the username to delete: ";
                cin >> username;
                admin.deleteUser(username);
                break;
            }
            case 5: {
                char choice;
                cout << "\t\t\t\t\tAre you sure you want to delete your admin account? (y/n): ";
                cin >> choice;

                if (choice != 'y' && choice != 'Y') {
                    cout << "\t\t\t\t\t\tAdmin account deletion canceled.\n";
                    break;
                } else if (choice == 'y' || choice == 'Y') {
                    admin.deleteAdminAccount();
                    return;
                } else {
                    cout << "\t\t\t\t\t\tInvalid choice. Select y or n.\n";
                }
            }
            case 6:
                admin.logout();
                cout << "\t\t\t\t\t\tLogging out...\n";
                return;
            default:
                cout << "\t\t\t\t\t\tInvalid choice! Please try again.\n";
        }
    } while (choice != 6);
}