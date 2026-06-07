#pragma once

#include <QtCharts/QChart>
#include "discreterandomvariable.h"

class ChartBuilder
{
public:
    static QChart* buildDistributionChart(const DiscreteRandomVariable& rv);
    static QChart* buildPolylineChart(const DiscreteRandomVariable& rv);
    static QChart* buildCdfChart(const DiscreteRandomVariable& rv);
};