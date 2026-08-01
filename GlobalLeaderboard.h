#ifndef GLOBALLEADERBOARD_H
#define GLOBALLEADERBOARD_H
#include "Users.h"
#include<iostream>
using namespace std;


//____CLASS FIVE______________________________
    
class GlobalLeaderBoard {
    private:
        User* users; // AGGREGATION
    public:
        // class func 
        vector<User> loadUsersFromJsonForGlobalLB(const string& filename);
        void bubbleSortForGlobalLeaderBoard(User* player, int size);
        void displayGlobalLeaderboard(User* player, int size, int limit);
    };


#endif //GLOBALLEADERBOARD_H
