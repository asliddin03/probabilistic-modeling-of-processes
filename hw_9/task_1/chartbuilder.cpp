#include "chartbuilder.h"

#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

QChart* ChartBuilder::buildDistributionChart(const DiscreteRandomVariable& rv)
{
    auto* lineSeries = new QLineSeries();
    auto* scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(10.0);

    const auto points = rv.distributionLaw();
    for (const auto& point : points) {
        lineSeries->append(point.first, point.second);
        scatterSeries->append(point.first, point.second);
    }

    auto* chart = new QChart();
    chart->addSeries(lineSeries);
    chart->addSeries(scatterSeries);
    chart->setTitle("Закон распределения");

    auto* axisX = new QValueAxis();
    axisX->setTitleText("x");

    auto* axisY = new QValueAxis();
    axisY->setTitleText("P(X = x)");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);

    return chart;
}

QChart* ChartBuilder::buildPolylineChart(const DiscreteRandomVariable& rv)
{
    auto* series = new QLineSeries();

    const auto points = rv.polylinePoints();
    for (const auto& point : points) {
        series->append(point.first, point.second);
    }

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Полилайн");

    auto* axisX = new QValueAxis();
    axisX->setTitleText("x");

    auto* axisY = new QValueAxis();
    axisY->setTitleText("p");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    return chart;
}

QChart* ChartBuilder::buildCdfChart(const DiscreteRandomVariable& rv)
{
    auto* series = new QLineSeries();
    const auto points = rv.cdfPoints();

    if (!points.isEmpty()) {
        series->append(points.first().first - 1.0, 0.0);
    }

    double prev = 0.0;
    for (const auto& point : points) {
        series->append(point.first, prev);
        series->append(point.first, point.second);
        prev = point.second;
    }

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Функция распределения F(x)");

    auto* axisX = new QValueAxis();
    axisX->setTitleText("x");

    auto* axisY = new QValueAxis();
    axisY->setTitleText("F(x)");
    axisY->setRange(0.0, 1.05);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    return chart;
}