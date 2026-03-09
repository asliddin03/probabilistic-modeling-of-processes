#pragma once

#include "config.h"
#include "point.h"
#include <QVector>

struct SimulationResult {
    QVector<Point> trajectory;
    int crossings = 0;
    double estimatedProbability = 0.0;
};

class Simulation {
    public:
    explicit Simulation(const Config& config);

    QVector<Point> generateTrajectory();
    int countCrossings(const QVector<Point>& trajectory) const;
    double estimateProbability();
    SimulationResult runFullSimulation();

private:
    Config m_config;
};