#pragma once

#include "config.h"
#include <random>

class DistributionGenerator
{
public:
  explicit DistributionGenerator(const Config& config);

  double generate();

private:
  double generateUniform();
  double generateBinomial();
  double generateFiniteGeometric();
  double generateTriangular();

private:
  Config m_config;
  std::mt19937 m_rng;
};