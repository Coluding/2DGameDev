#include "Game.h"
#include "../objects/obstacles.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "2D Game Example"),
      camera(800.0f, 600.0f, worldWidth, worldHeight),
      vehicle(0, 560, 30, 80, 30, 20, 600),
        factory(initializeFactory(&container, &fallingContainer)){
    window.setFramerateLimit(80);

    if (!backgroundTexture.loadFromFile("assets/bgg.png")) {
        std::cerr << "Error loading background texture!" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (gameState == GameState::PLAYING) {
                handleInput();
            } else if (gameState == GameState::GAME_OVER) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    restart();
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.close();
                }
            }
        }
    }
}


void Game::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        vehicle.move(0, -10);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vehicle.move(0, 10);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        vehicle.move(20, 0);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        vehicle.move(-20, 0);
        std::cout << vehicle.getPosition().x << " " << vehicle.getPosition().y << std::endl;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        vehicle.jump(150, 350);
    }
}

void Game::update(float deltaTime) {
    vehicle.update();
    camera.update(vehicle.getPosition());
    fallingContainer.activate(vehicle.getPosition().x);
    fallingContainer.fallAll();

    if (container.checkCollision(vehicle ) || fallingContainer.checkCollision(vehicle)) {
        std::cout << "Collision detected! Game Over!" << std::endl;
        gameState = GameState::GAME_OVER;
    }
}

void Game::render() {
    // Update background position to match the camera
    sf::Vector2f cameraCenter = camera.getView().getCenter();
    sf::Vector2f cameraSize = camera.getView().getSize();
    backgroundSprite.setPosition(cameraCenter.x - cameraSize.x / 2, cameraCenter.y - cameraSize.y / 2);

    window.clear();

    // Apply the camera view
    camera.applyTo(window);

    // Draw the background
    window.draw(backgroundSprite);

    // Draw other objects only if game is running
    if (gameState == GameState::PLAYING) {
        container.drawAll(window);
        fallingContainer.drawAll(window);
        vehicle.draw(window);
    } else if (gameState == GameState::GAME_OVER) {
        // Display "Game Over" popup
        sf::Font font;
        if (!font.loadFromFile("assets/Arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
        }

        sf::Text gameOverText("Game Over!", font, 50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(cameraCenter.x - 150, cameraCenter.y - 50);

        window.draw(gameOverText);
    }

    window.display();
}


ObstacleFactory Game::initializeFactory(ObstacleContainer* container, FallingObstacleContainer* fallingContainer) {
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
    const float gameTime = 0.0f; // Start game time at 0

    // Number of obstacles per screen
    const int numWalls = 10;
    const int numSpikeWalls = 10;
    const int numFallingObjects = 1;

    return ObstacleFactory(
        numWalls, numSpikeWalls, numFallingObjects,
        screenWidth, screenHeight, gameTime,
        container, fallingContainer
    );
}

void Game::restart() {
    // Reset game state
    gameState = GameState::PLAYING;
    vehicle.setPosition(0, 560); // Reset player position
    //container.reset();          // Reset obstacles
    //fallingContainer.reset();   // Reset falling objects
    camera.update(vehicle.getPosition());
}