#pragma once

#include "core/config.h"
#include "core/simulation.h"

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QWidget>

class DrawWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawWidget(QWidget* parent = nullptr);

    void setTrajectory(const Trajectory& trajectory);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Trajectory m_trajectory;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void loadConfig();
    void runSimulation();
    void showSelectedTrajectory(int row);

private:
    QString distributionToString(DistributionType type) const;

    AppConfig m_config;
    SimulationSummary m_summary;

    QPushButton* m_loadButton = nullptr;
    QPushButton* m_runButton = nullptr;
    QLabel* m_configLabel = nullptr;
    QLabel* m_statsLabel = nullptr;
    QListWidget* m_listWidget = nullptr;
    DrawWidget* m_drawWidget = nullptr;
};