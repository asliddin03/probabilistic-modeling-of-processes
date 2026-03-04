#pragma once
#include <vector>

struct Input {
    int AM = 0, RM = 0, n = 0;
    std::vector<int> q;     // 1..n
    int t_sb = 0, t_pr = 0;
    std::vector<int> s;     // 1..n
    int mA = 0, mR = 0;
};

Input read_args(int argc, char** argv);