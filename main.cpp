#include <SFML/Graphics.hpp>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My First SFML Program");

    // Create a circle shape
    sf::CircleShape circle(50.0f); // Radius 50
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(375, 275); // Center on screen

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Draw the circle
        window.draw(circle);

        // Display the frame
        window.display();
    }

    return 0;
}
