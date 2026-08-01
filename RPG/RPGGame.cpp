#include "RPGGame.h"
#include <fstream>
#include <random>
#include <ctime>
#include "../Users.h"
#include <iostream>
#include <limits>
#include <utility>
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"

using namespace std;

vector<RPGPlayer> allPlayers;

// Not Needed but still here.
//RPGItem() : itemName(""), itemID(0), price(0), statBoost() {}


// Missing an argument for statBoost; now added.
vector<RPGItem> globalItemList = {
    RPGItem("Health Potion", 0, 5, RPGStats(10, 0, 0, 0, 0, 0)),
    RPGItem("Strength Potion", 1, 5, RPGStats(0, 5, 0, 0, 0, 0)),
    RPGItem("Magic Potion", 2, 5, RPGStats(0, 0, 0, 0, 5, 5)),
    RPGItem("Defense Potion", 3, 5, RPGStats(0, 0, 5, 0, 0, 0)),
    RPGItem("Speed Potion", 4, 5, RPGStats(0, 0, 0, 5, 0, 0))
};

// RPGStats constructor
RPGStats::RPGStats(double h, double a, double d, double s, double at, double ad)
    : maxHp(h), hp(h), atk(a), def(d), spd(s), ats(at), adf(ad) {
}

// RPGItem constructor
RPGItem::RPGItem(string name, int id, int cost, const RPGStats &boost)
    : itemName(std::move(name)), itemID(id), price(cost), statBoost(boost) {
}

void RPGGame::play(User &user) {
    // Load player data from file
    loadData(); // Load players (including inventory) from saved data

    // Find the player in the loaded list
    RPGPlayer *player = nullptr;
    for (auto &p: allPlayers) {
        if (p.name == user.getName()) {
            player = &p;
            break;
        }
    }

    if (!player) {
        // If player doesn't exist in the file, create a new one
        cout << "Player data not found. Creating new character.\n";
        auto *inventory = new RPGInventory();
        player = new RPGPlayer(
            &user,
            user.getName(), // name
            0, // wavesSurvived
            getClass(2), // default class
            getRace(1), // default race
            inventory, // inventory
            user.getName(), // username
            user.getPassword(), // password
            0, // globalPoints
            0 // coins
        );

        allPlayers.push_back(*player); // Add to list
    }

    cout << "Welcome to the RPG Game, " << player->name << "!\n";
    createCharacter(*player);

    // Start the game loop
    while (player->stats.hp > 0 && waveNumber <= 10) {
        nextWave(*player);
    }

    cout << "Game Over! You survived until wave " << waveNumber << "!\n";

    // Fix: Update the player's wavesSurvived with the current waveNumber
    player->wavesSurvived = waveNumber;

    // Reset the HP to maxHp for next game
    player->stats.hp = player->stats.maxHp;

    // Save the existing player object with updated wavesSurvived
    saveData(*player);
    saveLeaderboardToFile();
}


// RPGInventory methods
void RPGInventory::addItem(const RPGItem &item) {
    items.push_back(item);
}

void RPGInventory::displayItems() const {
    if (items.empty()) {
        cout << "Inventory is empty.\n";
        return;
    }

    for (int i = 0; i < items.size(); ++i) {
        cout << "[" << i << "] " << items[i].itemName << " - Price: " << items[i].price << "\n";
    }
}

// RPGPlayerRace constructor
RPGPlayerRace::RPGPlayerRace(string name, const RPGStats &s)
    : raceName(std::move(name)), racialStats(s) {
}

// RPGPlayerClass constructor
RPGPlayerClass::RPGPlayerClass(string name, const RPGStats &s)
    : className(std::move(name)), baseStats(s) {
}

// RPGCreatures constructor
RPGCreatures::RPGCreatures(const RPGStats &s)
    : stats(s) {
}

