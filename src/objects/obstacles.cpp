#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "obstacles.h"


Wall::Wall(float height, float width, float x, float y, sf::Color color) {
        wall.setSize(sf::Vector2f(width, height)); // Set size (width, height)
        wall.setPosition(x, y);                   // Set initial position
        wall.setFillColor(color);
    }

Wall::Wall(float height, float width, float x, float y) {
    wall.setSize(sf::Vector2f(width, height)); // Set size (width, height)
    wall.setPosition(x, y);                   // Set initial position
    wall.setFillColor(color=sf::Color::Blue);
}

void Wall::setPosition(float x, float y) {
    wall.setPosition(x, y);
}

void Wall::setDeltaPosition(float dx, float dy) {
    wall.move(dx, dy);
}

const sf::RectangleShape &Wall::getShape() const {
    return wall;
};

sf::Vector2f Wall::getPosition() const {
        return wall.getPosition();
}

void Wall::setColor(sf::Color color){
    wall.setFillColor(color);
}



void ObstacleContainer::addObstacle(Obstacle* obs){
    obstacles.push_back(obs);
}

void ObstacleContainer::moveAll(float dx, float dy){
    for (auto &obstacle: obstacles){
        obstacle->setDeltaPosition(dx, dy);
    }
}

void ObstacleContainer::drawAll(sf::RenderWindow& window){
    for (auto &obstacle: obstacles){
        window.draw(obstacle->getShape());
    }
}

void ObstacleContainer::clear() {
    obstacles.clear();
}

