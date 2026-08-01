#include "TypingGame.h"

#include <iostream>
#include "fstream"

WordBank* wordBank = nullptr;

// OTHER FUNCTIONS

void pressKeyToReturn()
{
    cout << RED BOLD "\n>> Press any Key to Return..." RESET;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// SCORE SECTION

void from_json(const json& j, TypingScore& s)
{
    s = TypingScore(j["wpm"], chrono::system_clock::from_time_t(j["timestamp"].get<time_t>())); //.get<>() safety check
}

void to_json(json& j, const TypingScore& s)
{
    j["wpm"] = s.wpm;
    j["timestamp"] = chrono::system_clock::to_time_t(s.time);
}

bool TypingScore::operator>(const TypingScore& other) const
{
    return wpm > other.wpm;
}

bool TypingScore::operator<(const TypingScore& other) const
{
    return wpm < other.wpm;
}

bool TypingScore::operator==(const TypingScore& other) const {
    return wpm == other.wpm;
}

bool TypingScore::operator!=(const TypingScore& other) const {
    return !(*this == other);
}

TypingScore::operator string() const
{
    const time_t times = chrono::system_clock::to_time_t(time);
    char finalTime[20];
    strftime(finalTime, sizeof(finalTime), "%H:%M:%S %d-%m-%Y", localtime(&times));

    ostringstream oss;
    oss << MAGENTA "| " AQUA " WPM: " RESET << setw(3) << wpm
        << " | " AQUA "Time: " RESET << finalTime << string(8, ' ') << MAGENTA "|";
    return oss.str();
}

ostream& operator<<(ostream& os, const TypingScore& s)
{
    os << static_cast<string>(s) << "\n"; //now using type cast in TypingScore class
    return os;
}

// TypingPlayer SECTION

void TypingPlayer::addScore(const int wpm)
{
    scores.emplace_back(wpm); //constructor adds timestamp automagically
}

void TypingPlayer::addScore(const TypingScore& score) { //overloaded function`
    scores.emplace_back(score.getWPM());
}

vector<TypingScore> TypingPlayer::getScores() const
{
    return scores;
}

ostream& operator<<(ostream& os, const TypingPlayer& p)
{
    os << MAGENTA "| " BOLD "Username: " RESET << p.getUser()->getName() << string(35 - p.getUser()->getName().length(), ' ') << MAGENTA "|\n";
    os << MAGENTA "|----------------------------------------------|\n";
    os << MAGENTA "| " BOLD "Scores: " << string(37, ' ') << RESET MAGENTA "|\n";

    for (const auto& score : p.scores)
    {
        os << score;
    }

    os << MAGENTA "+----------------------------------------------+\n";
    os << MAGENTA "| " BOLD "Highest Score: " RESET << setw(3) << ScoreAnalyzer<TypingScore>::getHighestScore(p.scores) << string(27, ' ') << MAGENTA "|\n";
    return os;
}

// WORDBANK SECTION

void WordBank::setWords(const vector<string>& w)
{
    words = w;
}

string WordBank::getRandomWord()
{
    return words[rand() % words.size()];
}

void WordBank::initialize()
{
    srand(time(nullptr));
}

void WordBank::loadWords()
{
    ifstream file("TypingGame/wordBank.json");
    if (!file.is_open()) {
        throw runtime_error("Word file not found: " + string("wordBank.json"));
    }

    json j;
    file >> j;

    words = j.get<vector<string>>();

    try { // TRY CATCH
        if (words.empty()) { //THROW
            throw runtime_error("Word array is empty! Defaulting..\n");
        }
    }catch (runtime_error& e) {
        cout << e.what();
        words = {"mountain", "guitar", "computer", "mouse", "laptop", "cat", "book"};
        return;
    }

    cout << GREEN << "Loaded " << words.size() << " words\n" << RESET;
}


// TYPINGGAME SECTION

void TypingGame::gameMenu(User& user)
{
    loadFromJson();
    currentTypingPlayer = findOrCreateTypingPlayer(user.getName());

    WordBank::getInstance().loadWords();

    // MENU SYSTEM
    int choice = 0;
    do
    {
        cout << CLRSCR;
        cout << CYAN "+======================================+\n";
        cout << CYAN "| " BOLD "       Welcome to TypingGame!        " RESET CYAN "|\n";
        cout << CYAN "+======================================+\n";
        cout << CYAN "| " BOLD GREEN "1. " RESET "Start Game\t\t\t\t\b" CYAN "|\n";
        cout << CYAN "| " BOLD GREEN "2. " RESET "Show Leaderboard\t\t\t\b" CYAN "|\n";
        cout << CYAN "| " BOLD GREEN "3. " RESET "Show Results\t\t\t\b" CYAN "|\n";
        cout << CYAN "| " BOLD GREEN "4. " RESET "Exit\t\t\t\t\b" CYAN "|\n";
        cout << CYAN "+--------------------------------------+\n";
        cout << CYAN ">> " YELLOW BOLD "Enter your choice: " RESET;
        cin >> choice;

        if (cin.fail()) {
            choice = -1;
        }

        switch(choice)
        {
        case 1:
            startGame();
            break;
        case 2:
            showLeaderboard();
            break;
        case 3:
            showResults(*currentTypingPlayer);
            break;
        case 4:
            saveToJson();
            return;
        default:
            cout << BOLD RED "INVALID INPUT! Please try again.\n" RESET;
            pressKeyToReturn();
            break;
        }
    }while(choice != 4);
}

void TypingGame::startGame() const
{
    cin.ignore();
    cin.clear();
    cout << CLRSCR;
    const chrono::system_clock::time_point startTime = chrono::system_clock::now();
    const chrono::system_clock::time_point endTime = startTime + chrono::seconds(GAME_DURATION+2); //2s to let player adjust

    WordBank::initialize();
    string currentWord = WordBank::getInstance().getRandomWord();
    string nextWord = WordBank::getInstance().getRandomWord();

    int correctWords = 0;
    while(chrono::system_clock::now() < endTime)
    {
        cout << CLRSCR;
        const int remainingTime = chrono::duration_cast<chrono::seconds>(endTime - chrono::system_clock::now()).count(); //wow, this is terrible.
        cout << BOLD YELLOW "Time Left: " RESET << remainingTime
             << " || " GREEN "Current Word: " RESET << currentWord
             << " || " CYAN "Upcoming Word: " RESET << nextWord
             << YELLOW BOLD " >> " RESET GREEN;

        if (chrono::system_clock::now() >= endTime)
            break; // TIME IS UP, STOP IMMEDIATELY.

        string in;
        getline(cin, in);

        if(in == currentWord)
            correctWords++;

        currentWord = nextWord;
        nextWord = WordBank::getInstance().getRandomWord();
    }
    const int wpm = (correctWords * 60) / GAME_DURATION; //30s

    cout << BOLD MAGENTA "\n--- GAME ENDED ---\n" RESET;
    cout << GREEN "Words correct: " RESET << correctWords << "\n";
    cout << BOLD CYAN "Your typing speed: " RESET << wpm << " WPM\n";

    currentTypingPlayer->getUser()->addCoins(2);
    currentTypingPlayer->getUser()->updateGlobalPoints(2);

    currentTypingPlayer->addScore(wpm); //also supports currentTypingPlayer->addScore(TypingScoreInstance)
    pressKeyToReturn();
}


void TypingGame::wipeGame()
{
    remove("TypingGame/TypingGame.json");
    players.clear();
}

void TypingGame::deletePlayer(User& user)
{
    loadFromJson();
    for (int i = 0; i < players.size(); ++i) {
        if (players[i].getUser()->getName() == user.getName()) {
            players.erase(players.begin() + i); //erase() works via index only.
            saveToJson();
            return;
        }
    }
}


TypingPlayer* TypingGame::findOrCreateTypingPlayer(const string& name)
{
    for(auto & player : players)
    {
        if(name == player.getUser()->getName())
        {
            return &player;
        }
    }
    //NOT FOUND. MAKE TypingPlayer!
    players.emplace_back(arcadeSystem.getUser(name)); //faster than push_back apparently.
    return &players.back(); //pointer to last in vector
}

void TypingGame::showResults(const TypingPlayer& typingPlayer)
{
    cout << CLRSCR;
    cout << MAGENTA "+==============================================+\n";
    cout << MAGENTA "| " BOLD "               PLAYER PROFILE                " RESET MAGENTA "|\n";
    cout << MAGENTA "+==============================================+\n";
    cout << typingPlayer;
    cout << MAGENTA "+==============================================+\n";
    pressKeyToReturn();
}

void TypingGame::showLeaderboard()
{
    rankedPlayers.clear();
    if(players.empty())
    {
        cout << RED BOLD << "ERROR: NO PLAYERS FOUND...\n";
        return;
    }

    vector<pair<int, TypingPlayer*>> sortableScores; //vector of a "map"
    for (auto& player : players) {
        sortableScores.emplace_back(
            ScoreAnalyzer<TypingScore>::getHighestScore(player.getScores()),
            &player
        );
    }

    //SORT ALL PLAYERS FIRST BASED ON THEIR HIGHEST SCORE.
    sort(sortableScores.begin(), sortableScores.end(), //built-in sorter function
    [](const auto& a, const auto& b) {
        return a.first > b.first;
    });


    int rank = 1; //insert into rankedPlayers map (now it is ordered)
    for (const auto& [score, player] : sortableScores) {
        rankedPlayers[rank++] = player;
    }

    cout << CLRSCR << BLUE "+======================================+\n"
         <<           BLUE "| " BOLD "            LEADERBOARD              " RESET BLUE "|\n"
         <<           BLUE "+======================================+\n";

    for (int i = 1; i <= rankedPlayers.size() && i <= 8; ++i) {
        auto* player = rankedPlayers.at(i);
        cout << BLUE "| " BOLD "# " << i << ". Player: " RESET << player->getUser()->getName()
             << string(24 - player->getUser()->getName().length(), ' ') //spaces
             << BLUE "|\n"
             << BLUE "| " GREEN "  Highest Score: " RESET << setw(3) << ScoreAnalyzer<TypingScore>::getHighestScore(player->getScores())
             << string(17, ' ') << BLUE "|\n"
             << BLUE "+--------------------------------------+\n";
    }

    int currentRank = -1;
    for (const auto& [rank, player] : rankedPlayers) {
        if (player == currentTypingPlayer) {
            currentRank = rank;
            break;
        }
    }

    if (currentRank > 0) {
        cout << BLUE "| " BOLD YELLOW "Your Position: " RESET << "#" << currentRank
             << " - WPM: " << setw(3)
             << ScoreAnalyzer<TypingScore>::getHighestScore(currentTypingPlayer->getScores())
             << "\t\t\b" BLUE "|\n";
    }
    cout << BLUE "+======================================+\n";
    pressKeyToReturn();
}


void TypingGame::saveToJson() const
{
    try {
        json j;
        for (const auto& player : players)
        {
            json TypingPlayer;
            to_json(TypingPlayer, player);
            j.push_back(TypingPlayer);
        }

        ofstream file("TypingGame/TypingGame.json");
        if (!file.is_open()) {
            throw runtime_error("Could not open file for writing");
        }

        file << j.dump(4);
    } catch (const exception& e) {
        cerr << "Error saving: " << e.what() << endl;
    }
}

void TypingGame::loadFromJson() {
    players.clear();
    json j;
    try {
        ifstream file("TypingGame/TypingGame.json");
        if (!file.is_open()) {
            throw runtime_error("Could not open TypingGame.json");
        }

        file >> j;

        for (auto& it : j)
        {
            TypingPlayer p;
            from_json(it, p); //it = current json object
            players.push_back(p);
        }
    } catch (const exception& e) {
        cerr << "Error loading game data: " << e.what() << endl;
    }
}

void from_json(const json& j, TypingPlayer& p) //j = current json object in iterator
{
    if (j.contains("username")) {
        const string username = j.at("username").get<string>();
        p.user = arcadeSystem.getUser(username);
    }

    if (j.contains("scores") && j["scores"].is_array()) {
        for (const auto& sJ : j["scores"]) {
            TypingScore score;
            from_json(sJ, score);
            p.scores.push_back(score);
        }
    }
}

void to_json(json& j, const TypingPlayer& p)
{
    j["username"] = p.user->getName();
    for(const auto& score : p.scores)
    {
        json scoreJson;
        to_json(scoreJson, score);
        j["scores"].push_back(scoreJson);
    }
}