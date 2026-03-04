#pragma once
#include <vector>
#include <random>
#include "core/tree_types.h"

// Настройки модели
enum class ChildDistributionType {
    Uniform,
    LeftBiased
};

struct SimulationConfig {
    int M = 2;
    int H = 5;

    // Вероятность "залипнуть" в любом внутреннем узле
    double pStop = 0.15;

    // Закон выбора потомка
    ChildDistributionType distType = ChildDistributionType::Uniform;

    // Параметр перекоса (0..0.99). Чем ближе к 1, тем чаще выбирается childNo=0
    double bias = 0.40;
};

// Результат одного прогона
struct RunResult {
    std::vector<NodeKey> path;
    bool endedInLeaf = false;
    NodeKey endNode;
};

// Симулятор
class Simulator {
public:
    explicit Simulator(unsigned seed = std::random_device{}());

    RunResult runOne(const SimulationConfig& cfg);

    // Утилита: попадание в целевой лист
    static bool isTargetLeaf(const RunResult& rr, int H, int targetLeafIndex);

private:
    std::mt19937 rng_;

    std::vector<double> makeChildProbs(const SimulationConfig& cfg) const;
};