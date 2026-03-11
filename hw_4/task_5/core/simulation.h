#pragma once

#include "types.h"
#include "distributions.h"

class Simulation {
public:
    explicit Simulation(const SimulationConfig& config);

    SimulationResult run();

    static int stepCount(const SimulationConfig& config);
    static bool isStrictlyIncreasing(const std::vector<int>& levels);

private:
    StaircaseFigure generateFigure();

    SimulationConfig m_config;
    DistributionGenerator m_distribution;
};