#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "objects/obstacles.h"
#include "player/character.h"

#include <cmath>
#include <math.h>

using namespace  std;

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Obstacle Container Example");
    window.setFramerateLimit(50);

    sf::Clock clock;

    ObstacleContainer container;

    FallingObstacleContainer fallContainer;


    // Create a vehicle
    Vehicle vehicle(400, 500, 30, 80, 30, 20);

    // Add walls to the container
    container.addObstacle(make_unique<Wall>(Wall(100.0f, 20.0f, 200.0f, 500.0f)));
    container.addObstacle(make_unique<Wall>(Wall(150.0f, 25.0f, 400.0f, 500.0f)));
    container.addObstacle(make_unique<Wall>(Wall(120.0f, 30.0f, 600.0f, 500.0f)));
    container.addObstacle(make_unique<SpikeWall>(SpikeWall(80, 50, 300, 550,
    20,40, sf::Color(135, 135, 171))));

    fallContainer.addObstacle(make_unique<FallingObstacle>(40, 50, 2, 100, 0.5));
    fallContainer.addObstacle(make_unique<FallingObstacle>(40, 50, 2, 200, 1));
    fallContainer.addObstacle(make_unique<FallingObstacle>(40, 50, 2, 700, 2));

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
            if (event.type == sf::Event::KeyPressed){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                    vehicle.move(0, -10);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                    vehicle.move(0, 10);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                    vehicle.move(10, 0);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                    vehicle.move(-10, 0);
                    std::cout << vehicle.getPosition().x << " " << vehicle.getPosition().y;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                    //vehicle.move(10, -40);
                    vehicle.jump(10, 100);
                }
            }
        }

        vehicle.update(deltaTime);
        totalTimeElapsed += deltaTime;

        // Clear the window
        window.clear(sf::Color::White);

        // Draw everything
        container.drawAll(window);
        fallContainer.drawAll(window);
        vehicle.draw(window);

        // Move all obstacles to the left
        container.moveAll(-1.0f, 0.0f);
        fallContainer.activate(totalTimeElapsed);
        fallContainer.fallAll();

        // Display the frame
        window.display();
    }


    return 0;
}



