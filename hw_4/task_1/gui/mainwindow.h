#pragma once

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "../core/simulation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void runSimulation();

private:
    void setupUi();
    void updateChart(const std::vector<SimulationPoint>& data);
    void updateTable(const std::vector<SimulationPoint>& data);

private:
    Simulation m_simulation;

    QWidget *m_centralWidget = nullptr;

    QDoubleSpinBox *m_pSpin = nullptr;
    QSpinBox *m_bMinSpin = nullptr;
    QSpinBox *m_bMaxSpin = nullptr;
    QSpinBox *m_experimentsSpin = nullptr;
    QSpinBox *m_maxStepsSpin = nullptr;
    QPushButton *m_runButton = nullptr;

    QChartView *m_chartView = nullptr;
    QTableWidget *m_table = nullptr;
};