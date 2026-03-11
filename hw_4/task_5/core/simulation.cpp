#include "simulation.h"

Simulation::Simulation(const SimulationConfig& config)
    : m_config(config),
      m_distribution(config) {}

int Simulation::stepCount(const SimulationConfig& config) {
    return static_cast<int>(std::round(config.M / config.h));
}

bool Simulation::isStrictlyIncreasing(const std::vector<int>& levels) {
    for (size_t i = 1; i < levels.size(); ++i) {
        if (levels[i] <= levels[i - 1])
            return false;
    }
    return true;
}

StaircaseFigure Simulation::generateFigure() {
    StaircaseFigure fig;

    const int count = stepCount(m_config);
    fig.levels.reserve(static_cast<size_t>(count));

    for (int i = 0; i < count; ++i) {
        fig.levels.push_back(m_distribution.sampleLevel());
    }

    fig.isStrictlyIncreasing = isStrictlyIncreasing(fig.levels);
    return fig;
}

SimulationResult Simulation::run() {
    SimulationResult result;
    result.figures.reserve(static_cast<size_t>(m_config.N));

    for (int i = 0; i < m_config.N; ++i) {
        StaircaseFigure fig = generateFigure();

        if (fig.isStrictlyIncreasing)
            ++result.increasingCount;

        result.figures.push_back(std::move(fig));
    }

    result.probability = static_cast<double>(result.increasingCount) /
                         static_cast<double>(m_config.N);

    return result;
}