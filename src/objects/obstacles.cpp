#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>


#include "obstacles.h"

void checkRightAndLeftEdgeCollision(Vehicle& vehicle, Obstacle& wall,float& wheelX, float& wheelY, float& verticalBodyX, float& verticalBodyY,
        float& horizontalBodyX, float& horizontalBodyY);

void checkRightAndLeftEdgeCollision(Vehicle& vehicle, Obstacle& wall,
    float& wheelX, float& wheelY, float& verticalBodyX, float& verticalBodyY,
        float& horizontalBodyX, float& horizontalBodyY) {

        vehicle.getFullPosition(wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);


    float wheelBottomY = wheelY + vehicle.getWheelRadius();
    float wallTopY = wall.getPosition().y;
    float wallLeftX = wall.getPosition().x;
    float wallRightX = wallLeftX + wall.getSize().x;

    bool hitWallLeft = horizontalBodyX + (vehicle.widthHorizontal / 2) > wallLeftX - 10 && horizontalBodyX < wallLeftX + 10;
    bool hitWallRight = horizontalBodyX - (vehicle.widthHorizontal / 2) < wallRightX + 10 && horizontalBodyX > wallRightX - 10;

    if (hitWallLeft && wheelBottomY > wallTopY + 20) {
        vehicle.ForbidRight();
    } else {
        vehicle.AllowRight();
    }

    if (hitWallRight && wheelBottomY > wallTopY + 20) {
        vehicle.ForbidLeft();
    } else {
        vehicle.AllowLeft();
    }
}


bool checkWithinXBounds(Vehicle& vehicle, float wheelX, float wallLeftX, float wallRightX) {

    bool withinXBounds = (wheelX + vehicle.getWheelRadius() > wallLeftX &&
                     wheelX - vehicle.getWheelRadius() < wallRightX);

    return withinXBounds;
}

bool checkWithinYBounds(float wheelBottomY, float wallTopY, float wheelY, float wallBottomY) {

   bool withinYBounds = (wheelBottomY >= wallTopY && wheelY < wallBottomY);

   return withinYBounds;
}


Wall::Wall(float height, float width, float x, float y, sf::Color color) {
        wall.setSize(sf::Vector2f(width, height)); // Set size (width, height)
        wall.setPosition(x, y);                   // Set initial position
        wall.setFillColor(color);
    }

Wall::Wall(float height, float width, float x, float y) {
    wall.setSize(sf::Vector2f(width, height)); // Set size (width, height)
    wall.setPosition(x, y);                   // Set initial position
    wall.setFillColor(color=sf::Color(135, 135, 171));
}

void Wall::setPosition(float x, float y) {
    wall.setPosition(x, y);
}

void Wall::setDeltaPosition(float dx, float dy) {
    wall.move(dx, dy);
}


const sf::Vector2f Wall::getPosition() const {
    return wall.getPosition();
}

const void Wall::draw(sf::RenderWindow &window) const {
    window.draw(wall);
}

sf::Vector2f Wall::getSize() const {
    return wall.getSize();
}

void Wall::setColor(sf::Color color){
    wall.setFillColor(color);
}

bool Wall::checkCollision(Vehicle& vehicle) {
    float wheelX = 0.0f, wheelY = 0.0f, verticalBodyX = 0.0f, verticalBodyY = 0.0f,
          horizontalBodyX = 0.0f, horizontalBodyY = 0.0f;

    checkRightAndLeftEdgeCollision(vehicle, *this, wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);

    float wheelBottomY = wheelY + vehicle.getWheelRadius();
    float wallTopY = getPosition().y;
    float wallBottomY = wallTopY + wall.getSize().y;
    float wallLeftX = getPosition().x;
    float wallRightX = wallLeftX + wall.getSize().x;

    bool withinXBounds = checkWithinXBounds(vehicle, wheelX, wallLeftX, wallRightX);
    bool withinYBounds = checkWithinYBounds(wheelBottomY, wallTopY, wheelY, wallBottomY);

    if (withinXBounds && withinYBounds) {
        vehicle.setOnGround(true);
        vehicle.ForbidDown();
    } else {
        vehicle.setOnGround(false);
        vehicle.AllowDown();
    }

    return false;
}


