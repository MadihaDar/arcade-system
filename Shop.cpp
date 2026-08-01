#include "Shop.h"


void printBorderedLine(const string &text, const int width) {
    cout << CYAN << "||" << RESET;
    cout << " " << setw(width - 6) << left << text; // width-6 because 2 pipes + 2 spaces each side
    cout << CYAN << "||" << RESET << "\n";
}


//  Arcade Shop Functions
json ArcadeShopItem::toJson() const {
    // Returning shop items as json objects.
    return json{{"name", rewardName}, {"id", rewardID}, {"price", rewardPrice}};
}

ArcadeShopItem ArcadeShopItem::fromJson(const json &j) {
    // Converting the json objects back to the data types.
    return {j["name"], j["id"], j["price"]};
}

void ArcadeShop::initializeDefaultRewards() {
    // Default Rewards.
    rewardMap = {
        {1, ArcadeShopItem("Teddy Bear", 1, 100)},
        {2, ArcadeShopItem("RC Car", 2, 200)},
        {3, ArcadeShopItem("Wrist Watch", 3, 300)},
        {4, ArcadeShopItem("Gaming Mouse", 4, 500)},
        {5, ArcadeShopItem("Headphones", 5, 700)}
    };
    saveToJson();
}

void ArcadeShop::addReward(const string &name, int id, int price) {
    // Adding a reward to the map using ID as the key
    rewardMap[id] = ArcadeShopItem(name, id, price);
    saveToJson();
}

void ArcadeShop::deleteReward(int rewardID) {
    // Simply erase the item with the given ID from the map
    rewardMap.erase(rewardID);
    saveToJson();
}

void ArcadeShop::modifyReward(int rewardID, const string &newName, int newPrice) {
    // Find the reward by ID and update its properties
    auto it = rewardMap.find(rewardID);
    if (it != rewardMap.end()) {
        it->second.rewardName = newName;
        it->second.rewardPrice = newPrice;
        saveToJson();
    }
}

void ArcadeShop::displayRewards() const {
    // Iterate over the map and display to terminal
    for (const auto &[id, reward]: rewardMap) {
        cout << "ID: " << reward.rewardID << " - " << reward.rewardName
                << " | Price: " << reward.rewardPrice << " coins\n";
    }
}

void ArcadeShop::purchaseReward(User &user, int rewardID) {
    // Find the reward by ID using map's direct lookup
    auto it = rewardMap.find(rewardID);
    if (it != rewardMap.end()) {
        const ArcadeShopItem &reward = it->second;
        if (user.getCoins() >= reward.rewardPrice) {
            user.setCoins(user.getCoins() - reward.rewardPrice);
            // Wrap the reward in the template class
            ItemWrapper<std::string> wrappedReward(reward.rewardName);
            wrappedReward.show();
            cout << "Purchased: " << reward.rewardName << "!\n";
        } else {
            cout << "Not enough coins!\n";
        }
    } else {
        cout << "Invalid reward ID!\n";
    }
}

void ArcadeShop::saveToJson() const {
    // Exception handling using try and catch.
    try {
        // Convert the map values to a JSON array
        json j = json::array();
        for (const auto &[id, reward]: rewardMap) {
            j.push_back(reward.toJson());
        }
        ofstream file(filename);
        file << j.dump(4);
    } //Error Case Handling
    catch (const std::exception &e) {
        cerr << "Error saving to JSON: " << e.what() << endl;
    }
}

void ArcadeShop::loadFromJson() {
    // If file does not load throwing an error.
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("File not found: " + filename);
        }

        json j;
        file >> j; // This can throw if JSON is malformed

        rewardMap.clear();

        if (j.empty()) {
            initializeDefaultRewards();
            return;
        }

        for (const auto &item: j) {
            ArcadeShopItem reward = ArcadeShopItem::fromJson(item);
            rewardMap[reward.rewardID] = reward;
        }
    } catch (const std::exception &e) {
        cerr << "Error loading from JSON: " << e.what() << "\nInitializing default rewards.\n";
        initializeDefaultRewards(); // Fallback
    }
}

void ArcadeShop::shopMenu(User &user) {
    int choice;
    const int width = 36; // total width for the box

    do {
        cout << CYAN << string(width, '=') << "\n" << RESET;
        printBorderedLine(BOLD + string("ARCADE SHOP MENU") + RESET, width);
        cout << CYAN << string(width, '=') << "\n" << RESET;

        printBorderedLine(GREEN + string("1.") + RESET + " View All Rewards", width);
        printBorderedLine(GREEN + string("2.") + RESET + " Redeem Rewards", width);
        printBorderedLine(GREEN + string("3.") + RESET + " Reward Management Menu", width);
        printBorderedLine(GREEN + string("4.") + RESET + " Exit to Main Menu", width);

        cout << CYAN << string(width, '=') << "\n" << RESET;
        cout << BOLD << "Enter your choice: " << RESET;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number.\n" << RESET;
            continue;
        }

        switch (choice) {
            case 1:
               displayRewards(); //  Actually view all rewards
                break;
            case 2: {
                int rewardID;
                cout << "Enter the Reward ID you want to redeem: ";
                cin >> rewardID;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input for Reward ID.\n";
                    break;
                }
                purchaseReward(user, rewardID); //now correct
                break;
            }
            case 3:
                rewardManagementMenu(); // <- Jump to reward management
                break;
            case 4:
                cout << RED << "\n[Returning to Main Menu...]\n" << RESET;
                break;
            default:
                cout << RED << "Invalid choice. Please try again.\n" << RESET;
                break;
        }
    } while (choice != 4);
}


void ArcadeShop::rewardManagementMenu() {
    // Fi di da do di do fi do di da day.
    int choice;
    do {
        cout << "\nReward Management Menu:\n";
        cout << "1. Add Reward\n";
        cout << "2. Remove Reward\n";
        cout << "3. Modify Reward\n";
        cout << "4. Exit \n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int rewardID, rewardPrice;
                string rewardName;

                cout << "Enter new Reward ID: ";
                cin >> rewardID;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Reward ID.\n";
                    break;
                }

                cout << "Enter Reward Name: ";
                cin.ignore();
                getline(cin, rewardName);

                cout << "Enter Reward Price: ";
                cin >> rewardPrice;
                if (cin.fail() || rewardPrice < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Reward Price.\n";
                    break;
                }

                addReward(rewardName, rewardID, rewardPrice); // <- FIXED
                cout << "Reward added successfully.\n";
                break;
            }
            case 2: {
                int rewardID;
                cout << "Enter Reward ID to remove: ";
                cin >> rewardID;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Reward ID.\n";
                    break;
                }

                deleteReward(rewardID); // <- FIXED
                cout << "Reward removed successfully.\n";
                break;
            }
            case 3: {
                int rewardID, newPrice;
                string newName;

                cout << "Enter Reward ID to modify: ";
                cin >> rewardID;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Reward ID.\n";
                    break;
                }

                cout << "Enter new Reward Name: ";
                cin.ignore();
                getline(cin, newName);

                cout << "Enter new Reward Price: ";
                cin >> newPrice;
                if (cin.fail() || newPrice < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Reward Price.\n";
                    break;
                }

                modifyReward(rewardID, newName, newPrice);
                break;
            }
            case 4:
                cout << "Exiting Reward Management Menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);
}