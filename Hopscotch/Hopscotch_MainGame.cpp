#include "HopscotchFunctions.h"
#include "../GlobalLeaderboard.h"

// getting the userCount intialized and declared in Users.h
extern int userCount;
// intializing the instance (from singleton class)
ScoreManager* ScoreManager::instance = nullptr;

void PlayHopscotchGame::gameMenu(User& currentUser) {

    // using singleton instance
    ScoreManager* scoreMgr = ScoreManager::getInstance();
    // creating a global leaderbaord obj
    GlobalLeaderBoard globalLB;

    HopscotchPlayer hopPlayer;
    hopPlayer.setID(&currentUser);

    scoreMgr->readHopscotchScoresFromJson("Hopscotch/hopscotch_scores.json");

    int choice;
    do {
        cout << "\n\t\t\t\t--- Hopscotch Menu ---";
        cout << "\n\t\t\t1. View Rules";
        cout << "\n\t\t\t2. Play Game";
        cout << "\n\t\t\t3. View Leaderboard";
        cout << "\n\t\t\t4. Search My Hopscotch Score";
        cout << "\n\t\t\t5. Compare Scores";
        cout << "\n\t\t\t6. Go Back to Main Menu";
        cout << "\n\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
                case 1: {
                    hopPlayer.displayHopscotchRules(currentUser);
                    break;
                }

                case 2: {
                    PlayHopscotchGame* hopscotchGame = new PlayHopscotchGame();
                    hopscotchGame->playHopscotchGame(currentUser, hopPlayer);
                
                    HopscotchPlayer* existingPlayer = nullptr;
                    for (int i = 0; i < scoreMgr->getPlayers().size(); ++i) {
                        if (scoreMgr->getPlayers()[i].getID() == currentUser.getName()) {
                            existingPlayer = &scoreMgr->getPlayers()[i];
                            break;
                        }
                    }
                
                    if (existingPlayer) {
                        existingPlayer->setScore(hopPlayer.getScore());
                        existingPlayer->setTotalMoves(hopPlayer.getTotalMoves());
                    } else {
                        scoreMgr->getPlayers().push_back(hopPlayer);
                    }
                
                    scoreMgr->saveHopscotchScoresToJson();

                    delete hopscotchGame;
                    break;
                }
                
                case 3: {
                    vector<HopscotchPlayer> hopPlayers = scoreMgr->getPlayers();
                    int playerCount = hopPlayers.size();
    
                    // bubble sort and display
                    scoreMgr->bubbleSortForHopscotchLeaderboard(hopPlayers.data(), playerCount); 
                    scoreMgr->displayHopscotchLeaderboard(hopPlayers.data(), playerCount, 8);
    
                    break;
                }    


                case 4: {
                    string id;
                    // get an id from user for search
                    cout << "\n\t\t\tEnter your User ID to search your score: ";
                    cin >> id;
                    scoreMgr->searchScores(id);
                    break;
                }


                case 5: {
                    // create a map for players 
                    map<string, HopscotchPlayer> allPlayers;
                    for (const auto& player : scoreMgr->getPlayers()) {
                        allPlayers[player.getID()] = player;
                    }

                    // implement template
                    ScoreComparer<HopscotchPlayer> comparer;
                    // call compare function
                    comparer.compareWithAnotherPlayer(hopPlayer, allPlayers);
                    break;
                }


                case 6:
                cout << "\n\t\t\tReturning to main menu...\n";
                return;

            default:
                cout << "\n\t\t\tInvalid choice. Try again.\n";
        }


    } while (choice != 7);
}