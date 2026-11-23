#ifndef heist_h
#define heist_h
#include <iostream>
#include <string>
using namespace std;
// named constants, easier to read than plain numbers
enum GameState { state_menu, state_playing, state_won, state_lost, state_exit };
enum WordRow { word_1 = 1, word_2 = 2 };

// base class of all our exceptions, every message starts with heist error
class GameException {
private:
    string message;
public:
    GameException(const string& msg) { message = "heist error: " + msg; }
    virtual ~GameException() {}
    string what() const { return message; }
};

// each exception class is for one type of problem
class InvalidInputException : public GameException {
public:
    InvalidInputException(const string& m) : GameException(m) {}
};

class InvalidMoveException : public GameException {
public:
    InvalidMoveException(const string& m) : GameException(m) {}
};

class RackFullException : public GameException {
public:
    RackFullException(const string& m) : GameException(m) {}
};

class FileException : public GameException {
public:
    FileException(const string& m) : GameException(m) {}
};


struct Clue {
    char letter;
    int position;       // starts from 1
    WordRow row;
    Clue() : letter(' '), position(0), row(word_1) {}
};

struct Tile {
    char letter;
    bool filled;
    Tile() : letter(' '), filled(false) {}

    // two tiles are equal when their letters are equal
    bool operator==(const Tile& other) const { return letter == other.letter; }
};

struct Location {
    string name;
    Clue clue;          // every location has its own clue
    bool searched;
    Location() : searched(false) {}
};
#endif
