#include "distribution.h"

#include <stdexcept>

DistributionGenerator::DistributionGenerator(const Config& config)
    : m_config(config),
      m_rng(std::random_device{}())
{
}

double DistributionGenerator::generate()
{
    switch (m_config.distributionType)
    {
        case DistributionType::Uniform:
            return generateUniform();
        case DistributionType::Binomial:
            return generateBinomial();
        case DistributionType::FiniteGeometric:
            return generateFiniteGeometric();
        case DistributionType::Triangular:
            return generateTriangular();
    }

    throw std::runtime_error("Unsupported distribution");
}

double DistributionGenerator::generateUniform()
{
    std::uniform_int_distribution<int> dist(0, m_config.sValues.size() - 1);
    return m_config.sValues[dist(m_rng)];
}

double DistributionGenerator::generateBinomial()
{
    std::binomial_distribution<int> dist(m_config.trials, m_config.p);
    return static_cast<double>(dist(m_rng) - m_config.shift);
}

double DistributionGenerator::generateFiniteGeometric()
{
    const int n = m_config.sValues.size();
    std::vector<double> weights(n);

    for (int i = 0; i < n; ++i)
        weights[i] = std::pow(1.0 - m_config.p, i);

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return m_config.sValues[dist(m_rng)];
}

double DistributionGenerator::generateTriangular()
{
    const int n = m_config.sValues.size();
    std::vector<double> weights(n);

    for (int i = 0; i < n; ++i)
    {
        const int left = i + 1;
        const int right = n - i;
        weights[i] = std::min(left, right);
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return m_config.sValues[dist(m_rng)];
}