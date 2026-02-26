#pragma once
#include <QMainWindow>
#include <QAtomicInt>
#include <QThread>

class QSpinBox;
class QPushButton;
class QTextEdit;
class QTableWidget;
class QProgressBar;

#include "RumorSimulator.h"

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(int n, int r, int N, int K);

    signals:
        void logLine(const QString& s);
    void progress(int done, int total);
    void finished(long long okA, long long okB, int K);

public slots:
    void run();
    void stop();

private:
    int n_, r_, N_, K_;
    QAtomicInt stopFlag_{0};
    RumorSimulator sim_;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void onStart();
    void onStop();
    void onWorkerLog(const QString& s);
    void onWorkerProgress(int done, int total);
    void onWorkerFinished(long long okA, long long okB, int K);

private:
    QSpinBox *sbNKeys_, *sbR_, *sbGroupN_, *sbK_;
    QPushButton *btnStart_, *btnStop_;
    QTextEdit *log_;
    QTableWidget *table_;
    QProgressBar *bar_;

    QThread* thread_ = nullptr;
    Worker* worker_ = nullptr;

    void setRunning(bool running);
    void updateTable(long long okA, long long okB, int K);
};