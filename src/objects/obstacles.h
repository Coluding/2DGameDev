//
// Created by Lukas  Bierling  on 19.11.24.
//


#ifndef OBSTACLES_H
#define OBSTACLES_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual const sf::RectangleShape& getShape() const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setDeltaPosition(float x, float y) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual void setColor(sf::Color) = 0;
};

class Wall : public Obstacle{
private:
    sf::RectangleShape wall;
    sf::Color color;
public:
    Wall(float height, float width, float x, float y, sf::Color color);
    Wall(float height, float width, float x, float y);
    const sf::RectangleShape& getShape() const;
    void setPosition(float x, float y);
    void setDeltaPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setColor(sf::Color);

};

class ObstacleContainer{
private:
    std::vector<Obstacle*> obstacles;
public:
    void addObstacle(Obstacle* obs);

    void moveAll(float dx, float dy);

    void drawAll(sf::RenderWindow& window);

    void clear();

};

#endif //OBSTACLES_H
