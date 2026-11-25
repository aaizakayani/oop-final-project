#include "heist.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
const string roles[7] = { "cook", "guard", "servant", "partner_1", "partner_2", "pa", "child" };
//helper functions 
string lower(const string& s) {
    string result = s;
    for (int i = 0; i < (int)result.length(); i++)
        result[i] = (char)tolower((unsigned char)result[i]);
    return result;
}
int split(const string& line, string parts[], int maxparts) {
    stringstream ss(line);
    string piece;
    int n = 0;
    while (n < maxparts && getline(ss, piece, '|'))
        parts[n++] = piece;
    return n;
}
void TileRack::addclue(const Clue& c) {
    if (clues.getsize() >= maxtiles)
        throw RackFullException("the rack is full");
    clues.add(c);
}
void TileRack::display() const {
    if (clues.getsize() == 0) {
        cout << "(empty)";
        return;
    }
    for (int i = 0; i < clues.getsize(); i++)
        cout << clues[i].letter << "@" << clues[i].position << " ";
}
void Detective::showracks() const {
    cout << "  rack (word 1): ";
    rack1.display();
    cout << "\n";
    cout << "  rack (word 2): ";
    rack2.display();
    cout << "\n";
}
Scrabble::Scrabble() : solved1(false), solved2(false) {
    Tile empty;
    for (int i = 0; i < 7; i++) row1.add(empty);
    for (int i = 0; i < 8; i++) row2.add(empty);
}

void Scrabble::setwords(const string& w1, const string& w2) {
    if (w1.length() != 7 || w2.length() != 8)
        throw FileException("clue.txt words must have 7 and 8 letters");
    word1 = lower(w1);
    word2 = lower(w2);
    solved1 = false;
    solved2 = false;
}
// position starts from 1 but array start from 0
void Scrabble::place(const Clue& c) {
    DynamicArray<Tile>& row = (c.row == word_1) ? row1 : row2;
    int index = c.position - 1;
    if (index < 0 || index >= row.getsize())
        throw InvalidMoveException("that board position does not exist");
    if (row[index].filled)
        throw InvalidMoveException("that board slot already has a letter");
    row[index].letter = c.letter;
    row[index].filled = true;
}
bool Scrabble::rowfull(WordRow r) const {
    const DynamicArray<Tile>& row = (r == word_1) ? row1 : row2;
    for (int i = 0; i < row.getsize(); i++)
        if (!row[i].filled) return false;
    return true;
}
bool Scrabble::checkword(WordRow r, const string& guess) {
    const string& answer = (r == word_1) ? word1 : word2;
    if (guess.length() != answer.length()) return false;
    for (int i = 0; i < (int)answer.length(); i++) {
        Tile a, b;
        a.letter = guess[i];
        b.letter = answer[i];
        if (!(a == b)) return false;
    }
    if (r == word_1) solved1 = true;
    else solved2 = true;
    return true;
}
bool Scrabble::issolved(WordRow r) const { return (r == word_1) ? solved1 : solved2; }
bool Scrabble::bothsolved() const { return solved1 && solved2; }
string Scrabble::getword(WordRow r) const { return (r == word_1) ? word1 : word2; }
ostream& operator<<(ostream& os, const Scrabble& s) {
    os << "  word 1: ";
    for (int i = 0; i < s.row1.getsize(); i++)
        os << "[" << (s.row1[i].filled ? s.row1[i].letter : ' ') << "] ";
    os << "\n  word 2: ";
    for (int i = 0; i < s.row2.getsize(); i++)
        os << "[" << (s.row2[i].filled ? s.row2[i].letter : ' ') << "] ";
    os << "\n";
    return os;
}
// the culprit decides which column gives the letters
void Cottage::load(int culpritindex) {
    ifstream file("locations.txt");
    if (!file) throw FileException("locations.txt was not found");

    locations = DynamicArray<Location>();
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line == "") continue;
        string p[9];
        if (split(line, p, 9) < 9)
            throw FileException("locations.txt is not correct");
        Location loc;
        loc.name = lower(p[0]);
        loc.clue.position = atoi(p[1].c_str());
        loc.clue.letter = lower(p[2 + culpritindex])[0];
        loc.clue.row = word_1;
        locations.add(loc);
    }
    if (locations.getsize() != 7)
        throw FileException("locations.txt must have 7 locations");
}
void Cottage::show() const {
    cout << "\n--- locations ---\n";
    for (int i = 0; i < locations.getsize(); i++)
        cout << " [" << i + 1 << "] " << locations[i].name
        << (locations[i].searched ? "  (searched)" : "  (not searched)") << "\n";
}
void ScoreBoard::add(const string& name, const string& result, int credibility, int turnsleft, const string& culprit) const {
    ofstream file("leader/scoreBoard.txt", ios::app);     // app adds at the end of the file
    if (!file) throw FileException("could not open leader/scoreBoard.txt");
    file << name << "|" << result << "|" << credibility << "|" << turnsleft << "|" << culprit << "\n";
}
void ScoreBoard::show() const {
    ifstream file("leader/scoreBoard.txt");
    if (!file) throw FileException("leader/scoreBoard.txt was not found");
    string line;
    getline(file, line);
    cout << "\n--- score board (name | result | credibility | turns left | culprit) ---\n";
    int rows = 0;
    while (getline(file, line)) {
        if (line == "") continue;
        cout << "  " << line << "\n";
        rows++;
    }
    if (rows == 0) cout << "  no games played yet.\n";
}
GameEngine* GameEngine::instance = 0;
GameEngine::GameEngine()
    : state(state_menu), charcount(0), maxturns(25), startcredibility(100), steppenalty(5), wordpenalty(10) {
    for (int i = 0; i < 8; i++) chars[i] = 0;
    srand((unsigned)time(0));
}

