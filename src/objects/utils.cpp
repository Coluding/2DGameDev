//
// Created by Lukas  Bierling  on 30.12.24.
//
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


#include "obstacles.h"
#include "utils.h"

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
