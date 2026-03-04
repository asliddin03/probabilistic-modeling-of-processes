#pragma once
#include <QGraphicsScene>
#include <QPointF>
#include <unordered_map>
#include <vector>

#include "core/tree_types.h"

// Отвечает ТОЛЬКО за отображение дерева на QGraphicsScene.
// Не знает про вероятности, K экспериментов и т.д.
class TreeRenderer {
public:
    void rebuild(QGraphicsScene* scene, int M, int H);
    void highlightPath(QGraphicsScene* scene, const std::vector<NodeKey>& path);

private:
    // Координаты узлов
    struct NodeKeyHash {
        std::size_t operator()(const NodeKey& k) const noexcept {
            return (std::size_t)k.level * 1000003u + (std::size_t)k.idx;
        }
    };

    std::unordered_map<NodeKey, QPointF, NodeKeyHash> pos_;

    void computeLayout(int M, int H);
    void drawEdges(QGraphicsScene* scene, int M, int H);
    void drawNodes(QGraphicsScene* scene, int M, int H);
};