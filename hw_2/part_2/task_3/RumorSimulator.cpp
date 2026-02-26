#include "../RumorSimulator.h"
#include <algorithm>
#include <sstream>

RumorSimulator::RumorSimulator()
    : rng_(std::random_device{}()) {}

std::vector<int> RumorSimulator::pick_recipients(int n, int sender, int N) {
    // всего людей n+1: 0..n, исключаем sender -> кандидатов n
    std::vector<int> candidates;
    candidates.reserve(n);
    for (int i = 0; i <= n; ++i) {
        if (i != sender) candidates.push_back(i);
    }

    // N <= n должно выполняться в UI, но на всякий случай:
    if (N > (int)candidates.size()) N = (int)candidates.size();

    // Выбор без повторов внутри шага:
    std::shuffle(candidates.begin(), candidates.end(), rng_);
    candidates.resize(N);
    return candidates;
}

int RumorSimulator::pick_next_sender(const std::vector<int>& recipients) {
    std::uniform_int_distribution<int> dist(0, (int)recipients.size() - 1);
    return recipients[dist(rng_)];
}

ExperimentResult RumorSimulator::run_one(int n, int r, int N) {
    ExperimentResult res;

    const int first = 0;
    int sender = first;

    // informed[x] == true => человек уже получал новость (или первый знает с начала)
    std::vector<char> informed(n + 1, 0);
    informed[first] = 1;

    // шаги нумеруем 1..r
    for (int step = 1; step <= r; ++step) {
        auto recipients = pick_recipients(n, sender, N);

        // Проверка (a): не возвращаться к первому (0) — начиная со 2-го шага
        if (step >= 2 && res.okA) {
            if (std::find(recipients.begin(), recipients.end(), first) != recipients.end()) {
                res.okA = false;
                res.failStepA = step;
                res.failInfoA = "first person (0) was informed again";
            }
        }

        // Проверка (b): никто не должен получать повторно
        if (res.okB) {
            for (int x : recipients) {
                if (informed[x]) {
                    res.okB = false;
                    res.failStepB = step;
                    std::ostringstream oss;
                    oss << "person " << x << " received again";
                    res.failInfoB = oss.str();
                    break;
                }
            }
        }

        // Обновляем informed
        for (int x : recipients) informed[x] = 1;

        // Следующий рассказчик (для продолжения процесса)
        sender = (N == 1) ? recipients[0] : pick_next_sender(recipients);
    }

    return res;
}