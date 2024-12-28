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

    bool withinXBounds = (wheelX + vehicle.getWheelRadius() > wallLeftX - 10 &&
                          wheelX - vehicle.getWheelRadius() < wallRightX);

    return withinXBounds;
}

bool checkWithinYBounds(float wheelBottomY, float wallTopY, float wheelY, float wallBottomY) {

   bool withinYBounds = (wheelBottomY >= wallTopY && wheelY < wallBottomY);

   return withinYBounds;
}

bool doIntervalsOverlap(const std::pair<double, double>& interval1, const std::pair<double, double>& interval2) {
    return interval1.second >= interval2.first && interval1.first <= interval2.second;
}


Wall::Wall(float height, float width, float x, float y, sf::Color color) {
    shadow.setSize(sf::Vector2f(width, height));
    shadow.setPosition(x + 5, y + 5); // Offset for shadow
    shadow.setFillColor(sf::Color(50, 50, 50, 150)); // Semi-transparent shadow

    // Initialize border
    border.setSize(sf::Vector2f(width, height));
    border.setPosition(x, y);
    border.setFillColor(color);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(100, 100, 150));

    // Initialize main wall
    wall.setSize(sf::Vector2f(width, height));
    wall.setPosition(x, y);
    wall.setFillColor(color);
    }

Wall::Wall(float height, float width, float x, float y) {
    shadow.setSize(sf::Vector2f(width, height));
    shadow.setPosition(x + 5, y + 5); // Offset for shadow
    shadow.setFillColor(sf::Color(50, 50, 50, 150)); // Semi-transparent shadow

    // Initialize border
    border.setSize(sf::Vector2f(width, height));
    border.setPosition(x, y);
    border.setFillColor(color);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(100, 100, 150));

    // Initialize main wall
    wall.setSize(sf::Vector2f(width, height));
    wall.setPosition(x, y);
    wall.setFillColor(sf::Color(230, 247, 255));
}

void Wall::setPosition(float x, float y) {
    shadow.setPosition(x + 5, y + 5); // Adjust shadow position
    border.setPosition(x, y);        // Adjust border position
    wall.setPosition(x, y);
}

void Wall::setDeltaPosition(float dx, float dy) {
    shadow.move(dx, dy);
    border.move(dx, dy);
    wall.move(dx, dy);
}


const sf::Vector2f Wall::getPosition() const {
    return wall.getPosition();
}

const void Wall::draw(sf::RenderWindow &window) const {
    window.draw(shadow); // Draw shadow first
    window.draw(border); // Draw border second
    window.draw(wall);   // Draw main wall last
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
    sf::Color color = sf::Color(230, 247, 255);
    // Initialize the base platform
    base.setSize(sf::Vector2f(baseWidth, baseHeight + 100));
    base.setPosition(x, y);
    base.setFillColor(color);
    shadow.setSize(sf::Vector2f(baseWidth, baseHeight + 100));
    shadow.setPosition(x + 5, y + 105); // Offset for shadow
    shadow.setFillColor(color);

    border.setSize(sf::Vector2f(baseWidth, baseHeight + 100));
    border.setPosition(x, y);
    //border.setFillColor(color);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(100, 100, 150));

    // Create spikes on top of the base
    int numSpikes = static_cast<int>(baseWidth / spikeWidth);
    for (int i = 0; i < numSpikes; ++i) {
        sf::ConvexShape spike(3);
        spike.setPoint(0, sf::Vector2f(x + i * spikeWidth, y));                         // Bottom-left
        spike.setPoint(1, sf::Vector2f(x + (i + 0.5f) * spikeWidth, y - spikeHeight)); // Peak
        spike.setPoint(2, sf::Vector2f(x + (i + 1) * spikeWidth, y));                  // Bottom-right
        spike.setFillColor(sf::Color(225, 225, 234)); // Light color for spikes
        spikes.push_back(spike);

        // Shadow for spike
        sf::ConvexShape spikeShadow(3);
        spikeShadow.setPoint(0, sf::Vector2f(x + i * spikeWidth + 3, y + 3));                         // Bottom-left (offset)
        spikeShadow.setPoint(1, sf::Vector2f(x + (i + 0.5f) * spikeWidth + 3, y - spikeHeight + 3)); // Peak (offset)
        spikeShadow.setPoint(2, sf::Vector2f(x + (i + 1) * spikeWidth + 3, y + 3));                  // Bottom-right (offset)
        spikeShadow.setFillColor(sf::Color(100, 100, 100, 100)); // Darker and semi-transparent color for shadow
        spikeShadows.push_back(spikeShadow);

        //Border for spike
        sf::ConvexShape spikeBorder(3);
        spikeBorder.setPoint(0, sf::Vector2f(x + i * spikeWidth, y));                         // Bottom-left
        spikeBorder.setPoint(1, sf::Vector2f(x + (i + 0.5f) * spikeWidth, y - spikeHeight)); // Peak
        spikeBorder.setPoint(2, sf::Vector2f(x + (i + 1) * spikeWidth, y));                  // Bottom-right
        spikeBorder.setOutlineThickness(2.0f);
        spikeBorder.setOutlineColor(sf::Color(100, 100, 150));
        spikeBorders.push_back(spikeBorder);
    }
}

