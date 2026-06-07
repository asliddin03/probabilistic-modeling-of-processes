#include "mainwindow.h"
#include "chartbuilder.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* central = new QWidget(this);
    setCentralWidget(central);

    m_table = new QTableWidget(0, 2, this);
    m_table->setHorizontalHeaderLabels({"Значение", "Вероятность"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto* addRowButton = new QPushButton("Добавить строку", this);
    auto* removeRowButton = new QPushButton("Удалить строку", this);
    auto* createButton = new QPushButton("Создать СВ", this);
    auto* multiplyButton = new QPushButton("Умножить на число", this);
    auto* saveButton = new QPushButton("Сохранить", this);
    auto* loadButton = new QPushButton("Загрузить", this);

    m_scalarEdit = new QLineEdit(this);
    m_scalarEdit->setPlaceholderText("Скаляр");

    m_statsLabel = new QLabel("Статистики будут отображены здесь.", this);
    m_statsLabel->setWordWrap(true);

    m_distributionChartView = new QChartView(this);
    m_polylineChartView = new QChartView(this);
    m_cdfChartView = new QChartView(this);

    auto* tabs = new QTabWidget(this);
    tabs->addTab(m_distributionChartView, "Закон распределения");
    tabs->addTab(m_polylineChartView, "Полилайн");
    tabs->addTab(m_cdfChartView, "Функция распределения");

    auto* controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(addRowButton);
    controlsLayout->addWidget(removeRowButton);
    controlsLayout->addWidget(createButton);
    controlsLayout->addWidget(m_scalarEdit);
    controlsLayout->addWidget(multiplyButton);
    controlsLayout->addWidget(saveButton);
    controlsLayout->addWidget(loadButton);

    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(new QLabel("Таблица распределения:", this));
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(m_statsLabel);
    mainLayout->addWidget(tabs, 1);

    connect(addRowButton, &QPushButton::clicked, this, [this]() {
        m_table->insertRow(m_table->rowCount());
    });

    connect(removeRowButton, &QPushButton::clicked, this, [this]() {
        int row = m_table->currentRow();
        if (row >= 0) {
            m_table->removeRow(row);
        }
    });

    connect(createButton, &QPushButton::clicked, this, &MainWindow::createFromTable);
    connect(multiplyButton, &QPushButton::clicked, this, &MainWindow::multiplyByScalar);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);

    fillDemoData();

    setWindowTitle("Дискретная случайная величина");
    resize(1100, 700);
}

QVector<QPair<double, double>> MainWindow::readTable() const
{
    QVector<QPair<double, double>> result;

    for (int row = 0; row < m_table->rowCount(); ++row) {
        auto* valueItem = m_table->item(row, 0);
        auto* probItem = m_table->item(row, 1);

        if (!valueItem || !probItem) {
            continue;
        }

        bool okValue = false;
        bool okProb = false;

        double value = valueItem->text().toDouble(&okValue);
        double probability = probItem->text().toDouble(&okProb);

        if (!okValue || !okProb) {
            throw std::invalid_argument(
                QString("Ошибка преобразования данных в строке %1").arg(row + 1).toStdString()
            );
        }

        result.append(qMakePair(value, probability));
    }

    return result;
}

void MainWindow::fillDemoData()
{
    m_table->setRowCount(3);

    m_table->setItem(0, 0, new QTableWidgetItem("1"));
    m_table->setItem(0, 1, new QTableWidgetItem("0.2"));

    m_table->setItem(1, 0, new QTableWidgetItem("2"));
    m_table->setItem(1, 1, new QTableWidgetItem("0.5"));

    m_table->setItem(2, 0, new QTableWidgetItem("4"));
    m_table->setItem(2, 1, new QTableWidgetItem("0.3"));
}

void MainWindow::createFromTable()
{
    try {
        m_rv = DiscreteRandomVariable(readTable());
        m_hasRv = true;
        updateStats();
        updateCharts();
    } catch (const std::exception& ex) {
        showError(ex.what());
    }
}

void MainWindow::multiplyByScalar()
{
    if (!m_hasRv) {
        showError("Сначала создайте случайную величину.");
        return;
    }

    bool ok = false;
    double scalar = m_scalarEdit->text().toDouble(&ok);

    if (!ok) {
        showError("Некорректное значение скаляра.");
        return;
    }

    try {
        m_rv = m_rv * scalar;
        updateStats();
        updateCharts();
    } catch (const std::exception& ex) {
        showError(ex.what());
    }
}

void MainWindow::saveToFile()
{
    if (!m_hasRv) {
        showError("Нет данных для сохранения.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл",
        "",
        "Binary files (*.bin)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        showError("Не удалось открыть файл для записи.");
        return;
    }

    QDataStream out(&file);
    out << m_rv;
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть файл",
        "",
        "Binary files (*.bin)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        showError("Не удалось открыть файл для чтения.");
        return;
    }

    try {
        QDataStream in(&file);
        in >> m_rv;
        m_hasRv = true;

        const auto data = m_rv.table();
        m_table->setRowCount(data.size());

        for (int i = 0; i < data.size(); ++i) {
            m_table->setItem(i, 0, new QTableWidgetItem(QString::number(data[i].first)));
            m_table->setItem(i, 1, new QTableWidgetItem(QString::number(data[i].second)));
        }

        updateStats();
        updateCharts();
    } catch (const std::exception& ex) {
        showError(ex.what());
    }
}

void MainWindow::updateStats()
{
    if (!m_hasRv) {
        return;
    }

    QString text;
    text += QString("Математическое ожидание: %1\n").arg(m_rv.expectedValue(), 0, 'g', 12);
    text += QString("Дисперсия: %1\n").arg(m_rv.variance(), 0, 'g', 12);
    text += QString("Коэффициент асимметрии: %1\n").arg(m_rv.skewness(), 0, 'g', 12);
    text += QString("Коэффициент эксцесса: %1").arg(m_rv.kurtosis(), 0, 'g', 12);

    m_statsLabel->setText(text);
}

void MainWindow::updateCharts()
{
    if (!m_hasRv) {
        return;
    }

    m_distributionChartView->setChart(ChartBuilder::buildDistributionChart(m_rv));
    m_distributionChartView->setRenderHint(QPainter::Antialiasing);

    m_polylineChartView->setChart(ChartBuilder::buildPolylineChart(m_rv));
    m_polylineChartView->setRenderHint(QPainter::Antialiasing);

    m_cdfChartView->setChart(ChartBuilder::buildCdfChart(m_rv));
    m_cdfChartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::showError(const QString& text)
{
    QMessageBox::critical(this, "Ошибка", text);
}