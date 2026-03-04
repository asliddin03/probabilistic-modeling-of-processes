#include "ui/mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <algorithm>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      sim_(std::random_device{}())
{
    auto* central = new QWidget(this);
    setCentralWidget(central);

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_, this);
    view_->setRenderHint(QPainter::Antialiasing, true);

    // элементы управления
    mBox_ = new QSpinBox(this);
    mBox_->setRange(2, 8);
    mBox_->setValue(3);

    hBox_ = new QSpinBox(this);
    hBox_->setRange(1, 8);
    hBox_->setValue(4);

    pBox_ = new QDoubleSpinBox(this);
    pBox_->setRange(0.0, 1.0);
    pBox_->setSingleStep(0.05);
    pBox_->setValue(0.15);

    distBox_ = new QComboBox(this);
    distBox_->addItem("Равномерное (1/M)");
    distBox_->addItem("Смещённое к левым (bias)");

    biasBox_ = new QDoubleSpinBox(this);
    biasBox_->setRange(0.0, 0.99);
    biasBox_->setSingleStep(0.05);
    biasBox_->setValue(0.40);

    kBox_ = new QSpinBox(this);
    kBox_->setRange(1, 1'000'000);
    kBox_->setValue(20000);

    leafBox_ = new QSpinBox(this);
    leafBox_->setRange(0, 0);
    leafBox_->setValue(0);

    runOneBtn_ = new QPushButton("Один запуск", this);
    runKBtn_ = new QPushButton("K экспериментов", this);

    out_ = new QTextEdit(this);
    out_->setReadOnly(true);

    // раскладка
    auto* params = new QGroupBox("Параметры", this);
    auto* pLay = new QVBoxLayout(params);

    auto addRow = [&](const QString& title, QWidget* w) {
        auto* row = new QHBoxLayout();
        row->addWidget(new QLabel(title, this));
        row->addWidget(w);
        pLay->addLayout(row);
    };

    addRow("M (ветвление):", mBox_);
    addRow("Глубина H:", hBox_);
    addRow("p (залипание во внутр. узле):", pBox_);
    addRow("Распределение выбора потомка:", distBox_);
    addRow("bias (для смещённого):", biasBox_);
    addRow("K (число экспериментов):", kBox_);
    addRow("Целевой лист (индекс):", leafBox_);

    auto* btnRow = new QHBoxLayout();
    btnRow->addWidget(runOneBtn_);
    btnRow->addWidget(runKBtn_);
    pLay->addLayout(btnRow);

    auto* left = new QVBoxLayout();
    left->addWidget(params);
    left->addWidget(new QLabel("Лог:", this));
    left->addWidget(out_);

    auto* root = new QHBoxLayout(central);
    root->addLayout(left, 0);
    root->addWidget(view_, 1);

    // сигналы
    connect(runOneBtn_, &QPushButton::clicked, this, &MainWindow::onRunOne);
    connect(runKBtn_, &QPushButton::clicked, this, &MainWindow::onRunK);

    auto connectParam = [&](QObject* w) {
        connect(w, SIGNAL(valueChanged(int)), this, SLOT(onParamsChanged()));
        connect(w, SIGNAL(valueChanged(double)), this, SLOT(onParamsChanged()));
        connect(w, SIGNAL(currentIndexChanged(int)), this, SLOT(onParamsChanged()));
    };

    connectParam(mBox_);
    connectParam(hBox_);
    connectParam(pBox_);
    connectParam(distBox_);
    connectParam(biasBox_);

    // старт
    setWindowTitle("M-арное дерево: симуляция точки (Qt, разделено на core/ui)");
    resize(1200, 700);

    rebuildTree();
}

SimulationConfig MainWindow::readConfigFromUi() const {
    SimulationConfig cfg;
    cfg.M = mBox_->value();
    cfg.H = hBox_->value();
    cfg.pStop = pBox_->value();

    if (distBox_->currentIndex() == 0) cfg.distType = ChildDistributionType::Uniform;
    else cfg.distType = ChildDistributionType::LeftBiased;

    cfg.bias = biasBox_->value();
    return cfg;
}