const void SpikeWall::draw(sf::RenderWindow &window) const {
    window.draw(base);
    window.draw(shadow);
    window.draw(border);

    for (size_t i = 0; i < spikes.size(); ++i) {
        window.draw(spikes[i]);
        window.draw(spikeShadows[i]);
        window.draw(spikeBorders[i]);
    }
}


sf::Vector2f SpikeWall::getSize() const {
    return base.getSize();
}

void SpikeWall::setPosition(float x, float y) {
    // Update base position
    base.setPosition(x, y);
    shadow.setPosition(x + 5, y + 5); // Adjust shadow position
    border.setPosition(x, y);        // Adjust border position


    // Update spike positions relative to the new base position
    for (size_t i = 0; i < spikes.size(); ++i) {
        float spikeX = x + i * spikeWidth;
        float spikeY = y;
        spikes[i].setPoint(0, sf::Vector2f(spikeX, spikeY));
        spikes[i].setPoint(1, sf::Vector2f(spikeX + 0.5f * spikeWidth, spikeY - spikeHeight));
        spikes[i].setPoint(2, sf::Vector2f(spikeX + spikeWidth, spikeY));

        spikeShadows[i].setPoint(0, sf::Vector2f(spikeX + 3, spikeY + 3));
        spikeShadows[i].setPoint(1, sf::Vector2f(spikeX + 0.5f * spikeWidth + 3, spikeY - spikeHeight + 3));
        spikeShadows[i].setPoint(2, sf::Vector2f(spikeX + spikeWidth + 3, spikeY + 3));

        spikeBorders[i].setPoint(0, sf::Vector2f(spikeX, spikeY));
        spikeBorders[i].setPoint(1, sf::Vector2f(spikeX + 0.5f * spikeWidth, spikeY - spikeHeight));
        spikeBorders[i].setPoint(2, sf::Vector2f(spikeX + spikeWidth, spikeY));
    }
}

