#pragma once

#include "config.h"

#include <random>

class DirectionGenerator {
public:
    explicit DirectionGenerator(const AppConfig& config);

    int next();

private:
    int sampleUniform();
    int sampleBinomial();
    int sampleGeometric();
    int sampleTriangular();

private:
    AppConfig m_config;
    std::mt19937 m_rng;
};