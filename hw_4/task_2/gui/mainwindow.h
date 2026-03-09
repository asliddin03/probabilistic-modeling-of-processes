#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QString>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "../core/config.h"
#include "../core/logger.h"
#include "../core/simulation.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void loadConfig();
    void runSimulation();

private:
    void setupUi();
    void updateChart(const QVector<Point>& trajectory);
    void updatePointsTable(const QVector<Point>& trajectory);
    void updateLogsTable();
    QString distributionToString(DistributionType type) const;

private:
    Config m_config;
    bool m_configLoaded = false;
    QString m_configPath = "config.json";

    SimulationLogger m_logger;

    QWidget* m_centralWidget = nullptr;
    QPushButton* m_loadConfigButton = nullptr;
    QPushButton* m_runButton = nullptr;
    QLabel* m_statusLabel = nullptr;

    QChartView* m_chartView = nullptr;
    QTableWidget* m_pointsTable = nullptr;
    QTableWidget* m_logsTable = nullptr;
};