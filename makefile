CXX = g++
CXXFLAGS = -std=c++20
SRC = main.cpp Arcade.cpp Users.cpp Admin.cpp Shop.cpp GlobalLeaderboard.cpp MainMenu.cpp \
TriviaTacToeGame/TicTacToe.cpp \
TypingGame/TypingGame.cpp \
RPG/RPGGame.cpp \
Hopscotch/Hopscotch_MainGame.cpp Hopscotch/HopscotchFunctions.cpp Hopscotch/HopscotchGame.cpp
OBJDIR = build
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
TARGET = r.out

all: run

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files from source files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)  # Ensure subdirectories exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the project after building
run: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	rm -rf $(OBJDIR) $(TARGET)
