#include "character.h"
#include <iostream>
#include <cmath>
// Constructor
Vehicle::Vehicle(float x, float y, float widthVertical, float widthHorizontal, float heightVertical, float heightHorizontal, float screenHeight)
    : wheelRotationSpeed(100.0f), widthVertical(widthVertical),  heightVertical(heightVertical),
     widthHorizontal(widthHorizontal), allowDown(true), allowLeft(true), allowRight(true),
    allowUp(true), gravity(275), verticalSpeed(0.0f), isOnGround(true){
    verticalBody.setPointCount(6);
    verticalBody.setPoint(0, sf::Vector2f(x - widthVertical / 2, y - heightVertical));
    verticalBody.setPoint(1, sf::Vector2f(x + widthVertical / 2, y - heightVertical));
    verticalBody.setPoint(2, sf::Vector2f(x - widthVertical / 2 + 10, y));
    verticalBody.setPoint(3, sf::Vector2f(x + widthVertical / 2 - 10, y));
    verticalBody.setPoint(4, sf::Vector2f(x + widthVertical / 2, y));
    verticalBody.setPoint(5, sf::Vector2f(x - widthVertical / 2, y));
    verticalBody.setOutlineColor(sf::Color::Black);
    verticalBody.setOutlineThickness(1);

    verticalBody.setFillColor(sf::Color(194, 194, 214));

    horizontalBody.setPointCount(4);
    horizontalBody.setPoint(0, sf::Vector2f(x - widthHorizontal / 2, y));
    horizontalBody.setPoint(1, sf::Vector2f(x + widthHorizontal / 2, y));
    horizontalBody.setPoint(2, sf::Vector2f(x + widthHorizontal / 2, y + heightHorizontal));
    horizontalBody.setPoint(3, sf::Vector2f(x - widthHorizontal / 2, y + heightHorizontal));
    horizontalBody.setFillColor(sf::Color(208, 208, 225));
    horizontalBody.setOutlineColor(sf::Color::Black);
    horizontalBody.setOutlineThickness(1);

    leftWheel.setRadius(heightHorizontal / 2);
    //rightWheel.setFillColor(sf::Color(245, 233, 233));
    leftWheel.setFillColor(sf::Color::Black);
    leftWheel.setOrigin(leftWheel.getRadius(), leftWheel.getRadius()); // Center the wheel
    leftWheel.setPosition(x - widthHorizontal / 4, y + heightHorizontal + leftWheel.getRadius());
    leftWheelIntersectionLine = sf::VertexArray(sf::Lines, 2);


    // Initialize right wheel
    rightWheel.setRadius(heightHorizontal / 2);
    //rightWheel.setFillColor(sf::Color(245, 233, 233));
    rightWheel.setFillColor(sf::Color::Black);
    rightWheel.setOrigin(rightWheel.getRadius(), rightWheel.getRadius());
    rightWheel.setPosition(x + widthHorizontal / 4, y + heightHorizontal + rightWheel.getRadius());

    this->screenHeight = screenHeight - heightVertical;
}

void Vehicle::update() {
    updateMomentum();
    if (!isOnGround) {
        if (isJumping) {
            // Increment jump progress
             jumpProgress += jumpStepSize;

            float newX = jumpStartPos.x + jumpProgress * jumpDx;
            float newY = jumpStartPos.y - jumpHeight * (1 - (2 * jumpProgress - 1) * (2 * jumpProgress - 1));

            // Determine movement delta
             float dx = newX - getPosition().x;
             float dy = newY - getPosition().y;

            move(dx, dy);
            //move(dx, 100);

            if (jumpProgress >= 0.5f) {
                isJumping = false;
                momentum = dx;

            }
        }

        else {
            move(momentum, gravity * jumpStepSize);
            float rotation = wheelRotationSpeed * jumpStepSize;
            leftWheel.rotate(rotation);
            rightWheel.rotate(rotation);
        }

    }
}

void Vehicle::updateMomentum() {
     if (getPosition().y >= 0 || isOnGround) {
        momentum = 0;
    }

}


// Initiate the jump
void Vehicle::jump(float dx, float height) {

    //if (!isOnGround) return; // Prevent jumping if not on the ground

    if (jumpCount > 1) {
        jumpCount = 0;
        return;
    }

    if (isJumping) return; // Prevent starting a new jump if already jumping

    isJumping = true;
    jumpProgress = 0.0f; // Reset progress
    jumpStartPos = getPosition();
    jumpDx = dx;
    jumpHeight = height;
    isOnGround = false;
    jumpCount++;
}
// Method to set the on-ground state
void Vehicle::setOnGround(bool onGround) {
    isOnGround = onGround;
    //isJumping = false;
}

// Draw the vehicle
void Vehicle::draw(sf::RenderWindow& window) {
    window.draw(verticalBody);
    window.draw(horizontalBody);
    window.draw(leftWheel);
    window.draw(rightWheel);

}

void Vehicle::setPosition(float x, float y) {
    // Calculate the offset for each component relative to the new position
    sf::Vector2f verticalBodyOffset = verticalBody.getPosition() - leftWheel.getPosition();
    sf::Vector2f horizontalBodyOffset = horizontalBody.getPosition() - leftWheel.getPosition();
    sf::Vector2f rightWheelOffset = rightWheel.getPosition() - leftWheel.getPosition();

    // Update the position of the left wheel (base position)
    leftWheel.setPosition(x, y);

    // Update the positions of other components relative to the left wheel
    verticalBody.setPosition(x + verticalBodyOffset.x, y + verticalBodyOffset.y);
    horizontalBody.setPosition(x + horizontalBodyOffset.x, y + horizontalBodyOffset.y);
    rightWheel.setPosition(x + rightWheelOffset.x, y + rightWheelOffset.y);
}

void Vehicle::move(float dx, float dy) {

    if (getPosition().y - 7 + dy > 0) {
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

void Vehicle::ForbidRight() {
    allowRight = false;
}

void Vehicle::ForbidLeft() {
    allowLeft = false;
}

void Vehicle::ForbidUp() {
    allowUp = false;
}

void Vehicle::ForbidDown() {
    allowDown = false;
}

void Vehicle::AllowRight() {
    allowRight = true;
}

void Vehicle::AllowLeft() {
    allowLeft = true;
}

void Vehicle::AllowUp() {
    allowUp = true;
}

void Vehicle::AllowDown() {
    allowDown = true;
}

