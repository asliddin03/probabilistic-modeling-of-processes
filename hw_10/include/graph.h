#pragma once

#include <vector>

struct Edge {
    int u, v, w;
};

struct Graph {
    int n;
    std::vector<std::vector<int>> w;
    std::vector<std::vector<int>> adj;
    std::vector<Edge> edges;

    explicit Graph(int n = 0);

    bool hasEdge(int u, int v) const;
    void addEdge(int u, int v, int weight);
    int degree(int v) const;
};