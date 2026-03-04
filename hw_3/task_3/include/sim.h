#pragma once
#include <vector>
#include "input.h"

struct ResultTimes {
    std::vector<int> times3;
    std::vector<int> times4;
    std::vector<int> times5;
    int finished_time = 0;
};

ResultTimes simulate(const Input& in);