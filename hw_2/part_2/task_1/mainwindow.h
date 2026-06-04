#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

#include "epidemiclogic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void loadFile();
    void runSimulation();

    void showNotInfected();
    void showRecovered();
    void showPeopleWithNotRecoveredEnvironment();
    void showInfectedWithAllInfectedEnvironment();

private:
    EpidemicLogic logic;

    QTextEdit* output;

    QLineEdit* infectionProbabilityEdit;
    QLineEdit* recoveryProbabilityEdit;
    QLineEdit* stepsEdit;

    QPushButton* loadButton;
    QPushButton* simulateButton;
    QPushButton* notInfectedButton;
    QPushButton* recoveredButton;
    QPushButton* notRecoveredEnvironmentButton;
    QPushButton* allInfectedEnvironmentButton;

    void createInterface();
    void showResult(const QString& title, const std::vector<int>& people);
};