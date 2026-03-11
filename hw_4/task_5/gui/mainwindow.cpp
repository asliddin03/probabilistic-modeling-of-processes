#include "mainwindow.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "config.h"
#include "simulation.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi();
    updateInfo();
}

void MainWindow::setupUi() {
    setWindowTitle("Ступенчатые фигуры");
    resize(900, 650);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_loadConfigButton = new QPushButton("Загрузить JSON", this);
    m_runButton = new QPushButton("Запустить моделирование", this);
    m_prevButton = new QPushButton("Предыдущая", this);
    m_nextButton = new QPushButton("Следующая", this);

    m_configLabel = new QLabel(this);
    m_statsLabel = new QLabel(this);
    m_figureLabel = new QLabel(this);
    m_warningLabel = new QLabel(this);

    m_plotWidget = new PlotWidget(this);

    auto* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_loadConfigButton);
    buttonsLayout->addWidget(m_runButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_prevButton);
    buttonsLayout->addWidget(m_nextButton);

    auto* mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(m_configLabel);
    mainLayout->addWidget(m_statsLabel);
    mainLayout->addWidget(m_figureLabel);
    mainLayout->addWidget(m_warningLabel);
    mainLayout->addWidget(m_plotWidget, 1);

    connect(m_loadConfigButton, &QPushButton::clicked,
            this, &MainWindow::loadConfig);

    connect(m_runButton, &QPushButton::clicked,
            this, &MainWindow::runSimulation);

    connect(m_prevButton, &QPushButton::clicked,
            this, &MainWindow::showPreviousFigure);

    connect(m_nextButton, &QPushButton::clicked,
            this, &MainWindow::showNextFigure);
}

void MainWindow::loadConfig() {
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Выберите JSON-файл",
        "config",
        "JSON files (*.json)"
    );

    if (filePath.isEmpty())
        return;

    QString error;
    SimulationConfig loadedConfig;

    if (!ConfigLoader::loadFromFile(filePath, loadedConfig, error)) {
        QMessageBox::critical(this, "Ошибка конфигурации", error);
        return;
    }

    m_config = loadedConfig;
    m_result = SimulationResult{};
    m_currentIndex = -1;

    updateInfo();
    QMessageBox::information(this, "Успех", "Конфигурация успешно загружена.");
}

void MainWindow::runSimulation() {
    QString error;
    if (!ConfigLoader::validate(m_config, error)) {
        QMessageBox::critical(this, "Ошибка параметров", error);
        return;
    }

    Simulation simulation(m_config);
    m_result = simulation.run();

    if (!m_result.figures.empty()) {
        m_currentIndex = 0;
        showFigure(m_currentIndex);
    } else {
        m_currentIndex = -1;
    }

    updateInfo();
}

void MainWindow::showPreviousFigure() {
    if (m_result.figures.empty() || m_currentIndex <= 0)
        return;

    --m_currentIndex;
    showFigure(m_currentIndex);
    updateInfo();
}

void MainWindow::showNextFigure() {
    if (m_result.figures.empty() || m_currentIndex + 1 >= static_cast<int>(m_result.figures.size()))
        return;

    ++m_currentIndex;
    showFigure(m_currentIndex);
    updateInfo();
}

void MainWindow::showFigure(int index) {
    if (index < 0 || index >= static_cast<int>(m_result.figures.size()))
        return;

    m_plotWidget->setFigure(
        m_result.figures[static_cast<size_t>(index)],
        m_config.h,
        m_config.tau,
        m_config.m
    );
}

void MainWindow::updateInfo() {
    m_configLabel->setText(
        QString("M=%1, h=%2, tau=%3, m=%4, N=%5, distribution=%6")
            .arg(m_config.M)
            .arg(m_config.h)
            .arg(m_config.tau)
            .arg(m_config.m)
            .arg(m_config.N)
            .arg(m_config.distributionType)
    );

    m_statsLabel->setText(
        QString("Строго возрастающих фигур: %1, эмпирическая вероятность: %2")
            .arg(m_result.increasingCount)
            .arg(m_result.probability, 0, 'f', 6)
    );

    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_result.figures.size())) {
        const bool inc = m_result.figures[static_cast<size_t>(m_currentIndex)].isStrictlyIncreasing;

        m_figureLabel->setText(
            QString("Фигура %1 / %2, строго возрастающая: %3")
                .arg(m_currentIndex + 1)
                .arg(m_result.figures.size())
                .arg(inc ? "ДА" : "НЕТ")
        );
    } else {
        m_figureLabel->setText("Фигура не выбрана");
    }

    const int stepCount = Simulation::stepCount(m_config);
    if (stepCount > m_config.m + 1) {
        m_warningLabel->setText(
            "Предупреждение: число ступеней больше числа возможных уровней, "
            "поэтому строго возрастающая фигура невозможна."
        );
    } else {
        m_warningLabel->clear();
    }

    m_prevButton->setEnabled(!m_result.figures.empty() && m_currentIndex > 0);
    m_nextButton->setEnabled(!m_result.figures.empty() &&
                             m_currentIndex >= 0 &&
                             m_currentIndex + 1 < static_cast<int>(m_result.figures.size()));
}