//
// Created by Lukas  Bierling  on 28.12.24.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include "../player/character.h"

#ifndef OBJECTS_H
#define OBJECTS_H

class WinningPortal {
private:
    sf::RectangleShape portal;
    sf::RectangleShape shadow;
    sf::RectangleShape border;
    sf::Color color;
    float distance;
    sf::CircleShape glow;
    sf::Text portalText;
    sf::Font font;

public:
    WinningPortal(float width, float height, float x, float y);
    bool checkInPortal(Vehicle& vehicle);
    void draw(sf::RenderWindow& window) const;

};

#endif //OBJECTS_H
