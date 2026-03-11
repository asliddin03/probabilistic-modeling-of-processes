#include "distributions.h"

#include <stdexcept>

DistributionGenerator::DistributionGenerator(const SimulationConfig& config)
    : m_config(config),
      m_rng(std::random_device{}()) {}

int DistributionGenerator::sampleLevel() {
    if (m_config.distributionType == "uniform")
        return sampleUniform();

    if (m_config.distributionType == "binomial")
        return sampleBinomial();

    if (m_config.distributionType == "geometric")
        return sampleGeometricFinite();

    if (m_config.distributionType == "triangular")
        return sampleTriangular();

    throw std::runtime_error("Unknown distribution type");
}

int DistributionGenerator::sampleUniform() {
    std::uniform_int_distribution<int> dist(0, m_config.m);
    return dist(m_rng);
}

int DistributionGenerator::sampleBinomial() {
    std::binomial_distribution<int> dist(m_config.m, m_config.binomialP);
    return dist(m_rng);
}

int DistributionGenerator::sampleGeometricFinite() {
    std::vector<double> weights;
    weights.reserve(static_cast<size_t>(m_config.m + 1));

    const double p = m_config.geometricP;

    for (int k = 0; k <= m_config.m; ++k) {
        double w = std::pow(1.0 - p, k) * p;
        weights.push_back(w);
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(m_rng);
}

int DistributionGenerator::sampleTriangular() {
    std::vector<double> weights;
    weights.reserve(static_cast<size_t>(m_config.m + 1));

    const int mode = m_config.triangularMode;

    for (int k = 0; k <= m_config.m; ++k) {
        if (k <= mode)
            weights.push_back(static_cast<double>(k + 1));
        else
            weights.push_back(static_cast<double>(m_config.m - k + 1));
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(m_rng);
}