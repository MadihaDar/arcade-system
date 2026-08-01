#ifndef HOPSCOTCHFUNCTIONS_H
#define HOPSCOTCHFUNCTIONS_H

// including the required files 
#include<iostream>
#include<vector>
#include<map>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include"../Users.h"
#include"../Arcade.h"
#include"../json.hpp"

using json = nlohmann::json;
using namespace std;
 
//_____________CLASS ONE_________________________________________________________________________________________

class HopscotchPlayer {
// keeping the identity based members of the class protected
protected:
    string ID;
    int score;
    int totalMoves;
    User* user;

public:

    // constructor - holdin the initial values 
    // default constructor
    HopscotchPlayer() {
       ID = "";
       score = 10;
       totalMoves = 0;
       user = nullptr;
    }

    // parameterized constructor
    HopscotchPlayer(User* u) {
       ID = u->getName();
       score = 10;
       totalMoves = 0;
       user = u;
    }

    // getters & setters 
    string getID() const { return ID; }
    void setID(User* u) { this->ID = u->getName(); }

    // type casting to int -> return score 
    operator int() const {
        return score;
    }

    int getScore() const { return score; }
    void setScore(int s) {score = s;}

    int getTotalMoves() const { return totalMoves; }
    void setTotalMoves(int tM) {totalMoves = tM;}

    // methods of the class hadling score and move updates 
    void decreaseScore(int change) { score -= change; }
    void increaseScore(int change) {score += change;}
    void incrementMoves() { totalMoves++; }
    void displayHopscotchRules(User& user);

    friend class ScoreManager; // FRIEND CLASS 
    // displaying the scores and all
    friend ostream& operator<<(ostream& os, const HopscotchPlayer& player); // FRIEND & OPERATOR OVERLOAD 
};


//_____________CLASS TWO_________________________________________________________________________________________

class GameBoard {
private:
    int boardSize;
    vector<int> board; // VECTOR
    int stonePosition;
    int maxMoves;
    int playerPosition;

public:
    // constructor
    GameBoard(int boardSize = 10) : boardSize(boardSize), board(boardSize, 0) {}

    // getters & setters 
    int getStonePosition() const { return stonePosition; }
    int getMaxMoves() const { return maxMoves; }
    int getPlayerPosition() const { return playerPosition; }

    void setBoardSize(int size) { boardSize = size; board.resize(size, 0); }
    int getBoardSize() const { return boardSize; }


    void setBoard(const vector<int>& newBoard) { board = newBoard; }
    vector<int> getBoard() const { return board; }

    // initialize board & display methods 
    void initializeBoard();
    void displayBoard(int playerPos, int stonePos, int row, int count)const;
};


//_____________CLASS THREE_________________________________________________________________________________________

class PlayHopscotchGame :public Game { // INHERITENCE 
private:
    GameBoard board; // COMPOSITION
    HopscotchPlayer* player;

public:

    // constructor
    PlayHopscotchGame() : player(new HopscotchPlayer()), Game("HopscotchGame", true) {}
    // deleting to save from memory leakage
    ~PlayHopscotchGame() { delete player; }

    // class methods
    void playHopscotchGame(User& user, HopscotchPlayer& player);
    int displayHopscotchScore()const { return player->getScore(); }
    void gameMenu(User& user)override; // shift the HopscotchMenu system to this function here.
    void deletePlayer(User& user) override;
    void wipeGame() override;
    };



//_____________CLASS FOUR_________________________________________________________________________________________

// singleton class
class ScoreManager {
    private:
        vector<HopscotchPlayer> players; // FRIEND RELATION
        
        // singleton instance
        static ScoreManager* instance;
            
        // private constructor
        ScoreManager() {}

    public:
    
        // get instance func
        static ScoreManager* getInstance() {
            if (!instance) {
            instance = new ScoreManager();
            }
            return instance;
        }
    
        // class func
        void saveHopscotchScoresToJson();
        void readHopscotchScoresFromJson(const string& filename);
        void bubbleSortForHopscotchLeaderboard(HopscotchPlayer* playersArray, int size);
        void displayHopscotchLeaderboard(HopscotchPlayer* players, int size, int topLimit);
        void searchScores(const string& playerID);
        vector<HopscotchPlayer>& getPlayers() { return players; }
    };
    
    
//_____________CLASS FIVE_________________________________________________________________________________________
    
// global leaderboard
 
//_____________CLASS SIX_________________________________________________________________________________________

// template class for comparison of scores for any two players
template <typename T>
class ScoreComparer {
public:
    // passing map as a parameter 
    void compareWithAnotherPlayer(HopscotchPlayer& currentPlayer, map<string, HopscotchPlayer>& playerMap) {
    
    // read from the file 
    ScoreManager::getInstance()->readHopscotchScoresFromJson("Hopscotch/hopscotch_scores.json");

    cout << "\n\t\t\t\t--- Comparing Scores ---\n";
    
    string compareID;
    // get the id from the user
    cout << "\t\t\tEnter the Player ID you want to compare with: ";
    cin >> compareID;

    // store the id in auto object 
    auto it = playerMap.find(compareID);

    // check if the id is found
    if (it != playerMap.end()) {
        // get the player 
        HopscotchPlayer& player = it->second;
        cout << "\n\t\t\tComparing with player " << player.getID() << ":\n";
        cout << "\t\t\tScore: " << player.getScore() << " | Total Moves: " << player.getTotalMoves() << endl;

        // display comparison results 
        if (currentPlayer.getScore() > player.getScore()) {
            cout << "\t\t\t" << GREEN << "You"  << RESET  << " have a higher score!\n";
        }
        else if (currentPlayer.getScore() < player.getScore()) {
            cout << "\t\t\t" << RED << player << RESET << " has a higher score.\n";
        }
        else {
            cout << "\t\t\tYou both have the" << YELLOW << " same score." << RESET << "\n";
        }

        if (currentPlayer.getTotalMoves() < player.getTotalMoves()) {
            cout << "\t\t\tYou achieved your score in fewer moves!\n";
        }
        else if (currentPlayer.getTotalMoves() > player.getTotalMoves()) {
            cout << "\t\t\tThe other player achieved their score in fewer moves.\n";
        }
        else {
            cout << "\t\t\tYou both used the same number of moves.\n";
        }
    } 
    else {
        cout << "\t\t\tPlayer ID not found in the leaderboard.\n";
    }
}
};

//_______________________________________________________________________________________________________________


#endif