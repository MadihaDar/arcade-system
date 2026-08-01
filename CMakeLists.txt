cmake_minimum_required(VERSION 3.29)
project(PFPROJ1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(PFPROJ1 main.cpp
        Arcade.h
        Arcade.cpp
        main.cpp
        MainMenu.h
        MainMenu.cpp
        Admin.h
        Admin.cpp
        Users.h
        Users.cpp
        TypingGame/TypingGame.h
        TypingGame/TypingGame.cpp
        TriviaTacToeGame/TicTacToe.cpp
        TriviaTacToeGame/TicTacToe.h
        Hopscotch/HopscotchFunctions.cpp
        Hopscotch/HopscotchFunctions.h
        Hopscotch/Hopscotch_MainGame.cpp
        Hopscotch/HopscotchGame.cpp
        GlobalLeaderboard.cpp
        GlobalLeaderboard.h
        RPG/RPGGame.cpp
        RPG/RPGGame.h
        Shop.cpp
        Shop.h)
