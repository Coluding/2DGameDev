//
// Created by Lukas  Bierling  on 19.11.24.
//


#ifndef OBSTACLES_H
#define OBSTACLES_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual const void draw(sf::RenderWindow& window) const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setDeltaPosition(float x, float y) = 0;
    virtual const sf::Vector2f getPosition() const = 0;
    virtual void setColor(sf::Color) = 0;
    virtual bool checkCollision(sf::Vector2f playerPos, float playerWidth, float playerHeight) = 0;
};

class Wall : public Obstacle{
private:
    sf::RectangleShape wall;
    sf::Color color;
public:
    Wall(float height, float width, float x, float y, sf::Color color);
    Wall(float height, float width, float x, float y);
    const void draw(sf::RenderWindow &) const override;
    void setPosition(float x, float y) override;
    void setDeltaPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setColor(sf::Color) override;
    bool checkCollision(sf::Vector2f playerPos, float playerWidth, float playerHeight) override;
    ~Wall() override = default;

};

class SpikeWall : public Obstacle {
private:
    sf::RectangleShape base;                 // Base platform
    std::vector<sf::ConvexShape> spikes;     // Spikes on top of the base
    float spikeWidth;                        // Width of each spike
    float spikeHeight;                       // Height of each spike

public:
    SpikeWall(float baseWidth, float baseHeight, float x, float y, float spikeWidth, float spikeHeight);
    SpikeWall(float baseWidth, float baseHeight, float x, float y, float spikeWidth, float spikeHeight, sf::Color color);

    void setPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setDeltaPosition(float dx, float dy) override;
    void setColor(sf::Color color) override;
    bool checkCollision(sf::Vector2f playerPos, float playerWidth, float playerHeight) override;
    const void draw(sf::RenderWindow &window) const override;
    ~SpikeWall() override = default;
};

class FallingObstacle: public Obstacle {
private:
    sf::ConvexShape base;
    float fallingSpeed;
    bool inScreen;
    float activationTimeSeconds;


public:
    FallingObstacle(float baseWidth, float baseHeight, float fallingSpeed, float x, float activationTimeSeconds);

    void setPosition(float x, float y) override;
    const sf::Vector2f getPosition() const override;
    void setDeltaPosition(float dx, float dy) override;
    void setColor(sf::Color color) override;
    bool checkCollision(sf::Vector2f playerPos, float playerWidth, float playerHeight) override;
    const void draw(sf::RenderWindow &window) const override;
    ~FallingObstacle() override = default;
    float getActivationTime() const ;
    float getFallingSpeed() const;
    bool isInScreen() const;
    void setInScreen(bool val);
    void fall();
};


class ObstacleContainer{
private:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
public:
    void addObstacle(std::unique_ptr<Obstacle> obs);

    void moveAll(float dx, float dy) const;

    void drawAll(sf::RenderWindow& window) const;

    void clear();

};

class FallingObstacleContainer{
private:
std::vector<std::unique_ptr<FallingObstacle>> obstacles;
public:
    void addObstacle(std::unique_ptr<FallingObstacle>);
    void fallAll() const;
    void activate(float elapsedTimeSeconds) const;
    void drawAll(sf::RenderWindow& window) const;
    void clear();
};


class ObstacleFactory {
private:
    int numWallsPerScreen;
    int numSpikeWallsPerScreen;
    int numFallingObjectsPerScreen;
    float widthScreen;
    float heightScreen;
    float gameTime;
    ObstacleContainer* container;
    FallingObstacleContainer* fallingContainer;

public:
    ObstacleFactory(int numWalls, int numSpikeWalls, int numFallingObjects, float widthScreen,
                    float heightScreen, float gameTime,
                    ObstacleContainer* container, FallingObstacleContainer* fallingContainer);
    void createWalls();
    void createSpikeWalls();
    void createFallingObjects();
    std::unique_ptr<Wall>  createRandomWall(int x, int maxWidth, int maxHeight, int& gap) const;

};

#endif //OBSTACLES_H