SpikeWall::SpikeWall(float baseWidth, float baseHeight, float x, float y, float spikeWidth, float spikeHeight)
    : spikeWidth(spikeWidth), spikeHeight(spikeHeight) {

    // Initialize the base platform
    base.setSize(sf::Vector2f(baseWidth, baseHeight));
    base.setPosition(x, y);
    base.setFillColor(sf::Color::Green);

    int numSpikes = static_cast<int>(baseWidth / spikeWidth);
    for (int i = 0; i < numSpikes; ++i) {
        sf::ConvexShape spike(3); // Triangular spike
        spike.setPoint(0, sf::Vector2f(x + i * spikeWidth, y));                         // Bottom-left
        spike.setPoint(1, sf::Vector2f(x + (i + 0.5f) * spikeWidth, y - spikeHeight)); // Peak
        spike.setPoint(2, sf::Vector2f(x + (i + 1) * spikeWidth, y));                  // Bottom-right
        spike.setFillColor(sf::Color::Magenta); // Spikes are magenta
        spikes.push_back(spike);
    }
}


SpikeWall::SpikeWall(float baseWidth, float baseHeight, float x, float y, float spikeWidth, float spikeHeight,
                     sf::Color color)
    : spikeWidth(spikeWidth), spikeHeight(spikeHeight) {
    // Initialize the base platform
    base.setSize(sf::Vector2f(baseWidth, baseHeight + 100));
    base.setPosition(x, y);
    base.setFillColor(color);

    // Create spikes on top of the base
    int numSpikes = static_cast<int>(baseWidth / spikeWidth);
    for (int i = 0; i < numSpikes; ++i) {
        sf::ConvexShape spike(3); // Triangular spike
        spike.setPoint(0, sf::Vector2f(x + i * spikeWidth, y));                         // Bottom-left
        spike.setPoint(1, sf::Vector2f(x + (i + 0.5f) * spikeWidth, y - spikeHeight)); // Peak
        spike.setPoint(2, sf::Vector2f(x + (i + 1) * spikeWidth, y));                  // Bottom-right
        spike.setFillColor(sf::Color(225, 225, 234)); // Spikes are magenta
        spikes.push_back(spike);
    }
}

const void SpikeWall::draw(sf::RenderWindow &window) const {
    window.draw(base);
    for (auto &spike: spikes){
        window.draw(spike);
    }
}

sf::Vector2f SpikeWall::getSize() const {
    return base.getSize();
}

void SpikeWall::setPosition(float x, float y) {
    // Update base position
    base.setPosition(x, y);

    // Update spike positions relative to the new base position
    for (size_t i = 0; i < spikes.size(); ++i) {
        float spikeX = x + i * spikeWidth;
        float spikeY = y;
        spikes[i].setPoint(0, sf::Vector2f(spikeX, spikeY));
        spikes[i].setPoint(1, sf::Vector2f(spikeX + 0.5f * spikeWidth, spikeY - spikeHeight));
        spikes[i].setPoint(2, sf::Vector2f(spikeX + spikeWidth, spikeY));
    }
}

void SpikeWall::setDeltaPosition(float dx, float dy) {
    // Move base
    base.move(dx, dy);

    // Move spikes
    for (auto& spike : spikes) {
        spike.move(dx, dy);
    }
}

void SpikeWall::setColor(sf::Color color) {
    base.setFillColor(color); // Set color for the base
    for (auto& spike : spikes) {
        spike.setFillColor(sf::Color::Black); // Keep spikes as magenta (or change if needed)
    }
}

const sf::Vector2f SpikeWall::getPosition() const {
    return base.getPosition();
}

