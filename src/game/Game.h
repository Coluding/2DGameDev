#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "../objects/obstacles.h"
#include "../player/character.h"
#include "../player/camera.h"
#include "../objects/objects.h"

enum GameState {
    PLAYING,
    GAME_OVER,
    WIN
};


struct GameConfig {
    const float worldWidth;
    const float worldHeight;
    const float leftObstacleMovingSpeed;
    const float invincbleTimer;
    const float score;
    const int numWalls;
    const int numSpikeWalls;
    const int numFallingObjects;
    const int numRollingObjects;
    const float screenWidth;
    const float screenHeight;
    const int modeWidth;
    const int modeHeight;

    // Constructor
    GameConfig(
        float wWidth = 16000.0f,
        float wHeight = 600.0f,
        float obstacleSpeed = 0.5f,
        float invTimer = 0.0f,
        float initScore = 0.0f,
        int walls = 1,
        int spikeWalls = 1,
        int fallingObjects = 10,
        int rollingObjects = 1,
        float sWidth = 1000.0f,
        float sHeight = 600.0f,
        int mWidth = 1200,
        int mHeight = 800
    )
        : worldWidth(wWidth),
          worldHeight(wHeight),
          leftObstacleMovingSpeed(obstacleSpeed),
          invincbleTimer(invTimer),
          score(initScore),
          numWalls(walls),
          numSpikeWalls(spikeWalls),
          numFallingObjects(fallingObjects),
          numRollingObjects(rollingObjects),
          screenWidth(sWidth),
          screenHeight(sHeight),
          modeWidth(mWidth),
          modeHeight(mHeight) {}
};



class Game {
public:
    Game(GameConfig config);
    void run();
private:
    // Window and rendering


    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::Texture> heartTextures;
    std::vector<sf::Sprite> heartSprites;
    float score = 0.0f;
    bool isCounting = true;


    // Game objects
    ObstacleContainer container;
    FallingObstacleContainer fallingContainer;
    RollingObstacleContainer rollingContainer;
    ObstacleFactory factory;
    WinningPortal winningPortal;
    MajorLeftObstacle majorLeftObstacle;
    Vehicle vehicle;
    Camera camera;
    GameState gameState = PLAYING;

    float leftObstacleMovingSpeed = 0.5f;
    float invincbleTimer = 0.0f;


    // Game world dimensions
    const float worldWidth = 16000.0f;
    const float worldHeight = 600.0f;
    float modeWidth;
    float modeHeight;

    float screenWidth;
    float screenHeight;

    // Game methods
    void processEvents();           // Handles input and events
    void handleInput();             // Handles keyboard input
    void update(float deltaTime);   // Updates game state
    void render();
    void restart();// Renders the game
    void removeLife();
    ObstacleFactory initializeFactory(ObstacleContainer* container, FallingObstacleContainer* fallingContainer,
    RollingObstacleContainer* rollingContainer, GameConfig& config);
    WinningPortal initPortal(ObstacleContainer& container, GameConfig& config);
    MajorLeftObstacle initMajorLeftObstacle();
};

#endif // GAME_H
