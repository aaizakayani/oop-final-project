The Locker Heist
A console-based detective game built with C++ and Object-Oriented Programming.
A robbery has taken place at a wealthy business owner's cottage. A locker hidden behind a bookshelf in the Study Room has been stolen from—and one of the seven suspects is responsible. Investigate the cottage, interview its residents, assemble the clues, and identify the culprit before your credibility or available turns run out.

Project Idea
The Locker Heist turns OOP concepts into an interactive deduction game. Instead of revealing the culprit directly, the game distributes evidence across two clue-word puzzles:
- Location clues reveal letters for the first word.
- Character interviews reveal letters for the second word.
- Each clue provides a letter and its position, allowing the detective to fill the word boards and verify the completed words.
The player must manage limited turns and credibility while deciding which locations to search, whom to interview, when to solve the words, and when to make an accusation.
Features
- Randomized culprit role: A new game selects one of seven suspect roles.
- Location investigation: Search the cottage's seven locations to collect first-word letters.
- Character interviews: Interview the eight characters to collect second-word letters.
- Position-based clue boards: Letters are placed in their assigned slots on two boards—one with seven slots and one with eight.
- Word verification: The game checks completed words against the clue data.
- Credibility system: Repeated searches/interviews, incorrect words, and incorrect accusations can reduce credibility.
- Turn limit: Every game has a configurable maximum number of turns.
- Accusation and outcome: Accuse a suspect to win if the choice is correct; otherwise, the game applies a penalty.
- Persistent scoreboard: Results are appended to a file and can be viewed from the main menu.
- Configurable rules: Game settings can be loaded from rules.txt.
- Menu-driven interface: Start a game, view the scoreboard, read the project information, review rules, or exit.
Gameplay
1. Start a case
Choose New Game from the main menu and enter a detective name. The game initializes the case, selects the culprit role, loads the clue words and evidence, and displays the rules.
2. Investigate locations
Choose Visit a Location and select one of the seven cottage locations. A successful search reveals a letter and its position for Word 1.

Searching the same location again is not productive: it costs credibility and consumes a turn.
3. Interview characters
Choose Interview a Person and select a character. A successful interview reveals a letter and its position for Word 2.

Interviewing the same character again costs credibility and consumes a turn.
4. Assemble and solve the words
Choose Form a Word, select the relevant row, and enter the completed word once all its slots have been filled. The game checks the answer against the clue data. A correct answer marks that row as solved; an incorrect answer reduces credibility.

When both words are solved, the game confirms that they point toward the culprit.
5. Make an accusation
Choose Accuse a Suspect and select a suspect. The victim cannot be accused. A correct accusation wins the case; a wrong accusation reduces credibility.
6. Keep track of your resources
The status display shows:
- Current credibility
- Turns used and the maximum allowed
- Your collected clue racks
- The two word boards and their filled positions
The case is lost when credibility reaches zero or the turn limit is exhausted.
Rules and Configuration
The default settings are:
Setting	Default
Maximum turns	25
Starting credibility	100
Penalty for a repeated location/person	5
Penalty for an incorrect word	10
Win condition	Accuse the correct culprit
Lose condition	Credibility reaches 0 or turns are exhausted


The game reads these values from rules.txt. If the file is unavailable, the default values are used.
Object-Oriented Design & Concepts
The project applies OOP through a class-based game architecture and custom data structures.
Concept	Implementation in the project
Classes and objects	The game is modeled through classes such as GameEngine, Detective, Cottage, Scrabble, Character, and ScoreBoard.
Encapsulation	Class data is kept in private members and accessed through public operations and getters/setters.
Abstraction	Character defines a common interface for character objects while leaving character-specific display behavior to derived classes.
Inheritance	Character roles are represented through derived classes such as Victim, Detective, Suspect, HouseHelp, Cook, Guard, Servant, Partner, Child, and PA.
Runtime polymorphism	Character declares display() as a pure virtual function. Derived objects implement it, and the game displays characters through Character* pointers.
Virtual destructor	Character has a virtual destructor so derived objects can be safely deleted through base-class pointers.
Multiple inheritance	PA inherits from both BusinessRelations and CloseRelations.
Virtual inheritance	BusinessRelations and CloseRelations virtually inherit Suspect, preventing duplicate Suspect base subobjects in the PA diamond.
Friend class	GameEngine is a friend of Character, allowing it to set the private culprit flag when loading the case.
Friend function / operator overloading	operator<< is a friend of Scrabble and prints its private word-board data. Tile also overloads operator== for letter comparison.
Templates	DynamicArray<T> is a custom template container used for different data types, including clues, tiles, and locations.
Pointers and dynamic memory	DynamicArray manages a dynamically allocated array. GameEngine stores character objects through Character* and creates the appropriate derived type dynamically.
Rule of Three / deep copying	DynamicArray implements a copy constructor, copy-assignment operator, and destructor to manage its owned array.
Singleton design pattern	GameEngine uses a private constructor and static instance pointer. getinstance() returns the single game-engine instance, and destroyinstance() releases it.
Exception handling	Custom exceptions—`GameException`, InvalidInputException, InvalidMoveException, RackFullException, and FileException—represent game errors. The main game loop catches GameException and displays its message.
File handling	Text files provide character data, location data, clue words, rules, project information, and scoreboard records.
Operator overloading	Tile compares letters using ==; Scrabble supports board output through <<.
Enums and named states	GameState and WordRow represent game states and clue-board rows with named values.
Composition	GameEngine contains and coordinates the detective, cottage, word board, and scoreboard objects.
Input validation	The game validates menu choices, board positions, repeated actions, word lengths, and accusation restrictions.


Project Architecture
The game is coordinated by GameEngine, which manages the overall state and gameplay flow.
- GameEngine — Controls the menu, case initialization, turns, investigation actions, win/loss conditions, and character loading.
- Character hierarchy — Represents the detective, victim, and suspects. Derived classes provide role-specific display behavior.
- Detective — Tracks credibility, turns, and the two clue racks.
- Cottage / Location — Stores the searchable locations and their first-word clues.
- Scrabble / Tile — Maintains the two letter boards and checks the completed words.
- Clue / TileRack — Represents collected evidence and displays the detective's clue collection.
- DynamicArray<T> — Provides a reusable dynamically sized array for the project's data.
- ScoreBoard — Saves and displays completed game results.
- Custom exception classes — Provide structured error reporting for invalid input, moves, rack capacity, and file problems.
Files Used by the Game
The game reads and writes data through these text files:
File	Purpose
characters.txt	Character names, roles, nationalities, clue positions, and role-based clue letters
locations.txt	Cottage locations, clue positions, and role-based clue letters
clue.txt	The two target words associated with each possible culprit role
rules.txt	Configurable turn, credibility, and penalty values
about.txt	Project introduction displayed from the menu
leader/scoreBoard.txt	Stores completed game results


Runtime note: Keep the data files in the program's working directory. The source code writes and reads the scoreboard at leader/scoreBoard.txt, so ensure the leader directory exists and contains the scoreboard file with its header.
Build and Run
A C++ compiler such as g++ is required.

From the directory containing main.cpp, heist.cpp, heist.h, and the required data files, compile:
g++ -std=c++11 main.cpp heist.cpp -o locker-heist
Run on Linux/macOS:
./locker-heist
Run on Windows (MinGW):
locker-heist.exe
If your IDE is used, add both main.cpp and heist.cpp to the project and make sure the text data files are available from the configured working directory.
Course Project

Good luck, Detective.