void MainWindow::clampLeafRange(const SimulationConfig& cfg) {
    const int leaves = powIntSafe(cfg.M, cfg.H);
    leafBox_->setRange(0, std::max(0, leaves - 1));
    if (leafBox_->value() >= leaves) leafBox_->setValue(leaves - 1);
}

void MainWindow::rebuildTree() {
    auto cfg = readConfigFromUi();
    clampLeafRange(cfg);

    renderer_.rebuild(scene_, cfg.M, cfg.H);
    view_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::onParamsChanged() {
    // перестраиваем дерево при изменении параметров
    rebuildTree();
}

void MainWindow::onRunOne() {
    const auto cfg = readConfigFromUi();
    rebuildTree();

    RunResult rr = sim_.runOne(cfg);
    renderer_.highlightPath(scene_, rr.path);

    out_->append("=== Один запуск ===");
    out_->append(QString("M=%1, H=%2, p=%3").arg(cfg.M).arg(cfg.H).arg(cfg.pStop));
    out_->append(QString("Пройдено вершин l = %1").arg((int)rr.path.size()));

    if (rr.endedInLeaf) {
        out_->append(QString("Закончилось в ЛИСТЕ: уровень=%1, индекс=%2")
                     .arg(rr.endNode.level).arg(rr.endNode.idx));
    } else {
        out_->append(QString("Точка ЗАЛИПЛА: уровень=%1, индекс=%2")
                     .arg(rr.endNode.level).arg(rr.endNode.idx));
    }

    QString s = "Путь (level:idx): ";
    for (const auto& nk : rr.path) {
        s += QString("[%1:%2] ").arg(nk.level).arg(nk.idx);
    }
    out_->append(s);
    out_->append("");
}

void MainWindow::onRunK() {
    auto cfg = readConfigFromUi();
    clampLeafRange(cfg);
    rebuildTree();

    const int K = kBox_->value();
    const int targetLeaf = leafBox_->value();

    const int maxL = cfg.H + 1;
    std::vector<long long> cntL(maxL + 1, 0);
    long long hitTarget = 0;
    long long endedInLeaf = 0;
    long long stuckInner = 0;

    for (int i = 0; i < K; ++i) {
        RunResult rr = sim_.runOne(cfg);
        int l = (int)rr.path.size();
        if (l >= 1 && l <= maxL) cntL[l]++;

        if (rr.endedInLeaf) {
            endedInLeaf++;
            if (Simulator::isTargetLeaf(rr, cfg.H, targetLeaf)) hitTarget++;
        } else {
            stuckInner++;
        }
    }

    out_->append("=== K экспериментов ===");
    out_->append(QString("M=%1, H=%2, p=%3, K=%4").arg(cfg.M).arg(cfg.H).arg(cfg.pStop).arg(K));
    out_->append(QString("Целевой лист = %1 (всего листьев %2)")
                 .arg(targetLeaf).arg(powIntSafe(cfg.M, cfg.H)));

    const double pHit = (double)hitTarget / (double)K;
    const double pEndLeaf = (double)endedInLeaf / (double)K;
    const double pStuck = (double)stuckInner / (double)K;

    out_->append(QString("P(попасть в целевой лист) ≈ %1").arg(pHit, 0, 'f', 6));
    out_->append(QString("P(закончить в листе) ≈ %1").arg(pEndLeaf, 0, 'f', 6));
    out_->append(QString("P(залипнуть во внутреннем) ≈ %1").arg(pStuck, 0, 'f', 6));

    out_->append("Распределение P(L=l) по числу пройденных вершин:");
    for (int l = 1; l <= maxL; ++l) {
        double pl = (double)cntL[l] / (double)K;
        out_->append(QString("  l=%1 : %2").arg(l).arg(pl, 0, 'f', 6));
    }
    out_->append("");

    // Подсветим один путь для наглядности
    RunResult rr = sim_.runOne(cfg);
    renderer_.highlightPath(scene_, rr.path);
}