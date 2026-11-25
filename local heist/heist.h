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
// helper functions
string lower(const string& s);
int split(const string& line, string parts[], int maxparts);
// template class can work with any data type so we do not need to use stl
template <class T>
class DynamicArray {
private:
    T* data;
    int used;           // how many items are stored
    int capacity;       // means the capacity mean how many item which can actually fir
    void copyfrom(const DynamicArray<T>& other) {
        used = other.used;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < used; i++)
            data[i] = other.data[i];
    }
public:
    DynamicArray() : used(0), capacity(4) { data = new T[capacity]; }
    DynamicArray(const DynamicArray<T>& other) { copyfrom(other); }
    //assignment operator will del the old first then make copy
    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data;
            copyfrom(other);
        }
        return *this;
    }
    ~DynamicArray() {
        delete[] data;
    }
    void add(const T& item) {
        if (used == capacity) {
            T* bigger = new T[capacity * 2];
            for (int i = 0; i < used; i++)
                bigger[i] = data[i];
            delete[] data;
            data = bigger;
            capacity = capacity * 2;
        }
        data[used] = item;
        used++;
    }
    T& operator[](int i) {
        if (i < 0 || i >= used) throw InvalidMoveException("index out of range");
        return data[i];
    }
    //in case obj is const
    const T& operator[](int i) const {
        if (i < 0 || i >= used) throw InvalidMoveException("index out of range");
        return data[i];
    }
    int getsize() const { return used; }
};
class TileRack {
private:
    DynamicArray<Clue> clues;
    int maxtiles;
public:
    TileRack(int maxt = 8) : maxtiles(maxt) {}
    void addclue(const Clue& c);
    void display() const;
};

// the board, row 1 has 7 slots and row 2 has 8 slots
class Scrabble {
private:
    DynamicArray<Tile> row1;
    DynamicArray<Tile> row2;
    string word1, word2;
    bool solved1, solved2;
public:
    Scrabble();
    void setwords(const string& w1, const string& w2);
    void place(const Clue& c);
    bool rowfull(WordRow r) const;
    bool checkword(WordRow r, const string& guess);
    bool issolved(WordRow r) const;
    bool bothsolved() const;
    string getword(WordRow r) const;
    // friend function will print private rows
    friend ostream& operator<<(ostream& os, const Scrabble& s);
};
//cottage occupy 7 locations
class Cottage {
private:
    DynamicArray<Location> locations;
public:
    void load(int culpritindex);
    int getcount() const { return locations.getsize(); }
    Location& get(int i) { return locations[i]; }
    void show() const;
};
class GameEngine;

// abstarct class to deal with pure virtual
class Character {
private:

    string name, role, nationality;
    Clue clue;                  // the clue of this character (word 2)
    bool culprit, interviewed;
public:
    Character(const string& n, const string& r, const string& nat)
        : name(n), role(r), nationality(nat), culprit(false), interviewed(false) {
        clue.row = word_2;
    }

    //we use virtual destructor cus when base pointer is destroyed the execution of derived destructor can be carried out
    virtual ~Character() {}
    virtual void display() const = 0;
    string getname() const { return name; }
    string getrole() const { return role; }
    string getnationality() const { return nationality; }
    Clue getclue() const { return clue; }
    void setclue(char letter, int position) {
        clue.letter = letter;
        clue.position = position;
    }
    bool isculprit() const { return culprit; }
    bool isinterviewed() const { return interviewed; }
    void setinterviewed() { interviewed = true; }

    friend class GameEngine;
};
#endif
