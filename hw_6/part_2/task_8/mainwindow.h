#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QDoubleSpinBox;
class QSpinBox;
class QWidget;
QT_END_NAMESPACE

class DrawWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void doOneThrow();
    void doManyThrows();
    void clearSimulation();

private:
    bool crossesLine(double centerXInArea, double angle) const;
    void updateStatistics();

    DrawWidget *m_drawWidget;

    QDoubleSpinBox *m_lengthSpin;
    QDoubleSpinBox *m_distanceSpin;
    QSpinBox *m_batchSpin;

    QPushButton *m_oneThrowButton;
    QPushButton *m_manyThrowsButton;
    QPushButton *m_clearButton;

    QLabel *m_totalLabel;
    QLabel *m_crossedLabel;
    QLabel *m_empiricalLabel;
    QLabel *m_exactLabel;
    QLabel *m_errorLabel;

    long long m_totalThrows = 0;
    long long m_crossedThrows = 0;
};