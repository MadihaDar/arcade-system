#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include <string>
#include "../json.hpp"    
#include "../Users.h"
#include "../Arcade.h"

using json = nlohmann::json;
using namespace std;

// Structure for Question
struct Question {
    string question;
    string options[4];
    char ans;
};

class TriviaPlayer {
    private:
        string userID;
        int score;
        User* user;  // Aggregation

    public:
        // Constructor
        TriviaPlayer(const string &id, User* u, int initialScore = 0) : userID(id), user(u), score(initialScore) {}
        
        // Getters
        string getUserID() const { 
            return userID; 
        }

        inline int getScore() const { // inline function
            return score; 
        }

        // Setter
        void setScore(int newScore) { 
            score = newScore; 
        }

        // Update global points using User
        void updateGlobalPoints(int points) {
            user->updateGlobalPoints(points);
        }

        // Update coins using User
        void addCoins(int previousCoins){
            user->addCoins(previousCoins);
        }       

        // Type cast operator to int
        operator int() const {
            return score;
        }
};

// TriviaQuestions: Manages trivia-related logic
class TriviaQuestions {
    private:
        vector<Question> questions;     // Composition

    public:    
        TriviaQuestions(){
            loadQuestions();
        };      // Constructor to load questions

        bool askQuestion(Question &);   // Ask a question and return true/false based on the answer
        void loadQuestions();

        // friend class declaration
        friend class TriviaTacToe;
};  
    

// TicTacToe: Manages the board and game logic
class TicTacToeBoard {
    private:
        char grid[3][3]; // 3x3 grid for Tic-Tac-Toe
        
    public:
        TicTacToeBoard(){
            initializeGrid();
        }; // Constructor initializes the board

        void initializeGrid(); 
        void displayGrid(); // Displays the grid
        bool checkWin(char player); // Checks for a win
        
        // Inline getter function
        inline char getGridValue(int row, int col) const {
            return grid[row][col];
        }
    
        // Setter for grid value
        void setGridValue(int row, int col, char value) {
            grid[row][col] = value;
        }

};

// TriviaTacToe: Main controller combining Trivia and Tic-Tac-Toe
class TriviaTacToe : public Game {
    private:
        TriviaPlayer* player;   // Aggregation
        string username;
        int userScore;
        int compScore;

        static json readTriviaJsonFile(const string&);
        static void writeTriviaJsonFile(const string&, const json&);

        friend ostream& operator<<(ostream& os, const TriviaTacToe& game) {
            os << BOLD RED "\n\t\t\t\t\t\t         Username: " RESET << game.player->getUserID() << "\n";
            os << BOLD RED "\n\t\t\t\t\t\t         User Score: " RESET << game.player->getScore() << "\n";
            os << BOLD RED "\n\t\t\t\t\t\t         Computer Score: " RESET << game.compScore << "\n";
            return os;
        }

        TriviaQuestions trivia; // Composition
        TicTacToeBoard tictactoe; // Composition

    public:
        // Constructor
        TriviaTacToe():userScore(0), compScore(0), trivia(), tictactoe(), player(nullptr), Game("TriviaTacToe", true){}

        // Destructor
        ~TriviaTacToe() {
            delete player;
        }
        
        void viewTriviaInstructions();

        // Function Overloading
        void updateTriviaScore(int userPoints, int compPoints);
        void updateTriviaScore();

        void viewTotalTriviaScore(User& user);
        
        void displayTriviaLeaderboard(const string& filename);

        void deletePlayer(User& user) override; // overriden function
        void gameMenu(User& user) override;   // overriden function
        void playTriviaGame(User& user);
        void wipeGame() override;   // overriden function
};

// Singleton class to track how many times my game is played
class TriviaGameCountManager {
    private:
        static TriviaGameCountManager* instance;
        int totalGamesPlayed;
    
        // Private constructor
        TriviaGameCountManager() {
            totalGamesPlayed = 0;
        }
    
    public:
        // static method to create an instnace if none exists
        static TriviaGameCountManager* getInstance() {
            if (!instance)
                instance = new TriviaGameCountManager;
            return instance;
        }
    
        void incrementGameCount() {
            totalGamesPlayed++;
        }
    
        int getGameCount() const {
            return totalGamesPlayed;
        }
    };

// Template class for sorting an array (to display the leaderboard)
template <typename T, typename U>
class SortTriviaScoresArray {
    public:
        static void sortArray(T arr[], U usernames[], int size) {   // Bubble Sort
            for (int i = 0; i < size - 1; ++i) {
                for (int j = 0; j < size - i - 1; ++j) {
                    if (arr[j] < arr[j + 1]) {
                        swap(arr[j], arr[j + 1]);
                        swap(usernames[j], usernames[j + 1]);
                    }
                }
            }
        }
    };    
    
#endif