#ifndef VEHICLE_H
#define VEHICLE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Vehicle {
private:
    sf::ConvexShape verticalBody;
    sf::ConvexShape horizontalBody;
    sf::CircleShape leftWheel, rightWheel;
    bool isJumping = false;
    float jumpTime = 0.0f;
    float totalJumpTime = 1.0f;
    sf::Vector2f jumpStartPos;
    float jumpDx = 0.0f;
    float jumpHeight = 0.0f;
    float wheelRotationSpeed;
    sf::VertexArray leftWheelIntersectionLine;

public:
    Vehicle(float x, float y, float widthVertical, float widthHoriztonal, float heightVertical, float heightHorizontal);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    void setPosition(float x, float y);

    void move(float dx, float dy);

    void jump(float dx, float height);

    sf::Vector2f getPosition() const;

    void updateIntersectionLine();
};

#endif // VEHICLE_H