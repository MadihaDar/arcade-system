#include "TicTacToe.h" 

// initialize the static member
TriviaGameCountManager* TriviaGameCountManager::instance = nullptr;

// function for initializing the grid
void TicTacToeBoard::initializeGrid() {
    char num = '1';
    // double for loop for 3x3 grid
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            grid[i][j] = num++;     // assign the number to each block
        }
    }
}

// function for displaying the grid
void TicTacToeBoard::displayGrid() {
    const string padding = "\t\t\t\t\t\t            ";  // for center alignment

    for (int i = 0; i < 3; i++) {
        cout << padding;    // add spacing before each line to center align
        for (int j = 0; j < 3; j++) {
            cout << " " << grid[i][j] << " ";
            if (j < 2) cout << "|";
        }
        cout << endl;

        if (i < 2) {
            cout << padding << "-----------" << endl;
        }
    }
}

json TriviaTacToe::readTriviaJsonFile(const string& filename) {
    ifstream file(filename);
    json data;
    if (file.is_open()) {
        file >> data;
        file.close();
    }
    return data;
}

// function for writing to trivia_scores file
void TriviaTacToe::writeTriviaJsonFile(const string& filename, const json& data) {
    ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    }
}

// function to load all the questions in the questions vector
void TriviaQuestions::loadQuestions() {
    questions.push_back(Question{
        "What is the capital of France?",
        {"\t\t\t\t\tA) Madrid", "B) Berlin", "C) Paris", "D) Rome"},
        'C'
    });

    questions.push_back(Question{
        "Which planet is known as the Red Planet?",
        {"\t\t\t\t\tA) Venus", "B) Mars", "C) Jupiter", "D) Saturn"},
        'B'
    });

    questions.push_back(Question{
        "Who wrote 'Hamlet'?",
        {"\t\t\t\t\tA) Charles Dickens", "B) Shakespeare", "C) Mark Twain", "D) Jane Austen"},
        'B'
    });

    questions.push_back(Question{
        "What is the square root of 64?",
        {"\t\t\t\t\tA) 6", "B) 7", "C) 8", "D) 9"},
        'C'
    });

    questions.push_back(Question{
        "Which element has the chemical symbol 'O'?",
        {"\t\t\t\t\tA) Oxygen", "B) Gold", "C) Silver", "D) Helium"},
        'A'
    });

    questions.push_back(Question{
        "Which movie features the quote “I’m the king of the world!”?",
        {"\t\t\t\t\tA) Titanic", "B) The Lion King", "C) Avatar", "D) Inception"},
        'A'
    });

    questions.push_back(Question{
        "What is the name of Harry Potter’s pet owl?",
        {"\t\t\t\t\tA) Crookshanks", "B) Fluffly", "C) Scabbers", "D) Hedwig"},
        'D'
    });

    questions.push_back(Question{
        "Which band recorded the song Bohemian Rhapsody?",
        {"\t\t\t\t\tA) The Beatles", "B)The Rolling Stones", "C) Queen", "D) Pink Floyd"},
        'C'
    });
}

