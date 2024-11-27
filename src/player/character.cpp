#include "character.h"
#include <iostream>
#include <cmath>
// Constructor
Vehicle::Vehicle(float x, float y, float widthVertical, float widthHorizontal, float heightVertical, float heightHorizontal, float screenHeight)
    : wheelRotationSpeed(100.0f), widthVertical(widthVertical), allowDown(true), allowLeft(true), allowRight(true),
    allowUp(true){
    verticalBody.setPointCount(6);
    verticalBody.setPoint(0, sf::Vector2f(x - widthVertical / 2, y - heightVertical));
    verticalBody.setPoint(1, sf::Vector2f(x + widthVertical / 2, y - heightVertical));
    verticalBody.setPoint(2, sf::Vector2f(x - widthVertical / 2 + 10, y));
    verticalBody.setPoint(3, sf::Vector2f(x + widthVertical / 2 - 10, y));
    verticalBody.setPoint(4, sf::Vector2f(x + widthVertical / 2, y));
    verticalBody.setPoint(5, sf::Vector2f(x - widthVertical / 2, y));

    verticalBody.setFillColor(sf::Color(194, 194, 214));

    horizontalBody.setPointCount(4);
    horizontalBody.setPoint(0, sf::Vector2f(x - widthHorizontal / 2, y));
    horizontalBody.setPoint(1, sf::Vector2f(x + widthHorizontal / 2, y));
    horizontalBody.setPoint(2, sf::Vector2f(x + widthHorizontal / 2, y + heightHorizontal));
    horizontalBody.setPoint(3, sf::Vector2f(x - widthHorizontal / 2, y + heightHorizontal));
    horizontalBody.setFillColor(sf::Color(208, 208, 225));

    leftWheel.setRadius(heightHorizontal / 2);
    leftWheel.setFillColor(sf::Color::Black);
    leftWheel.setOrigin(leftWheel.getRadius(), leftWheel.getRadius()); // Center the wheel
    leftWheel.setPosition(x - widthHorizontal / 4, y + heightHorizontal + leftWheel.getRadius());
    leftWheelIntersectionLine = sf::VertexArray(sf::Lines, 2);


    // Initialize right wheel
    rightWheel.setRadius(heightHorizontal / 2);
    rightWheel.setFillColor(sf::Color::Black);
    rightWheel.setOrigin(rightWheel.getRadius(), rightWheel.getRadius());
    rightWheel.setPosition(x + widthHorizontal / 4, y + heightHorizontal + rightWheel.getRadius());

    screenHeight = screenHeight - heightVertical;
}

// Update the vehicle (e.g., rotate the wheels)
void Vehicle::update(float deltaTime) {
    if (isJumping) {
        jumpTime += deltaTime;
        float t = jumpTime / totalJumpTime;

        // Calculate the new position incrementally
        float newX = jumpStartPos.x + t * jumpDx;
        float newY = jumpStartPos.y - jumpHeight * (1 - (2 * t - 1) * (2 * t - 1));

        // Determine movement delta
        float dx = newX - getPosition().x;
        float dy = newY - getPosition().y;

        // Use move to adjust position seamlessly
        move(dx, dy);

        // End jump if time is up
        if (t >= 1.0f) {
            isJumping = false;

            // Ensure final position is accurate
            float finalDx = (jumpStartPos.x + jumpDx) - getPosition().x;
            float finalDy = jumpStartPos.y - getPosition().y;
            move(finalDx, finalDy);
        }
    } else {
        // Rotate wheels when not jumping
        float rotation = wheelRotationSpeed * deltaTime;
        leftWheel.rotate(rotation);
        rightWheel.rotate(rotation);

    }
}

// Draw the vehicle
void Vehicle::draw(sf::RenderWindow& window) {
    window.draw(verticalBody);
    window.draw(horizontalBody);
    window.draw(leftWheel);
    window.draw(rightWheel);

}

void Vehicle::setPosition(float x, float y) {
    if (x > widthVertical + 50) {
        x = widthVertical + 50;
    }

    sf::Vector2f verticalOffset = verticalBody.getPosition() - sf::Vector2f(x, y);
    sf::Vector2f horizontalOffset = horizontalBody.getPosition() - sf::Vector2f(x, y);
    sf::Vector2f leftWheelOffset = leftWheel.getPosition() - sf::Vector2f(x,y);
    sf::Vector2f rightWeelOffset = rightWheel.getPosition() - sf::Vector2f(x,y);

    verticalBody.setPosition(x - verticalOffset.x, y - verticalOffset.y);
    horizontalBody.setPosition(x - horizontalOffset.x, y - horizontalOffset.y);

    leftWheel.setPosition(x - leftWheelOffset.x, y - leftWheelOffset.y);
    rightWheel.setPosition(x - rightWeelOffset.x, y - rightWeelOffset.y);

    //rightWheel.setPosition(x + horizontalOffset.x / 2, y + horizontalOffset.y + rightWheel.getRadius());
}

void Vehicle::move(float dx, float dy) {

    if (getPosition().y + dy > screenHeight) {
        dy = 0;
    };

    if (dx > 0 && !allowRight) {
        dx = 0;
    } else if (dx < 0 && !allowLeft) {
        dx = 0;
    }else if (dy > 0 && !allowDown) {
        dy = 0;
    } else if (dy < 0 && !allowUp) {
        dy = 0;
    }

    verticalBody.move(dx, dy);
    horizontalBody.move(dx, dy);
    leftWheel.move(dx, dy);
    rightWheel.move(dx, dy);
}


// Get the vehicle's position
sf::Vector2f Vehicle::getPosition() const {
    return verticalBody.getPosition();
}

void Vehicle::jump(float dx, float height) {
    if (isJumping) return; // Prevent starting a new jump if already jumping


    isJumping = true;
    jumpTime = 0.0f;
    totalJumpTime = 1.0f; // Example duration: 1 second
    jumpStartPos = getPosition();
    jumpDx = dx;
    jumpHeight = height;

}

void Vehicle::updateIntersectionLine() {
    // Get the left wheel's position
    sf::Vector2f wheelCenter = leftWheel.getPosition();

    float lineLength = leftWheel.getRadius();

    float angle = 0.0f;

    float dx = std::cos(angle) * lineLength;
    float dy = std::sin(angle) * lineLength;

    leftWheelIntersectionLine[0].position = sf::Vector2f(wheelCenter.x - dx, wheelCenter.y - dy);
    leftWheelIntersectionLine[1].position = sf::Vector2f(wheelCenter.x + dx, wheelCenter.y + dy);

    leftWheelIntersectionLine[0].color = sf::Color::Red;
    leftWheelIntersectionLine[1].color = sf::Color::Red;
}

void Vehicle::getFullPosition(float &wheelX, float &wheelY, float &verticalBodyX, float &verticalBodyY, float &horizontalBodyX, float &horizontalBodyY) const {

    wheelX = leftWheel.getPosition().x;
    wheelY = leftWheel.getPosition().y;
    verticalBodyX = verticalBody.getPosition().x;
    verticalBodyY = verticalBody.getPosition().y;
    horizontalBodyX = horizontalBody.getPosition().x;
    horizontalBodyY = horizontalBody.getPosition().y;


}

float Vehicle::getWheelRadius() const {
    return leftWheel.getRadius();
}

void Vehicle::interruptJump() {
    isJumping = false;
}
