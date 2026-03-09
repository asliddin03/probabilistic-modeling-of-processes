#include "simulation.h"
#include <chrono>

Simulation::Simulation()
    : m_rng(static_cast<unsigned int>(
          std::chrono::steady_clock::now().time_since_epoch().count())),
      m_dist(0.0, 1.0)
{}

WalkResult Simulation::simulateOneWalk(int B, double p, int maxSteps) {
    int x = B;

    for (int step = 0; step < maxSteps; ++step)
    {
        double r = m_dist(m_rng);

        if (r < p)
        {
            x += 1;   // шаг от обрыва
        }
        else
        {
            x -= 1;   // шаг к обрыву
        }

        if (x == 0)
        {
            return WalkResult::FellIntoCliff;
        }

        if (x == B)
        {
            return WalkResult::ReturnedToCafe;
        }
    }

    return WalkResult::Unknown;
}

ProbabilityResult Simulation::estimateForB(int B, double p, int experiments, int maxSteps) {
    ProbabilityResult result;

    int cliffCount = 0;
    int cafeCount = 0;
    int unknownCount = 0;

    for (int i = 0; i < experiments; ++i) {
        WalkResult walkResult = simulateOneWalk(B, p, maxSteps);

        switch (walkResult) {
            case WalkResult::FellIntoCliff:
                ++cliffCount;
                break;
            case WalkResult::ReturnedToCafe:
                ++cafeCount;
                break;
            case WalkResult::Unknown:
                ++unknownCount;
                break;
        }
    }

    result.cliffProbability = static_cast<double>(cliffCount) / static_cast<double>(experiments);
    result.cafeProbability = static_cast<double>(cafeCount) / static_cast<double>(experiments);
    result.unknownProbability = static_cast<double>(unknownCount) / static_cast<double>(experiments);

    return result;
}

std::vector<SimulationPoint> Simulation::buildSeries(int bLeft, int bRight, double p, int experiments, int maxSteps) {
    std::vector <SimulationPoint> data;
    for (int B = bLeft; B <= bRight; ++B) {
        ProbabilityResult result = estimateForB(B, p, experiments, maxSteps);
        SimulationPoint point;
        point.B = B;
        point.cliffProbability = result.cliffProbability;
        point.cafeProbability = result.cafeProbability;
        point.unknownProbability = result.unknownProbability;

        data.push_back(point);
    }

    return data;
}
