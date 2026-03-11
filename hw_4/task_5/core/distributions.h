#pragma once

#include "types.h"

#include <random>

class DistributionGenerator {
public:
    explicit DistributionGenerator(const SimulationConfig& config);

    int sampleLevel();

private:
    int sampleUniform();
    int sampleBinomial();
    int sampleGeometricFinite();
    int sampleTriangular();

    SimulationConfig m_config;
    std::mt19937 m_rng;
};