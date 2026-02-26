#include "MainWindow.h"
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

Worker::Worker(int n, int r, int N, int K) : n_(n), r_(r), N_(N), K_(K) {}

void Worker::run() {
    long long okA = 0, okB = 0;

    for (int i = 1; i <= K_; ++i) {
        if (stopFlag_.loadAcquire() != 0) break;

        auto res = sim_.run_one(n_, r_, N_);

        if (res.okA) ++okA;
        if (res.okB) ++okB;

        // Лог для каждого эксперимента
        QString line = QString("Exp %1: A=%2, B=%3")
                           .arg(i)
                           .arg(res.okA ? "OK" : ("FAIL@" + QString::number(res.failStepA)))
                           .arg(res.okB ? "OK" : ("FAIL@" + QString::number(res.failStepB)));

        if (!res.okA) line += " | " + QString::fromStdString(res.failInfoA);
        if (!res.okB) line += " | " + QString::fromStdString(res.failInfoB);

        emit logLine(line);
        emit progress(i, K_);
    }

    emit finished(okA, okB, K_);
}

void Worker::stop() { stopFlag_.storeRelease(1); }

MainWindow::MainWindow() {
    auto *central = new QWidget(this);
    auto *mainLay = new QVBoxLayout(central);

    auto *top = new QHBoxLayout();
    mainLay->addLayout(top);

    // n: в условии город n+1 человек; в UI вводим n
    top->addWidget(new QLabel("n (city has n+1 people):"));
    sbNKeys_ = new QSpinBox();
    sbNKeys_->setRange(1, 100000);
    sbNKeys_->setValue(20);
    top->addWidget(sbNKeys_);

    top->addWidget(new QLabel("r (steps):"));
    sbR_ = new QSpinBox();
    sbR_->setRange(1, 100000);
    sbR_->setValue(10);
    top->addWidget(sbR_);

    top->addWidget(new QLabel("N (group size):"));
    sbGroupN_ = new QSpinBox();
    sbGroupN_->setRange(1, 100000);
    sbGroupN_->setValue(1);
    top->addWidget(sbGroupN_);

    top->addWidget(new QLabel("K (experiments):"));
    sbK_ = new QSpinBox();
    sbK_->setRange(1, 1000000);
    sbK_->setValue(1000);
    top->addWidget(sbK_);

    btnStart_ = new QPushButton("Start");
    btnStop_  = new QPushButton("Stop");
    top->addWidget(btnStart_);
    top->addWidget(btnStop_);

    bar_ = new QProgressBar();
    bar_->setRange(0, 100);
    mainLay->addWidget(bar_);

    table_ = new QTableWidget(2, 3);
    table_->setHorizontalHeaderLabels({"Outcome", "Empirical P", "Count"});
    table_->setVerticalHeaderLabels({"(a) no return to first", "(b) no repeats"});
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLay->addWidget(table_);

    log_ = new QTextEdit();
    log_->setReadOnly(true);
    mainLay->addWidget(log_, 1);

    setCentralWidget(central);

    connect(btnStart_, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(btnStop_,  &QPushButton::clicked, this, &MainWindow::onStop);

    setRunning(false);
    updateTable(0, 0, 1);
}

void MainWindow::setRunning(bool running) {
    btnStart_->setEnabled(!running);
    btnStop_->setEnabled(running);
    sbNKeys_->setEnabled(!running);
    sbR_->setEnabled(!running);
    sbGroupN_->setEnabled(!running);
    sbK_->setEnabled(!running);
}

void MainWindow::onStart() {
    log_->clear();
    bar_->setValue(0);

    int n = sbNKeys_->value();
    int r = sbR_->value();
    int N = sbGroupN_->value();
    int K = sbK_->value();

    if (N > n) {
        log_->append("ERROR: N must be <= n (because sender can choose only n other people).");
        return;
    }

    thread_ = new QThread(this);
    worker_ = new Worker(n, r, N, K);
    worker_->moveToThread(thread_);

    connect(thread_, &QThread::started, worker_, &Worker::run);
    connect(worker_, &Worker::logLine, this, &MainWindow::onWorkerLog);
    connect(worker_, &Worker::progress, this, &MainWindow::onWorkerProgress);
    connect(worker_, &Worker::finished, this, &MainWindow::onWorkerFinished);

    connect(worker_, &Worker::finished, thread_, &QThread::quit);
    connect(thread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(thread_, &QThread::finished, thread_, &QObject::deleteLater);

    setRunning(true);
    thread_->start();
}

void MainWindow::onStop() {
    if (worker_) worker_->stop();
}

void MainWindow::onWorkerLog(const QString& s) {
    log_->append(s);
}

void MainWindow::onWorkerProgress(int done, int total) {
    int pct = (total == 0) ? 0 : (int)(100.0 * done / total);
    bar_->setValue(pct);
}

void MainWindow::updateTable(long long okA, long long okB, int K) {
    auto setRow = [&](int row, const QString& name, long long ok){
        table_->setItem(row, 0, new QTableWidgetItem(name));
        double p = (K > 0) ? (double)ok / (double)K : 0.0;
        table_->setItem(row, 1, new QTableWidgetItem(QString::number(p, 'f', 6)));
        table_->setItem(row, 2, new QTableWidgetItem(QString("%1 / %2").arg(ok).arg(K)));
    };

    setRow(0, "(a) no return to person 0", okA);
    setRow(1, "(b) no repeated recipients", okB);
}

void MainWindow::onWorkerFinished(long long okA, long long okB, int K) {
    updateTable(okA, okB, K);
    setRunning(false);
    worker_ = nullptr;
    thread_ = nullptr;
}