bool SpikeWall::checkCollision(Vehicle& vehicle) {
    float wheelX = 0.0f, wheelY = 0.0f, verticalBodyX = 0.0f, verticalBodyY = 0.0f,
          horizontalBodyX = 0.0f, horizontalBodyY = 0.0f;

    checkRightAndLeftEdgeCollision(vehicle, *this, wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);

    float wheelBottomY = wheelY + vehicle.getWheelRadius();
    float wallTopY = getPosition().y;
    float wallBottomY = wallTopY + base.getSize().y;
    float wallLeftX = getPosition().x;
    float wallRightX = wallLeftX + base.getSize().x;

    bool withinXBounds = checkWithinXBounds(vehicle, wheelX, wallLeftX, wallRightX);
    bool withinYBounds = checkWithinYBounds(wheelBottomY, wallTopY, wheelY, wallBottomY);

    if (withinXBounds && withinYBounds) {
        return true;
    }

    return false;


}


FallingObstacle::FallingObstacle(float baseWidth, float baseHeight, float fallingSpeed, float x, float activationDistance)
    : fallingSpeed(fallingSpeed), inScreen(false), activationDistance(activationDistance), baseWidth(baseWidth),
      baseHeight(baseHeight) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> spikesDis(5, 8); // Number of spikes on the star (odd is recommended)
    int numSpikes = spikesDis(gen);

    // Ensure an odd number of points for symmetry
    if (numSpikes % 2 == 0) {
        numSpikes += 1;
    }

    base.setPointCount(numSpikes * 2); // Double for inner and outer points

    float radiusOuter = baseWidth / 2.0f;  // Outer radius
    float radiusInner = baseHeight / 2.5f; // Inner radius (smaller than outer)
    float centerX = baseWidth / 2.0f;      // Center x
    float centerY = baseHeight / 2.0f;     // Center y

    // Generate star points
    for (int i = 0; i < numSpikes * 2; i++) {
        float angle = i * (360.0f / (numSpikes * 2)); // Divide full circle evenly
        float radians = angle * (3.14159f / 180.0f);  // Convert to radians

        float radius = (i % 2 == 0) ? radiusOuter : radiusInner; // Alternate radii
        float px = centerX + radius * cos(radians);
        float py = centerY + radius * sin(radians);

        base.setPoint(i, sf::Vector2f(px, py));
    }

    base.setPosition(x, -10000);

    base.setFillColor(sf::Color(133, 173, 173));
}

const sf::Vector2f FallingObstacle::getPosition() const {
    return base.getPosition();
}

void FallingObstacle::setPosition(float x, float y) {
    base.setPosition(x,y);
}

const void FallingObstacle::draw(sf::RenderWindow &window) const {
    window.draw(base);
}

void FallingObstacle::setColor(sf::Color color) {
    base.setFillColor(color);
}

void FallingObstacle::setDeltaPosition(float dx, float dy) {
    base.move(dx, dy);
}

bool FallingObstacle::checkCollision(Vehicle& vehicle) {
    return false;
}

float FallingObstacle::getActivationDistance() const {
    return activationDistance;
}

float FallingObstacle::getFallingSpeed() const {
    return fallingSpeed;
}

bool FallingObstacle::isInScreen() const {
    return inScreen;
}

void FallingObstacle::setInScreen(bool val) {
    inScreen = val;
}

void FallingObstacle::fall() {
    if (inScreen) {
    if (base.getPosition().y < -9999){
        base.setPosition(base.getPosition().x, 10);
    }
     base.move(0, fallingSpeed);
    }
}

sf::Vector2f FallingObstacle::getSize() const {
    return sf::Vector2f(baseWidth, baseHeight);
}

void ObstacleContainer::addObstacle(std::unique_ptr<Obstacle> obs){
    obstacles.push_back(std::move(obs));
}

void ObstacleContainer::moveAll(float dx, float dy) const {
    for (auto &obstacle: obstacles){
        obstacle->setDeltaPosition(dx, dy);
    }
}

void ObstacleContainer::drawAll(sf::RenderWindow& window) const {
    for (auto &obstacle: obstacles){
        obstacle->draw(window);
    }
}

void ObstacleContainer::clear() {
    obstacles.clear();
}

bool ObstacleContainer::checkCollision(Vehicle& vehicle) {
    for (auto &obstacle: obstacles){
        if (vehicle.getPosition().x > obstacle->getPosition().x - 200) {

            if (obstacle->checkCollision(vehicle)){
                return true;
            }
        }
    }
    return false;
}


