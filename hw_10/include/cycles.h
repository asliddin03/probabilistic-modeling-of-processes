#pragma once

#include "graph.h"

struct CycleInfo {
    int maxLenCycle = 0;
    int edgesInMaxWeightCycle = 0;
    int maxWeightCycle = -1;
};

CycleInfo analyzeCycles(const Graph& g);