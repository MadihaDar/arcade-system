#include"HopscotchFunctions.h"
using namespace std;

//_________________________________________________________________________________________

// definition of the operator overload freind func
ostream& operator<<(ostream& os, const HopscotchPlayer& player) {
    os << "\t\t\tPlayer ID: " << player.getID() << " | Score: " << static_cast<int>(player) << " | Total Moves: " << player.getTotalMoves();
    return os;
}

//_________________________________________________________________________________________

void GameBoard::initializeBoard() {
    // initialize board with cells
    for (int i = 0; i < boardSize; i++) {
        board.at(i) = i + 1;
    }

    // initialize random seed for random stone pos
    srand(time(0));

    // set player starting position
    playerPosition = 1;

    // set stone position (NOT at start or end, and not at player position)
    do {
        stonePosition = 1 + rand() % (boardSize - 2);
    } while (stonePosition == playerPosition);

    // set move limit at a time 
    maxMoves = boardSize / 4;
}


//_________________________________________________________________________________________


    // display game board to play
    void GameBoard:: displayBoard(int playerPos, int stonePos, int row = 1, int count = 0)const {
        // if count (which is used for tracking) exceeds the board size stop recursion
        if (count >= boardSize) return;

        // format with a new line
        cout << endl;

        // for odd rows
        if (row % 2 == 1) {
            cout << "\t\t\t";

            // check for count validity
            if (count < boardSize) {
              // display board entities
                if (stonePos == count + 1) {
                    cout << RED << "\t\t\t[ S ]" << RESET;
                }
                else if (playerPos == count + 1) {
                    cout << GREEN << "\t\t\t[ P ]" << RESET;
                }
                else {
                    cout << "\t\t\t[ " << count + 1 << " ]";
                }
                // update count
                count++;
            }
        }
        // for even rows - display two columns
        else {
          // check for count validity
            if (count < boardSize) {
              // display board entities
                if (stonePos == count + 1) {
                    cout << RED << "\t\t\t[ S ]" << RESET;
                }
                else if (playerPos == count + 1) {
                    cout << GREEN << "\t\t\t[ P ]" << RESET;
                }
                else {
                    cout << "\t\t\t[ " << count + 1 << " ]";
                }
                // update count
                count++;
            }

            // check for count validity
            if (count < boardSize) {
                // display board entities
                if (stonePos == count + 1) {
                    cout << RED << "\t\t\t\t\t\t[ S ]" << RESET;
                }
                else if (playerPos == count + 1) {
                    cout << GREEN << "\t\t\t\t\t\t[ P ]" << RESET;
                }
                else {
                    cout <<  "\t\t\t\t\t\t[ " << count + 1 << " ]";
                }
                // update count
                count++;
            }
        }
        // format
        cout << endl << endl;
        // recursion
        displayBoard(playerPos, stonePos, row + 1, count);
    };

 //_________________________________________________________________________________________


// play hopscotch game function
void PlayHopscotchGame::playHopscotchGame(User& user, HopscotchPlayer& player){
    int size;
    cout << "\t\t\tEnter board size (must be between 11 and 99): ";
    cin >> size;

    while (size <= 10 || size >= 100) {
        cout << "\t\t\tInvalid size! Please enter a number between 11 and 99: ";
        cin >> size;
    }

    // setter to resize board
    board.setBoardSize(size); 

    // initialize board 
    board.initializeBoard();  

    // assigning values
    int stonePosition = board.getStonePosition();
    int maxMoves = board.getMaxMoves();
    int playerPosition = board.getPlayerPosition();

    // display stone position as a precaution
    cout << "\t\t\tHey " << user.getName() << "!\n";
    cout << "\t\t\tGame Initialized. Stone is at position: " << stonePosition << endl;

    // display game board
    board.displayBoard(playerPosition, stonePosition);

    // game movements start from here
    while (true) {

        // next move input
        int nextMove;
        cout << "\n\t\t\tEnter number of moves to jump: ";
        cin >> nextMove;

        // check if next move is in bounds
        while (nextMove < 1 || nextMove > maxMoves) {
            cout << "\n\t\t\tInvalid Move! Enter between 1 and " << maxMoves << ": ";
            cin >> nextMove;

            // handle scores on the basis of next move updates
            player.decreaseScore(2);
        }

        // update the the position of the
        int newPosition = playerPosition + nextMove;

        // check for the limit of the next move (must be in the bounds of the board)
        while (newPosition < 1 || newPosition > board.getBoardSize()) {
            cout << "\n\t\t\tInvalid Move! Try again: ";
            cin >> nextMove;

            // handle scores on the basis of next move updates
            player.decreaseScore(3);
            newPosition = playerPosition + nextMove;
        }

        // update player position on the board once the move is valid
        playerPosition = newPosition;

        // update move count
        player.incrementMoves();

        // display board with new positions
        board.displayBoard(playerPosition,stonePosition);

        // game finalization checks
        if (playerPosition == stonePosition) {
            cout << "\n\t\t\tOops! You landed on the stone. Game Over.\n";
            player.decreaseScore(4);
            break;
        }

        // win condition
        else if (playerPosition == board.getBoardSize()) {
            cout << "\n\t\t\tCongratulations! You won!\n";
            if(player.getScore() > 0 && player.getScore() <= 5) {
                player.increaseScore(3);
                user.updateGlobalPoints(3);
            }
            else {
                player.increaseScore(2);
                user.updateGlobalPoints(2);
            }
            break;
        }
    }

    // save hopscotch scores
    ScoreManager* scoreMgr = ScoreManager::getInstance();
    scoreMgr->saveHopscotchScoresToJson();  

    // display score after game is over
    cout << player;
}


 //_________________________________________________________________________________________