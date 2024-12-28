//
// Created by Lukas  Bierling  on 28.12.24.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

#include "objects.h"
#include "../player/character.h"

WinningPortal::WinningPortal(float width, float height, float x, float y) {
    // Portal setup

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    sf::Text portalText("Win Game!", font, 20);
    portalText.setFillColor(sf::Color::White);

    portalText.setPosition(x + width / 2 + distance, y - height + height / 2);

    portal.setSize(sf::Vector2f(width, height));
    portal.setPosition(x + distance, y - height);
    portal.setFillColor(sf::Color(100, 200, 255)); // A soothing blue to represent a portal

    // Shadow setup
    shadow.setSize(sf::Vector2f(width, height));
    shadow.setPosition(x + 10 + distance, y + 10 - height);
    shadow.setFillColor(sf::Color(0, 0, 0, 100)); // Semi-transparent shadow

    // Border setup
    border.setSize(sf::Vector2f(width, height));
    border.setPosition(x + distance, y - height);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White); // White border for emphasis
    border.setOutlineThickness(4); // Thicker border for better visibility

    // Glow effect (optional, animated in update/draw loop)
    glow.setRadius(std::max(width, height) / 2.0f); // Circular glow surrounding the portal
    glow.setPosition(
        x + distance + width / 2.0f - glow.getRadius(),
        y - height / 2.0f - glow.getRadius()
    );
    glow.setFillColor(sf::Color(150, 200, 255, 80)); // Soft glow effect
}

void WinningPortal::draw(sf::RenderWindow &window) const {
    window.draw(glow);    // Draw the glow first, so it's behind other elements
    window.draw(shadow);  // Draw shadow for depth
    window.draw(border);  // Draw border to outline the portal
    window.draw(portal);  // Draw the main portal
    window.draw(portalText); // Draw the text
}