GameEngine::~GameEngine() { clearchars(); }

// makes the object only the first time, after that it returns the same one
GameEngine* GameEngine::getinstance() {
    if (instance == 0) instance = new GameEngine();
    return instance;
}
void GameEngine::destroyinstance() {
    delete instance;
    instance = 0;
}
void GameEngine::clearchars() {
    for (int i = 0; i < 8; i++) {
        delete chars[i];                    // virtual destructor deletes the right object
        chars[i] = 0;
    }
    charcount = 0;
}
string GameEngine::input(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) {
        state = state_exit;
        throw InvalidInputException("input was closed");
    }
    return s;
}

// atoi gives 0 for text, so wrong text is also caught by the range check
int GameEngine::readnumber(const string& prompt, int lo, int hi) {
    int n = atoi(input(prompt).c_str());
    if (n < lo || n > hi) {
        stringstream ss;
        ss << "please type a number from " << lo << " to " << hi;
        throw InvalidInputException(ss.str());
    }
    return n;
}
// if rules.txt is missing the default rules are used
void GameEngine::loadrules() {
    maxturns = 25;
    startcredibility = 100;
    steppenalty = 5;
    wordpenalty = 10;
    ifstream file("rules.txt");
    if (!file) {
        cout << "rules.txt was not found, default rules are used\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        int eq = (int)line.find('=');
        if (eq == (int)string::npos) continue;
        string key = lower(line.substr(0, eq));
        int value = atoi(line.substr(eq + 1).c_str());
        if (value < 0) value = -value;      // file has -5 but we keep 5
        if (key == "max_turns") maxturns = value;
        else if (key == "starting_credibility") startcredibility = value;
        else if (key == "every_wrong_turn") steppenalty = value;
        else if (key == "every_wrong_word") wordpenalty = value;
    }
}

// finds the culprit line in clue.txt and gives back the two words
void GameEngine::loadclue(string& w1, string& w2) {
    ifstream file("clue.txt");
    if (!file) throw FileException("clue.txt was not found");
    string line;
    getline(file, line);
    while (getline(file, line)) {
        string p[3];
        if (split(line, p, 3) < 3) continue;
        if (lower(p[0]) == culpritrole) {
            w1 = p[1];
            w2 = p[2];
            return;
        }
    }
    throw FileException("clue.txt has no line for " + culpritrole);
}
Character* GameEngine::makecharacter(const string& name, const string& role, const string& nat) {
    if (role == "victim") return new Victim(name, nat);
    if (role == "cook") return new Cook(name, nat);
    if (role == "guard") return new Guard(name, nat);
    if (role == "servant") return new Servant(name, nat);
    if (role == "child") return new Child(name, nat);
    if (role == "pa") return new PA(name, nat);
    if (role == "partner_1" || role == "partner_2") return new Partner(name, role, nat);
    throw FileException("characters.txt has an unknown role: " + role);
}