// RPGMobs constructor
RPGMobs::RPGMobs(string n, string t, const RPGStats &s)
    : RPGCreatures(s), name(std::move(n)), type(std::move(t)) {
}

// RPGPlayer constructor
RPGPlayer::RPGPlayer(User *user, string n, int ws, const RPGPlayerClass &pc, const RPGPlayerRace &pr, RPGInventory *inv,
                     const string &uname, const string &pwd, int gPoints, int coins)
    : RPGCreatures(pc.baseStats + pr.racialStats), user(user),
      name(std::move(n)), wavesSurvived(ws), pClass(pc), pRace(pr), backpack(inv) {
}


// Display stats in a clean format
ostream &operator<<(ostream &out, const RPGStats &s) {
    out << "HP: " << s.hp
            << " | Max HP: " << s.maxHp
            << " | ATK: " << s.atk
            << " | DEF: " << s.def
            << " | ATS: " << s.ats
            << " | ADF: " << s.adf;
    return out;
}

//------------------- RPGGame Implementation -------------------//
void RPGGame::createCharacter(RPGPlayer &player) {
    int raceChoice = 0;
    bool validInput = false;

    while (!validInput) {
        cout << "Choose your race:\n[1] Human\n[2] Elf\n[3] Orc\n[4] Demon\n";
        cin >> raceChoice;

        // Check for invalid input
        if (cin.fail() || raceChoice < 1 || raceChoice > 4) {
            cin.clear(); // Clears the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discards the invalid input
            cout << "Invalid input. Please enter a valid race choice (1-4).\n";
        } else {
            validInput = true;
        }
    }

    RPGPlayerRace race = getRace(raceChoice);

    int classChoice = 0;
    validInput = false;

    while (!validInput) {
        cout << "Choose your class:\n[1] Marksman\n[2] Swordsman\n[3] Wizard\n";
        cin >> classChoice;

        // Check for invalid input
        if (cin.fail() || classChoice < 1 || classChoice > 3) {
            cin.clear(); // Clears the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discards the invalid input
            cout << "Invalid input. Please enter a valid class choice (1-3).\n";
        } else {
            validInput = true;
        }
    }

    RPGPlayerClass pClass = getClass(classChoice);

    player.pRace = race;
    player.pClass = pClass;
    player.stats = race.racialStats + pClass.baseStats;
    player.stats.maxHp = player.stats.hp;
}

RPGPlayerRace RPGGame::getRace(int choice) {
    switch (choice) {
        case 1: return {"Human", RPGStats(100, 10, 10, 10, 10, 10)};
        case 2: return {"Elf", RPGStats(90, 8, 9, 12, 14, 11)};
        case 3: return {"Orc", RPGStats(120, 14, 12, 8, 6, 10)};
        case 4: return {"Demon", RPGStats(110, 12, 10, 9, 12, 13)};
        default: return {"Human", RPGStats(100, 10, 10, 10, 10, 10)};
    }
}

RPGPlayerClass RPGGame::getClass(int choice) {
    switch (choice) {
        case 1: return {"Marksman", RPGStats(50, 95, 60, 20, 8, 50)};
        case 2: return {"Swordsman", RPGStats(90, 75, 90, 10, 6, 80)};
        case 3: return {"Wizard", RPGStats(30, 6, 5, 65, 90, 140)};
        default: return {"Swordsman", RPGStats(90, 75, 90, 10, 6, 80)};
    }
}


void RPGPlayer::displayInventory() const {
    if (backpack->items.empty()) {
        cout << "Your inventory is empty.\n";
        return;
    }

    cout << "Your Inventory:\n";
    for (int i = 0; i < backpack->items.size(); ++i) {
        cout << "[" << i << "] " << backpack->items[i].itemName << endl;
    }
}

void RPGShop::displayItems() {
    cout << "Shop Inventory:\n";
    for (int i = 0; i < globalItemList.size(); ++i) {
        cout << "[" << i << "] " << globalItemList[i].itemName << endl;
    }
}

