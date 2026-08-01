// Shop.h
//
// Created by huzaifa on 11/03/2025.
//
#pragma once


#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "Users.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class ArcadeShopItem {
public:
    string rewardName;
    int rewardID;
    int rewardPrice;

    ArcadeShopItem(): rewardName(""), rewardID(0), rewardPrice(0) {}

    ArcadeShopItem(string name, int id, int price)
        : rewardName(std::move(name)), rewardID(id), rewardPrice(price) {
    }

    json toJson() const;

    static ArcadeShopItem fromJson(const json &j);
};

class ArcadeShop {
    map<int, ArcadeShopItem> rewardMap;
    string filename = "arcade_shop.json";

    void initializeDefaultRewards();

    //  Private constructor for Singleton
    ArcadeShop() {
        loadFromJson();
        if (rewardMap.empty()) {
            initializeDefaultRewards();
        }
    }

public:
    //  Singleton access point
    static ArcadeShop &getInstance() {
        static ArcadeShop instance;
        return instance;
    }

    //  Disable copy and assignment
    ArcadeShop(const ArcadeShop &) = delete;

    ArcadeShop &operator=(const ArcadeShop &) = delete;

    void addReward(const string &name, int id, int price);

    void deleteReward(int rewardID);

    void shopMenu(User &user);

    void modifyReward(int rewardID, const string &newName, int newPrice);

    void displayRewards() const;

    void purchaseReward(User &user, int rewardID);

    void saveToJson() const;

    void loadFromJson();

    void rewardManagementMenu();
};

// Template class for a basic item wrapper
template<typename T>
class ItemWrapper {
public:
    T item;

    explicit ItemWrapper(const T &i) : item(i) {
    }

    void show() const {
        std::cout << "Wrapped item: " << item << std::endl;
    }
};