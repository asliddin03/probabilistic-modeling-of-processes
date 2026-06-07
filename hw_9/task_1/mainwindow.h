#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QtCharts/QChartView>

#include "discreterandomvariable.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void createFromTable();
    void multiplyByScalar();
    void saveToFile();
    void loadFromFile();

private:
    QTableWidget* m_table = nullptr;
    QLineEdit* m_scalarEdit = nullptr;
    QLabel* m_statsLabel = nullptr;

    QChartView* m_distributionChartView = nullptr;
    QChartView* m_polylineChartView = nullptr;
    QChartView* m_cdfChartView = nullptr;

    DiscreteRandomVariable m_rv;
    bool m_hasRv = false;

    QVector<QPair<double, double>> readTable() const;
    void fillDemoData();
    void updateStats();
    void updateCharts();
    void showError(const QString& text);
};