#pragma once

#include <iostream>
#include <fstream>
#include "json.hpp" // Include the JSON library

using json = nlohmann::json;
using namespace std;

// Forward declaration
class ArcadeSystem;

class Admin {
    private:
        string adminUsername;
        string adminPassword;
        static json readAdminJsonFile(const string &filename);
        static void writeAdminJsonFile(const string &filename, const json &data);
    
    public:
        // Getters
        string getAdminUsername() const { 
            return adminUsername; 
        }
        
        string getAdminPassword() const { 
            return adminPassword; 
        } 

        // Setters
        void setAdminUsername(const string& username) { 
            adminUsername = username; 
        }

        void setAdminPassword(const string& password) { 
            adminPassword = password; 
        } 
        
        Admin();
        bool login();
        void modifyUserScore();
        void modifyUserCoins();
        void wipeGame(const string& gameName);
        void deleteUser(const string& username);
        void logout();
        static bool isAdminExists();
        static void createAdmin(Admin& admin);
        void deleteAdminAccount();
        static void adminMenu(Admin& admin);
};