#include "HopscotchFunctions.h"

//_________________________________________________________________________________________


void HopscotchPlayer::displayHopscotchRules(User& user) {
    // display rules for the hopscotch game 
    cout << "\n\t\t\tHey " << user.getName() << " . Welcome!";
    cout << "\n\t\t\tThis game is based on the traditional Hopscotch Game played in South Asia with twists"
         << "\n\t\t\tand turns:\n"
         << "\t\t\tRule 1: You are allowed only a few boxes to jump per turn. You won't be made aware of\n"
         << "\t\t\tthose at least before you make an invalid jump.\n"
         << "\t\t\tRule 2: Your score decreases if you make invalid jumps or step on the invisible stone.\n"
         << "\t\t\tRule 3: The stone will only be displayed when you land on it!\n\n";
}

//_________________________________________________________________________________________


  // save function
void ScoreManager::saveHopscotchScoresToJson() {
    json data = json::array();

    for (const auto& player : players) {
        json playerData;
        playerData["playerID"] = player.getID();
        playerData["score"] = player.getScore();
        playerData["totalMoves"] = player.getTotalMoves();
        data.push_back(playerData);
    }

    ofstream outFile("Hopscotch/hopscotch_scores.json");
    if (!outFile.is_open()) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    outFile << data.dump(4);  // Pretty print with indentation
    outFile.close();
}



//_________________________________________________________________________________________


// load function 
void ScoreManager::readHopscotchScoresFromJson(const string& filename) {
    ifstream inFile(filename);
    players.clear();
    // exception handling 
    if(!inFile.is_open()) {
        ofstream(filename);
        return;
    }
    
    // make a json object and retrieve the data into it
    json data;
    inFile >> data;
    inFile.close();

    for (int i = 0; i < data.size(); i++)
    {
        HopscotchPlayer p;
        p.score = data[i]["score"];
        p.totalMoves = data[i]["totalMoves"];
        p.user = arcadeSystem.getUser(data[i]["playerID"]);
        p.ID = data[i]["playerID"];
        players.push_back(p);
    }
}

//_________________________________________________________________________________________


void ScoreManager::bubbleSortForHopscotchLeaderboard(HopscotchPlayer* playersArray, int size) {
    // outer loop 
    for (int i = 0; i < size - 1; ++i) {
        // inner loop
        for (int j = 0; j < size - i - 1; ++j) {
            // comparing scores
            if (playersArray[j].getScore() < playersArray[j + 1].getScore()) {
                // swapping on the basis of scores
                swap(playersArray[j], playersArray[j + 1]);
            }
        }
    }
}

//_________________________________________________________________________________________


void ScoreManager::displayHopscotchLeaderboard(HopscotchPlayer* playersArray, int size, int topLimit) {
    cout << "\n\t\t\t  --- Hopscotch Leaderboard ---\n";
    // loop for the display of the top 8 players 
    for (int i = 0; i < size && i < topLimit; ++i) {
        cout << "\t\t\tRank " << i + 1 << ": " << playersArray[i].getID() 
             << " | Score: " << playersArray[i].getScore() 
             << " | Total Moves: " << playersArray[i].getTotalMoves() << endl;
    }
}


//_________________________________________________________________________________________


void ScoreManager::searchScores(const string& playerID) {
    // check the opening of the file 
    ifstream inFile("Hopscotch/hopscotch_scores.json");
    // exception handling 
    if(!inFile.is_open()) {
        cout << "\n\t\t\tUser not found!";
        return;
    }

    // get the already stored data
    json data;
    inFile >> data;
    inFile.close();

    bool found = false;

    // loop in the file for the desired name of player
    for (const auto& entry : data) {
        if (entry["playerID"] == playerID) {
            cout << "\n\t\t\tUser ID: " << entry["playerID"]
                 << " | Moves: " << entry["totalMoves"]
                 << " | Score: " << entry["score"] << endl;

            int score = entry["score"];

            // remarks on the found scores 
            if (score < 5) {
                cout << "\n\tHINT for better score: Make sure to jump the least numbers and reach the end! Have Fun!\n";
            } 
            else if (score >= 5 && score <= 8) {
                cout << "\n\t\t\tYour Score is" << BLUE << " good!" << RESET << "\n";
            } 
            else {
                cout << "\n\t\t\tYour score is" << MAGENTA << " brilliant!" << RESET << "\n";
            }

            // once found break loop
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\t\t\tPlease enter a valid ID!\n" << endl;
    }
}

//_________________________________________________________________________________________


void PlayHopscotchGame::wipeGame() {
    // clear function to wipe the game data
    ScoreManager::getInstance()->getPlayers().clear();
    remove("Hopscotch/hopscotch_scores.json");
}


//_________________________________________________________________________________________


void PlayHopscotchGame::deletePlayer(User& user) {
    // delete player function - reading players from the file
    ScoreManager::getInstance()->readHopscotchScoresFromJson("Hopscotch/hopscotch_scores.json");
    // loop through the players 
    for (int i = 0; i < ScoreManager::getInstance()->getPlayers().size(); ++i) {
        // match the user name 
        if (ScoreManager::getInstance()->getPlayers()[i].getID() == user.getName()) {
            // erase function for deletion
            ScoreManager::getInstance()->getPlayers().erase(ScoreManager::getInstance()->getPlayers().begin() + i);
            break;
        }
    }

    // saving the updates
    ScoreManager::getInstance()->saveHopscotchScoresToJson();
}


//_________________________________________________________________________________________