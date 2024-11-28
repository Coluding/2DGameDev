#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "objects/obstacles.h"
#include "player/character.h"
#include "player/camera.h"

#include <cmath>
#include <math.h>

using namespace  std;


ObstacleFactory initializeFactory(ObstacleContainer* container, FallingObstacleContainer* fallingContainer){
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
    const float gameTime = 0.0f; // Start game time at 0

    // Number of obstacles per screen
    const int numWalls = 5;
    const int numSpikeWalls = 3;
    const int numFallingObjects = 4;


    // Initialize the ObstacleFactory
    ObstacleFactory factory(
        numWalls, numSpikeWalls, numFallingObjects,
        screenWidth, screenHeight, gameTime,
        container, fallingContainer
    );

    return factory;
}


int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Obstacle Container Example");
    window.setFramerateLimit(50);

    sf::Clock clock;

    auto container = ObstacleContainer();
    auto fallingContainer = FallingObstacleContainer();

    ObstacleFactory factory = initializeFactory(&container, &fallingContainer);

    // Create a vehicle
    Vehicle vehicle(0, 560, 30, 80, 30, 20, 600);

    // Add walls to the container
    /*
    container.addObstacle(make_unique<Wall>(Wall(100.0f, 20.0f, 200.0f, 500.0f)));
    container.addObstacle(make_unique<Wall>(Wall(150.0f, 25.0f, 400.0f, 500.0f)));
    container.addObstacle(make_unique<Wall>(Wall(120.0f, 30.0f, 600.0f, 500.0f)));
    container.addObstacle(make_unique<SpikeWall>(SpikeWall(80, 50, 300, 550,
    20,40, sf::Color(135, 135, 171))));
     */

    //fallingContainer.addObstacle(make_unique<FallingObstacle>(100, 100, 2, 500, 100));
    //fallingContainer.addObstacle(make_unique<FallingObstacle>(100, 100, 2, 700, 100));
    //fallingContainer.addObstacle(make_unique<FallingObstacle>(100, 100, 2, 900, 100));

    // Define the world size (e.g., larger than the screen)
    float worldWidth = 16000.0f; // Example world width
    float worldHeight = 600.0f; // Example world height

    // Create the camera
    Camera camera(800.0f, 600.0f, worldWidth, worldHeight);

    float totalTimeElapsed = 0;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    vehicle.move(0, -10);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    vehicle.move(0, 10);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    vehicle.move(10, 0);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    vehicle.move(-10, 0);
                    std::cout << vehicle.getPosition().x << " " << vehicle.getPosition().y;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    //vehicle.move(10, -40);
                    vehicle.jump(350, 350);
                }
            }
        }

        vehicle.update(deltaTime);
        totalTimeElapsed += deltaTime;

        // Update the camera to follow the player
        camera.update(vehicle.getPosition());

        // Apply the camera view to the window
        camera.applyTo(window);

        // Clear the window
        window.clear(sf::Color::White);

        // Draw everything
        container.drawAll(window);
        container.checkCollision(vehicle);
        fallingContainer.drawAll(window);
        vehicle.draw(window);

        // Move all obstacles to the left
        //container.moveAll(-2.0f, 0.0f);
        fallingContainer.activate(vehicle.getPosition().x);
        fallingContainer.fallAll();

        // Display the frame
        window.display();
    }

    return 0;
}



