//
// Created by lubi on 11/27/24.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class Camera {
private:
    sf::View view;
    float worldWidth;
    float worldHeight;
    float screenWidth;
    float screenHeight;

public:
    // Constructor
    Camera(float screenWidth, float screenHeight, float worldWidth, float worldHeight);

    // Update the camera's position to follow the player
    void update(const sf::Vector2f& playerPosition);

    // Apply the view to the given window
    void applyTo(sf::RenderWindow& window);

    // Getters
    sf::View getView() const;
};

#endif //CAMERA_H
