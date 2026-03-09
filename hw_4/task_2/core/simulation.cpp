#include "simulation.h"
#include "distribution.h"

Simulation::Simulation(const Config& config)
    : m_config(config)
{
}

QVector<Point> Simulation::generateTrajectory()
{
    QVector<Point> points;
    points.reserve(m_config.steps + 1);

    DistributionGenerator generator(m_config);

    double x = 0.0;
    double y = m_config.Y;

    points.push_back({x, y});

    for (int i = 0; i < m_config.steps; ++i)
    {
        const double s = generator.generate();

        x += m_config.h;
        y += s;

        points.push_back({x, y});
    }

    return points;
}

int Simulation::countCrossings(const QVector<Point>& trajectory) const
{
    int crossings = 0;

    for (int i = 1; i < trajectory.size(); ++i)
    {
        const double yPrev = trajectory[i - 1].y;
        const double yCurr = trajectory[i].y;

        if (yPrev * yCurr < 0.0)
            ++crossings;
    }

    return crossings;
}

double Simulation::estimateProbability()
{
    int countL = 0;

    for (int i = 0; i < m_config.experiments; ++i)
    {
        const QVector<Point> tr = generateTrajectory();
        const int crossings = countCrossings(tr);

        if (crossings == m_config.l)
            ++countL;
    }

    return static_cast<double>(countL) / static_cast<double>(m_config.experiments);
}

SimulationResult Simulation::runFullSimulation()
{
    SimulationResult result;
    result.trajectory = generateTrajectory();
    result.crossings = countCrossings(result.trajectory);
    result.estimatedProbability = estimateProbability();
    return result;
}