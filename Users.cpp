#include "Users.h"
#include <fstream>

#include "Arcade.h"

using namespace std;

int User::userCount = 0;

void User::updateGlobalPoints(int points)
{
    globalPoints += points;
    arcadeSystem.saveUsers();
}

void User::addCoins(int previousCoins){
    coins += previousCoins;
    arcadeSystem.saveUsers();
}

// Function to validate username
bool User::validateUsername(const string &username) {
    return username.length() > 0 && username.length() <= 8;
}

// Function to check if username is unique
bool User::isUniqueUsername(const string &username) {
    return arcadeSystem.getUser(username) == nullptr; // True if user was not found
}

bool User::validatePassword(const string &password) {
    if (password.length() < 6) return false; // Must be at least 6 characters long

    bool hasLetter = false, hasDigit = false;
    for (char ch : password) {
        if (isalpha(ch)) hasLetter = true;
        if (isdigit(ch)) hasDigit = true;
    }
    return hasLetter && hasDigit; // Must contain at least one letter and one digit
}

json User::readJsonFileUsers(const string &filename) { // Used in arcadeSystem.saveUsers()
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "\n\t\t\t\t\t\t\t\tCould not open " << filename << endl;
        return json();
    }

    json data;
    file >> data;
    file.close();
    return data;
}

// void User::writeJsonFileUsers(const string &filename) { // Shifted to using arcadeSystem.saveUsers()
//     json data;
//
//     // Load existing data
//     ifstream inFile(filename);
//     if (inFile) {
//         inFile >> data;
//         inFile.close();
//     }
//
//     data[name] = {
//         {"password", password},
//         {"globalPoints", globalPoints},
//         {"coins", coins}
//     };
//
//     ofstream outFile(filename);
//     if (outFile) {
//         outFile << data.dump(4);
//         outFile.close();
//     }
// }

void User::profileViewer() const
{

    cout << "\n\t\t\t\t\t\t\t\t   ========= USER PROFILE =========" << endl << endl;
    cout << "\t\t\t\t\t\t\t\t   Username      : " << name << endl << endl;
    cout << "\t\t\t\t\t\t\t\t   Global Points : " << globalPoints << endl << endl;
    cout << "\t\t\t\t\t\t\t\t   Coins         : " << coins << endl << endl;
    cout << "\t\t\t\t\t\t\t\t   ================================\n" << endl;
}

User* User::registerUser() {
    // load all data
    //json data = readJsonFileUsers("users.json"); arcadeSystem.users

    string choice;
    while (true) {
        cout << "\n\t\t\t\t\tDo you want to (1) Log in or (2) Sign up? Enter 1 or 2: ";
        cin >> choice;
        cout << endl;

        if (choice == "1" || choice == "2") {
            break;
        } else {
            cout << "\n\t\t\t\t\tInvalid input. Please enter 1 to Log in or 2 to Sign up.\n";
        }
    }

    string username, password;
    if (choice == "1") {    // Log in
        cout << "Enter your username: ";
        cin >> username;

        // Check if entered username matches the data's username
        if (arcadeSystem.getUser(username) != nullptr) {
            cout << "Enter your password: ";
            cin >> password;

            // Check if the password matches
            if (arcadeSystem.getUser(username)->getPassword() == password) {
                cout << "\nLogin successful! Welcome back, " << username << "!\n";
                return arcadeSystem.getUser(username);
            } else {
                cout << "\nIncorrect password! Try again.\n";
                return registerUser();
            }
        } else {
            cout << "\nUsername not found! Try signing up.\n";
            return registerUser();
        }
    }

    // If user chooses sign-up
    cout << "Enter a username (1-8 characters): ";
    cin >> username;
    while (!validateUsername(username) || !isUniqueUsername(username)) {
        cout << "\nInvalid or taken username. Try again: ";
        cin >> username;
    }

    cout << "\nEnter a password: ";
    cin >> password;
    while (!validatePassword(password)) {
        cout << "\nInvalid or short password (should contain at least one letter and digit). Try again: ";
        cin >> password;
    }

    User u = User(username, password, 0, 0);
    // Store user data directly under the username key
    arcadeSystem.addUser(u);

    // Save user if they signed-up
    arcadeSystem.saveUsers();

    cout << "\n\t\t\t\t\t     Sign-up successful! Welcome, " << username << "!\n";
    return arcadeSystem.getUser(username);
}
