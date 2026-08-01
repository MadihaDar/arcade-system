#ifndef RPGGAME_H
#define RPGGAME_H

#include <iostream>
#include <vector>
#include <string>
#include "../Users.h"
#include "../Arcade.h"

using namespace std;

//------------------- RPGStats Class -------------------//
class RPGStats {
public:
    double maxHp, hp, atk, def, spd, ats, adf;

    RPGStats(double h, double a, double d, double s, double at, double ad);

    // Add stats together (e.g., leveling up, buffs)
    RPGStats operator+(const RPGStats &other) const {
        return {
            hp + other.hp,
            atk + other.atk,
            def + other.def,
            ats + other.ats,
            adf + other.adf,
            maxHp + other.maxHp
        };
    }

    // Subtract stats (e.g., debuffs)
    RPGStats operator-(const RPGStats &other) const {
        return {
            hp - other.hp,
            atk - other.atk,
            def - other.def,
            ats - other.ats,
            adf - other.adf,
            maxHp - other.maxHp
        };
    }

    // Scale stats (e.g., wave multiplier)
    RPGStats operator*(double multiplier) const {
        return {
            hp * multiplier,
            atk * multiplier,
            def * multiplier,
            ats * multiplier,
            adf * multiplier,
            maxHp * multiplier
        };
    }
};

ostream &operator<<(ostream &out, const RPGStats &s);

//------------------- RPGItem and RPGInventory -------------------//
class RPGItem {
public:
    string itemName;
    int itemID, price;
    RPGStats statBoost;

    RPGItem(string name, int id, int cost, const RPGStats &boost);
};

class RPGInventory {
public:
    vector<RPGItem> items;

    void addItem(const RPGItem &item);

    void displayItems() const; // Display items in the backpack
};

//------------------- RPGPlayerRace -------------------//
class RPGPlayerRace {
public:
    string raceName;
    RPGStats racialStats;

    RPGPlayerRace(string name, const RPGStats &s);
};

//------------------- RPGPlayerClass -------------------//
class RPGPlayerClass {
public:
    string className;
    RPGStats baseStats;

    RPGPlayerClass(string name, const RPGStats &s);
};

//------------------- RPGCreatures & RPGMobs -------------------//
class RPGCreatures {
public:
    RPGStats stats;

    explicit RPGCreatures(const RPGStats &s);
};

class RPGMobs : public RPGCreatures {
public:
    string name, type;

    RPGMobs(string n, string t, const RPGStats &s);
};

//------------------- RPGPlayer -------------------//
class RPGPlayer : public RPGCreatures {
public:
    User* user;
    string name;
    int wavesSurvived;
    RPGPlayerClass pClass;
    RPGPlayerRace pRace;
    RPGInventory *backpack;

    RPGPlayer(User* user, string n, int ws, const RPGPlayerClass &pc, const RPGPlayerRace &pr, RPGInventory *inv,
              const string &uname, const string &pwd, int gPoints, int coins);


    void displayInventory() const; // Method to display items in inventory
};

//------------------- RPGShop -------------------//
class RPGShop {
public:
    vector<RPGItem> inventory;


    static void displayItems();

    static void sellItem(const RPGPlayer &player, int itemID);
};

//------------------- RPGGame -------------------//
class RPGGame : public Game {
public:
    int waveNumber;
    vector<RPGMobs> waveMobs;

    RPGGame() : Game("RPGGame", true), waveNumber(0) {
    }

    void play(User &user);

    static void createCharacter(RPGPlayer &player);

    static RPGPlayerRace getRace(int choice);

    static RPGPlayerClass getClass(int choice);

    void nextWave(RPGPlayer &player);

    static void turnBasedCombat(RPGPlayer &player, RPGMobs &mob);

    static void saveData(const RPGPlayer &player);

    static void loadData();

    static void RPGLeaderboard();

     static void saveLeaderboardToFile();

    void gameMenu(User &user) override;

    void deletePlayer(User &user) override;

    void wipeGame() override;
};

#endif // RPGGAME_H