auto RPGShop::sellItem(const RPGPlayer &player, int itemID) -> void {
    if (itemID >= 0 && itemID < globalItemList.size()) {
        const RPGItem &selectedItem = globalItemList[itemID];

        // Add item to player's backpack
        player.backpack->items.push_back(selectedItem);
        cout << "You bought " << selectedItem.itemName << ".\n";
    } else {
        cout << "Invalid item ID.\n";
    }
}


void RPGGame::nextWave(RPGPlayer &player) {
    waveNumber++;

    // Define possible mobs for random selection
    vector<RPGMobs> possibleMobs;
    if (waveNumber >= 10) {
        // Only add dragon after wave 10
        possibleMobs = {
            RPGMobs("Goblin", "Wave " + to_string(waveNumber), RPGStats(70, 40, 40, 6, 5, 5)),
            RPGMobs("Elf", "Wave " + to_string(waveNumber), RPGStats(145, 55, 20, 9, 25, 20)),
            RPGMobs("Mercenary", "Wave " + to_string(waveNumber), RPGStats(100, 5, 50, 10, 15, 10)),
            RPGMobs("Dragon", "Wave " + to_string(waveNumber), RPGStats(1000, 400, 400, 20, 500, 200))
        };
    } else {
        possibleMobs = {
            RPGMobs("Goblin", "Wave " + to_string(waveNumber), RPGStats(70, 40, 4, 5, 5, 5)),
            RPGMobs("Elf", "Wave " + to_string(waveNumber), RPGStats(145, 55, 2, 9, 25, 20)),
            RPGMobs("Mercenary", "Wave " + to_string(waveNumber), RPGStats(150, 105, 5, 10, 15, 10))
        };
    }

    // Initialize random seed (should be done once, at program startup, but doing it here for simplicity)
    // Initialize random device and engine
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<size_t> dis(0, possibleMobs.size() - 1); // Uniform distribution

    // Select a random mob from the list
    size_t randomIndex = dis(gen); // Get a random index

    RPGMobs enemy = possibleMobs[randomIndex];

    // Apply wave scaling to enemy stats
    double multiplier = (waveNumber % 10 == 0) ? 0.25 : 0.35;
    enemy.stats = enemy.stats * multiplier;


    cout << "Wave " << waveNumber << " begins! A wild " << enemy.name << " appears!\n";
    turnBasedCombat(player, enemy);

    if (player.stats.hp > 0) {
        int statBoost = (waveNumber < 5) ? 2 : (waveNumber < 8) ? 4 : 5;
        player.stats = player.stats + RPGStats(statBoost, statBoost, statBoost, statBoost, statBoost, statBoost);

        // Increase max_hp as well, with similar scaling
        player.stats.maxHp += statBoost; // Increment max_hp as well

        player.user->setCoins(player.user->getCoins() + 5);
        if (waveNumber % 5 == 0) {
            player.user->setCoins(player.user->getCoins() + 3); // Extra coin bonus
            player.user->setGlobalPoints(player.user->getGlobalPoints() + 3); // Add 3 global points
            cout << GREEN << "You earned 3 global points for surviving wave " << waveNumber << "!\n" << RESET;
        }
    }
}

