#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "../objects/obstacles.h"
#include "../player/character.h"
#include "../player/camera.h"
enum GameState {
    PLAYING,
    GAME_OVER
};


class Game {
public:
    Game();
    void run();
private:
    // Window and rendering
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Game objects
    ObstacleContainer container;
    FallingObstacleContainer fallingContainer;
    RollingObstacleContainer rollingContainer;
    ObstacleFactory factory;
    Vehicle vehicle;
    Camera camera;
    GameState gameState = PLAYING;


    // Game world dimensions
    const float worldWidth = 16000.0f;
    const float worldHeight = 600.0f;

    // Game methods
    void processEvents();           // Handles input and events
    void handleInput();             // Handles keyboard input
    void update(float deltaTime);   // Updates game state
    void render();
    void restart();// Renders the game
    ObstacleFactory initializeFactory(ObstacleContainer* container, FallingObstacleContainer* fallingContainer,
    RollingObstacleContainer* rollingContainer);
};

#endif // GAME_H
