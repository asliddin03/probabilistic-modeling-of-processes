#include "components.h"
#include <queue>

std::vector<std::vector<int>> connectedComponents(const Graph& g) {
    std::vector<std::vector<int>> comps;
    std::vector<int> used(g.n, 0);

    for (int s = 0; s < g.n; ++s) {
        if (used[s]) continue;

        std::vector<int> comp;  
        std::queue<int> q;
        q.push(s);
        used[s] = 1;

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            comp.push_back(v);

            for (int to : g.adj[v]) {
                if (!used[to]) {
                    used[to] = 1;
                    q.push(to);
                }
            }
        }

        comps.push_back(comp);
    }

    return comps;
}

bool isConnected(const Graph& g) {
    return connectedComponents(g).size() == 1;
}

int countConnectedComponents(const Graph& g) {
    return static_cast<int>(connectedComponents(g).size());
}

int countIsolatedVertices(const Graph& g) {
    int cnt = 0;
    for (int v = 0; v < g.n; ++v) {
        if (g.degree(v) == 0) cnt++;
    }
    return cnt;
}

bool isCliqueComponent(const Graph& g, const std::vector<int>& comp) {
    int k = static_cast<int>(comp.size());
    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (!g.hasEdge(comp[i], comp[j])) return false;
        }
    }
    return true;
}

int countCliqueComponents(const Graph& g) {
    auto comps = connectedComponents(g);
    int cnt = 0;
    for (const auto& comp : comps) {
        if (isCliqueComponent(g, comp)) cnt++;
    }
    return cnt;
}