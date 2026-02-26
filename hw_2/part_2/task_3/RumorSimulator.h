#pragma once
#include <random>
#include <string>
#include <vector>

struct ExperimentResult {
    bool okA = true;         // (a) не вернулись к первому (0)
    bool okB = true;         // (b) не было повторных получателей
    int failStepA = -1;      // на каком шаге нарушили (a), если нарушили
    int failStepB = -1;      // на каком шаге нарушили (b), если нарушили
    std::string failInfoA;   // кто получил 0
    std::string failInfoB;   // кто получил повторно
};

class RumorSimulator {
public:
    RumorSimulator();

    // n: (n+1) человек, индексы 0..n
    // r: число шагов (передач)
    // N: размер группы на шаге (N=1 -> один человек)
    ExperimentResult run_one(int n, int r, int N);

private:
    std::mt19937 rng_;

    // выбрать N разных получателей из {0..n}\{sender}
    std::vector<int> pick_recipients(int n, int sender, int N);
    int pick_next_sender(const std::vector<int>& recipients);
};