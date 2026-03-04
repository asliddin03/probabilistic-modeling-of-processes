#include "ui/tree_renderer.h"
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QLineF>
#include <algorithm>

void TreeRenderer::rebuild(QGraphicsScene* scene, int M, int H) {
    scene->clear();
    pos_.clear();

    computeLayout(M, H);
    drawEdges(scene, M, H);
    drawNodes(scene, M, H);

    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-50, -50, 50, 50));
}

void TreeRenderer::computeLayout(int M, int H) {
    const double levelDy = 90.0;
    const double minDx = 50.0;

    for (int lvl = 0; lvl <= H; ++lvl) {
        const int cnt = powIntSafe(M, lvl);

        // ширина уровня — адаптивная, чтобы не слипались узлы
        const double dx = std::max(minDx, 700.0 / std::max(1, cnt - 1));
        const double y = lvl * levelDy;

        double startX = (cnt <= 1) ? 0.0 : -dx * (cnt - 1) / 2.0;

        for (int i = 0; i < cnt; ++i) {
            pos_[NodeKey{lvl, i}] = QPointF(startX + i * dx, y);
        }
    }
}

void TreeRenderer::drawEdges(QGraphicsScene* scene, int M, int H) {
    QPen edgePen(Qt::gray, 1.5);

    for (int lvl = 0; lvl < H; ++lvl) {
        const int cnt = powIntSafe(M, lvl);
        for (int i = 0; i < cnt; ++i) {
            NodeKey p{lvl, i};
            for (int c = 0; c < M; ++c) {
                NodeKey ch = childOf(p, c, M);
                scene->addLine(QLineF(pos_[p], pos_[ch]), edgePen);
            }
        }
    }
}

void TreeRenderer::drawNodes(QGraphicsScene* scene, int M, int H) {
    const double R = 14.0;

    QPen nodePen(Qt::black, 1.5);
    QBrush innerBrush(Qt::white);
    QBrush leafBrush(QColor(245, 245, 245));

    for (int lvl = 0; lvl <= H; ++lvl) {
        const int cnt = powIntSafe(M, lvl);
        for (int i = 0; i < cnt; ++i) {
            NodeKey nk{lvl, i};
            QPointF p = pos_[nk];

            QRectF rect(p.x() - R, p.y() - R, 2*R, 2*R);
            bool isLeaf = (lvl == H);
            scene->addEllipse(rect, nodePen, isLeaf ? leafBrush : innerBrush);

            // подпись idx
            auto* t = scene->addText(QString::number(i));
            t->setDefaultTextColor(Qt::darkGray);
            t->setScale(0.8);
            t->setPos(p.x() - 6, p.y() - 30);
        }
    }

    // подпись уровней слева
    for (int lvl = 0; lvl <= H; ++lvl) {
        auto* lab = scene->addText(QString("Уровень %1").arg(lvl));
        lab->setDefaultTextColor(Qt::darkBlue);
        lab->setScale(0.9);
        lab->setPos(-460, lvl * 90.0 - 10);
    }
}

void TreeRenderer::highlightPath(QGraphicsScene* scene, const std::vector<NodeKey>& path) {
    if (path.empty()) return;

    QPen pathPen(QColor(200, 0, 0), 4.0);
    QPen nodePen(QColor(200, 0, 0), 2.0);
    QBrush nodeBrush(QColor(255, 210, 210));
    const double R = 16.0;

    for (size_t i = 1; i < path.size(); ++i) {
        scene->addLine(QLineF(pos_[path[i-1]], pos_[path[i]]), pathPen);
    }

    for (const auto& nk : path) {
        QPointF p = pos_[nk];
        QRectF rect(p.x() - R, p.y() - R, 2*R, 2*R);
        scene->addEllipse(rect, nodePen, nodeBrush);
    }
}