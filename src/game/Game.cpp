#include "Game.h"
#include "../objects/obstacles.h"

Game::Game()
    : window(sf::VideoMode(1200, 800), "2D Game Example"),
      camera(1000.0f, 600.0f, worldWidth, worldHeight),
      vehicle(0, 560, 30, 80, 30, 20, 600),
        factory(initializeFactory(&container, &fallingContainer, &rollingContainer)) {
    window.setFramerateLimit(80);

    if (!backgroundTexture.loadFromFile("assets/bgg.png")) {
        std::cerr << "Error loading background texture!" << std::endl;
    }

    heartTextures.resize(vehicle.getLife());
    heartSprites.resize(vehicle.getLife());
    for (int i = 0; i < vehicle.getLife(); ++i) {
        heartTextures[i].loadFromFile("./assets/heart.png");
        heartSprites[i].setTexture(heartTextures[i]);
        heartSprites[i].setScale(0.05f, 0.05f);
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
            if (gameState == PLAYING) {
                handleInput();
            } else if (gameState == GAME_OVER) {
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
    rollingContainer.activate(vehicle.getPosition().x);
    fallingContainer.fallAll();
    rollingContainer.rollAll();

    if (vehicle.isInvincible()) {
        invincbleTimer += deltaTime;
        if (invincbleTimer > 2.0f) {
            vehicle.setInvincible(false);
        }
    }

    if (container.checkCollision(vehicle ) || fallingContainer.checkCollision(vehicle)) {
        if (!vehicle.isInvincible()) {
            removeLife();
        }
    }
}

void Game::render() {
    // Update background position to match the camera
    sf::Vector2f cameraCenter = camera.getView().getCenter();
    sf::Vector2f cameraSize = camera.getView().getSize();
    backgroundSprite.setPosition(cameraCenter.x - cameraSize.x / 2, cameraCenter.y - cameraSize.y / 2);

    window.clear();

    camera.applyTo(window);
    window.draw(backgroundSprite);

    for (int i = 0; i < vehicle.lives; ++i) {
        heartSprites[i].setPosition(cameraCenter.x - cameraSize.x / 2 + 10 + i * 30, cameraCenter.y - cameraSize.y / 2 + 10);
        window.draw(heartSprites[i]);
    }

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

void Game::removeLife() {
    vehicle.setInvincible(true);
    invincbleTimer = 0.0f;
    vehicle.setLife(vehicle.getLife() - 1);

    if (vehicle.getLife() == 0) {
        gameState = GameState::GAME_OVER;
        return ;
    }

    heartTextures[vehicle.getLife() - 1].loadFromFile(".assets/heart_empty.png");
    heartSprites[vehicle.getLife() - 1].setTexture(heartTextures[vehicle.getLife()]);
    heartSprites[vehicle.getLife() - 1].setScale(0.05f, 0.05f);

}


ObstacleFactory Game::initializeFactory(ObstacleContainer* container, FallingObstacleContainer* fallingContainer,
RollingObstacleContainer* rollingContainer) {
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
    const float gameTime = 0.0f; // Start game time at 0

    // Number of obstacles per screen
    const int numWalls = 10;
    const int numSpikeWalls = 10;
    const int numFallingObjects = 1;
    const int numRollingObjects = 1;

    return ObstacleFactory(
        numWalls, numSpikeWalls, numFallingObjects,screenWidth,
        numRollingObjects,
        screenHeight, gameTime,
        container, fallingContainer, rollingContainer
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