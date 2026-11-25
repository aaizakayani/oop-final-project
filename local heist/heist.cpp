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