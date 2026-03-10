#include "mainwindow.h"
#include "core/config.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QFileInfo>

DrawWidget::DrawWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(700, 500);
    setAutoFillBackground(true);
}

void DrawWidget::setTrajectory(const Trajectory& trajectory) {
    m_trajectory = trajectory;
    update();
}

void DrawWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int w = width();
    const int h = height();
    const QPoint center(w / 2, h / 2);

    painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    painter.drawLine(0, center.y(), w, center.y());
    painter.drawLine(center.x(), 0, center.x(), h);

    if (m_trajectory.points.empty())
        return;

    double minX = m_trajectory.points[0].x;
    double maxX = m_trajectory.points[0].x;
    double minY = m_trajectory.points[0].y;
    double maxY = m_trajectory.points[0].y;

    for (const Point& p : m_trajectory.points) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    const double rangeX = std::max(std::abs(minX), std::abs(maxX));
    const double rangeY = std::max(std::abs(minY), std::abs(maxY));
    const double range = std::max(1.0, std::max(rangeX, rangeY));

    const double margin = 30.0;
    const double scaleX = (w / 2.0 - margin) / range;
    const double scaleY = (h / 2.0 - margin) / range;
    const double scale = std::min(scaleX, scaleY);

    auto mapPoint = [&](const Point& p) -> QPointF {
        const double px = center.x() + p.x * scale;
        const double py = center.y() - p.y * scale;
        return QPointF(px, py);
    };

    painter.setPen(QPen(Qt::blue, 2));

    for (size_t i = 1; i < m_trajectory.points.size(); ++i) {
        painter.drawLine(mapPoint(m_trajectory.points[i - 1]),
                         mapPoint(m_trajectory.points[i]));
    }

    painter.setPen(QPen(Qt::darkGreen, 2));
    painter.setBrush(Qt::green);
    painter.drawEllipse(mapPoint(m_trajectory.points.front()), 5, 5);

    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::red);
    painter.drawEllipse(mapPoint(m_trajectory.points.back()), 5, 5);

    painter.setPen(Qt::black);
    painter.drawText(10, 20, "Зелёный — старт, красный — конец");
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    m_loadButton = new QPushButton("Загрузить config.json", this);
    m_runButton = new QPushButton("Запустить моделирование", this);
    m_configLabel = new QLabel("Конфигурация не загружена", this);
    m_statsLabel = new QLabel("Статистика отсутствует", this);
    m_listWidget = new QListWidget(this);
    m_drawWidget = new DrawWidget(this);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_loadButton);
    leftLayout->addWidget(m_runButton);
    leftLayout->addWidget(m_configLabel);
    leftLayout->addWidget(m_statsLabel);
    leftLayout->addWidget(new QLabel("Траектории:", this));
    leftLayout->addWidget(m_listWidget, 1);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->addLayout(leftLayout, 0);
    mainLayout->addWidget(m_drawWidget, 1);

    connect(m_loadButton, &QPushButton::clicked, this, &MainWindow::loadConfig);
    connect(m_runButton, &QPushButton::clicked, this, &MainWindow::runSimulation);
    connect(m_listWidget, &QListWidget::currentRowChanged, this, &MainWindow::showSelectedTrajectory);

    setWindowTitle("Моделирование движения точки");
    resize(1100, 650);
}

QString MainWindow::distributionToString(DistributionType type) const {
    switch (type) {
        case DistributionType::Uniform: return "uniform";
        case DistributionType::Binomial: return "binomial";
        case DistributionType::Geometric: return "geometric";
        case DistributionType::Triangular: return "triangular";
    }

    return "unknown";
}

void MainWindow::loadConfig() {
    QString error;
    if (!ConfigLoader::loadFromJson("config.json", m_config, error)) {
        QMessageBox::critical(this, "Ошибка", error);
        return;
    }

    m_configLabel->setText(
        QString("rho=%1, n=%2, M=%3, K=%4, distribution=%5")
            .arg(m_config.rho)
            .arg(m_config.n)
            .arg(m_config.M)
            .arg(m_config.K)
            .arg(distributionToString(m_config.distribution))
    );

    QMessageBox::information(this, "Успех", "Конфигурация успешно загружена");
}

void MainWindow::runSimulation() {
    Simulator simulator(m_config);
    m_summary = simulator.runAll();

    m_listWidget->clear();

    for (int i = 0; i < static_cast<int>(m_summary.trajectories.size()); ++i) {
        const Trajectory& tr = m_summary.trajectories[i];

        QString text = QString("Запуск %1: ").arg(i + 1);
        if (tr.returnedToOrigin)
            text += QString("возврат на шаге %1").arg(tr.returnStep);
        else
            text += "без возврата";

        m_listWidget->addItem(text);
    }

    const double probability =
        static_cast<double>(m_summary.returnedCount) / static_cast<double>(m_config.M);

    m_statsLabel->setText(
        QString("Вернулось: %1 из %2, оценка вероятности = %3")
            .arg(m_summary.returnedCount)
            .arg(m_config.M)
            .arg(probability, 0, 'f', 4)
    );

    if (!m_summary.trajectories.empty()) {
        m_listWidget->setCurrentRow(0);
        m_drawWidget->setTrajectory(m_summary.trajectories[0]);
    }
}

void MainWindow::showSelectedTrajectory(int row) {
    if (row < 0 || row >= static_cast<int>(m_summary.trajectories.size()))
        return;

    m_drawWidget->setTrajectory(m_summary.trajectories[row]);
}