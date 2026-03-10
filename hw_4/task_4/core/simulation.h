#pragma once

#include "config.h"
#include "types.h"

#include <vector>

struct SimulationSummary {
    std::vector<Trajectory> trajectories;
    int returnedCount = 0;
};

class Simulator {
public:
    explicit Simulator(const AppConfig& config);

    Trajectory runSingle();
    SimulationSummary runAll();

private:
    bool isOrigin(const Point& p) const;

    AppConfig m_config;
};