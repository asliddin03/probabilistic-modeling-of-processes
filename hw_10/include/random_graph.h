#pragma once

#include "graph.h"
#include <random>

Graph generateRandomGraph(int n, double p, std::mt19937_64& rng);