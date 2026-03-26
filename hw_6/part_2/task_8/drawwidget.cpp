#include "drawwidget.h"
#include <QPainter>
#include <QPen>

DrawWidget::DrawWidget(QWidget *parent)
    : QWidget(parent) {
    setMinimumSize(800, 450);
    setAutoFillBackground(true);
}

void DrawWidget::setLineDistance(double d) {
    m_lineDistance = d;
    update();
}

double DrawWidget::lineDistance() const {
    return m_lineDistance;
}

void DrawWidget::addNeedle(const Needle &needle) {
    m_needles.push_back(needle);

    // Чтобы экран не переполнялся
    if (m_needles.size() > 3000) {
        m_needles.remove(0, m_needles.size() - 3000);
    }

    update();
}

void DrawWidget::clearNeedles() {
    m_needles.clear();
    update();
}

QRect DrawWidget::drawingRect() const {
    return rect().adjusted(10, 10, -10, -10);
}

void DrawWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white);

    QRect area = drawingRect();

    // Параллельные прямые
    painter.setPen(QPen(Qt::black, 2));
    for (double x = area.left(); x <= area.right(); x += m_lineDistance) {
        painter.drawLine(QPointF(x, area.top()), QPointF(x, area.bottom()));
    }

    // Иглы
    for (const Needle &needle : m_needles) {
        painter.setPen(QPen(needle.crossed ? Qt::red : Qt::darkGreen, 2));
        painter.drawLine(needle.p1, needle.p2);
    }

    painter.setPen(Qt::black);
    painter.drawRect(area);
}