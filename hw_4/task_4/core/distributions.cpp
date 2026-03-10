#include "distributions.h"

DirectionGenerator::DirectionGenerator(const AppConfig& config)
    : m_config(config),
      m_rng(std::random_device{}()) {}

int DirectionGenerator::next() {
    switch (m_config.distribution) {
        case DistributionType::Uniform:
            return sampleUniform();
        case DistributionType::Binomial:
            return sampleBinomial();
        case DistributionType::Geometric:
            return sampleGeometric();
        case DistributionType::Triangular:
            return sampleTriangular();
    }

    return sampleUniform();
}

int DirectionGenerator::sampleUniform() {
    std::uniform_int_distribution<int> dist(0, m_config.n - 1);
    return dist(m_rng);
}

int DirectionGenerator::sampleBinomial() {
    std::binomial_distribution<int> dist(m_config.n - 1, m_config.binomialP);
    return dist(m_rng);
}

int DirectionGenerator::sampleGeometric() {
    std::vector<double> weights(m_config.n, 0.0);

    const double q = m_config.geometricQ;
    double sum = 0.0;

    for (int k = 0; k < m_config.n; ++k) {
        weights[k] = q * std::pow(1.0 - q, k);
        sum += weights[k];
    }

    for (double& w : weights)
        w /= sum;

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(m_rng);
}

int DirectionGenerator::sampleTriangular() {
    std::vector<double> weights(m_config.n, 0.0);
    const int mode = m_config.triangularMode;

    for (int k = 0; k < m_config.n; ++k) {
        weights[k] = static_cast<double>(m_config.n - std::abs(k - mode));
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return dist(m_rng);
}