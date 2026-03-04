#pragma once
#include <cstddef>

struct NodeKey {
    int level = 0;
    int idx   = 0; // 0..M^level-1

    bool operator==(const NodeKey& o) const {
        return level == o.level && idx == o.idx;
    }
};

inline NodeKey childOf(const NodeKey& parent, int childNo, int M) {
    return NodeKey{ parent.level + 1, parent.idx * M + childNo };
}

inline NodeKey parentOf(const NodeKey& child, int M) {
    if (child.level == 0) return child;
    return NodeKey{ child.level - 1, child.idx / M };
}

inline int powIntSafe(int a, int b) {
    long long r = 1;
    for (int i = 0; i < b; ++i) r *= a;
    if (r > 2'000'000'000LL) return 2'000'000'000; // грубая защита
    return (int)r;
}