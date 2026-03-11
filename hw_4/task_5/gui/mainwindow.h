#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

#include "types.h"
#include "plotwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void loadConfig();
    void runSimulation();
    void showPreviousFigure();
    void showNextFigure();

private:
    void setupUi();
    void updateInfo();
    void showFigure(int index);

    SimulationConfig m_config;
    SimulationResult m_result;
    int m_currentIndex = -1;

    QWidget* m_centralWidget = nullptr;

    QPushButton* m_loadConfigButton = nullptr;
    QPushButton* m_runButton = nullptr;
    QPushButton* m_prevButton = nullptr;
    QPushButton* m_nextButton = nullptr;

    QLabel* m_configLabel = nullptr;
    QLabel* m_statsLabel = nullptr;
    QLabel* m_figureLabel = nullptr;
    QLabel* m_warningLabel = nullptr;

    PlotWidget* m_plotWidget = nullptr;
};