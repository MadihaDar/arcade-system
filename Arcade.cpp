#include "Arcade.h"

ArcadeSystem arcadeSystem = ArcadeSystem();  // Definiton of the extern variable

Game* ArcadeSystem::selectGame(const string& gameName) const
{
    for (const auto game : games) {
        if (game->getName() == gameName) {
            return game;
        }
    }
    return nullptr;
}

void ArcadeSystem::removeUser(const string& username) {
    for (auto it = users.begin(); it < users.end(); ++it) {
        if (username == it->getName()) {
            users.erase(it);
            saveUsers();
            return;
        }
    }
}

void ArcadeSystem::saveUsers() const
{
    json data;

    for (const auto& u : users)
    {
        data[u.getName()] = {
            {"password", u.getPassword()},
            {"globalPoints", u.getGlobalPoints()},
            {"coins", u.getCoins()}
        };
    }

    ofstream file("users.json");
    file << data.dump(4);
    file.close();
}

void ArcadeSystem::loadUsers()
{
    users.clear();
    json data = User::readJsonFileUsers("users.json");

    for (auto& [name, info] : data.items())
    {
        User u;
        u.setName(name); //name = key, info = value
        u.setPassword(info["password"].get<string>());
        u.setGlobalPoints(info["globalPoints"].get<int>());
        u.setCoins(info["coins"].get<int>());
        users.emplace_back(u);
    }
}

User *ArcadeSystem::getUser(const string &userName)
{
    for (auto & user : users)
    {
        if (userName == user.getName())
            return &user;
    }
    return nullptr;
}

void ArcadeSystem::addUser(const User& user)
{
    users.push_back(user);
}

vector<Game*>& ArcadeSystem::getGames() { return games; }