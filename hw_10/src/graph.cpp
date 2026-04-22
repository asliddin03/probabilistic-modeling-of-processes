#include "graph.h"

Graph::Graph(int n) : n(n), w(n, std::vector<int>(n, 0)), adj(n) {}

bool Graph::hasEdge(int u, int v) const {
    return w[u][v] != 0;
}

void Graph::addEdge(int u, int v, int weight) {
    if (u == v || w[u][v] != 0) return;
    w[u][v] = w[v][u] = weight;
    adj[u].push_back(v);
    adj[v].push_back(u);
    edges.push_back({u, v, weight});
}

int Graph::degree(int v) const {
    return static_cast<int>(adj[v].size());
}