void FallingObstacleContainer::addObstacle(std::unique_ptr<FallingObstacle> obs) {
    obstacles.push_back(std::move(obs));
}


void FallingObstacleContainer::activate(float xPosition) const {
    for (auto& obstacle : obstacles) { // Use a reference here
        if (abs(xPosition - obstacle->getPosition().x) <= obstacle->getActivationDistance()) {
            obstacle->setInScreen(true);
        }
    }
}

void FallingObstacleContainer::clear() {
    obstacles.clear();
}

void FallingObstacleContainer::fallAll() const {
    for (auto& obstacle : obstacles) {
        obstacle->fall();
    }
}

void FallingObstacleContainer::drawAll(sf::RenderWindow &window) const {
    for (auto& obstacle : obstacles) {
        obstacle->draw(window);
    }
}




ObstacleFactory::ObstacleFactory(int numWalls, int numSpikeWalls, int numFallingObjects, float widthScreen,
                                 float heightScreen, float gameTime,
                                 ObstacleContainer* container, FallingObstacleContainer* fallingContainer)
    : numWallsPerScreen(numWalls), numSpikeWallsPerScreen(numSpikeWalls),
      widthScreen(widthScreen), numFallingObjectsPerScreen(numFallingObjects),
      gameTime(gameTime), heightScreen(heightScreen),
      container(container), fallingContainer(fallingContainer) {

      createWalls();
      createFallingObjects();
}


void ObstacleFactory::createWalls() {
    int maxWalls = 30; //hard code
    int gapAccumulator = 0;
    srand( (unsigned)time( NULL ) );
    for (int i = 0; i < maxWalls; i++){
    float r = (float) rand()/RAND_MAX;

    if (r < 0.5){

        container->addObstacle(std::move(createRandomWall(i, 150, 200, gapAccumulator)));
    }else {
        container->addObstacle(std::move(createRandomSpikeWall(i, 90, 100, gapAccumulator)));
    }

    }
}

void ObstacleFactory::createFallingObjects() {
    int maxObjects = 30; //hard code
    int gapAccumulator = 0;

    for (int i = 0; i < maxObjects; i++){
        fallingContainer->addObstacle(std::move(createRandomFallingObject(i, 60, 60, gapAccumulator)));
    }
}


std::unique_ptr<Wall> ObstacleFactory::createRandomWall(int x, int maxWidth, int maxHeight, int& gap) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disWidth(80, maxWidth);
    std::uniform_int_distribution<> disHeight(50, maxHeight);
    std::uniform_int_distribution<> disOffset(200, 400);

    float width = disWidth(gen);
    float height = disHeight(gen);

    float y = heightScreen - height;
    x += gap;
    x += disOffset(gen);

    gap = x + width;


    return std::make_unique<Wall>(height, width, x, y);
}


std::unique_ptr<SpikeWall> ObstacleFactory::createRandomSpikeWall(int x, int maxWidth, int maxHeight, int &gap) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disWidth(maxWidth, maxWidth);
    std::uniform_int_distribution<> disHeight(50, maxHeight);
    std::uniform_int_distribution<> disOffset(200, 400);

    float width = disWidth(gen);
    float height = disHeight(gen);
    float spikeWidth = 10;
    float spikeHeight = 40;


    float y = heightScreen - height;
    x += gap;
    x += disOffset(gen);

    gap = x + width;


    return std::make_unique<SpikeWall>(height, width, x, y, spikeWidth, spikeHeight,
                                        sf::Color(135, 135, 171));
}

std::unique_ptr<FallingObstacle> ObstacleFactory::createRandomFallingObject(int x, int maxWidth, int maxHeight,
int &gap) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disWidth(50, maxWidth);
    std::uniform_int_distribution<> disHeight(50, maxHeight);
    std::uniform_int_distribution<> disOffset(200, 400);
    std::uniform_int_distribution<> xPosOffset(5, 30);

    int xPos = (x + xPosOffset(gen)) * 100;

    float width = disWidth(gen);
    float height = disHeight(gen);

    x += gap;
    x += disOffset(gen);

    gap = x + width;

    //TODO: remove hardcoded values
    return std::make_unique<FallingObstacle>(height, width, 2, xPos, 100);
}




