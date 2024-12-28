//
// Created by Lukas  Bierling  on 19.11.24.
//


#ifndef OBSTACLES_H
#define OBSTACLES_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include "../player/character.h"

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual const void draw(sf::RenderWindow& window) const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setDeltaPosition(float x, float y) = 0;
    virtual const sf::Vector2f getPosition() const = 0;
    virtual void setColor(sf::Color) = 0;
    virtual bool checkCollision(Vehicle& vehicle)  = 0;
    virtual sf::Vector2f getSize() const = 0;
};

class Wall : public Obstacle{
private:
    sf::Color color;
    sf::RectangleShape wall;   // Main wall
    sf::RectangleShape shadow; // Shadow for 3D effect
    sf::RectangleShape border;
public:
    Wall(float height, float width, float x, float y, sf::Color color);
    Wall(float height, float width, float x, float y);
    const void draw(sf::RenderWindow &) const override;
    void setPosition(float x, float y) override;
    void setDeltaPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setColor(sf::Color) override;
    bool checkCollision(Vehicle& vehicle) override;
    ~Wall() override = default;
    sf::Vector2f getSize() const override;

};

class MajorLeftObstacle{

private:
    sf::RectangleShape wall;
    sf::RectangleShape shadow;
    sf::RectangleShape border;
    float movingSpeed = 0.0f;
    float baseWidth;
    float baseHeight;
    std::vector<sf::ConvexShape> spikes;
    std::vector<sf::ConvexShape> spikeShadows;
    std::vector<sf::ConvexShape> spikeBorders;

public:
    MajorLeftObstacle(float baseWidth, float baseHeight, float x, float y, float movingSpeed, float spikeWidth, float spikeHeight);
    void setPosition(float x, float y);
    const sf::Vector2f getPosition() const;
    void setDeltaPosition(float dx, float dy);
    void setColor(sf::Color color);
    bool checkCollision(Vehicle& vehicle);
    const void draw(sf::RenderWindow &window) const;
    ~MajorLeftObstacle() = default;
    sf::Vector2f getSize() const;
    void move();
    void setMovingSpeed(float speed);

};



class SpikeWall : public Obstacle {
private:
    sf::RectangleShape base;
    sf::RectangleShape shadow;
    sf::RectangleShape border;
    std::vector<sf::ConvexShape> spikes;     // Spikes on top of the base
    std::vector<sf::ConvexShape> spikeShadows; // Shadows for the spikes
    std::vector<sf::ConvexShape> spikeBorders; // Borders for the spikes
    float spikeWidth;                        // Width of each spike
    float spikeHeight;                       // Height of each spike

public:
    SpikeWall(float baseWidth, float baseHeight, float x, float y, float spikeWidth, float spikeHeight);

    void setPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setDeltaPosition(float dx, float dy) override;
    void setColor(sf::Color color) override;
    bool checkCollision(Vehicle& vehicle)  override;
    const void draw(sf::RenderWindow &window) const override;
    ~SpikeWall() override = default;
    sf::Vector2f getSize() const override;
};



class FallingObstacle: public Obstacle {
private:
    sf::ConvexShape base;
    sf::ConvexShape shadow;
    float fallingSpeed;
    bool inScreen;
    float activationDistance;
    float baseWidth;
    float baseHeight;


public:
    FallingObstacle(float baseWidth, float baseHeight, float fallingSpeed,float x, float activationDistance);

    void setPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setDeltaPosition(float dx, float dy) override;
    void setColor(sf::Color color) override;
    bool checkCollision(Vehicle& vehicle)  override;
    const void draw(sf::RenderWindow &window) const override;
    ~FallingObstacle() override = default;
    float getActivationDistance() const ;
    float getFallingSpeed() const;
    bool isInScreen() const;
    void setInScreen(bool val);
    void fall();
    sf::Vector2f getSize() const override;
};

class RollingObstacle: public Obstacle {
private:
    sf::CircleShape base;
    sf::CircleShape shadow;
    sf::CircleShape border;
    float xPos;
    float rollingSpeed;
    bool inScreen;
    float activationDistance;
    float baseRadius;

public:
    RollingObstacle(float baseRadius, float rollingSpeed, float y, float x, float activationDistance);
    void setPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setDeltaPosition(float dx, float dy) override;
    void setColor(sf::Color color) override;
    bool checkCollision(Vehicle& vehicle)  override;
    const void draw(sf::RenderWindow &window) const override;
    ~RollingObstacle() override = default;
    float getActivationDistance() const ;
    float getRollingSpeed() const;
    bool isInScreen() const;
    void setInScreen(bool val);
    void roll();
    sf::Vector2f getSize() const override;
};


class ObstacleContainer{
private:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
public:
    void addObstacle(std::unique_ptr<Obstacle> obs);

    void moveAll(float dx, float dy) const;

    void drawAll(sf::RenderWindow& window) const;

    void clear();

    bool checkCollision(Vehicle& vehicle);

    void getLastObstacle(int &x, int& y) const;

};

class FallingObstacleContainer{
private:
std::vector<std::unique_ptr<FallingObstacle>> obstacles;
public:
    void addObstacle(std::unique_ptr<FallingObstacle>);
    void fallAll() const;
    void activate(float xPosition) const;
    void drawAll(sf::RenderWindow& window) const;
    void clear();
    bool checkCollision(Vehicle& vehicle);
};

class RollingObstacleContainer{
private:
    std::vector<std::unique_ptr<RollingObstacle>> obstacles;
public:
    void addObstacle(std::unique_ptr<RollingObstacle>);
    void rollAll();
    void activate(float xPosition) const;
    void drawAll(sf::RenderWindow& window) const;
    void clear();
    bool checkCollision(Vehicle& vehicle);
};


class ObstacleFactory {
private:
    int numWalls;
    int numSpikeWalls;
    int numFallingObjects;
    int numRollingObjects;
    float widthScreen;
    float heightScreen;
    float gameTime;
    ObstacleContainer* container;
    FallingObstacleContainer* fallingContainer;
    RollingObstacleContainer* rollingContainer;

public:
    ObstacleFactory(int numWalls, int numSpikeWalls, int numFallingObjects, float widthScreen,
                    int numRollingObjects, float heightScreen, float gameTime,
                    ObstacleContainer* container, FallingObstacleContainer* fallingContainer,
                    RollingObstacleContainer* rollingContainer);
    void createWalls();
    void createSpikeWalls();
    void createFallingObjects();
    void createRollingObjects();
    void getLastObstacle(int& x, int& y ) const;
    std::unique_ptr<Wall>  createRandomWall(int x, int maxWidth, int maxHeight, int& gap) const;
    std::unique_ptr<SpikeWall> createRandomSpikeWall(int x, int maxWidth, int maxHeight, int& gap) const;
    std::unique_ptr<FallingObstacle> createRandomFallingObject(int x, int maxWidth, int maxHeight, int& gap) const;
    std::unique_ptr<RollingObstacle> createRandomRollingObject(int x, int maxRadius, int minRadius, int& gap) const;

};






#endif //OBSTACLES_H

