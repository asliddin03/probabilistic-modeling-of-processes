#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    auto *mainLayout = new QVBoxLayout(m_centralWidget);
    auto *controlsLayout = new QHBoxLayout();
    auto *formLayout = new QFormLayout();

    m_pSpin = new QDoubleSpinBox(this);
    m_pSpin->setRange(0.0, 1.0);
    m_pSpin->setDecimals(3);
    m_pSpin->setSingleStep(0.01);
    m_pSpin->setValue(0.55);

    m_bMinSpin = new QSpinBox(this);
    m_bMinSpin->setRange(1, 100000);
    m_bMinSpin->setValue(1);

    m_bMaxSpin = new QSpinBox(this);
    m_bMaxSpin->setRange(1, 100000);
    m_bMaxSpin->setValue(20);

    m_experimentsSpin = new QSpinBox(this);
    m_experimentsSpin->setRange(1, 1000000);
    m_experimentsSpin->setValue(5000);

    m_maxStepsSpin = new QSpinBox(this);
    m_maxStepsSpin->setRange(1, 10000000);
    m_maxStepsSpin->setValue(10000);

    formLayout->addRow("p:", m_pSpin);
    formLayout->addRow("B min:", m_bMinSpin);
    formLayout->addRow("B max:", m_bMaxSpin);
    formLayout->addRow("Число экспериментов:", m_experimentsSpin);
    formLayout->addRow("Максимум шагов:", m_maxStepsSpin);

    m_runButton = new QPushButton("Смоделировать", this);
    connect(m_runButton, &QPushButton::clicked, this, &MainWindow::runSimulation);

    controlsLayout->addLayout(formLayout);
    controlsLayout->addWidget(m_runButton);
    controlsLayout->addStretch();

    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels(
        QStringList() << "B"
                      << "P(обрыв)"
                      << "P(кафе)"
                      << "P(не определено)");
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(new QLabel("График вероятностей:", this));
    mainLayout->addWidget(m_chartView, 2);
    mainLayout->addWidget(new QLabel("Таблица результатов:", this));
    mainLayout->addWidget(m_table, 1);

    setWindowTitle("Случайное блуждание: кафе и обрыв");
    resize(1000, 700);
}

void MainWindow::runSimulation()
{
    const double p = m_pSpin->value();
    const int bMin = m_bMinSpin->value();
    const int bMax = m_bMaxSpin->value();
    const int experiments = m_experimentsSpin->value();
    const int maxSteps = m_maxStepsSpin->value();

    if (bMin > bMax)
    {
        QMessageBox::warning(this, "Ошибка", "Значение B min не должно быть больше B max.");
        return;
    }

    std::vector<SimulationPoint> data =
        m_simulation.buildSeries(bMin, bMax, p, experiments, maxSteps);

    updateChart(data);
    updateTable(data);
}

void MainWindow::updateChart(const std::vector<SimulationPoint>& data)
{
    auto *cliffSeries = new QLineSeries();
    cliffSeries->setName("Вероятность упасть в обрыв");

    auto *cafeSeries = new QLineSeries();
    cafeSeries->setName("Вероятность вернуться в кафе");

    for (const SimulationPoint& point : data)
    {
        cliffSeries->append(point.B, point.cliffProbability);
        cafeSeries->append(point.B, point.cafeProbability);
    }

    auto *chart = new QChart();
    chart->addSeries(cliffSeries);
    chart->addSeries(cafeSeries);
    chart->setTitle("Эмпирические вероятности в зависимости от B");

    auto *axisX = new QValueAxis();
    axisX->setTitleText("B");
    axisX->setLabelFormat("%d");

    auto *axisY = new QValueAxis();
    axisY->setTitleText("Вероятность");
    axisY->setRange(0.0, 1.0);
    axisY->setLabelFormat("%.2f");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    cliffSeries->attachAxis(axisX);
    cliffSeries->attachAxis(axisY);
    cafeSeries->attachAxis(axisX);
    cafeSeries->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setChart(chart);
}

void MainWindow::updateTable(const std::vector<SimulationPoint>& data)
{
    m_table->setRowCount(static_cast<int>(data.size()));

    for (int row = 0; row < static_cast<int>(data.size()); ++row)
    {
        const SimulationPoint& point = data[row];

        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(point.B)));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::number(point.cliffProbability, 'f', 4)));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number(point.cafeProbability, 'f', 4)));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::number(point.unknownProbability, 'f', 4)));
    }
}