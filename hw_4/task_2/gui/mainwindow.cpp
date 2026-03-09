#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_logger(10)
{
    setupUi();
}

void MainWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    auto* mainLayout = new QVBoxLayout(m_centralWidget);
    auto* topLayout = new QHBoxLayout();

    m_loadConfigButton = new QPushButton("Загрузить JSON", this);
    m_runButton = new QPushButton("Запустить моделирование", this);
    m_statusLabel = new QLabel("Конфиг не загружен", this);

    topLayout->addWidget(m_loadConfigButton);
    topLayout->addWidget(m_runButton);
    topLayout->addWidget(m_statusLabel);

    m_chartView = new QChartView(this);
    m_chartView->setMinimumHeight(350);

    m_pointsTable = new QTableWidget(this);
    m_pointsTable->setColumnCount(2);
    m_pointsTable->setHorizontalHeaderLabels({"x", "y"});

    m_logsTable = new QTableWidget(this);
    m_logsTable->setColumnCount(4);
    m_logsTable->setHorizontalHeaderLabels({"Время", "Распределение", "Пересечения", "Вероятность"});

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_chartView);
    mainLayout->addWidget(new QLabel("Точки траектории:", this));
    mainLayout->addWidget(m_pointsTable);
    mainLayout->addWidget(new QLabel("Последние логи:", this));
    mainLayout->addWidget(m_logsTable);

    connect(m_loadConfigButton, &QPushButton::clicked, this, &MainWindow::loadConfig);
    connect(m_runButton, &QPushButton::clicked, this, &MainWindow::runSimulation);

    resize(1000, 800);
    setWindowTitle("Моделирование движения точки");
}

QString MainWindow::distributionToString(DistributionType type) const
{
    switch (type)
    {
    case DistributionType::Uniform:
        return "Uniform";
    case DistributionType::Binomial:
        return "Binomial";
    case DistributionType::FiniteGeometric:
        return "Finite Geometric";
    case DistributionType::Triangular:
        return "Triangular";
    }

    return "Unknown";
}

void MainWindow::loadConfig()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        "Выберите JSON конфиг",
        "",
        "JSON files (*.json)"
    );

    if (path.isEmpty())
        return;

    try
    {
        m_config = Config::fromJsonFile(path);
        m_configLoaded = true;
        m_configPath = path;

        m_logger = SimulationLogger(m_config.K);

        m_statusLabel->setText("Конфиг загружен: " + path);
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(this, "Ошибка", ex.what());
    }
}

void MainWindow::runSimulation()
{
    if (!m_configLoaded)
    {
        try
        {
            m_config = Config::fromJsonFile(m_configPath);
            m_configLoaded = true;
            m_logger = SimulationLogger(m_config.K);
        }
        catch (const std::exception& ex)
        {
            QMessageBox::critical(this, "Ошибка", ex.what());
            return;
        }
    }

    try
    {
        Simulation simulation(m_config);
        const SimulationResult result = simulation.runFullSimulation();

        updateChart(result.trajectory);
        updatePointsTable(result.trajectory);

        LogEntry entry;
        entry.timestamp = QDateTime::currentDateTime();
        entry.distributionName = distributionToString(m_config.distributionType);
        entry.crossings = result.crossings;
        entry.estimatedProbability = result.estimatedProbability;
        entry.summary = QString("crossings=%1, probability=%2")
                            .arg(result.crossings)
                            .arg(result.estimatedProbability);

        m_logger.addEntry(entry);
        updateLogsTable();

        m_statusLabel->setText(
            QString("Пересечения: %1 | P(N=%2) = %3")
                .arg(result.crossings)
                .arg(m_config.l)
                .arg(result.estimatedProbability)
        );
    }
    catch (const std::exception& ex)
    {
        QMessageBox::critical(this, "Ошибка", ex.what());
    }
}

void MainWindow::updateChart(const QVector<Point>& trajectory)
{
    auto* series = new QLineSeries();

    for (const Point& p : trajectory)
        series->append(p.x, p.y);

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Траектория движения точки");
    chart->legend()->hide();

    auto* axisX = new QValueAxis();
    axisX->setTitleText("X");

    auto* axisY = new QValueAxis();
    axisY->setTitleText("Y");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    m_chartView->setChart(chart);
}

void MainWindow::updatePointsTable(const QVector<Point>& trajectory)
{
    m_pointsTable->setRowCount(trajectory.size());

    for (int i = 0; i < trajectory.size(); ++i)
    {
        m_pointsTable->setItem(i, 0, new QTableWidgetItem(QString::number(trajectory[i].x)));
        m_pointsTable->setItem(i, 1, new QTableWidgetItem(QString::number(trajectory[i].y)));
    }
}

void MainWindow::updateLogsTable()
{
    const auto& entries = m_logger.entries();
    m_logsTable->setRowCount(entries.size());

    for (int i = 0; i < entries.size(); ++i)
    {
        m_logsTable->setItem(i, 0, new QTableWidgetItem(entries[i].timestamp.toString("dd.MM.yyyy HH:mm:ss")));
        m_logsTable->setItem(i, 1, new QTableWidgetItem(entries[i].distributionName));
        m_logsTable->setItem(i, 2, new QTableWidgetItem(QString::number(entries[i].crossings)));
        m_logsTable->setItem(i, 3, new QTableWidgetItem(QString::number(entries[i].estimatedProbability)));
    }
}