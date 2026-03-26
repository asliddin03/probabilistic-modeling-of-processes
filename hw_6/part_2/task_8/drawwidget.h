#pragma once

#include <QWidget>
#include <QVector>
#include <QPointF>

struct Needle {
    QPointF p1;
    QPointF p2;
    bool crossed = false;
};

class DrawWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawWidget(QWidget *parent = nullptr);

    void setLineDistance(double d);
    double lineDistance() const;

    void addNeedle(const Needle &needle);
    void clearNeedles();

    QRect drawingRect() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<Needle> m_needles;
    double m_lineDistance = 120.0;
};