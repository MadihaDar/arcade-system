#pragma once

#include <iostream>
#include <fstream>
#include "Admin.h"
#include "json.hpp" // Include the JSON library

using json = nlohmann::json;
using namespace std;

#define UNDERLINE "\033[4m"
#define BABY_BLUE "\033[38;5;153m"
#define PEACH "\033[38;5;223m"
#define PINK "\033[38;5;217m"
#define BLINK "\033[5m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define LIGHT_MAGENTA "\033[95m"
#define AQUA "\033[36m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define CLRSCR "\033[2J\033[1;1H"

class ArcadeSystem;

class TriviaTacToe;

class User {
    private:
        string name, password;
        int globalPoints;
        int coins;
    public:
        static int userCount; // Static variable to count users
    
        User(): name(""), password(""), globalPoints(0), coins(0) {}
        User(const string &name, const string &password, const int &globalPoints, const int& coins): name(name), password(password), globalPoints(globalPoints), coins(coins) {}
    
        //IMPORTANT: just to reiterate; all users will be loaded into arcadesystem_variable.users array when the program is started.
    
        static User* registerUser(); //registration menu
    
        static bool validateUsername(const string &);
        static bool isUniqueUsername(const string &);
        static bool validatePassword(const string &);
    
        void profileViewer() const;
        static json readJsonFileUsers(const string &filename);
        //void writeJsonFileUsers(const string &filename);
        void updateGlobalPoints(int points);
        void addCoins(int previousCoins);
    
        // getters and setters
        string getName() const{
            return name;
        }

        void setName(const string& name){
            this->name = name;
        }
    
        string getPassword() const{
            return password;
        }

        void setPassword(const string& password){
            this->password = password;
        }
    
        int getGlobalPoints() const{
            return globalPoints;
        }

        void setGlobalPoints(int points){
            this->globalPoints = points;
        }

        void setCoins(int coins){
            this->coins = coins;
        }

        int getCoins() const{
            return coins;
        }
    };

   