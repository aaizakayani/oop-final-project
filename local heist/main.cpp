#include "heist.h"

int main() {
    GameEngine* game = GameEngine::getinstance();   // get the only object
    game->run();                                    // start the game
    GameEngine::destroyinstance();                  // delete the object
    return 0;
}