void SpikeWall::setDeltaPosition(float dx, float dy) {
    // Move base
    base.move(dx, dy);
    shadow.move(dx, dy);
    border.move(dx, dy);

    // Move spikes
    for (size_t i = 0; i < spikeShadows.size(); ++i){
        spikes[i].move(dx, dy);
        spikeShadows[i].move(dx, dy);
        spikeBorders[i].move(dx, dy);
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
    shadow.setPointCount(numSpikes * 2);

    float radiusOuter = baseWidth / 2.0f;  // Outer radius
    float radiusInner = baseHeight / 2.5f; // Inner radius (smaller than outer)
    float centerX = baseWidth / 2.0f;      // Center x
    float centerY = baseHeight / 2.0f;     // Center y

    // Offset for the shadow
    float shadowOffsetX = 5.0f;
    float shadowOffsetY = 5.0f;

    // Generate star points and shadow points
    for (int i = 0; i < numSpikes * 2; i++) {
        float angle = i * (360.0f / (numSpikes * 2)); // Divide full circle evenly
        float radians = angle * (3.14159f / 180.0f);  // Convert to radians

        float radius = (i % 2 == 0) ? radiusOuter : radiusInner; // Alternate radii

        // Main star points
        float px = centerX + radius * cos(radians);
        float py = centerY + radius * sin(radians);
        base.setPoint(i, sf::Vector2f(px, py));

        // Shadow points (offset)
        float shadowPx = px + shadowOffsetX;
        float shadowPy = py + shadowOffsetY;
        shadow.setPoint(i, sf::Vector2f(shadowPx, shadowPy));
    }

    base.setPosition(x, -10000);
    shadow.setPosition(x, -10000);

    base.setFillColor(sf::Color(230, 247, 255));
    shadow.setFillColor(sf::Color(100, 100, 100, 100)); // Semi-transparent dark color for shadow

    // Add a border outline to the base
    base.setOutlineThickness(2.0f);
    base.setOutlineColor(sf::Color(50, 80, 80));
}

const sf::Vector2f FallingObstacle::getPosition() const {
    return base.getPosition();
}

void FallingObstacle::setPosition(float x, float y) {
    base.setPosition(x,y);
}

const void FallingObstacle::draw(sf::RenderWindow &window) const {
    window.draw(base);
    window.draw(shadow);
}

void FallingObstacle::setColor(sf::Color color) {
    base.setFillColor(color);
}

void FallingObstacle::setDeltaPosition(float dx, float dy) {
    base.move(dx, dy);
    shadow.move(dx, dy);
}

bool FallingObstacle::checkCollision(Vehicle& vehicle) {

    float wheelX = 0.0f, wheelY = 0.0f, verticalBodyX = 0.0f, verticalBodyY = 0.0f,
          horizontalBodyX = 0.0f, horizontalBodyY = 0.0f;

   vehicle.getFullPosition(wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);

    std::pair<double, double> vehicleXInterval = std::pair<double, double> {horizontalBodyX - vehicle.widthHorizontal / 2,verticalBodyX + vehicle.widthHorizontal / 2};
    std::pair<double, double> obstacleXInterval = std::pair<double, double> {base.getPosition().x, base.getPosition().x + baseWidth};

    bool xOverlap = doIntervalsOverlap(vehicleXInterval, obstacleXInterval);
    bool yOverlap = wheelY - vehicle.heightVertical * 3 < base.getPosition().y && wheelY > base.getPosition().y;

    if (xOverlap && yOverlap) {
        return true;
    }

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


RollingObstacle::RollingObstacle(float baseRadius, float rollingSpeed, float y, float x, float activationDistance) {
    base.setRadius(baseRadius);
    shadow.setRadius(baseRadius);
    border.setRadius(baseRadius);

    base.setPosition(100000, 200);
    shadow.setPosition(100000, y);
    border.setPosition(100000, y);

    base.setFillColor(sf::Color(230, 247, 255));
    shadow.setFillColor(sf::Color(100, 100, 100, 100));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(50, 80, 80));

    this -> rollingSpeed = rollingSpeed;
    this -> activationDistance = activationDistance;
    this -> baseRadius = baseRadius;
    this -> inScreen = false;
    this -> xPos = x;
}


void RollingObstacle::setPosition(float x, float y) {
    base.setPosition(x, y);
    shadow.setPosition(x + 5, y + 5); // Adjust shadow position
    border.setPosition(x, y);        // Adjust border position
}

void RollingObstacle::setDeltaPosition(float dx, float dy) {
    base.move(dx, dy);
    shadow.move(dx, dy);
    border.move(dx, dy);
}

const sf::Vector2f RollingObstacle::getPosition() const {
    return sf::Vector2f(xPos, base.getPosition().y);
}

const void RollingObstacle::draw(sf::RenderWindow &window) const {
    window.draw(base);
    window.draw(shadow);
    window.draw(border);
}

void RollingObstacle::setColor(sf::Color color) {
    base.setFillColor(color);
}

void RollingObstacle::roll() {
    if (inScreen) {
        base.move(-rollingSpeed, 0);
        shadow.move(-rollingSpeed, 0);
         border.move(-rollingSpeed, 0);
    }
}

float RollingObstacle::getActivationDistance() const {
    return activationDistance;
}

float RollingObstacle::getRollingSpeed() const {
    return rollingSpeed;
}

sf::Vector2f RollingObstacle::getSize() const {
    return sf::Vector2f(baseRadius, baseRadius);
}

bool RollingObstacle::isInScreen() const {
    return inScreen;
}

void RollingObstacle::setInScreen(bool val) {
    if (!isInScreen()) {
       base.setPosition(xPos, base.getPosition().y);
         shadow.setPosition(xPos + 5, base.getPosition().y + 5);
            border.setPosition(xPos, base.getPosition().y);
    }
    inScreen = val;
}

bool RollingObstacle::checkCollision(Vehicle &vehicle) {
    float wheelX = 0.0f, wheelY = 0.0f, verticalBodyX = 0.0f, verticalBodyY = 0.0f,
          horizontalBodyX = 0.0f, horizontalBodyY = 0.0f;

    vehicle.getFullPosition(wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);

    std::pair<double, double> vehicleXInterval = std::pair<double, double> {horizontalBodyX - vehicle.widthHorizontal / 2,verticalBodyX + vehicle.widthHorizontal / 2};
    std::pair<double, double> obstacleXInterval = std::pair<double, double> {base.getPosition().x - baseRadius, base.getPosition().x + baseRadius};

    bool xOverlap = doIntervalsOverlap(vehicleXInterval, obstacleXInterval);
    bool yOverlap = wheelY - vehicle.heightVertical * 3 < base.getPosition().y && wheelY > base.getPosition().y;

    if (xOverlap && yOverlap) {
        return true;
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

void ObstacleContainer::getLastObstacle(int &x, int& y) const {
    Obstacle& lastObstacle = *obstacles.back();
    x = lastObstacle.getPosition().x;
    y = lastObstacle.getPosition().y;

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


bool FallingObstacleContainer::checkCollision(Vehicle& vehicle) {
    for (auto& obstacle: obstacles) {
        if (vehicle.getPosition().x > obstacle->getPosition().x - 200) {
            if (obstacle->checkCollision(vehicle)){
                return true;
            }
        }
    }
}

void RollingObstacleContainer::addObstacle(std::unique_ptr<RollingObstacle> obs) {
    obstacles.push_back(std::move(obs));
}

void RollingObstacleContainer::activate(float xPosition) const {
    for (auto& obstacle : obstacles) {
        if (abs(xPosition - obstacle->getPosition().x) <= obstacle->getActivationDistance()) {
            obstacle->setInScreen(true);
        }
    }
}

void RollingObstacleContainer::clear() {
    obstacles.clear();
}

bool RollingObstacleContainer::checkCollision(Vehicle &vehicle) {
    for (auto& obstacle: obstacles) {
        if (vehicle.getPosition().x > obstacle->getPosition().x - 200) {
            if (obstacle->checkCollision(vehicle)){
                return true;
            }
        }
    }
}

void RollingObstacleContainer::drawAll(sf::RenderWindow &window) const {
    for (auto& obstacle : obstacles) {
        obstacle->draw(window);
    }
}


void RollingObstacleContainer::rollAll() {
    for (auto& obstacle : obstacles) {
        obstacle->roll();
    }
}



ObstacleFactory::ObstacleFactory(int numWalls, int numSpikeWalls, int numFallingObjects, float widthScreen,
                                 int numRollingObjects, float heightScreen, float gameTime,
                                 ObstacleContainer* container, FallingObstacleContainer* fallingContainer,
                                 RollingObstacleContainer* rollingContainer)
    : numWalls(numWalls), numSpikeWalls(numSpikeWalls),
      widthScreen(widthScreen), numFallingObjects(numFallingObjects), numRollingObjects(numRollingObjects),
      gameTime(gameTime), heightScreen(heightScreen),
      container(container), fallingContainer(fallingContainer), rollingContainer(rollingContainer) {

      createWalls();
      createFallingObjects();
      createRollingObjects();
}


void ObstacleFactory::createWalls() {
    int maxWalls = numWalls + numSpikeWalls;

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
    int maxObjects = numFallingObjects;
    int gapAccumulator = 0;

    for (int i = 0; i < maxObjects; i++){
        fallingContainer->addObstacle(std::move(createRandomFallingObject(i, 60, 60, gapAccumulator)));
    }
}

void ObstacleFactory::createRollingObjects() {
    int maxObjects = numRollingObjects;
    int gapAccumulator = 0;

    for (int i = 0; i < maxObjects; i++){
        rollingContainer->addObstacle(std::move(createRandomRollingObject(i, 60, 60, gapAccumulator)));
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
    float spikeHeight = 20;


    float y = heightScreen - height;
    x += gap;
    x += disOffset(gen);

    gap = x + width;


    return std::make_unique<SpikeWall>(height, width, x, y, spikeWidth, spikeHeight);
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

std::unique_ptr<RollingObstacle> ObstacleFactory::createRandomRollingObject(int x, int maxRadius, int minRadius, int& gap) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disRadius(minRadius, maxRadius);
    std::uniform_int_distribution<> disOffset(200, 400);
    std::uniform_int_distribution<> xPosOffset(5, 30);

    int xPos = (x + xPosOffset(gen)) * 100;

    float radius = disRadius(gen);

    x += gap;
    x += disOffset(gen);

    gap = x + radius;

    return std::make_unique<RollingObstacle>(radius, 2, heightScreen, xPos, 300);

}

void ObstacleFactory::getLastObstacle(int &x, int &y) const {
    container->getLastObstacle(x, y);
}

MajorLeftObstacle::MajorLeftObstacle(float baseWidth, float baseHeight, float x, float y, float movingSpeed,
float spikeWidth, float spikeHeight):
 baseHeight(baseHeight),     baseWidth(baseWidth), movingSpeed(movingSpeed){

    shadow.setSize(sf::Vector2f(baseWidth, baseHeight));
    shadow.setPosition(x + 5, y + 5); // Offset for shadow
    shadow.setFillColor(sf::Color(50, 50, 50, 150)); // Semi-transparent shadow

    // Initialize border
    border.setSize(sf::Vector2f(baseWidth, baseHeight));
    border.setPosition(x, y);
    border.setOutlineThickness(2.0f);
    border.setOutlineColor(sf::Color(100, 100, 150));

    // Initialize main wall
    wall.setSize(sf::Vector2f(baseWidth, baseHeight));
    wall.setPosition(x, y);
    wall.setFillColor(sf::Color(230, 247, 255));


    //create spikes on right side of wall
    int numSpikes = baseHeight / 4;

    for (int i = 0; i < numSpikes; i++){
        sf::ConvexShape spike(3);

        spike.setPoint(0, sf::Vector2f((x + baseWidth), y + i * spikeWidth));                         // Bottom-left
        spike.setPoint(1, sf::Vector2f((x + baseWidth) + spikeHeight, y + (i + 0.5f) * spikeWidth)); // Peak
        spike.setPoint(2, sf::Vector2f((x + baseWidth), y +  (i + 1) * spikeWidth));                  // Bottom-right
        spike.setFillColor(sf::Color(225, 225, 234)); // Light color for spikes
        spikes.push_back(spike);

        // Shadow for spike
        sf::ConvexShape spikeShadow(3);
        spikeShadow.setPoint(0, sf::Vector2f((x + baseWidth) + 3, y + i * spikeWidth + 3));                         // Bottom-left (offset)
        spikeShadow.setPoint(1, sf::Vector2f((x + baseWidth) + spikeHeight + 3, y + (i + 0.5f) * spikeWidth + 3)); // Peak (offset)
        spikeShadow.setPoint(2, sf::Vector2f((x + baseWidth) + 3, y +  (i + 1) * spikeWidth + 3));                  // Bottom-right (offset)
        spikeShadow.setFillColor(sf::Color(100, 100, 100, 100)); // Darker and semi-transparent color for shadow
        spikeShadows.push_back(spikeShadow);

        //Border for spike
        sf::ConvexShape spikeBorder(3);
        spikeBorder.setPoint(0, sf::Vector2f((x + baseWidth), y + i * spikeWidth));                         // Bottom-left
        spikeBorder.setPoint(1, sf::Vector2f((x + baseWidth) + spikeHeight, y + (i + 0.5f) * spikeWidth)); // Peak
        spikeBorder.setPoint(2, sf::Vector2f((x + baseWidth), y +  (i + 1) * spikeWidth));                  // Bottom-right
        spikeBorder.setOutlineThickness(2.0f);
        spikeBorder.setOutlineColor(sf::Color(100, 100, 150));
        spikeBorders.push_back(spikeBorder);


    }



}


const void MajorLeftObstacle::draw(sf::RenderWindow &window) const {
    window.draw(wall);
    window.draw(shadow);
    window.draw(border);

    for (size_t i = 0; i < spikes.size(); ++i) {
        window.draw(spikes[i]);
        window.draw(spikeShadows[i]);
        window.draw(spikeBorders[i]);
    }
}

const sf::Vector2f MajorLeftObstacle::getPosition() const {
    return wall.getPosition();
}

sf::Vector2f MajorLeftObstacle::getSize() const {
    return wall.getSize();
}

bool MajorLeftObstacle::checkCollision(Vehicle &vehicle) {

    float wheelX = 0.0f, wheelY = 0.0f, verticalBodyX = 0.0f, verticalBodyY = 0.0f,
          horizontalBodyX = 0.0f, horizontalBodyY = 0.0f;

    vehicle.getFullPosition(wheelX, wheelY, verticalBodyX, verticalBodyY, horizontalBodyX, horizontalBodyY);

    std::pair<double, double> vehicleXInterval = std::pair<double, double> {horizontalBodyX - vehicle.widthHorizontal / 2,verticalBodyX + vehicle.widthHorizontal / 2};
    std::pair<double, double> obstacleXInterval = std::pair<double, double> {wall.getPosition().x, wall.getPosition().x + baseWidth};

    return  doIntervalsOverlap(vehicleXInterval, obstacleXInterval);
}

void MajorLeftObstacle::setPosition(float x, float y) {
}

void MajorLeftObstacle::setColor(sf::Color color) {

}

void MajorLeftObstacle::move() {
    setDeltaPosition(movingSpeed, 0);
}

void MajorLeftObstacle::setMovingSpeed(float speed) {
    movingSpeed = speed;
}


void MajorLeftObstacle::setDeltaPosition(float dx, float dy) {
    wall.move(dx, dy);
    shadow.move(dx, dy);
    border.move(dx, dy);

    for (size_t i = 0; i < spikeShadows.size(); ++i){
        spikes[i].move(dx, dy);
        spikeShadows[i].move(dx, dy);
        spikeBorders[i].move(dx, dy);
    }
}







