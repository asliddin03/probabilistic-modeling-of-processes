#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>

#include "core/simulator.h"
#include "ui/tree_renderer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onRunOne();
    void onRunK();
    void onParamsChanged();

private:
    // UI
    QGraphicsView* view_ = nullptr;
    QGraphicsScene* scene_ = nullptr;

    QSpinBox* mBox_ = nullptr;
    QSpinBox* hBox_ = nullptr;
    QDoubleSpinBox* pBox_ = nullptr;

    QComboBox* distBox_ = nullptr;
    QDoubleSpinBox* biasBox_ = nullptr;

    QSpinBox* kBox_ = nullptr;
    QSpinBox* leafBox_ = nullptr;

    QPushButton* runOneBtn_ = nullptr;
    QPushButton* runKBtn_ = nullptr;

    QTextEdit* out_ = nullptr;

    // Core + Renderer
    Simulator sim_;
    TreeRenderer renderer_;

private:
    SimulationConfig readConfigFromUi() const;
    void rebuildTree();
    void clampLeafRange(const SimulationConfig& cfg);
};