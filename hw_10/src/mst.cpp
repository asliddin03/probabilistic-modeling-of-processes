#include "mst.h"
#include "components.h"
#include <algorithm>
#include <numeric>
#include <vector>


struct DSU {
    std::vector<int> p, r;

    explicit DSU(int n) : p(n), r(n, 0) {
        std::iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        if (p[x] == x) return x;
        return p[x] = find(p[x]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) std::swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
        return true;
    }
};

int mstWeightKruskal(const Graph& g) {
    if (!isConnected(g)) return -1;

    std::vector<Edge> edges = g.edges;
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    DSU dsu(g.n);
    int total = 0;
    int used = 0;

    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            used++;
            if (used == g.n - 1) break;
        }
    }

    return (used == g.n - 1) ? total : -1;
}