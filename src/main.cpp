#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "game/Game.h"
using namespace  std;


int main() {
    GameConfig config = GameConfig();
    Game game(config);
    game.run();
    return 0;
}

