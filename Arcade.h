#ifndef ARCADE_H
#define ARCADE_H
#include "Users.h"
using namespace std;
class Game
{
protected:
    string name;
    int isEnabled;

public:
    Game(): name(""), isEnabled(true) {}

    Game(string name, const int isEnabled): name(move(name)), isEnabled(isEnabled) {}

    virtual void gameMenu(User& user) = 0; //ASSOCIATION
    //pass a user to the game that the game will then convert to a custom GamePlayer object
    virtual void deletePlayer(User& user) = 0;
    virtual void wipeGame() = 0;

    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    int getEnabled() const { return isEnabled; }
    void setEnabled(const bool& isEnabled) { this->isEnabled = isEnabled; }
};

class ArcadeSystem
{
private:
    vector<Game*> games; //AGGREGATION & VECTOR
    vector<User> users; //COMPOSITION

public:
    ArcadeSystem() {};

    //Association
    template <typename T>
    void registerGame()
    {
        games.push_back(new T());
    }

    Game* selectGame(const string& gameName) const;

    void addUser(const User& user);

    vector<Game*>& getGames();

    User* getUser(const string& userName);

    void loadUsers(); //JSON

    void saveUsers() const; //JSON

    void removeUser(const string& username);
};

extern ArcadeSystem arcadeSystem;

#endif //ARCADE_H
