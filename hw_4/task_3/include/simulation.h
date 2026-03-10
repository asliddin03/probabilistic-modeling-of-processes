#pragma once

#include "config.h"

struct SimulationResult
{
    int successCount = 0;
    int totalExperiments = 0;
    double probability = 0.0;
};

class Simulation
{
public:
    explicit Simulation(const AppConfig& config);

    bool firstReturnAtStepN();
    SimulationResult run();

private:
    AppConfig m_config;
};