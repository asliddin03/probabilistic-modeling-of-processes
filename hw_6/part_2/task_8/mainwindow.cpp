#include "mainwindow.h"
#include "drawwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Задача Буффона");
    resize(1000, 700);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    m_drawWidget = new DrawWidget(this);

    m_lengthSpin = new QDoubleSpinBox(this);
    m_lengthSpin->setRange(10.0, 300.0);
    m_lengthSpin->setValue(80.0);
    m_lengthSpin->setPrefix("L = ");

    m_distanceSpin = new QDoubleSpinBox(this);
    m_distanceSpin->setRange(20.0, 300.0);
    m_distanceSpin->setValue(120.0);
    m_distanceSpin->setPrefix("d = ");

    m_batchSpin = new QSpinBox(this);
    m_batchSpin->setRange(1, 10000);
    m_batchSpin->setValue(100);
    m_batchSpin->setPrefix("N = ");

    m_oneThrowButton = new QPushButton("Один бросок", this);
    m_manyThrowsButton = new QPushButton("Серия бросков", this);
    m_clearButton = new QPushButton("Очистить", this);

    m_totalLabel = new QLabel("Всего бросков: 0", this);
    m_crossedLabel = new QLabel("Пересечений: 0", this);
    m_empiricalLabel = new QLabel("Эмпирическая вероятность: 0", this);
    m_exactLabel = new QLabel("Точная вероятность: 0", this);
    m_errorLabel = new QLabel("Абсолютная ошибка: 0", this);

    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->addWidget(m_lengthSpin, 0, 0);
    controlLayout->addWidget(m_distanceSpin, 0, 1);
    controlLayout->addWidget(m_batchSpin, 0, 2);
    controlLayout->addWidget(m_oneThrowButton, 0, 3);
    controlLayout->addWidget(m_manyThrowsButton, 0, 4);
    controlLayout->addWidget(m_clearButton, 0, 5);

    QVBoxLayout *statsLayout = new QVBoxLayout;
    statsLayout->addWidget(m_totalLabel);
    statsLayout->addWidget(m_crossedLabel);
    statsLayout->addWidget(m_empiricalLabel);
    statsLayout->addWidget(m_exactLabel);
    statsLayout->addWidget(m_errorLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(statsLayout);
    mainLayout->addWidget(m_drawWidget, 1);

    connect(m_oneThrowButton, &QPushButton::clicked, this, &MainWindow::doOneThrow);
    connect(m_manyThrowsButton, &QPushButton::clicked, this, &MainWindow::doManyThrows);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::clearSimulation);

    connect(m_distanceSpin, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
                m_drawWidget->setLineDistance(value);
                updateStatistics();
            });

    connect(m_lengthSpin, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, [this](double) {
                updateStatistics();
            });

    m_drawWidget->setLineDistance(m_distanceSpin->value());
    updateStatistics();
}

bool MainWindow::crossesLine(double centerXInArea, double angle) const {
    double L = m_lengthSpin->value();
    double d = m_distanceSpin->value();

    double nearestLineX = std::round(centerXInArea / d) * d;
    double x = std::fabs(centerXInArea - nearestLineX);

    return x <= (L / 2.0) * std::sin(angle);
}

void MainWindow::doOneThrow() {
    double L = m_lengthSpin->value();
    double d = m_distanceSpin->value();

    if (L > d) {
        QMessageBox::warning(this, "Ошибка", "Нужно, чтобы L <= d.");
        return;
    }

    QRect area = m_drawWidget->drawingRect();

    double left = static_cast<double>(area.left());
    double right = static_cast<double>(area.right());
    double top = static_cast<double>(area.top());
    double bottom = static_cast<double>(area.bottom());

    double centerX = left + QRandomGenerator::global()->generateDouble() * (right - left);
    double centerY = top + QRandomGenerator::global()->generateDouble() * (bottom - top);
    double phi = QRandomGenerator::global()->generateDouble() * std::acos(-1.0);

    double dx = (L / 2.0) * std::sin(phi);
    double dy = (L / 2.0) * std::cos(phi);

    Needle needle;
    needle.p1 = QPointF(centerX - dx, centerY - dy);
    needle.p2 = QPointF(centerX + dx, centerY + dy);
    needle.crossed = crossesLine(centerX - area.left(), phi);

    m_drawWidget->addNeedle(needle);

    ++m_totalThrows;
    if (needle.crossed) {
        ++m_crossedThrows;
    }

    updateStatistics();
}

void MainWindow::doManyThrows() {
    int count = m_batchSpin->value();
    for (int i = 0; i < count; ++i) {
        doOneThrow();
    }
}

void MainWindow::clearSimulation() {
    m_totalThrows = 0;
    m_crossedThrows = 0;
    m_drawWidget->clearNeedles();
    updateStatistics();
}

void MainWindow::updateStatistics() {
    double L = m_lengthSpin->value();
    double d = m_distanceSpin->value();

    double empirical = 0.0;
    if (m_totalThrows > 0) {
        empirical = static_cast<double>(m_crossedThrows) / m_totalThrows;
    }

    double exact = 0.0;
    if (L <= d && d > 0.0) {
        exact = 2.0 * L / (std::acos(-1.0) * d);
    }

    double error = std::fabs(empirical - exact);

    m_totalLabel->setText(QString("Всего бросков: %1").arg(m_totalThrows));
    m_crossedLabel->setText(QString("Пересечений: %1").arg(m_crossedThrows));
    m_empiricalLabel->setText(
        QString("Эмпирическая вероятность: %1").arg(empirical, 0, 'f', 6));
    m_exactLabel->setText(
        QString("Точная вероятность: %1").arg(exact, 0, 'f', 6));
    m_errorLabel->setText(
        QString("Абсолютная ошибка: %1").arg(error, 0, 'f', 6));
}