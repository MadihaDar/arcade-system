#ifndef TYPINGGAME_H
#define TYPINGGAME_H

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define AQUA "\033[36m"
#define RESET "\033[0m"
#define CLRSCR "\033[2J\033[1;1H"

#include "../Arcade.h"
#include "chrono"
constexpr int GAME_DURATION = 30;
using namespace std;

class TypingScore;

//TEMPLATE CLASS
template <typename ScoreType>
class ScoreAnalyzer {
public:
    static typename ScoreType::WPMType getHighestScore(const vector<ScoreType>& scores) {
        if (scores.empty()) return 0;

        return max_element(scores.begin(), scores.end(), //check scores vector
            [](const ScoreType& a, const ScoreType& b) { //a and b are the values to compare (like i, j)
                return a < b; //(comparison based on operator overload)
            })->getWPM(); //get wpm of highest element (max_element returns a pointer to it)
    }

    static double getAverageScore(const vector<ScoreType>& scores) {
        if (scores.empty()) return 0.0;

        double total = 0;
        for (const auto& score : scores) {
            total += score.getWPM();
        }
        return total / scores.size();
    }
};

class TypingScore
{
private:
    int wpm;
    chrono::system_clock::time_point time; //store starting time. UNIX
public:
    using WPMType = int;
    TypingScore(): wpm(0) {}
    TypingScore(const int &wpm, const chrono::system_clock::time_point time): wpm(wpm), time(time) {}

    TypingScore(const int &w) : wpm(w), time(std::chrono::system_clock::now()) {}

    int getWPM() const { return wpm; }
    chrono::system_clock::time_point getTimestamp() const { return time; }

    //For simpler leaderboard calculations
    //INLINE FUNCTIONS
    inline bool operator>(const TypingScore& other) const;
    inline bool operator<(const TypingScore& other) const;
    inline bool operator==(const TypingScore& other) const;
    inline bool operator!=(const TypingScore& other) const;

    operator string() const;

    //FRIEND FUNC
    friend void to_json(json& j, const TypingScore& s);
    friend void from_json(const json& j, TypingScore& s);

    friend ostream& operator<<(ostream& os, const TypingScore& s);
    //FRIEND CLASS & TEMPLATE CLASS
    template <typename T>
    friend class ScoreAnalyzer;
};

class TypingPlayer
{
protected:
    User* user; //Aggregation - User can exist without TypingPlayer
    vector<TypingScore> scores; //scores of all the games the TypingPlayer has ever played.
public:
    TypingPlayer(): user(nullptr) {}
    TypingPlayer(User* user): user(user) {}

    vector<TypingScore> getScores() const;
    void addScore(int wpm); //add to scores vector
    void addScore(const TypingScore& score);
    User* getUser() const { return user; }

    friend void to_json(json& j, const TypingPlayer& p);
    friend void from_json(const json& j, TypingPlayer& p);

    friend ostream& operator<<(ostream& os, const TypingPlayer& p);
};

//SINGLETON CLASS
class WordBank {
private:
    vector<string> words;
    WordBank() {}

public:
    static WordBank& getInstance() {
        static WordBank instance; //SINGLETON INSTANCE
        return instance;
    }

    static void initialize();

    WordBank(const WordBank&) = delete;
    WordBank& operator=(const WordBank&) = delete;

    string getRandomWord();

    void setWords(const vector<string>& w);

    void loadWords();
};

class TypingGame : public Game { //INHERITANCE
private:
    vector<TypingPlayer> players; //COMPOSITION (load every TypingPlayer into array on initial run so we don't have to run slow File IO.)
    TypingPlayer* currentTypingPlayer; //points to a TypingPlayer in the vector
    map<int, TypingPlayer*> rankedPlayers; //MAP

    TypingPlayer* findOrCreateTypingPlayer(const string& name); //find TypingPlayer in vector (or create new TypingPlayer if not found)
    static void showResults(const TypingPlayer& typingPlayer); //show specific TypingPlayer
    void showLeaderboard();
    void loadFromJson(); //load all TypingPlayers into TypingPlayer vector.
    void saveToJson() const; //save only the edited (current) TypingPlayer to the json? or the entire vector. hmm.
    void startGame() const; //accessed from play(...)

public:
    TypingGame(): Game("TypingGame", true), currentTypingPlayer(nullptr) {}

    //PLAY() is ASSOCIATION. Sure, TypingGame knows User exists, but it NEVER manages/handles it. No ownership involved.
    void gameMenu(User& user) override; //menu first. handles loading and everything.  the currentTypingPlayer is also set here. TypingPlayer can select start() in menu.
    void wipeGame() override; //wipe all game data
    void deletePlayer(User& user) override;
};


#endif //TYPINGGAME_H