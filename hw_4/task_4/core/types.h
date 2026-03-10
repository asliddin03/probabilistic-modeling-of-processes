#pragma once

#include <vector>
#include <QString>

struct Point {
    double x = 0.0;
    double y = 0.0;
};

struct Trajectory {
    std::vector<Point> points;
    bool returnedToOrigin = false;
    int returnStep = -1;
};