void RPGGame::turnBasedCombat(RPGPlayer &player, RPGMobs &mob) {
    while (player.stats.hp > 0 && mob.stats.hp > 0) {
        cout << BLUE BOLD "Your HP: " << player.stats.hp << " | Enemy HP: " << mob.stats.hp << "\n";

        cout << GREEN "Choose an action: [1] Attack [2] Magic [3] Item [4] Shop\n";
        int choice;
        cin >> choice;

        // Check for invalid input
        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear(); // Clears the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discards the invalid input
            cout << "Invalid input. Please enter a valid action (1-4).\n";
            continue;
        }

        switch (choice) {
            case 1: {
                cout << BABY_BLUE "You attack the " << mob.name << "!\n";
                mob.stats.hp -= max(0.0, player.stats.atk - mob.stats.def);
                mob.stats.hp = max(0.0, mob.stats.hp);

                break;
            }
            case 2: {
                cout << CYAN "Casting magic...\n";
                mob.stats.hp -= max(0.0, player.stats.ats - mob.stats.adf);
                mob.stats.hp = max(0.0, mob.stats.hp);

                break;
            }
            case 3: {
                cout << YELLOW "Using an item...\n";
                if (!player.backpack->items.empty()) {
                    // Display player's inventory
                    for (int i = 0; i < player.backpack->items.size(); ++i) {
                        cout << "[" << i + 1 << "] " << player.backpack->items[i].itemName << endl;
                    }

                    int itemChoice;
                    cout << "Choose an item to use (1-" << player.backpack->items.size() << "): ";
                    cin >> itemChoice;

                    // Apply the item effects based on selected item
                    if (itemChoice >= 1 && itemChoice <= player.backpack->items.size()) {
                        RPGItem selectedItem = player.backpack->items[itemChoice - 1];

                        if (selectedItem.itemName == "Health Potion") {
                            player.stats.hp = player.stats.maxHp;
                            cout << "You feel refreshed! HP fully restored.\n";
                        } else if (selectedItem.itemName == "Strength Potion") {
                            player.stats.atk += 5;
                            cout << "Your muscles surge with power! +5 ATK.\n";
                        } else if (selectedItem.itemName == "Magic Potion") {
                            player.stats.ats += 5;
                            player.stats.adf += 5;
                            cout << "Arcane energy flows through you! +5 ATS & ADF.\n";
                        } else if (selectedItem.itemName == "Defense Potion") {
                            player.stats.def += 5;
                            cout << "You feel tougher. +5 DEF.\n";
                        }

                        // Remove the used item from the inventory
                        player.backpack->items.erase(player.backpack->items.begin() + itemChoice - 1);
                    } else {
                        cout << "Invalid choice.\n";
                    }
                } else {
                    cout << "Your inventory is empty!\n";
                }
                break;
            }

            case 4: {
                // No more cheese
                static int shopUsesThisCombat = 0;
                constexpr int MAX_SHOP_USES = 1; // Limit to 1 shop visit per combat

                if (shopUsesThisCombat >= MAX_SHOP_USES) {
                    cout << CYAN "The shop is closed during this battle. You've already visited once!\n";
                    break;
                }
                // Display the shop's inventory
                RPGShop shop;
                RPGShop::displayItems();
                cout << BABY_BLUE "Enter the item number to buy (or 0 to go back): ";
                int buyChoice;
                cin >> buyChoice;

                if (buyChoice > 0 && buyChoice <= globalItemList.size()) {
                    RPGShop::sellItem(player, buyChoice - 1); // Pass the item index (0-based) to the shop
                } else if (buyChoice == 0) {
                    cout << YELLOW "Returning to menu...\n";
                } else {
                    cout << "Invalid choice.\n";
                }
                break;
            }

            default:
                cout << "Invalid choice.\n";
                continue;
        }
        cout << BLUE BOLD "Your HP: " << player.stats.hp << " | Enemy HP: " << mob.stats.hp << "\n";
        if (mob.stats.hp > 0) {
            cout << "The " << mob.name << " attacks you!\n";
            player.stats.hp -= max(0.0, mob.stats.atk - player.stats.def);
        }
    }
    if (player.stats.hp > 0) {
        cout << GREEN "You defeated the " << mob.name << "!\n" RESET;
    } else {
        cout << RED "You were defeated by the " << mob.name << "...\n" RESET;
    }
}

