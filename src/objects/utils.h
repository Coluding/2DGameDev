//
// Created by Lukas  Bierling  on 30.12.24.
//

#include "obstacles.h"

#ifndef UTILS_H
#define UTILS_H


void checkRightAndLeftEdgeCollision(Vehicle& vehicle, Obstacle& wall,float& wheelX, float& wheelY, float& verticalBodyX, float& verticalBodyY,
        float& horizontalBodyX, float& horizontalBodyY);

bool checkWithinXBounds(Vehicle& vehicle, float wheelX, float wallLeftX, float wallRightX);

bool checkWithinYBounds(float wheelBottomY, float wallTopY, float wheelY, float wallBottomY);


bool doIntervalsOverlap(const std::pair<double, double>& interval1, const std::pair<double, double>& interval2);

#endif //UTILS_H
