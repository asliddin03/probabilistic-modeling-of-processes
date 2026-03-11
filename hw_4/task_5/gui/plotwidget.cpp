#include "plotwidget.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <algorithm>

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(700, 400);
    setAutoFillBackground(true);
}

void PlotWidget::setFigure(const StaircaseFigure& figure, double h, double tau, int m) {
    m_figure = figure;
    m_h = h;
    m_tau = tau;
    m_m = std::max(1, m);
    update();
}

void PlotWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(rect(), Qt::white);

    const int leftMargin = 50;
    const int rightMargin = 20;
    const int topMargin = 20;
    const int bottomMargin = 40;

    const QRect drawRect(
        leftMargin,
        topMargin,
        width() - leftMargin - rightMargin,
        height() - topMargin - bottomMargin
    );

    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(drawRect);

    if (m_figure.levels.empty())
        return;

    const int n = static_cast<int>(m_figure.levels.size());

    const double xMax = n * m_h;
    const double yMax = std::max(1, m_m) * m_tau;

    auto mapX = [&](double x) -> int {
        return drawRect.left() + static_cast<int>((x / xMax) * drawRect.width());
    };

    auto mapY = [&](double y) -> int {
        return drawRect.bottom() - static_cast<int>((y / yMax) * drawRect.height());
    };

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));

    for (int i = 0; i <= n; ++i) {
        const double x = i * m_h;
        const int px = mapX(x);
        painter.drawLine(px, drawRect.top(), px, drawRect.bottom());
    }

    for (int level = 0; level <= m_m; ++level) {
        const double y = level * m_tau;
        const int py = mapY(y);
        painter.drawLine(drawRect.left(), py, drawRect.right(), py);
    }

    painter.setPen(QPen(Qt::blue, 3));

    for (int i = 0; i < n; ++i) {
        const double x1 = i * m_h;
        const double x2 = (i + 1) * m_h;
        const double y = m_figure.levels[static_cast<size_t>(i)] * m_tau;

        painter.drawLine(mapX(x1), mapY(y), mapX(x2), mapY(y));

        if (i + 1 < n)
        {
            const double yNext = m_figure.levels[static_cast<size_t>(i + 1)] * m_tau;
            painter.drawLine(mapX(x2), mapY(y), mapX(x2), mapY(yNext));
        }
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.drawText(10, 20, "y");
    painter.drawText(width() - 15, height() - 10, "x");
}