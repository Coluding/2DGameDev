#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "objects/obstacles.h"
#include <cmath>
#include <math.h>

using namespace  std;

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Obstacle Container Example");
    window.setFramerateLimit(60);

    // Create the obstacle container
    ObstacleContainer container;

    Wall* wall1 = new Wall(100.0f, 20.0f, 200.0f, 500.0f);
    Wall* wall2 =  new Wall(150.0f, 25.0f, 400.0f, 500.0f);
    Wall* wall3 = new Wall(120.0f, 30.0f, 600.0f, 500.0f);

    // Add some walls to the container
    container.addObstacle(wall1);
    container.addObstacle(wall2);
    container.addObstacle(wall3);


    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Move all obstacles to the left
        container.moveAll(-1.0f, 0.0f);

        // Render
        window.clear(sf::Color::Black);
        container.drawAll(window);
        window.display();
    }

    delete(wall1);
    delete(wall2);
    delete(wall3);

    return 0;
}
