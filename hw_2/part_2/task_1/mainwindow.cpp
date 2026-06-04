#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createInterface();
}

void MainWindow::createInterface()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QGroupBox* settingsGroup = new QGroupBox("Параметры моделирования");
    QGridLayout* settingsLayout = new QGridLayout(settingsGroup);

    QLabel* p1Label = new QLabel("Вероятность заражения p1:");
    QLabel* p2Label = new QLabel("Вероятность выздоровления p2:");
    QLabel* stepsLabel = new QLabel("Количество шагов:");

    infectionProbabilityEdit = new QLineEdit();
    infectionProbabilityEdit->setText("0.3");
    infectionProbabilityEdit->setPlaceholderText("Например: 0.3");

    recoveryProbabilityEdit = new QLineEdit();
    recoveryProbabilityEdit->setText("0.2");
    recoveryProbabilityEdit->setPlaceholderText("Например: 0.2");

    stepsEdit = new QLineEdit();
    stepsEdit->setText("10");
    stepsEdit->setPlaceholderText("Например: 10");

    loadButton = new QPushButton("Загрузить файл");
    simulateButton = new QPushButton("Запустить моделирование");

    settingsLayout->addWidget(p1Label, 0, 0);
    settingsLayout->addWidget(infectionProbabilityEdit, 0, 1);

    settingsLayout->addWidget(p2Label, 1, 0);
    settingsLayout->addWidget(recoveryProbabilityEdit, 1, 1);

    settingsLayout->addWidget(stepsLabel, 2, 0);
    settingsLayout->addWidget(stepsEdit, 2, 1);

    settingsLayout->addWidget(loadButton, 0, 2);
    settingsLayout->addWidget(simulateButton, 1, 2);

    QGroupBox* searchGroup = new QGroupBox("Поиск по результатам");
    QGridLayout* searchLayout = new QGridLayout(searchGroup);

    notInfectedButton = new QPushButton("Все незаразившиеся");
    recoveredButton = new QPushButton("Все исцелившиеся");
    notRecoveredEnvironmentButton = new QPushButton("Окружение не исцелилось");
    allInfectedEnvironmentButton = new QPushButton("Заражённые, если всё окружение заражено");

    searchLayout->addWidget(notInfectedButton, 0, 0);
    searchLayout->addWidget(recoveredButton, 0, 1);
    searchLayout->addWidget(notRecoveredEnvironmentButton, 1, 0);
    searchLayout->addWidget(allInfectedEnvironmentButton, 1, 1);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setText("Загрузите файл с входными данными.");

    mainLayout->addWidget(settingsGroup);
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(output);

    connect(loadButton, &QPushButton::clicked,
            this, &MainWindow::loadFile);

    connect(simulateButton, &QPushButton::clicked,
            this, &MainWindow::runSimulation);

    connect(notInfectedButton, &QPushButton::clicked,
            this, &MainWindow::showNotInfected);

    connect(recoveredButton, &QPushButton::clicked,
            this, &MainWindow::showRecovered);

    connect(notRecoveredEnvironmentButton, &QPushButton::clicked,
            this, &MainWindow::showPeopleWithNotRecoveredEnvironment);

    connect(allInfectedEnvironmentButton, &QPushButton::clicked,
            this, &MainWindow::showInfectedWithAllInfectedEnvironment);
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Выберите файл с входными данными",
        "",
        "Текстовые файлы (*.txt);;Все файлы (*.*)"
    );

    if (fileName.isEmpty())
        return;

    std::string errorMessage;

    bool loaded = logic.loadFromFile(fileName.toStdString(), errorMessage);

    if (!loaded)
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            QString::fromStdString(errorMessage)
        );
        return;
    }

    output->setText(
        "Файл успешно загружен.\n\n" +
        QString::fromStdString(logic.getAllPeopleInfo())
    );
}

void MainWindow::runSimulation()
{
    if (logic.peopleCount() == 0)
    {
        QMessageBox::warning(
            this,
            "Предупреждение",
            "Сначала загрузите файл."
        );
        return;
    }

    QString p1Text = infectionProbabilityEdit->text();
    QString p2Text = recoveryProbabilityEdit->text();
    QString stepsText = stepsEdit->text();

    p1Text.replace(',', '.');
    p2Text.replace(',', '.');

    bool okP1 = false;
    bool okP2 = false;
    bool okSteps = false;

    double p1 = p1Text.toDouble(&okP1);
    double p2 = p2Text.toDouble(&okP2);
    int steps = stepsText.toInt(&okSteps);

    if (!okP1 || !okP2 || !okSteps)
    {
        QMessageBox::warning(
            this,
            "Ошибка ввода",
            "Введите корректные числовые значения.\n\n"
            "Пример:\n"
            "p1 = 0.3\n"
            "p2 = 0.2\n"
            "Количество шагов = 10"
        );
        return;
    }

    if (p1 < 0.0 || p1 > 1.0)
    {
        QMessageBox::warning(
            this,
            "Ошибка ввода",
            "Вероятность заражения p1 должна быть от 0 до 1."
        );
        return;
    }

    if (p2 < 0.0 || p2 > 1.0)
    {
        QMessageBox::warning(
            this,
            "Ошибка ввода",
            "Вероятность выздоровления p2 должна быть от 0 до 1."
        );
        return;
    }

    if (steps <= 0)
    {
        QMessageBox::warning(
            this,
            "Ошибка ввода",
            "Количество шагов должно быть положительным целым числом."
        );
        return;
    }

    logic.setProbabilities(p1, p2);
    logic.simulate(steps);

    QString text;

    text += "Моделирование выполнено.\n";
    text += "p1 = " + QString::number(p1) + "\n";
    text += "p2 = " + QString::number(p2) + "\n";
    text += "Количество шагов = " + QString::number(steps) + "\n\n";
    text += QString::fromStdString(logic.getAllPeopleInfo());

    output->setText(text);
}

void MainWindow::showNotInfected()
{
    showResult(
        "Все незаразившиеся люди",
        logic.findNotInfectedPeople()
    );
}

void MainWindow::showRecovered()
{
    showResult(
        "Все исцелившиеся люди",
        logic.findRecoveredPeople()
    );
}

void MainWindow::showPeopleWithNotRecoveredEnvironment()
{
    showResult(
        "Люди, окружение которых не исцелилось",
        logic.findPeopleWithNotRecoveredEnvironment()
    );
}

void MainWindow::showInfectedWithAllInfectedEnvironment()
{
    showResult(
        "Заражённые люди, если всё их окружение заражено",
        logic.findInfectedWithAllInfectedEnvironment()
    );
}

void MainWindow::showResult(const QString& title, const std::vector<int>& people)
{
    if (logic.peopleCount() == 0)
    {
        QMessageBox::warning(
            this,
            "Предупреждение",
            "Сначала загрузите файл."
        );
        return;
    }

    output->append("\n------------------------------");
    output->append(title + ":");
    output->append(QString::fromStdString(logic.vectorToString(people)));
}