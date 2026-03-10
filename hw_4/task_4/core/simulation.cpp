#include "simulation.h"
#include "distributions.h"

namespace {
    constexpr double PI = 3.14159265358979323846;
    constexpr double EPS = 1e-9;
}

Simulator::Simulator(const AppConfig& config)
    : m_config(config) {}

bool Simulator::isOrigin(const Point& p) const {
    return std::abs(p.x) < EPS && std::abs(p.y) < EPS;
}

Trajectory Simulator::runSingle() {
    DirectionGenerator generator(m_config);

    Trajectory result;
    Point current {0.0, 0.0};
    result.points.push_back(current);

    for (int step = 1; step <= m_config.K; ++step)
    {
        const int xi = generator.next();
        const double angle = 2.0 * PI * static_cast<double>(xi) / static_cast<double>(m_config.n);

        current.x += m_config.rho * std::cos(angle);
        current.y += m_config.rho * std::sin(angle);

        if (std::abs(current.x) < EPS) current.x = 0.0;
        if (std::abs(current.y) < EPS) current.y = 0.0;

        result.points.push_back(current);

        if (isOrigin(current))
        {
            result.returnedToOrigin = true;
            result.returnStep = step;
            break;
        }
    }

    return result;
}

SimulationSummary Simulator::runAll() {
    SimulationSummary summary;

    for (int i = 0; i < m_config.M; ++i)
    {
        Trajectory tr = runSingle();
        if (tr.returnedToOrigin)
            ++summary.returnedCount;

        summary.trajectories.push_back(std::move(tr));
    }

    return summary;
}