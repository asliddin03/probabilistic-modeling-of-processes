#include "cycles.h"
#include <algorithm>
#include <vector>

static void dfsCycles(
    const Graph& g,
    int start,
    int current,
    std::vector<int>& used,
    int pathLen,
    int pathWeight,
    CycleInfo& info
) {
    for (int to : g.adj[current]) {
        if (to < start) continue;

        if (to == start) {
            if (pathLen >= 3) {
                info.maxLenCycle = std::max(info.maxLenCycle, pathLen);

                if (pathWeight > info.maxWeightCycle) {
                    info.maxWeightCycle = pathWeight;
                    info.edgesInMaxWeightCycle = pathLen;
                } else if (pathWeight == info.maxWeightCycle) {
                    info.edgesInMaxWeightCycle =
                        std::max(info.edgesInMaxWeightCycle, pathLen);
                }
            }
            continue;
        }

        if (!used[to]) {
            used[to] = 1;
            dfsCycles(g, start, to, used, pathLen + 1,
                      pathWeight + g.w[current][to], info);
            used[to] = 0;
        }
    }
}

CycleInfo analyzeCycles(const Graph& g) {
    CycleInfo info;
    std::vector<int> used(g.n, 0);

    for (int s = 0; s < g.n; ++s) {
        std::fill(used.begin(), used.end(), 0);
        used[s] = 1;

        for (int to : g.adj[s]) {
            if (to < s) continue;
            used[to] = 1;
            dfsCycles(g, s, to, used, 2, g.w[s][to], info);
            used[to] = 0;
        }
    }

    if (info.maxWeightCycle == -1) {
        info.edgesInMaxWeightCycle = 0;
    }

    return info;
}