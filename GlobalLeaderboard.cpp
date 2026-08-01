
#include "GlobalLeaderboard.h"

//_________________________________________________________________________________________

vector<User> GlobalLeaderBoard::loadUsersFromJsonForGlobalLB(const string& filename) {
    // vector of users 
    vector<User> users;

    // check for file opening 
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return users;
    }

    // get the data
    json data;
    file >> data;
    file.close();

    // loop through the lines 
    for (auto& entry : data.items()) {
        // as the key is the id use to it to find the value of global points
        string playerID = entry.key();
        int globalPoints = entry.value()["globalPoints"];

        // set the desired members and push them back
        User u;
        u.setName(playerID);
        u.setGlobalPoints(globalPoints);
        users.push_back(u);
    }

    // return users
    return users;
}



//_________________________________________________________________________________________


void GlobalLeaderBoard::bubbleSortForGlobalLeaderBoard(User* players, int size) {
    // outter loop
    for (int i = 0; i < size - 1; ++i) {
        // inner loop
        for (int j = 0; j < size - i - 1; ++j) {
            // compare the corresponding memeber's globalPoints
            if (players[j].getGlobalPoints() < players[j + 1].getGlobalPoints()) {
                // swap 
                swap(players[j], players[j + 1]);
            }
        }
    }
}


//_________________________________________________________________________________________


void GlobalLeaderBoard::displayGlobalLeaderboard(User* players, int size, int limit) {
    cout << CLRSCR;
    const string colors[] = {
        YELLOW, CYAN, MAGENTA, GREEN, BLUE, RED, PINK, PEACH
    };

    cout << BOLD << "\n\n\t\t\t\t\t\t\t--- GLOBAL LEADERBOARD ---" << RESET << "\n\n\n";

    for (int i = 0; i < size && i < limit; ++i) {
        string color = colors[i % (sizeof(colors)/sizeof(colors[0]))];  

        cout << color << "\t\t\t\t\t\t     " << i + 1 << ". " << players[i].getName()
             << "  | Global Points: " << players[i].getGlobalPoints() << RESET << "\n\n";
    }
}

//_________________________________________________________________________________________