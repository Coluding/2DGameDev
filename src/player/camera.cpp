#include "camera.h"



Camera::Camera(float screenWidth, float screenHeight, float worldWidth, float worldHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), worldWidth(worldWidth), worldHeight(worldHeight) {
    view.setSize(screenWidth, screenHeight);
    view.setCenter(screenWidth / 2.0f, screenHeight / 2.0f);
}


void Camera::update(const sf::Vector2f& playerPosition) {
    float centerX = playerPosition.x;
    float centerY = playerPosition.y;

    centerY = screenHeight / 2;
    view.setCenter(centerX, centerY);
}


void Camera::applyTo(sf::RenderWindow& window) {
    window.setView(view);
}

sf::View Camera::getView() const {
    return view;
}