void RPGGame::loadData() {
    // Open the JSON file where the data is saved
    ifstream inFile("RPGPlayers_Data.json");
    if (!inFile.is_open()) {
        std::cout << "Failed to load the player data." << std::endl;
        return;
    }

    try {
        // Read the data from the file
        json data;
        inFile >> data;
        inFile.close();

        // Clear the previous list of all players
        allPlayers.clear();

        // Iterate over the data array and load each player
        for (const auto &p: data) {
            // Parse player stats
            RPGStats stats(
                p["stats"]["hp"],
                p["stats"]["atk"],
                p["stats"]["def"],
                p["stats"]["spd"],
                p["stats"]["ats"],
                p["stats"]["adf"]
            );

            // Parse race & class
            RPGPlayerRace race(p["race"], stats);
            RPGPlayerClass pclass(p["class"], stats);

            // Create the user object
            auto user = new User(p["user_name"], "password", 0, 0); // might need to adjust password handling

            // Parse inventory
            auto *inv = new RPGInventory();
            for (const auto &item: p["inventory"]) {
                RPGStats boost(
                    item["stat_boost"]["hp"],
                    item["stat_boost"]["atk"],
                    item["stat_boost"]["def"],
                    item["stat_boost"]["spd"],
                    item["stat_boost"]["ats"],
                    item["stat_boost"]["adf"]
                );
                inv->addItem(RPGItem(item["item_name"], item["item_id"], item["price"], boost));
            }

            // Load waves survived
            int wavesSurvived = p["waves_survived"]; // Add this line for wavesSurvived

            // Create the RPGPlayer object
            RPGPlayer player(user, p["character_name"], wavesSurvived, pclass, race, inv, user->getName(), "password",
                             0, 0);


            // Add the loaded player to the list
            allPlayers.push_back(player);
        }

        cout << "Player data loaded successfully!" << std::endl;
    } catch (const std::exception &e) {
        cout << "Error reading player data: " << e.what() << "\n";
        cout << "Continuing without loaded data.\n";
        allPlayers.clear(); // clear corrupted partial loads if needed
    }
}

void RPGGame::saveData(const RPGPlayer &player) {
    loadData(); // Load existing saves

    // Remove any existing save for this user
    allPlayers.erase(std::remove_if(allPlayers.begin(), allPlayers.end(),
                                    [&](const RPGPlayer &p) { return p.name == player.name; }),
                     allPlayers.end());

    // Add updated player
    allPlayers.push_back(player);

    // Convert all players to JSON
    json data = json::array();
    for (const auto &p: allPlayers) {
        json j;
        j["user_name"] = p.user->getName();
        j["waves_survived"] = p.wavesSurvived;
        j["character_name"] = p.name;
        j["race"] = p.pRace.raceName;
        j["class"] = p.pClass.className;

        j["stats"] = {
            {"hp", p.stats.hp},
            {"atk", p.stats.atk},
            {"def", p.stats.def},
            {"spd", p.stats.spd},
            {"ats", p.stats.ats},
            {"adf", p.stats.adf}
        };

        j["inventory"] = json::array();
        for (const auto &item: p.backpack->items) {
            j["inventory"].push_back({
                {"item_name", item.itemName},
                {"item_id", item.itemID},
                {"price", item.price},
                {
                    "stat_boost", {
                        {"hp", item.statBoost.hp},
                        {"atk", item.statBoost.atk},
                        {"def", item.statBoost.def},
                        {"spd", item.statBoost.spd},
                        {"ats", item.statBoost.ats},
                        {"adf", item.statBoost.adf}
                    }
                }
            });
        }

        data.push_back(j);
    }

    // Save to file - fix the path if needed
    std::ofstream outFile("RPG/RPGPlayers_Data.json");
    if (!outFile.is_open()) {
        // Try alternative path
        outFile.open("RPG/RPGPlayers_Data.json");
    }

    if (outFile.is_open()) {
        outFile << data.dump(4); // formatted JSON
        cout << "Player data saved successfully!\n";
    } else {
        cout << "Error: Could not open file for saving player data.\n";
    }
}

