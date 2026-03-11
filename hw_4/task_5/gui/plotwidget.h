#pragma once

#include <QWidget>
#include "types.h"

class PlotWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlotWidget(QWidget* parent = nullptr);

    void setFigure(const StaircaseFigure& figure, double h, double tau, int m);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    StaircaseFigure m_figure;
    double m_h = 1.0;
    double m_tau = 1.0;
    int m_m = 1;
};