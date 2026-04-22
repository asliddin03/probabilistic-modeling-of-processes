#pragma once

#include "graph.h"
#include <vector>

std::vector<std::vector<int>> connectedComponents(const Graph& g);
bool isConnected(const Graph& g);
int countConnectedComponents(const Graph& g);
int countIsolatedVertices(const Graph& g);
bool isCliqueComponent(const Graph& g, const std::vector<int>& comp);
int countCliqueComponents(const Graph& g);