void RPGGame::RPGLeaderboard() {
    cout << "Leaderboard:\n";
    std::sort(allPlayers.begin(), allPlayers.end(), [](const RPGPlayer &a, const RPGPlayer &b) {
        return a.wavesSurvived > b.wavesSurvived;
    });
    for (const auto &player: allPlayers) {
        cout << player.name << " - Wave: " << player.wavesSurvived << "\n";
    }
}

// Save the leaderboard to a file
void RPGGame::saveLeaderboardToFile() {
    // Sort players by waves survived
    std::sort(allPlayers.begin(), allPlayers.end(), [](const RPGPlayer &a, const RPGPlayer &b) {
        return a.wavesSurvived > b.wavesSurvived;
    });

    // First try default path
    std::ofstream leaderboardFile("RPG/RPGLeaderboard.txt");
    if (!leaderboardFile.is_open()) {
        // Try alternative path
        leaderboardFile.open("RPG/RPGLeaderboard.txt");
    }

    if (leaderboardFile.is_open()) {
        leaderboardFile << "RPG Game Leaderboard\n";
        leaderboardFile << "====================\n\n";

        for (const auto &player: allPlayers) {
            leaderboardFile << player.name << " - Wave: " << player.wavesSurvived << "\n";
        }
        leaderboardFile.close();
        cout << "Leaderboard saved successfully!\n";
    } else {
        cout << "Unable to open leaderboard file for writing.\n";
    }
}

void RPGGame::gameMenu(User &user) {
    int choice = 0;
    do {
        // Clear screen
        cout << "\033[H\033[J"; // This will clear the screen in most terminals

        // Menu display
        cout << RED BOLD "+---------------------------------------+\n";
        cout << RED BOLD "|       Welcome to the RPG Adventure!   |\n";
        cout << RED BOLD "+---------------------------------------+\n";
        cout << BLUE "1. " RESET "Start Game\t\t\t\t" << BLUE "|\n";
        cout << BLUE "2. " RESET "View Leaderboard\t\t\t" << BLUE "|\n";
        cout << BLUE "3. " RESET "Exit Game\t\t\t\t" << BLUE "|\n";
        cout << "+---------------------------------------+\n";

        // User prompt
        cout << YELLOW ">> " RESET BOLD "Please select an option: " RESET;
        cin >> choice;

        if (cin.fail()) {
            choice = -1;
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore bad input
        }

        // Handling user input
        switch (choice) {
            case 1: {
                // Call play game function (replace with actual game start logic)
                cout << "Starting the game...\n";
                play(user);
                break;
            }
            case 2: {
                // Call leaderboard function (replace with actual leaderboard display)
                cout << "Showing leaderboard...\n";
                RPGLeaderboard();
                break;
            }
            case 3: {
                // Exit the game
                cout << "Exiting the game. Goodbye!\n";
                break;
            }
            default:
                cout << RED "INVALID INPUT! Please try again.\n" RESET;
                break;
        }

        // Wait for user to press a key before clearing screen
        cout << "\nPress any key to continue...";
        cin.get(); // Wait for key press
        cin.get(); // Consume extra new line character
    } while (choice != 3);
}

void RPGGame::deletePlayer(User &user) {
    allPlayers.erase(
        remove_if(allPlayers.begin(), allPlayers.end(),
                  [&](const RPGPlayer &p) { return p.name == user.getName(); }),
        allPlayers.end()
    );
    cout << "Deleted player data for " << user.getName() << ".\n";
    saveData(RPGPlayer(&user, "", 0, getClass(2), getRace(1), new RPGInventory(), user.getName(), user.getPassword(), 0,
                       0)); // optional clean slate save
}

void RPGGame::wipeGame() {
    allPlayers.clear();
    std::ofstream ofs("RPGPlayers_Data");
    ofs << "[]"; // overwrite with empty array
    ofs.close();
    cout << "All RPG player data wiped.\n";
}
