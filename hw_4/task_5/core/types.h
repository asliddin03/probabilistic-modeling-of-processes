#pragma once

#include <QString>
#include <vector>

struct SimulationConfig {
    double M = 10.0;
    double h = 1.0;
    double tau = 1.0;
    int m = 5;
    int N = 100;

    QString distributionType = "uniform";

    double binomialP = 0.5;
    double geometricP = 0.5;
    int triangularMode = 2;
};

struct StaircaseFigure {
    std::vector<int> levels;
    bool isStrictlyIncreasing = false;
};

struct SimulationResult {

    std::vector<StaircaseFigure> figures;
    int increasingCount = 0;
    double probability = 0.0;
};