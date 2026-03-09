#pragma once

#include <vector>
#include <random>

enum class WalkResult {
    FellIntoCliff,
    ReturnedToCafe,
    Unknown
};

struct ProbabilityResult {
    double cliffProbability = 0.0;
    double cafeProbability = 0.0;
    double unknownProbability = 0.0;
};

struct SimulationPoint {
    int B = 0;
    double cliffProbability = 0.0;
    double cafeProbability = 0.0;
    double unknownProbability = 0.0;
};

class Simulation {
    public:
    Simulation();

    WalkResult simulateOneWalk(int B, double p, int maxSteps);
    ProbabilityResult estimateForB(int B, double p, int experiments, int maxSteps);
    std::vector<SimulationPoint> buildSeries(int bLeft, int bRight, double p, int experiments, int maxSteps);

    private:
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_dist;

};