#include "core/simulator.h"
#include <cmath>

Simulator::Simulator(unsigned seed)
    : rng_(seed) {}

std::vector<double> Simulator::makeChildProbs(const SimulationConfig& cfg) const {
    std::vector<double> q(cfg.M, 0.0);

    if (cfg.distType == ChildDistributionType::Uniform) {
        for (int i = 0; i < cfg.M; ++i) q[i] = 1.0 / cfg.M;
        return q;
    }

    // LeftBiased
    // w_i = (1-bias)^i
    const double bias = cfg.bias;
    double sum = 0.0;
    for (int i = 0; i < cfg.M; ++i) {
        q[i] = std::pow(1.0 - bias, i);
        sum += q[i];
    }
    for (int i = 0; i < cfg.M; ++i) q[i] /= sum;
    return q;
}

RunResult Simulator::runOne(const SimulationConfig& cfg) {
    RunResult rr;
    rr.path.reserve(cfg.H + 2);

    NodeKey cur{0, 0};
    rr.path.push_back(cur);

    std::uniform_real_distribution<double> U(0.0, 1.0);

    const auto probs = makeChildProbs(cfg);
    std::discrete_distribution<int> childDist(probs.begin(), probs.end());

    while (true) {
        // Если лист
        if (cur.level == cfg.H) {
            rr.endedInLeaf = true;
            rr.endNode = cur;
            return rr;
        }

        // Внутренний узел — может залипнуть
        if (U(rng_) < cfg.pStop) {
            rr.endedInLeaf = false;
            rr.endNode = cur;
            return rr;
        }

        // Иначе выбираем потомка
        int c = childDist(rng_);
        cur = childOf(cur, c, cfg.M);
        rr.path.push_back(cur);
    }
}

bool Simulator::isTargetLeaf(const RunResult& rr, int H, int targetLeafIndex) {
    return rr.endedInLeaf && rr.endNode.level == H && rr.endNode.idx == targetLeafIndex;
}