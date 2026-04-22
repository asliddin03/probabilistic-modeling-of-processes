#include "random_graph.h"

Graph generateRandomGraph(int n, double p, std::mt19937_64& rng) {
    Graph g(n);
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    std::uniform_int_distribution<int> weightDist(1, 10);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (prob(rng) < p) {
                g.addEdge(i, j, weightDist(rng));
            }
        }
    }

    return g;
}