bool TriviaQuestions::askQuestion(Question& q) {
    // get the first question
    q = questions.front();
    
    // remove it so it won't be asked again
    questions.erase(questions.begin());
    
    // display the question
    cout << "\n\t\t\t\t\t" BOLD BLUE << q.question << RESET << endl << endl;

    // display answer options
    for (int i = 0; i < 4; i++) {  
        cout << q.options[i] << "\t";
    }
    cout << endl;

    // get user's answer
    char userAnswer;

    // keep asking until valid input is given
    while (true) {
        cout << "\n\t\t\t\t\tEnter your answer (A, B, C, D): ";
        cin >> userAnswer;

        try {
            userAnswer = toupper(userAnswer);

            if (userAnswer != 'A' && userAnswer != 'B' && userAnswer != 'C' && userAnswer != 'D') {
                throw invalid_argument("Invalid input. Please enter A, B, C, or D.");
            }

            // if input is valid, break out of loop
            break;

        } catch (const invalid_argument& e) {
            cout << RED "\n\t\t\t\t\tError: " << e.what() << RESET << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // check if the answer is correct
    if (userAnswer == q.ans) {
        cout << GREEN "\n\t\t\t\t\tCorrect! You can play your turn." RESET << endl;
        return true;
    } else {
        cout << RED "\n\t\t\t\t\tWrong answer! You lose your turn." RESET << endl;
        return false;
    }
}

// function to check if the player or computer wins
bool TicTacToeBoard::checkWin(char player) {
    for (int i = 0; i < 3; i++) {
        if ((grid[i][0] == player && grid[i][1] == player && grid[i][2] == player) ||
            (grid[0][i] == player && grid[1][i] == player && grid[2][i] == player)) {
            return true;
        }
    }
    return ((grid[0][0] == player && grid[1][1] == player && grid[2][2] == player) ||
            (grid[2][0] == player && grid[1][1] == player && grid[0][2] == player));

}

void TriviaTacToe::wipeGame() {
    remove("TriviaTacToeGame/trivia_scores.json");
    cout << "\n\t\t\tTriviaTacToe File deleted successfully!" << endl;
}

// function to display leaderboard
void TriviaTacToe::displayTriviaLeaderboard(const string& filename) {
    json data = readTriviaJsonFile(filename);

    if (data.empty() || !data.contains("TriviaTacToe")) {
        cout << "\n\t\t\t\t\tNo data found in the file!\n";
        return;
    }

    // map to collect usernames and scores
    map<string, int> userScoreMap;
    for (auto& [username, scoresData] : data["TriviaTacToe"].items()) {
        userScoreMap[username] = scoresData["userScore"];
    }

    int size = userScoreMap.size();
    string usernames[size];
    int scores[size];

    // populate arrays from map
    int index = 0;
    for (const auto& [username, score] : userScoreMap) {
        usernames[index] = username;
        scores[index] = score;
        index++;
    }

    // using template class to sort scores
    SortTriviaScoresArray<int, string>::sortArray(scores, usernames, size);

    cout << CLRSCR;
    // display sorted leaderboard
    cout << BOLD PINK "\n\t\t\t\t\t\t  === TRIVIA-TAC-TOE LEADERBOARD ===" << RESET << endl;
    cout << MAGENTA "\n\t\t\t\t\t\t      Rank  " BOLD PINK "|" RESET MAGENTA " Username  " RESET BOLD PINK "|" RESET MAGENTA " Score" RESET;
    cout << BOLD PINK "\n\t\t\t\t\t\t      -------------------------" RESET;
    for (int i = 0; i < size; i++) {
        cout << "\n\t\t\t\t\t\t      " BOLD MAGENTA << i + 1 << RESET BOLD PINK "     | " RESET << usernames[i] << RESET BOLD PINK "   | " RESET << scores[i] << endl;
    }
}

// function to update the user's score automatically
void TriviaTacToe::updateTriviaScore(int userPoints, int compPoints) {
    json data = readTriviaJsonFile("TriviaTacToeGame/trivia_scores.json");

    string userID = player->getUserID(); // get current player's ID

    // if user exists, add points to their existing score, else create a new entry
    if (data["TriviaTacToe"].contains(userID)) {
        data["TriviaTacToe"][userID]["userScore"] = data["TriviaTacToe"][userID]["userScore"].get<int>() + userPoints;
        data["TriviaTacToe"][userID]["compScore"] = data["TriviaTacToe"][userID]["compScore"].get<int>() + compPoints;
    }
    else {
        data["TriviaTacToe"][userID] = {
            {"userScore", userPoints},
            {"compScore", compPoints}
        };
    }

    // save updated scores
    writeTriviaJsonFile("TriviaTacToeGame/trivia_scores.json", data);
    cout << "\n\t\t\t\t\t\t   Scores updated successfully!\n";

    // update global points using Player's method
    player->updateGlobalPoints(player->getScore());
}

// overloaded function (updates current player)
void TriviaTacToe::updateTriviaScore() {
    if (player == nullptr) {
        cout << "\n\t\t\t\t\tNo active player. Cannot update score." << endl;
        return;
    }

    updateTriviaScore(player->getScore(), compScore);
}

void TriviaTacToe::viewTotalTriviaScore(User& user) {
    ifstream file("TriviaTacToeGame/trivia_scores.json");
    json scores;

    if (file) {
        file >> scores;
        file.close();
    } else {
        cout << "\n\t\t\t\t\tError: Could not open scores file!\n";
        return;
    }

    string username = user.getName();

    // check if the username exists in the scores
    if (scores.contains("TriviaTacToe") && scores["TriviaTacToe"].contains(username)) {
        cout << CLRSCR;
        cout << BABY_BLUE "\n\t\t\t\t\t\t      ==== YOUR TOTAL SCORE ====" << RESET << endl;
        cout << BOLD MAGENTA "\n\t\t\t\t\t\t      Username: " RESET << username << endl;
        cout << BOLD MAGENTA "\n\t\t\t\t\t\t      User Points: " RESET << scores["TriviaTacToe"][username]["userScore"] << endl;
        cout << BOLD MAGENTA "\n\t\t\t\t\t\t      Computer Points: " RESET << scores["TriviaTacToe"][username]["compScore"] << endl;
        cout << BABY_BLUE "\n\t\t\t\t\t\t      ==========================" RESET << endl;
    }
}

// function for the trivia game instructions
void TriviaTacToe::viewTriviaInstructions() {
    cout << CLRSCR;
    cout << BOLD YELLOW "\n\t\t\t\t\t=========    " RESET YELLOW "TRVIA-TAC-TOE INSTRUCTIONS" RESET BOLD YELLOW "   =========" RESET << endl << endl;

    cout << "\t\t\t\t\tHere's how it works: " << endl;
    cout << BOLD YELLOW "\n\t\t\t\t\t---------------------------------------------------" RESET << endl;
    cout << "\n\t\t\t\t\t" YELLOW "1. " RESET BOLD PEACH "A tic-tac-toe grid will appear." RESET << endl;
    cout << "\t\t\t\t\t" YELLOW "2. " RESET BOLD PEACH"You'll face some MCQs. Answer them right and " << endl;
    cout << "\t\t\t\t\t   earn points." RESET << endl;
    cout << "\t\t\t\t\t" YELLOW "3. " RESET BOLD PEACH "Correct answers let you place 'X' on the grid." RESET << endl;
    cout << "\t\t\t\t\t" YELLOW "4. " RESET BOLD PEACH " Wrong answers? Your turn gets skipped. Oops!" RESET << endl;
    cout << "\t\t\t\t\t" YELLOW "5. " RESET BOLD PEACH "Keep playing until the grid gets full, and we'll " << endl;
    cout << "\t\t\t\t\t   reveal your score!" RESET << endl;
    cout << endl;
    cout << "\n\t\t\t\t\tLet's dive in! Shall we?" << endl;
    cout << BOLD YELLOW "\t\t\t\t\t===================================================" RESET << endl;
    cout << endl;
}

// Function which takes care of the trivia game menu
void TriviaTacToe::gameMenu(User& user) {
    int choice;
    cout << CLRSCR;

    do {
        cout << BOLD CYAN BLINK "\n\t\t\t\t\t====================================================" RESET << endl;
        cout << BOLD YELLOW BLINK "\t\t\t\t\t           WELCOME TO TRIVIA-TAC-TOE GAME           " RESET << endl;
        cout << BOLD CYAN BLINK "\t\t\t\t\t====================================================" RESET << endl;
        cout << BOLD CYAN BLINK "\n\t\t\t\t\t\t1." RESET CYAN " View Trivia-Tac-Toe Instructions" RESET << endl;
        cout << BOLD CYAN BLINK "\t\t\t\t\t\t2." RESET CYAN " Start the Game" RESET << endl;
        cout << BOLD CYAN BLINK "\t\t\t\t\t\t3." RESET CYAN " Display Trivia-Tac-Toe Leaderboard" RESET << endl;
        cout << BOLD CYAN BLINK "\t\t\t\t\t\t4." RESET CYAN " View Your Total Trivia-Tac-Toe Score" RESET << endl;
        cout << BOLD CYAN BLINK "\t\t\t\t\t\t0." RESET CYAN " Exit" RESET << endl;

        while (true) {
            cout << BOLD YELLOW "\n\t\t\t\t\t\tEnter your choice: " RESET;
            cin >> choice;

            if (cin.fail()) {
                cout << RED "\n\t\t\t\t\t\tInvalid input! Please enter a number (0-4)." RESET << endl;
                cin.clear();  
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  
                break; 
            }
        }

        cout << endl;

        switch (choice) {
            case 1:
                viewTriviaInstructions();
                break;

            case 2: {
                char playAgain;
                do {
                    playTriviaGame(user);
                    cout << BOLD BLUE "\n\t\t\t\t\t\tDo you want to play again? (y/n): " RESET;
                    cin >> playAgain;
                    playAgain = tolower(playAgain);
                } while (playAgain == 'y');
                break;
            }

            case 3:
                displayTriviaLeaderboard("TriviaTacToeGame/trivia_scores.json");
                break;

            case 4:
                viewTotalTriviaScore(user);
                break;

            case 0:
                cout << BOLD PINK "\n\t\t\t\t\t    Thank you for playing TriviaTacToe! Goodbye." << RESET << endl;
                break;

            default:
                cout << YELLOW "\n\t\t\t\t\t\tInvalid choice! Please try again." << RESET << endl;
        }

    } while (choice != 0);
}

void TriviaTacToe::playTriviaGame(User& user) {
    cout << CLRSCR;

    // resetting the scores for every new game
    userScore = 0;   
    compScore = 0;

    TriviaGameCountManager::getInstance()->incrementGameCount();
    cout << BOLD PINK "\n\t\t\t\t\t\t\tTotal Games Played: " << TriviaGameCountManager::getInstance()->getGameCount() << RESET << endl;
    cout << BOLD PINK "\t\t\t\t\t\t\t----------------------" RESET << endl << endl;

    player = new TriviaPlayer(user.getName(), &user, 0); // Pass the User object's address
    int playerScore;    // convert player to score

    tictactoe.initializeGrid();
    trivia.loadQuestions();

    tictactoe.displayGrid();
    srand(time(0));

    for (int turn = 0; turn < 9; turn++) {
        if (turn % 2 == 0) {
            cout << "\n\t\t\t\t\t\t\t  ----------------\n";
            cout << "\t\t\t\t\t\t\t  |" RED BLINK " User's Turn! " RESET "|\n";
            cout << "\t\t\t\t\t\t\t  ----------------\n";

            // Access the private vector<Question> of TriviaQuestions (friend class making private vector<Question> accessible)
            Question randomQ = trivia.questions[rand() % trivia.questions.size()];  

            if (!trivia.askQuestion(randomQ)) {
                continue; // Skip directly to the next iteration
            }
                
            int position;
            while (true) {
                cout << "\n\t\t\t\t\tEnter the position for your move (1-9): ";
                cin >> position;
                cout << endl;

                // Calculate row and column positions based on the selected position
                int row = (position - 1) / 3;
                int col = (position - 1) % 3;

                // If the chosen cell is not already taken, allow the user to place 'X'
                if (tictactoe.getGridValue(row, col) != 'X' && tictactoe.getGridValue(row, col) != 'O') {
                    tictactoe.setGridValue(row, col, 'X');
                    player->setScore(player->getScore() + 1);
                    playerScore = static_cast<int>(*player); 
                    break;
                }
              else {
                    cout << YELLOW "\n\t\t\t\t\tPosition already taken!" RESET << endl;
                }
            }
                // Check if the user has already won
                if (tictactoe.checkWin('X')) {
                    cout << endl;
                    tictactoe.displayGrid();
                    cout << "\n\t\t\t\t\t\t\t     " BOLD CYAN UNDERLINE "USER WINS!" RESET << endl << endl;
                    player->addCoins(3);
                    updateTriviaScore(playerScore, compScore);
                    cout << *this << endl;
                    return;
                }
            }
         else {
            cout << "\n\t\t\t\t\t\t\t--------------------\n";
            cout << "\t\t\t\t\t\t\t|" MAGENTA BLINK " Computer's Turn! " RESET "|\n";
            cout << "\t\t\t\t\t\t\t--------------------\n\n";
            
            int position;
            do {
                // Randomly select a position for the computer's move
                position = rand() % 9 + 1;

                // Calculate row and column positions
                int row = (position - 1) / 3;
                int col = (position - 1) % 3;

                // If the chosen cell is not already taken, allow the computer to place 'O'
                if (tictactoe.getGridValue(row, col) != 'X' && tictactoe.getGridValue(row, col) != 'O') {
                    tictactoe.setGridValue(row, col, 'O');
                    compScore++;

                    // Display the grid after user and computer have taken their turns
                    tictactoe.displayGrid();

                    // Check if the computer has won
                    if (tictactoe.checkWin('O')) {
                        cout << "\n\t\t\t\t\t\t\t " BOLD MAGENTA UNDERLINE "COMPUTER WINS!" RESET << endl << endl;
                        updateTriviaScore(player->getScore(), compScore);
                        cout << *this << endl;
                        return;
                    } 
                break;
            }
        } while(true);
    } 
  }
    cout << endl;
    tictactoe.displayGrid();
    cout << "\n\t\t\t\t\t\t\t   " YELLOW UNDERLINE "IT'S A TIE!" RESET << endl << endl;
    updateTriviaScore();
    cout << *this << endl;
}

// Function to delete a trivia player (will be used by Admin)
void TriviaTacToe::deletePlayer(User& user) {
    json js;
    string filename = "TriviaTacToeGame/trivia_scores.json";

    ifstream inFile(filename);
    if (inFile) {
        inFile >> js;
        inFile.close();

        string username = user.getName();
        json data = js["TriviaTacToe"];
        if (data.contains(username)) {
            data.erase(username);   // Delete the user
            ofstream outFile(filename);
            outFile << data.dump(4);
            outFile.close();
            cout << "\n\t\t\t\t\tUser data removed." << endl;
        } else {
            cout << "\n\t\t\t\t\tNo data found for this user." << endl;
        }
    } else {
        cout << "\n\t\t\t\tFailed to open trivia_scores file." << endl;
    }
}